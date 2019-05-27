all: client server

client: p3client.c
		gcc -o p3client p3client.c

server: p3server.c
		gcc -o p3server p3server.c	