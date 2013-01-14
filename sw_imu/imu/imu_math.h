#ifndef __IMU_MATH_H_
#define __IMU_MATH_H_

#include "ch.h"
#include "hal.h"
#include "math.h"

/*!
 @brief A 3-dimensional vector in euclidean space
 @tparam datatype The datatype for each dimension
 */
template <typename datatype>
class Euclidean3 {
public:
	Euclidean3(){}
	
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

#endif //__IMU_MATH_H_
