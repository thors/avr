SOURCES=src/main.c

MMCU=attiny85
AVRDUDE_TARGET=t85

OBJFILES=$(subst .c,.o,$(SOURCES))
TARGET=blinker
GCC=avr-gcc
OBJCPY=avr-objcopy
CFLAGS= -mmcu=$(MMCU) -Wall -Os


%.o : %.c
	$(GCC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

%.hex : %.elf
	$(OBJCPY) -j .text -j .data -O ihex $< $@

$(TARGET).elf : $(OBJFILES)
	$(GCC) $(CPPFLAGS) -o $(TARGET).elf $(OBJFILES)

build: $(TARGET).elf $(TARGET).hex

clean:
	rm -f $(OBJFILES) $(TARGET).hex $(TARGET).elf

flash:
	avrdude -p $(AVRDUDE_TARGET) -cusbasp -b4800 -e -U flash:w:$(TARGET).hex
