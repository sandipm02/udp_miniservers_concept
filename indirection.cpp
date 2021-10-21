#include <stdio.h>
#include <string.h> //strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>	   //write

int main(int argc, char *argv[])
{
	int socket_desc, new_socket, c, read_size;
	struct sockaddr_in server, client;
	char *message;
	char client_message[2000];
	char *server_reply;

	//Create socket
	socket_desc = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_desc == -1)
	{
		printf("Could not create socket");
	}
	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(8888);
	//Bind
	if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		puts("bind failed");
		return 1;
	}
	puts("bind done");
	//Listen
	listen(socket_desc, 3);
	//Accept and incoming connection
	puts("Waiting for incoming connections...");
	c = sizeof(struct sockaddr_in);
	while (new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&c))
	{
		puts("Connection accepted");
		if ((read_size = recv(new_socket, client_message, 2000, 0)) > 0)
		{
			if(client_message[0] == 'o') {
				puts("Option 1 Selected -> Translator");
				write(new_socket, client_message, strlen(client_message));	
			}
			else if(client_message[0] == 't' && client_message[1] =='w') {
				puts("Option 2 Selected -> Currency Convertor");
				write(new_socket, client_message, strlen(client_message));	
			}
			else {
				puts("Option 3 Selected -> Voting");
				write(new_socket, client_message, strlen(client_message));	
			}
		}
	}
	return 0;
}