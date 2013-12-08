/*!
 @file imu_math.h
 @brief A (currently small) library of basic IMU math-related types and functions
 @author Ben Nahill <bnahill@gmail.com>
 */

#ifndef __IMU_MATH_H_
#define __IMU_MATH_H_

#include "ch.h"
#include "hal.h"
#include "math.h"

#include <stdint.h>
#include <cmath>
#include <algorithm>

/*!
 * @brief A class for accessing usable types from arbitrary bit lengths
 * @tparam n_bits The minimum number of bits required
 */
template <size_t n_bits>
class IntLength {
public:
	typedef typename IntLength<n_bits+1>::unsigned_fast_t unsigned_fast_t;
	typedef typename IntLength<n_bits+1>::unsigned_t unsigned_t;
	typedef typename IntLength<n_bits+1>::signed_fast_t signed_fast_t;
	typedef typename IntLength<n_bits+1>::signed_t signed_t;
};

template<>
class IntLength<8>{
public:
	typedef uint_fast8_t unsigned_fast_t;
	typedef uint8_t unsigned_t;
	typedef int_fast8_t signed_fast_t;
	typedef int8_t signed_t;
};
template<>
class IntLength<16>{
public:
	typedef uint_fast16_t unsigned_fast_t;
	typedef uint16_t unsigned_t;
	typedef int_fast16_t signed_fast_t;
	typedef int16_t signed_t;
};
template<>
class IntLength<32>{
public:
	typedef uint_fast32_t unsigned_fast_t;
	typedef uint32_t unsigned_t;
	typedef int_fast32_t signed_fast_t;
	typedef int32_t signed_t;
};
template<>
class IntLength<64>{
public:
	typedef uint_fast64_t unsigned_fast_t;
	typedef uint64_t unsigned_t;
	typedef int_fast64_t signed_fast_t;
	typedef int64_t signed_t;
};



template <size_t i_bits, size_t f_bits>
class uFractional {
public:
	typedef typename IntLength<i_bits + f_bits>::unsigned_t internal_t;

	constexpr uFractional(const uFractional<i_bits, f_bits> &val) :
			 i(val.i){}
	constexpr uFractional(const internal_t &val) :
			 i(val){}
	constexpr uFractional(const double &val) :
		i(val * (1 << f_bits)){}
	constexpr uFractional(const float &val) :
		i(val * (1 << f_bits)){}
	constexpr uFractional(const int &val) :
		i(((unsigned int)val) << f_bits){}
	double asDouble() const{
		return ((double)i) / (1 << f_bits);
	}

	/*!
	 * @brief Convert a fractional number to a different format
	 */
	template<size_t ni_bits, size_t nf_bits>
	uFractional<ni_bits, nf_bits> normalize(){
		typedef uFractional<ni_bits, nf_bits> n_t;
		return n_t((typename n_t::internal_t) (i >> (f_bits - nf_bits)));
	}

	/*!
	 * @brief Resize the integer part of a number
	 *
	 * This will not change the value unless it exceeds the final format
	 */
	template<size_t ni_bits>
	uFractional<ni_bits, f_bits> resize(){
		typedef uFractional<ni_bits, f_bits> n_t;
		return n_t((typename n_t::internal_t) i);
	}

	template<size_t mi_bits, size_t mf_bits>
	uFractional<mi_bits + i_bits, mf_bits + f_bits> operator * (uFractional<mi_bits, mf_bits> &m){
		return m.i * i;
	}

	template<size_t mi_bits, size_t mf_bits>
	uFractional<std::max(mi_bits, i_bits), std::max(mf_bits, f_bits)> operator + (uFractional<mi_bits, mf_bits> &m){
		return m.i + i;
	}

	template<size_t mi_bits, size_t mf_bits>
	uFractional<std::max(mi_bits, i_bits), std::max(mf_bits, f_bits)> operator - (uFractional<mi_bits, mf_bits> &m){
		return m.i - i;
	}

	static char * format_str(char * dst) {
		return imu_sprint(dst, "u", (uint32_t)i_bits, ".", (uint32_t)f_bits);
	}

private:
	internal_t i : i_bits + f_bits;
};



/*!
 * @brief A signed fractional integer
 * @tparam i_bits The number of integer bits (not including sign)
 * @tparam f_bits The number of fractional bits
 */
template <size_t i_bits, size_t f_bits>
class sFractional {
public:
	typedef typename IntLength<i_bits + f_bits + 1>::signed_t internal_t;

	constexpr sFractional(const sFractional<i_bits, f_bits> &val) :
			 i(val.i){}

	/*!
	 * @brief Constructor for arbitrary signed fractionals
	 */
	template<size_t mi_bits, size_t mf_bits>
	constexpr sFractional(const sFractional<mi_bits, mf_bits> &val) :
		i(val.normalize<mi_bits, mf_bits>().i){}
	constexpr sFractional(const internal_t &val) :
			 i(val){}
	constexpr sFractional(const double &val) :
		i(val * (1 << f_bits)){}
	constexpr sFractional(const float &val) :
		i(val * (1 << f_bits)){}
	constexpr sFractional(const int &val) :
		i(((signed int)val) << f_bits){}
	double asDouble() const{
		return ((double)i) / (1 << f_bits);
	}

	/*!
	 * @brief Convert a fractional number to a different format
	 */
	template<size_t ni_bits, size_t nf_bits>
	sFractional<ni_bits, nf_bits> normalize(){
		typedef sFractional<ni_bits, nf_bits> n_t;
		return n_t((typename n_t::internal_t) (i >> (f_bits - nf_bits)));
	}

	/*!
	 * @brief Resize the integer part of a number
	 *
	 * This will not change the value unless it exceeds the final format
	 */
	template<size_t ni_bits>
	sFractional<ni_bits, f_bits> resize(){
		typedef sFractional<ni_bits, f_bits> n_t;
		return n_t((typename n_t::internal_t) i);
	}

	template<size_t mi_bits, size_t mf_bits>
	sFractional<mi_bits + i_bits, mf_bits + f_bits> operator * (sFractional<mi_bits, mf_bits> &m){
		return m.i * i;
	}

	template<size_t mi_bits, size_t mf_bits>
	sFractional<std::max(mi_bits, i_bits), std::max(mf_bits, f_bits)> operator + (sFractional<mi_bits, mf_bits> &m){
		return m.i + i;
	}

	template<size_t mi_bits, size_t mf_bits>
	sFractional<std::max(mi_bits, i_bits), std::max(mf_bits, f_bits)> operator - (sFractional<mi_bits, mf_bits> &m){
		return m.i - i;
	}

	static char * format_str(char * dst) {
		return imu_sprint(dst, "s", (uint32_t)i_bits, ".", (uint32_t)f_bits);
	}

private:
	internal_t i : i_bits + f_bits + 1;
};

/*!
 @brief A 3-dimensional vector in euclidean space
 @tparam datatype The datatype for each dimension
 */
template <typename datatype>
class Euclidean3 {
public:
	Euclidean3() : x(0),y(0),z(0) {}
	
	//! Constructor from 3 data values
	template <typename typex, typename typey, typename typez>
	Euclidean3(typex x, typey y, typez z) :
		x((datatype)x), y((datatype)y), z((datatype)z)
		{}
	
	//! Constructor from another euclidean3 type
	template <typename othertype>
	Euclidean3(Euclidean3<othertype> const &other) :
		x((datatype)other.x), y((datatype)other.y), z((datatype)other.z)
		{}
	
	/*!
	 @brief Multiplication with a scalar
	 @tparam othertype The scalar type
	 */
	template <typename othertype>
	const Euclidean3<datatype> operator* (const othertype mult) const{
		return Euclidean3(x*mult, y*mult, z*mult);
	}
	
	/*!
	 @brief Addition of a scalar
	 @tparam othertype The scalar type
	 */
	template <typename othertype>
	const Euclidean3<datatype> operator+ (const othertype add) const{
		return Euclidean3(x + add, y + add, z + add);
	}
	
	/*!
	 @brief Subtraction of a scalar
	 @tparam othertype The scalar type
	 */
	template <typename othertype>
	const Euclidean3<datatype> operator- (const othertype sub) const{
		return Euclidean3(x - sub, y - sub, z - sub);
	}
	
	template <typename othertype>
	const Euclidean3<datatype> operator*= (const othertype mult){
		x *= mult; y *= mult; z *= mult;
		return *this;
	}
	
	template <typename othertype>
	const Euclidean3<datatype> operator-= (const othertype sub){
		x -= sub; y -= sub; z -= sub;
		return *this;
	}
	
	template <typename othertype>
	const Euclidean3<datatype> operator+= (const othertype add){
		x += add; y += add; z += add;
		return *this;
	}
	
	/*!
	 @brief Multiplication with another vector
	 @tparam othertype The other vector type
	 */
	template <typename othertype>
	const Euclidean3<datatype> operator* (const Euclidean3< othertype > mult) const{
		return Euclidean3(x*mult.x, y*mult.y, z*mult.z);
	}
	
	/*!
	 @brief Addition of another vector
	 @tparam othertype The other vector type
	 */
	template <typename othertype>
	const Euclidean3<datatype> operator+ (const Euclidean3< othertype > add) const{
		return Euclidean3(x + add.x, y + add.y, z + add.z);
	}
	
	/*!
	 @brief Subtraction of another vector
	 @tparam othertype The other vector type
	 */
	template <typename othertype>
	const Euclidean3<datatype> operator- (const Euclidean3< othertype > sub) const{
		return Euclidean3(x - sub.x, y - sub.y, z - sub.z);
	}
	
	template <typename othertype>
	const Euclidean3<datatype> operator*= (const Euclidean3< othertype > mult){
		x *= mult.x; y *= mult.y; z *= mult.z;
		return *this;
	}
	
	template <typename othertype>
	const Euclidean3<datatype> operator-= (const Euclidean3< othertype > sub){
		x -= sub.x; y -= sub.y; z -= sub.z;
		return *this;
	}
	
	template <typename othertype>
	const Euclidean3<datatype> operator+= (const Euclidean3< othertype > add){
		x += add.x; y += add.y; z += add.z;
		return *this;
	}
	
	/*!
	 @brief Return the magnitude squared
	 @return The minimum of a and b
	*/
	datatype mag_squared() const {
		return x*x + y*y + z*z;
	}
	
	datatype mag() const {
		return sqrtf(mag_squared());
	}
	
	union {
		struct {
			float x,y,z;
		};
		struct {
			float roll, pitch, yaw;
		};
		float xyz[3];
	};
};

typedef Euclidean3<float> Euclidean3_f32;

/*!
 @brief Return the maximum of two values
 @return The maximum of a and b
 */
template<typename typea, typename typeb>
typea max(typea const &a, typeb const &b){
	return (a > (typea)b) ? a : (typea)b;
}

/*!
 @brief Return the minimum of two values
 @return The minimum of a and b
 */
template<typename typea, typename typeb>
typea min(typea const &a, typeb const &b){
	return (a < (typea)b) ? a : (typea)b;
}

/*!
 @brief Clamp a value to a range
 @param value The value to clamp
 @param min The minimum value to output
 @param max The maximum values to output
 @return The clamped value
 */
template<typename t, typename comptype>
t clamp(t value, comptype min, comptype max){
	if(value > (t)max) return (t)max;
	if(value < (t)min) return (t)min;
	return value;
}

/*!
 @brief A function to produce a type string for a given type

 This must be defined for whatever types you need, but by default it will
 try to use the class's format_str method
 */
template <typename T>
static char * get_type_str(char * dst){
	return T::format_str(dst);
}

template <>
char * get_type_str<float>(char *dst){return imu_sprint(dst,"f32");}

template <>
char * get_type_str<uint32_t>(char *dst){return imu_sprint(dst,"u32");}

template <>
char * get_type_str<uint16_t>(char *dst){return imu_sprint(dst,"u16");}

template <>
char * get_type_str<uint8_t>(char *dst){return imu_sprint(dst,"u8");}

template <>
char * get_type_str<int32_t>(char *dst){return imu_sprint(dst,"s32");}

template <>
char * get_type_str<int16_t>(char *dst){return imu_sprint(dst,"s16");}

template <>
char * get_type_str<int8_t>(char *dst){return imu_sprint(dst,"s8");}

#endif //__IMU_MATH_H_
