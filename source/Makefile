# Created on: 2015. 2. 3.
#     Author: innocentevil


BASE_SRC_DIR=$(ROOT_DIR)/source
BASE_OBJ_DIR=$(BUILD_DIR)
BASE_INC_DIR=$(ROOT_DIR)/include

BASE_SRC=main.c\
		elf_parser.c

BASE_OBJ=$(BASE_SRC:%.c=$(BASE_OBJ_DIR)/%.o)
OBJS += $(BASE_OBJ)
INC += -I$(BASE_INC_DIR)

$(BASE_OBJ_DIR)/%.o: $(BASE_SRC_DIR)/%.c
	$(CC) -c $< -o $@  $(INC)

