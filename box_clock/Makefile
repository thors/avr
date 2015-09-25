SOURCES=src/box_clock.c

MMCU=attiny85
AVRDUDE_TARGET=t85

OBJFILES=$(subst .c,.o,$(SOURCES))
ASMFILES=$(subst .c,.o.s,$(SOURCES))
TARGET=box_clock
GCC=avr-gcc
OBJCPY=avr-objcopy
CFLAGS= -mmcu=$(MMCU) -DF_CPU=1000000 -Wall -O0 -g
CLINKFLAGS=-lm

%.o : %.c
	$(GCC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@
	$(GCC) -c $(CFLAGS) -S $(CPPFLAGS) $< -o $(@).s

%.hex : %.elf
	$(OBJCPY) -j .text -j .data -O ihex $< $@

$(TARGET).elf : $(OBJFILES)
	$(GCC) $(CFLAGS) $(CLINKFLAGS) -o $(TARGET).elf $(OBJFILES)

build: $(TARGET).elf $(TARGET).hex

clean:
	rm -f $(OBJFILES) $(ASMFILES) $(TARGET).hex $(TARGET).elf

flash:
	/home/tmoeller/bin/avrdude -p $(AVRDUDE_TARGET) -cusbasp -b4800 -e -U flash:w:$(TARGET).hex
