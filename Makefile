HEAD = -I includes

.PHONY: all server client clean start_server start_client

all: bin/server bin/client

server: bin/server

client: bin/client

bin/server: obj/socket/socket.o obj/server/server.o obj/server/main.o
	mkdir -p bin
	gcc -Wall -o $@ $^

obj/socket/socket.o: src/socket/create_socket.c includes/create_socket.h
	mkdir -p obj
	mkdir -p obj/socket
	gcc -Wall -o $@ -c src/socket/create_socket.c $(HEAD) -lpthread
#--------------------------------------------

obj/server/server.o: src/server/server.c includes/server.h includes/create_socket.h
	mkdir -p obj/server
	gcc -Wall -o $@ -c src/server/server.c $(HEAD) -lpthread

obj/server/main.o: src/server/main.c includes/server.h includes/create_socket.h
	gcc -Wall -o $@ -c src/server/main.c $(HEAD) -lpthread
#--------------------------------------------

bin/client: obj/socket/socket.o obj/client/client.o obj/client/main.o
	mkdir -p bin
	gcc -Wall -o $@ $^

obj/client/client.o: src/client/client.c includes/client.h includes/create_socket.h
	mkdir -p obj/client
	gcc -Wall -o $@ -c src/client/client.c $(HEAD) -lpthread

obj/client/main.o: src/client/main.c includes/client.h includes/create_socket.h
	gcc -Wall -o $@ -c src/client/main.c $(HEAD) -lpthread
#--------------------------------------------

clean:
	rm -rf obj bin

start_client:
	./bin/client

start_server:
	./bin/server
