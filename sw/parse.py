import struct
import pylab

frame_len = 8192

def get_float(arr):
	return struct.unpack_from('f', arr[:4])

def parse(fname):
	f = open(fname, 'r')
	accx = []
	accy = []
	accz = []
	gyrp = []
	gyrr = []
	gyry = []
	magx = []
	magy = []
	magz = []
	pressure = []

	while True:
		b = f.read(frame_len)
		if len(b) == 0 :
			break
		num_samples = struct.unpack_from('I', b[:4])[0]
		b = b[4:]
		while num_samples:
			num_samples -= 1
			gyrp += get_float(b)
			b = b[4:]
			gyrr += get_float(b)
			b = b[4:]
			gyry += get_float(b)
			b = b[4:]
			accx += get_float(b)
			b = b[4:]
			accy += get_float(b)
			b = b[4:]
			accz += get_float(b)
			b = b[4:]
			magx += get_float(b)
			b = b[4:]
			magy += get_float(b)
			b = b[4:]
			magz += get_float(b)
			b = b[4:]
			pressure += get_float(b)
			b = b[4:]
			
	f.close()
	return ((accx, accy, accz), (gyrp, gyrr,gyry), (magx, magy, magz), pressure)

