from serial.tools.list_ports import *
import argparse
import serial
import sys
import time
import binascii
import subprocess
import os

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
  if not os.path.exists('./out'):
    os.mkdir('./out')

  tmp_binary_path = './out/' + os.path.basename(args.file).split('.')[0] + '.tmp.bin'
  out_binary_path = './out/' + os.path.basename(args.file).split('.')[0] + '.bin'
  final_c_path = os.path.basename(args.file).split('.')[0] + '.c'

  print subprocess.check_output(['hex2bin', args.file, tmp_binary_path])

  # tail -c +32769 out/wiem_slave.bin | head -c 31744 > wiem_slave.bin
  print subprocess.Popen('tail -c +32769 ' + tmp_binary_path + '| head -c 31744 > ' +  out_binary_path, shell=True, stdout=subprocess.PIPE).stdout.read()

  print subprocess.Popen('cd out; xxd -i ' + os.path.basename(args.file).split('.')[0] + '.bin > ' + final_c_path, shell=True, stdout=subprocess.PIPE).stdout.read()

  print 'Finished!'

if __name__ == "__main__":
  parser = argparse.ArgumentParser()
  parser.add_argument("-f", "--file", type=str, required=True, help="Specify the hex file to convert")

  main(parser.parse_args())

