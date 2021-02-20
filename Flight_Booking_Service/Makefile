all: server agent 

server: server.c
	gcc -Wall -g server.c -o server
client: client.c
	gcc -Wall -g agent.c -o agent

clean:
	rm agent server
