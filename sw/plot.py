from parse import parse
import pylab
import sys

if len(sys.argv) != 2:
	sys.exit("Please specify a file")

(acc, gyro, mag, press) = parse(sys.argv[1])

pylab.subplot("411")
pylab.title("Acceleration")
pylab.plot(acc[0])
pylab.plot(acc[1])
pylab.plot(acc[2])
pylab.subplot("412")
pylab.title("Rotation")
pylab.plot(gyro[0])
pylab.plot(gyro[1])
pylab.plot(gyro[2])
pylab.subplot("413")
pylab.title("Magnetic Field")
pylab.plot(mag[0])
pylab.plot(mag[1])
pylab.plot(mag[2])
pylab.subplot("414")
pylab.title("Pressure")
pylab.plot(press)
pylab.show()


