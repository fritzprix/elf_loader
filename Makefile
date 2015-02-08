#Makefile
# Created on: 2015. 2. 3.
#     Author: innocentevil



ifeq ($(BUILD),)
	BUILD=Debug
endif

CC=gcc
CPP=g++
MK=mkdir


ROOT_DIR = $(CURDIR)
BUILD_DIR = $(ROOT_DIR)/$(BUILD)

TARGET=$(BUILD_DIR)/parser.exe

include $(ROOT_DIR)/source/Makefile

all : $(BUILD_DIR) $(TARGET)

$(BUILD_DIR):
	$(MK) $@
	
	
$(TARGET): $(OBJS) 
	$(CC) -o $@  $(INC)  $(OBJS) 
	
clean :
	rm -rf $(TARGET) $(OBJS)