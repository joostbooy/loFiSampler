# ------------------------------------------------
#  Makefile for stm32
#
# all 		: builds the application
# clean 	: deletes the build
#	erase		:	mass erase mcu
# table 	: makes lookupTable source & header files
# size 		: size of the build
# flash 	: upload firmware with st-link
#
# ------------------------------------------------


TARGET = LoFiSampler
OPT = -O2	#-Og

BUILD_DIR = Build

LIB_DIR = /Users/joostbooy/Desktop/Code/stm32/lib
STM_REPO = /Users/joostbooy/STM32Cube/Repository/STM32Cube_FW_F7_V1.16.2
UTILS_DIR = $(LIB_DIR)/utils
FATFS_LIB = $(LIB_DIR)/ff14b/source
USB_DIR = $(LIB_DIR)/tinyusb
UI_DIR = Src/Ui
ENGINE_DIR = Src/Engine
SETTINGS_DIR = Src/Settings
TABLE_DIR = LookupTables
FILESYSTEM_DIR = Src/Disk


#######################################
# model specifics
#######################################

MODEL_DEF = -DSTM32F767xx
F_CPU = 216000000UL

CMSIS_DIR = $(STM_REPO)/Drivers/CMSIS
HAL_DIR = $(STM_REPO)/Drivers/STM32F7xx_HAL_Driver
DEVICE_DIR = $(CMSIS_DIR)/Device/ST/STM32F7xx

CPU = -mcpu=cortex-m7
LDSCRIPT = stm32/STM32F767ZGTx_FLASH.ld
STARTUP = stm32/startup_stm32f767xx.s
SYSTEM = $(DEVICE_DIR)/Source/Templates/system_stm32f7xx.c

FPU = -mfpu=fpv5-d16
FLOAT_ABI = -mfloat-abi=hard


#*************************************************************************
#
#	SHOULD NOT NEED CHANGE
#
#*************************************************************************

######################################
# sources
######################################

# C sources
C_SOURCES =  \
$(wildcard Src/*.c) \
$(wildcard Drivers/*.c) \
$(wildcard Drivers/Usb/*.c) \
$(wildcard $(USB_DIR)/hw/*.c) \
$(wildcard $(USB_DIR)/src/*.c) \
$(wildcard $(USB_DIR)/src/common/*.c) \
$(wildcard $(USB_DIR)/src/device/*.c) \
$(wildcard $(USB_DIR)/src/class/midi/*.c) \
$(wildcard $(USB_DIR)/src/portable/synopsys/dwc2/*.c) \
$(wildcard $(USB_DIR)/src/portable/st/stm32_fsdev/*.c) \
$(wildcard $(FATFS_LIB)/*.c) \
$(filter-out $(wildcard $(HAL_DIR)/Src/*template*), $(wildcard $(HAL_DIR)/Src/*.c)) \
$(SYSTEM)

# CPP sources
CPP_SOURCES = \
$(wildcard Src/*.cpp) \
$(wildcard Drivers/*.cpp) \
$(wildcard Drivers/Usb/*.cpp) \
$(wildcard LookupTables/*.cpp) \
$(wildcard $(ENGINE_DIR)/*.cpp) \
$(wildcard $(SETTINGS_DIR)/*.cpp) \
$(wildcard $(UTILS_DIR)/*.cpp) \
$(wildcard $(UI_DIR)/*.cpp) \
$(wildcard $(UI_DIR)/Pages/*.cpp) \
$(wildcard $(UI_DIR)/Painters/*.cpp) \
$(wildcard $(UI_DIR)/Lists/*.cpp) \
$(wildcard $(FILESYSTEM_DIR)/*.cpp)
#$(wildcard $(FATFS_LIB)/*.cpp)

# ASM sources
ASM_SOURCES =  \
$(STARTUP)

#######################################
# binaries
#######################################
CC = arm-none-eabi-gcc
CXX = arm-none-eabi-g++
AS = arm-none-eabi-gcc -x assembler-with-cpp
CP = arm-none-eabi-objcopy
SZ = arm-none-eabi-size
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

#######################################
# DEFINES
#######################################
# AS defines
AS_DEFS =

# C defines
C_DEFS =  \
$(MODEL_DEF) \
-DF_CPU=$(F_CPU) \
-DUSE_HAL_DRIVER

#######################################
# INCLUDES
#######################################
# AS includes
AS_INCLUDES =

# C includes
C_INCLUDES = \
-ISrc \
-IDrivers \
-IDrivers/Usb \
-IlookupTables \
-Istm32 \
-I$(HAL_DIR)/Inc \
-I$(HAL_DIR)/Inc/Legacy \
-I$(DEVICE_DIR)/Include \
-I$(CMSIS_DIR)/Include \
-I$(ENGINE_DIR) \
-I$(SETTINGS_DIR) \
-I$(UTILS_DIR) \
-I$(TABLE_DIR) \
-I$(UI_DIR) \
-I$(UI_DIR)/Pages \
-I$(UI_DIR)/Painters \
-I$(UI_DIR)/Lists \
-I$(USB_DIR)/hw \
-I$(USB_DIR)/src \
-I$(USB_DIR)/src/device \
-I$(USB_DIR)/src/class/midi \
-I$(USB_DIR)/src/portable/synopsys/dwc2 \
-I$(USB_DIR)/src/portable/st/stm32_fsdev \
-I$(FATFS_LIB) \
-I$(FILESYSTEM_DIR)


#######################################
# FLAGS
#######################################
# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT_ABI)
# -mthumb-interwork

# ASM flags
ASFLAGS = \
$(MCU) \
$(AS_DEFS) \
$(AS_INCLUDES) \
$(OPT) \
-g -Wall -Wdouble-promotion \
-fdata-sections \
-ffunction-sections \
-Wno-strict-aliasing

# C flags
CFLAGS = \
$(MCU) \
$(C_DEFS) \
$(C_INCLUDES) \
$(OPT) \
-g -c -Wall -Wdouble-promotion -Wno-strict-aliasing \
-fdata-sections \
-ffunction-sections \
-fshort-enums \
-finline \
-finline-functions-called-once \
-funroll-loops \
-fno-unwind-tables \
-fno-move-loop-invariants \
-MMD -MP -MF"$(@:%.o=%.d)"
#-fasm
#-flto
#-gdwarf-2

# CPP flags
CPPFLAGS = \
$(CFLAGS) \
-fno-exceptions \
-fno-rtti

# linker flags
LDFLAGS = \
$(MCU) \
-specs=nano.specs \
-T$(LDSCRIPT) -lc -lm -lnosys \
-Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref \
-Wl,--gc-sections \
-Wl,--print-memory-usage \
#-lstdc++
#-flto
# lrdimon



#######################################
# build the application
#######################################
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin

# list of C objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of CPP objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(CPP_SOURCES:.cpp=.o)))
vpath %.cpp $(sort $(dir $(CPP_SOURCES)))
# list of ASM objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

#	@$(CC) $(CFLAGS) -S $< -o $(BUILD_DIR)/$*.asm

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	@$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@
	@echo "$<"

#	@$(CC) $(CPPFLAGS) -S $< -o $(BUILD_DIR)/$*.asm

$(BUILD_DIR)/%.o: %.cpp Makefile | $(BUILD_DIR)
	@$(CXX) -c $(CPPFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.cpp=.lst)) $< -o $@
	@echo "$<"

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	@$(AS) -c $(CFLAGS) $< -o $@
	@echo "$<"

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	@echo "-------------------------SIZE-------------------------"
	@$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	@echo "------------------------------------------------------"
	@echo "$@"

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	@$(HEX) $< $@
	@echo "$@"

$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	@$(BIN) $< $@
	@echo "$@"

$(BUILD_DIR):
	mkdir $@

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)

#######################################
# mass erase
#######################################
erase:
	st-flash erase

#######################################
# flash firmware
#######################################
flash:
	st-flash write $(BUILD_DIR)/$(TARGET).bin 0x8000000

#######################################
# code size
#######################################
size:
	$(SZ) $(BUILD_DIR)/$(TARGET).elf

#######################################
# probe
#######################################
probe:
	st-info --probe

#######################################
# lookup table creation
#######################################
table:
	@echo "\n ---creating tables---"
	python3 $(TABLE_DIR)/lookupTables.py $(TABLE_DIR)/
	@echo "---tables finished--- \n"

#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)
