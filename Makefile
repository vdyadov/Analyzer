FLAGS = -Wall

CC = gcc
# Флаг для библиотеки pthread
INCLUDES = -I include $(PTHREAD_FLAGS)
PTHREAD_FLAGS = -lpthread
# Путь к исполняемым файлам
BIN_PATH = bin
# Путь к объектным файлам
OBJ_PATH = obj
# Путь к исходным файлам
SRC_PATH = src
# Путь к заголовочным файлам
INCLUDE_PATH = include

SERVER = server
SOCKET = socket
CLIENT = client

SHELL = /bin/bash

all: dirs $(BIN_PATH)/$(SERVER) $(BIN_PATH)/$(CLIENT)

$(BIN_PATH)/$(SERVER): $(OBJ_PATH)/$(SOCKET).o $(OBJ_PATH)/$(SERVER).o $(OBJ_PATH)/$(SERVER)/main.o
	gcc $(FLAGS) -o $@ $^ $(PTHREAD_FLAGS)

$(OBJ_PATH)/$(SOCKET).o: $(SRC_PATH)/$(SOCKET)/create_socket.c $(INCLUDE_PATH)/create_socket.h
	gcc $(FLAGS) -o $@ -c $(SRC_PATH)/$(SOCKET)/create_socket.c $(INCLUDES)
#--------------------------------------------

$(OBJ_PATH)/$(SERVER).o: $(SRC_PATH)/$(SERVER)/$(SERVER).c $(INCLUDE_PATH)/$(SERVER).h $(INCLUDE_PATH)/create_socket.h
	gcc $(FLAGS) -o $@ -c $(SRC_PATH)/$(SERVER)/$(SERVER).c $(INCLUDES)

$(OBJ_PATH)/$(SERVER)/main.o: $(SRC_PATH)/$(SERVER)/main.c $(INCLUDE_PATH)/$(SERVER).h $(INCLUDE_PATH)/create_socket.h
	gcc $(FLAGS) -o $@ -c $(SRC_PATH)/$(SERVER)/main.c $(INCLUDES)
#--------------------------------------------

$(BIN_PATH)/$(CLIENT): $(OBJ_PATH)/$(SOCKET).o $(OBJ_PATH)/$(CLIENT).o $(OBJ_PATH)/$(CLIENT)/main.o
	gcc $(FLAGS) -o $@ $^

$(OBJ_PATH)/$(CLIENT).o: $(SRC_PATH)/$(CLIENT)/$(CLIENT).c $(INCLUDE_PATH)/$(CLIENT).h $(INCLUDE_PATH)/create_socket.h
	gcc $(FLAGS) -o $@ -c $(SRC_PATH)/$(CLIENT)/$(CLIENT).c $(INCLUDES)

$(OBJ_PATH)/$(CLIENT)/main.o: $(SRC_PATH)/$(CLIENT)/main.c $(INCLUDE_PATH)/$(CLIENT).h $(INCLUDE_PATH)/create_socket.h
	gcc $(FLAGS) -o $@ -c $(SRC_PATH)/$(CLIENT)/main.c $(INCLUDES)

dirs:
	@mkdir -p $(BIN_PATH)
	@mkdir -p $(OBJ_PATH)/$(CLIENT)
	@mkdir -p $(OBJ_PATH)/$(SERVER)

start_client:
	./$(BIN_PATH)/$(CLIENT)

start_server:
	./$(BIN_PATH)/$(SERVER)

clean:
	$(RM) -r $(BIN_PATH) $(OBJ_PATH)
