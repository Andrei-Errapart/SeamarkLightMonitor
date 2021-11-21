PRG		:= 6ch_Prose_Ctrl
SRC		:= main.cxx usart.cxx
MCU_TARGET	:= atmega8
OPTIMIZE	:= -O2 

DEFS		:= -DF_CPU=11059200UL
LIBS		:=

# You should not have to change anything below here.
OBJ		:= $(SRC:.cxx=.o)
CC		:= avr-gcc
CXX		:= avr-g++
OBJCOPY		:= avr-objcopy
OBJDUMP		:= avr-objdump

CFLAGS		:= -g -Wall $(OPTIMIZE) -mmcu=$(MCU_TARGET) $(DEFS) -I.
LDFLAGS		:= #-Wl,-Map,$(PRG).map

# Targets (default).
all:	$(PRG).hex

# Object compilation.
%.o:	%.cxx
	$(CXX) $(CFLAGS) -c -o $@ $<

$(PRG).elf: $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

%.hex: %.elf
	$(OBJCOPY) -j .text -j .data -O ihex $< $@

# CLEAN
clean:
	rm -rf *.o $(PRG).elf *.bak 
	rm -rf *.lst *.map *.hex *.bin *.srec

