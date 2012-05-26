from parse import parse
from scipy.io import savemat
import sys

if len(sys.argv) != 2:
	sys.exit("Please specify a file")

(acc, gyro, mag, press) = parse(sys.argv[1])

savemat(sys.argv[1] + ".mat", dict(acc=acc, gyro=gyro, mag=mag, pressure=press))
