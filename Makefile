EXEC_SERVER := server
EXEC_CLIENT := client
TARGET_SOCKET := socket
TARGET_HANDLER := packet_handler

CC := gcc
CFLAGS = -Wall -Wextra -pthread
LDFLAGS = -lpthread#-ldl

SHELL = /bin/bash

# Bin
BIN_DIR = ./bin
# Obj
OBJ_DIR = ./obj
# Src
SRC_DIR = ./src
# Headers
INCLUDE_DIR = ./include

SRC_SERVER := $(wildcard $(SRC_DIR)/$(EXEC_SERVER)/*.c)
OBJ_SERVER := $(SRC_SERVER:$(SRC_DIR)/$(EXEC_SERVER)/%.c=$(OBJ_DIR)/$(EXEC_SERVER)/%.o)

SRC_CLIENT := $(wildcard $(SRC_DIR)/$(EXEC_CLIENT)/*.c)
OBJ_CLIENT := $(SRC_CLIENT:$(SRC_DIR)/$(EXEC_CLIENT)/%.c=$(OBJ_DIR)/$(EXEC_CLIENT)/%.o)

SRC_SOCKET := $(wildcard $(SRC_DIR)/$(TARGET_SOCKET)/*.c)
OBJ_SOCKET := $(SRC_SOCKET:$(SRC_DIR)/$(TARGET_SOCKET)/%.c=$(OBJ_DIR)/$(TARGET_SOCKET)/%.o)

SRC_HANDLER := $(wildcard $(SRC_DIR)/$(TARGET_HANDLER)/*.c)
OBJ_HANDLER := $(SRC_HANDLER:$(SRC_DIR)/$(TARGET_HANDLER)/%.c=$(OBJ_DIR)/$(TARGET_HANDLER)/%.o)

INCFLAG := $(addprefix -I,$(INCLUDE_DIR))

#$(info SRC_SERVER="$(SRC_SERVER)")
#$(info OBJ_SERVER="$(OBJ_SERVER)")
#$(info INCFLAG="$(INCFLAG)")
#$(info SRC_CLIENT="$(SRC_CLIENT)")
#$(info OBJ_CLIENT="$(OBJ_CLIENT)")
#$(info SRC_SOCKET="$(SRC_SOCKET)")
#$(info OBJ_SOCKET="$(OBJ_SOCKET)")
#$(info SRC_HANDLER="$(SRC_HANDLER)")
#$(info OBJ_HANDLER="$(OBJ_HANDLER)")


.PHONY: all dirs clean start_server start_client


all: dirs $(BIN_DIR)/$(EXEC_SERVER) $(BIN_DIR)/$(EXEC_CLIENT)

#------------------- Server

$(BIN_DIR)/$(EXEC_SERVER): $(OBJ_SERVER) $(OBJ_SOCKET) $(OBJ_HANDLER)
		$(CC) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/$(EXEC_SERVER)/%.o: $(SRC_DIR)/$(EXEC_SERVER)/%.c
		$(CC) $(CFLAGS) -c $< -o $@ $(INCFLAG)

#------------------- Client

$(BIN_DIR)/$(EXEC_CLIENT): $(OBJ_CLIENT) $(OBJ_SOCKET) $(OBJ_HANDLER)
		$(CC) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/$(EXEC_CLIENT)/%.o: $(SRC_DIR)/$(EXEC_CLIENT)/%.c
		$(CC) $(CFLAGS) -c $< -o $@ $(INCFLAG)

#------------------- Socket

$(OBJ_DIR)/$(TARGET_SOCKET)/%.o: $(SRC_DIR)/$(TARGET_SOCKET)/%.c
		$(CC) $(CFLAGS) -c $< -o $@ $(INCFLAG)

#------------------- Handler

$(OBJ_DIR)/$(TARGET_HANDLER)/%.o: $(SRC_DIR)/$(TARGET_HANDLER)/%.c
		$(CC) $(CFLAGS) -c $< -o $@ $(INCFLAG)

#-------------------


dirs:
		@mkdir -p $(BIN_DIR)
		@mkdir -p $(OBJ_DIR)/$(EXEC_SERVER)
		@mkdir -p $(OBJ_DIR)/$(EXEC_CLIENT)
		@mkdir -p $(OBJ_DIR)/$(TARGET_SOCKET)
		@mkdir -p $(OBJ_DIR)/$(TARGET_HANDLER)

start_client:
		$(BIN_DIR)/$(EXEC_CLIENT)

start_server:
		@clear
		$(BIN_DIR)/$(EXEC_SERVER) $(filter-out $@,$(MAKECMDGOALS))
%:
		@:

clean:
		rm -rf $(BIN_DIR) $(OBJ_DIR)
