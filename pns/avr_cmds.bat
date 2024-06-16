echo avr-gcc -Wall -Os -mmcu=atmega8 -o %1.o %1.c > avrc.bat
echo avr-objcopy -j .text -j .data -O ihex %1.o %1.hex > avrh.bat
echo avrdude -p ATmega8 -c arduino -P COM3 -b 19200 -U flash:w:%1.hex:i > avru.bat
type avr*.bat
