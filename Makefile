# FLAGS = -Wall

# CC = gcc
# # Флаг для библиотеки pthread
# INCLUDES = -I include $(PTHREAD_FLAGS)
# PTHREAD_FLAGS = -lpthread
# # Путь к исполняемым файлам
# BIN_PATH = bin
# # Путь к объектным файлам
# OBJ_PATH = obj
# # Путь к исходным файлам
# SRC_PATH = src
# # Путь к заголовочным файлам
# INCLUDE_PATH = include

# SERVER = server
# SOCKET = socket
# CLIENT = client
# HANDLER = packet_handler

# SHELL = /bin/bash

EXEC_SERVER := server
EXEC_CLIENT := client
TARGET_SOCKET := socket
TARGET_HANDLER := packet_handler

CC := gcc
CFLAGS = -Wextra -pthread #-Wall
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
$(info SRC_HANDLER="$(SRC_HANDLER)")
$(info OBJ_HANDLER="$(OBJ_HANDLER)")
$(info SRC_SOCKET="$(SRC_SOCKET)")
$(info OBJ_SOCKET="$(OBJ_SOCKET)")


.PHONY: all dirs clean start_server start_client


all: dirs $(BIN_DIR)/$(EXEC_SERVER) $(BIN_DIR)/$(EXEC_CLIENT)

#------------------- Server

$(BIN_DIR)/$(EXEC_SERVER): $(OBJ_SERVER) $(OBJ_HANDLER) $(OBJ_SOCKET) 
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
		./$(BIN_DIR)/$(EXEC_CLIENT)

start_server:
		@clear
		./$(BIN_DIR)/$(EXEC_SERVER) $(filter-out $@,$(MAKECMDGOALS))
%:
		@:

clean:
		rm -rf $(BIN_DIR) $(OBJ_DIR)






















# all: dirs $(BIN_PATH)/$(SERVER) $(BIN_PATH)/$(CLIENT)

# $(BIN_PATH)/$(SERVER): $(OBJ_PATH)/$(SOCKET).o $(OBJ_PATH)/$(SERVER).o $(OBJ_PATH)/$(HANDLER).o $(OBJ_PATH)/$(SERVER)/main.o
# 	gcc $(FLAGS) -o $@ $^ $(PTHREAD_FLAGS)

# $(OBJ_PATH)/$(SOCKET).o: $(SRC_PATH)/$(SOCKET)/create_socket.c $(INCLUDE_PATH)/create_socket.h
# 	gcc $(FLAGS) -o $@ -c $(SRC_PATH)/$(SOCKET)/create_socket.c $(INCLUDES)

# $(OBJ_PATH)/$(HANDLER).o: $(SRC_PATH)/$(HANDLER)/packet_handler.c $(INCLUDE_PATH)/packet_handler.h 
# 	gcc $(FLAGS) -o $@ -c $(SRC_PATH)/$(HANDLER)/packet_handler.c  $(INCLUDES)

# $(OBJ_PATH)/$(HANDLER)/client_structs.o: $(OBJ_PATH)/$(HANDLER)/client_structs.c $(INCLUDE_PATH)/client_structs.h
# 	$(CC) $(FLAGS) -o $@ -c  $(SRC_PATH)/$(HANDLER)/client_structs.c
# #--------------------------------------------

# $(OBJ_PATH)/$(SERVER).o: $(SRC_PATH)/$(SERVER)/$(SERVER).c $(INCLUDE_PATH)/$(SERVER).h $(INCLUDE_PATH)/create_socket.h
# 	gcc $(FLAGS) -o $@ -c $(SRC_PATH)/$(SERVER)/$(SERVER).c $(INCLUDES)

# $(OBJ_PATH)/$(SERVER)/main.o: $(SRC_PATH)/$(SERVER)/main.c $(INCLUDE_PATH)/$(SERVER).h $(INCLUDE_PATH)/create_socket.h
# 	gcc $(FLAGS) -o $@ -c $(SRC_PATH)/$(SERVER)/main.c $(INCLUDES)

# #--------------------------------------------

# $(BIN_PATH)/$(CLIENT): $(OBJ_PATH)/$(SOCKET).o $(OBJ_PATH)/$(CLIENT).o $(OBJ_PATH)/$(HANDLER).o $(OBJ_PATH)/$(CLIENT)/main.o $(OBJ_PATH)/$(HANDLER)/client_structs.o
# 	gcc $(FLAGS) -o $@ $^

# $(OBJ_PATH)/$(CLIENT).o: $(SRC_PATH)/$(CLIENT)/$(CLIENT).c $(INCLUDE_PATH)/$(CLIENT).h $(INCLUDE_PATH)/create_socket.h
# 	gcc $(FLAGS) -o $@ -c $(SRC_PATH)/$(CLIENT)/$(CLIENT).c $(INCLUDES)

# $(OBJ_PATH)/$(CLIENT)/main.o: $(SRC_PATH)/$(CLIENT)/main.c $(INCLUDE_PATH)/$(CLIENT).h $(INCLUDE_PATH)/create_socket.h
# 	gcc $(FLAGS) -o $@ -c $(SRC_PATH)/$(CLIENT)/main.c $(INCLUDES)

# dirs:
# 	@mkdir -p $(BIN_PATH)
# 	@mkdir -p $(OBJ_PATH)/$(CLIENT)
# 	@mkdir -p $(OBJ_PATH)/$(SERVER)

# start_client:
# 	./$(BIN_PATH)/$(CLIENT)

# start_server:
# 	./$(BIN_PATH)/$(SERVER)

# clean:
# 	$(RM) -r $(BIN_PATH) $(OBJ_PATH)
