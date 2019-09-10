from serial.tools.list_ports import *
import argparse
import serial
import sys
import time
import binascii

VERBOSE = False
TIMEOUT = 5

def send_command(ser, cmd, waitForResponse=True):
	global VERBOSE

	if VERBOSE:
		print cmd

	ser.write(cmd + '\n')

	subsystem = cmd.split(' ')[0]

	if waitForResponse:
		s = ''
		try:
			s = ser.read_until('> %s:' % subsystem)
			s = s + ser.read_until('\n')
		except SerialException:
			print 'ERROR: \"%s\": No response' % cmd
			sys.exit()
			return (False, 'No response')

		if VERBOSE:
			print 'Response: %s' % s

		try:
			i = s.find('> %s:ok' % subsystem)
			return (True, s[:i])

		except ValueError:
			print 'Error:\n%s' % s
			sys.exit()
			return (False, '')
	else:
		return (True, '')


def main(args):
	global VERBOSE

	if args.verbose:
	    print "verbosity turned on"
	    VERBOSE = True

	with serial.Serial(port=args.device, timeout=args.timeout, writeTimeout=args.timeout) as ser:
		print 'Boot CC85xx into bootloader...'
		send_command(ser, 'rf bl_reset')

		print 'Unlock CC85xx bootloader for SPI access...'
		send_command(ser, 'rf bl_unlock')

		print 'Perform mass erase...'
		send_command(ser, 'rf bl_erase')

		# Flash pages
		with open(args.file, 'r') as f:
			while True:
				line = f.readline().strip()
				#print 'line = %s' % line
				if line == '' or line[:3] != ':10' or line[:5] == ':10FC':
					break

				addr = '0x%s' % line[3:7]
				hexline = line[9:-2]

				print 'Writing to addr %s...' % addr

				#print 'hexline = %s' % hexline
				if send_command(ser, 'rf bl_flash %s %s' % (addr, hexline))[0] == False:
					sys.exit()

		print 'Verify CRC...'
		send_command(ser, 'rf bl_verify')

		print 'Boot into new image...'
		send_command(ser, 'sys reset', waitForResponse=False)

	print 'Finished!'

if __name__ == "__main__":
	parser = argparse.ArgumentParser()
	parser.add_argument("-v", "--verbose", help="increase output verbosity",
	                    action="store_true")
	parser.add_argument("-t", "--timeout", type=int, default=TIMEOUT, help="Specify timeout in waiting for response (default: 1s)")
	parser.add_argument("-d", "--device", required=True, help="USB device (e.g. '/dev/cu.usbmodem143301')")
	parser.add_argument("-f", "--file", type=str, required=True, help="Specify the hex file to flash")

	main(parser.parse_args())

