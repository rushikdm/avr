import re, os, subprocess
from enum import Enum

class MCU(Enum):
	ATmega8   = 1
	ATmega16  = 2
	ATmega32  = 3
	ATmega64  = 4
	ATmega128 = 5
	ATmega256 = 6

mcus = {MCU.ATmega8  : ["atmega8",   "ATmega8"],
		MCU.ATmega16 : ["atmega16",  "ATmega16"],
		MCU.ATmega32 : ["atmega32",  "ATmega32"],
		MCU.ATmega64 : ["atmega64",  "ATmega64"],
		MCU.ATmega128: ["atmega128", "ATmega128"],
		MCU.ATmega256: ["atmega256", "ATmega256"],
	   }

class AVRUtilities:
	
	@staticmethod
	def ExecuteCommand(cmd):
		print(cmd)
		subprocess.call(cmd, shell=True)
		
	@staticmethod
	def Compile(mcu, cfile, ofile, clean=True):
		if not os.path.exists(cfile):
			print(f"ERROR: {cfile} file does not exist.")
			return False
		if clean and os.path.exists(ofile):
			try:
				os.remove(ofile)
				print(f"Deleted {ofile} before compile.")
			except OSError as e:
				print(f"ERROR: {e.filename} - {e.strerror}")
		mcu = mcus[mcu][0]
		cmd = f"avr-gcc -Wall -Os -mmcu={mcu} -o {ofile} {cfile}"
		AVRUtilities.ExecuteCommand(cmd)
		if os.path.exists(ofile):
			print(f"Successfully generated {ofile}")
		else:
			print(f"ERROR in compilation for generation of {ofile}")
			return False
		return True
			
	@staticmethod
	def GenerateHexFile(ofile, hexfile, clean=True):
		if not os.path.exists(ofile):
			print(f"ERROR: {ofile} file does not exist.")
			return False
		if clean and os.path.exists(hexfile):
			try:
				os.remove(hexfile)
				print(f"Deleted {hexfile} before generation of hex file.")
			except OSError as e:
				print(f"ERROR: {e.filename} - {e.strerror}")
				return False
		cmd = f"avr-objcopy -j .text -j .data -O ihex {ofile} {hexfile}"
		AVRUtilities.ExecuteCommand(cmd)
		if os.path.exists(hexfile):
			print(f"Successfully generated {hexfile}")
		else:
			print(f"Error in generation of {hexfile}")
			return False
		return True
	
	@staticmethod
	def Upload(mcu, hexfile, port):
		if not os.path.exists(hexfile):
			print(f"ERROR: {hexfile} does not exist.")
			return False
		mcu = mcus[mcu][1]
		cmd = f"avrdude -p {mcu} -c arduino -P {port} -b 19200 -U flash:w:{hexfile}:i"
		AVRUtilities.ExecuteCommand(cmd)
		
	@staticmethod
	def PerformAll(mcu, cfile, port):
		if not os.path.exists(cfile):
			print(f"ERROR: {cfile} does not exist.")
			return False
		base_file_name = re.sub(".c", "", cfile)
		ofile = base_file_name+".o"
		success = AVRUtilities.Compile(mcu, cfile, ofile)
		if not success:
			return
		hexfile = base_file_name+".hex"
		success = AVRUtilities.GenerateHexFile(ofile, hexfile)
		if not success:
			return
		AVRUtilities.Upload(mcu, hexfile, port)
		
	@staticmethod
	def GetSingleCFile():
		cfiles = [name for name in os.listdir('.') if os.path.isfile(name) and re.search("\.c", name)]
		if 1 == len(cfiles):
			return True, cfiles[0]
		else:
			return False, 'ERROR'

#AVRUtilities.ExecuteCommand("avr-gcc -Wall -Os -mmcu=atmega8 -o blower_pwm.o blower_pwm.c")
#AVRUtilities.ExecuteCommand("avr-objcopy -j .text -j .data -O ihex blower_pwm.o blower_pwm.hex")
#AVRUtilities.ExecuteCommand("avrdude -p ATmega8 -c arduino -P COM3 -b 19200 -U flash:w:blower_pwm.hex:i")

#AVRUtilities.Compile(MCU.ATmega8, "blower_pwm.c", "blower_pwm.o")
#AVRUtilities.GenerateHexFile("blower_pwm.o", "blower_pwm.hex")
#AVRUtilities.Upload(MCU.ATmega8, "blower_pwm.hex", "COM3")
#AVRUtilities.PerformAll(MCU.ATmega8, "blower_pwm.c", "COM18")

