TARGET = main

######################################
# building variables
######################################
DEBUG = 1
NOWARN = 1
OPT = -Og

#######################################
# paths
#######################################
BUILD_DIR = build
INSTALL_DIR = /opt/ti/msp430-gcc
SUPPORT_FILE_DIRECTORY = $(INSTALL_DIR)/include

C_SOURCES = \
src/main.c \
driverlib/wdt_a.c \
driverlib/usci_b_spi.c \
driverlib/usci_b_i2c.c \
driverlib/usci_a_uart.c \
driverlib/usci_a_spi.c \
driverlib/ucs.c \
driverlib/tlv.c \
driverlib/timer_d.c \
driverlib/timer_b.c \
driverlib/timer_a.c \
driverlib/tec.c \
driverlib/sysctl.c \
driverlib/sfr.c \
driverlib/sd24_b.c \
driverlib/rtc_c.c \
driverlib/rtc_b.c \
driverlib/rtc_a.c \
driverlib/ref.c \
driverlib/ram.c \
driverlib/pmm.c \
driverlib/pmap.c \
driverlib/oa.c \
driverlib/mpy32.c \
driverlib/ldopwr.c \
driverlib/lcd_c.c \
driverlib/lcd_b.c \
driverlib/gpio.c \
driverlib/flashctl.c \
driverlib/eusci_b_spi.c \
driverlib/eusci_b_i2c.c \
driverlib/eusci_a_uart.c \
driverlib/eusci_a_spi.c \
driverlib/dma.c \
driverlib/dac12_a.c \
driverlib/ctsd16.c \
driverlib/crc.c \
driverlib/comp_b.c \
driverlib/battbak.c \
driverlib/aes.c \
driverlib/adc12_a.c \
driverlib/adc10_a.c


#######################################
# CFLAGS
#######################################
DEVICE  = msp430f5529
MCU = -mmcu=$(DEVICE)

# for MSP430 CPU generation 1 and 2
CPU_GEN = -mlarge -mcode-region=either -mdata-region=lower


C_DEFS =
# -D

C_INCLUDES = \
-I$(SUPPORT_FILE_DIRECTORY) \
-Idriverlib \
-Isrc

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) $(CPU_GEN) -Wall

ifeq ($(DEBUG), 1)
CFLAGS += -g
endif

ifeq ($(NOWARN), 1)
CFLAGS += -w
endif


#######################################
# LDFLAGS
#######################################
LDSCRIPT = $(DEVICE).ld

LIBS =
LIBDIRS = \
-L$(SUPPORT_FILE_DIRECTORY)

LDFLAGS = $(MCU) $(CPU_GEN) $(LIBDIRS) -T$(DEVICE).ld -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--gc-sections


#######################################
# binaries
#######################################
PREFIX  = msp430-elf-
ifdef GCC_PATH
CC  = $(GCC_PATH)/$(PREFIX)gcc
AS  = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
GDB = $(GCC_PATH)/$(PREFIX)gdb
CP  = $(GCC_PATH)/$(PREFIX)objcopy
SZ  = $(GCC_PATH)/$(PREFIX)size
else
CC  = $(INSTALL_DIR)/bin/$(PREFIX)gcc
AS  = $(INSTALL_DIR)/bin/$(PREFIX)gcc -x assembler-with-cpp
GDB = $(INSTALL_DIR)/bin/$(PREFIX)gdb
CP  = $(INSTALL_DIR)/bin/$(PREFIX)objcopy
SZ  = $(INSTALL_DIR)/bin/$(PREFIX)size
endif
HEX = $(CP) -O ihex

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex



#######################################
# build the application
#######################################
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@

$(BUILD_DIR):
	mkdir $@

clean:
	-rm -fR $(BUILD_DIR)

debug: all
	$(GDB) $(BUILD_DIR)/$(TARGET).elf

prog: all
	LD_LIBRARY_PATH=$(INSTALL_DIR)/bin mspdebug tilib "prog $(BUILD_DIR)/$(TARGET).hex"