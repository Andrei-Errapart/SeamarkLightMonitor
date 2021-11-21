PRG            = lampboard
OBJ            = uart.o main.o gps.o
MCU_TARGET     = atmega88
OPTIMIZE       = -O3 
AVRDUDE		:= avrdude -p m88 -c pony-stk200 -P lpt1 -i 1

DEFS           =
LIBS           =

# You should not have to change anything below here.

CC             = avr-gcc

# Override is only needed by avr-lib build system.

override CFLAGS        = -g -Wall -Winline $(OPTIMIZE) -mmcu=$(MCU_TARGET) $(DEFS) $(EXTRA_FLAGS)
override LDFLAGS       = -Wl,-Map,$(PRG).map

OBJCOPY        = avr-objcopy
OBJDUMP        = avr-objdump

all: text

$(OBJ):	project.h

$(PRG).elf: $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -rf *.o $(PRG).elf *.bak 
	rm -rf *.lst *.map *.hex *.bin *.srec

dude:	all
	$(AVRDUDE) -U flash:w:"lampboard.hex":i

rdude:	all
	$(AVRDUDE) -U flash:r:"lampboard2.hex":i

reset:
	$(AVRDUDE) -n

# no fuse setting for atmega88 with 1.8432 MHz quartz.
fuse:
	$(AVRDUDE) -U hfuse:w:0xDF:m -U lfuse:w:0xEB:m

# READ FUSES
rfuse:
	$(AVRDUDE) -U hfuse:r:hfuse.bin:r -U lfuse:r:lfuse.bin:r

#lst:  $(PRG).lst

#%.lst: %.elf
#	$(OBJDUMP) -h -S $< > $@

# Rules for building the .text rom images

text: hex

hex:  $(PRG).hex
bin:  $(PRG).bin
srec: $(PRG).srec

%.hex: %.elf
	$(OBJCOPY) -j .text -j .data -O ihex $< $@


