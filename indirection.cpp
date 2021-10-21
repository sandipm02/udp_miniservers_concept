#include <iostream>		// For cerr and cout
#include <cstdlib>		// For atoi()
#include <sys/types.h>	// For data types
#include <sys/socket.h> // For socket(), connect(), send(), and recv()
#include <netdb.h>		// For gethostbyname()
#include <arpa/inet.h>	// For inet_addr()
#include <unistd.h>		// For close()
#include <netinet/in.h> // For sockaddr_in
#include <string.h>		// strcmp

// functions
#define BUFSIZE 256
using namespace std;

void print_buffer(string action, char buffer[], size_t bytes)
{
	printf("%s: %s (%ld bytes)\n", action.c_str(), buffer, bytes);
}

int main(int argc, char *argv[])
{

	// udp server
    addrinfo hints, *micro_info;
    char buffer[BUFSIZE];
    memset(&hints, 0, sizeof(addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_protocol = 0;
    getaddrinfo(NULL, argv[1], &hints, &micro_info);
    int micro_socket = socket(micro_info->ai_family, micro_info->ai_socktype, micro_info->ai_protocol);
    bind(micro_socket, micro_info->ai_addr, micro_info->ai_addrlen);
    printf("Indirection Server Is \033[1;32m[Live]\033[0m on Port 2000\n");




	// tcp server
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
		memset(client_message,0,sizeof(client_message));
		memset(client_message,0,strlen(client_message));
		if ((read_size = recv(new_socket, client_message, 2000, 0)) > 0)
		{
			if (client_message[0] == 'o')
			{
				// UDP SEND
				puts("Option 1 Selected -> Translator");
				struct addrinfo hints, *server_info;
				memset(&hints, 0, sizeof hints);
				hints.ai_family = AF_INET; // set to AF_INET6 to use IPv6
				hints.ai_socktype = SOCK_DGRAM;
				if ((getaddrinfo("127.0.0.1", "3000", &hints, &server_info)) != 0)
				{
					fprintf(stderr, "getaddrinfo() failed.\n");
					return 1;
				}
				int sockfd = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);
				size_t numbytes;
				if ((numbytes = sendto(sockfd, client_message, strlen(client_message), 0, server_info->ai_addr, server_info->ai_addrlen)) == -1)
				{
					perror("sendto() failed");
					exit(1);
				}
				printf("sent %d bytes to %s\n", numbytes, argv[1]);

				// UDP RECV
        		sockaddr_in *address = (sockaddr_in *)micro_info->ai_addr;
        		char ip_str[INET_ADDRSTRLEN];
        		inet_ntop(micro_info->ai_family, &(address->sin_addr), ip_str, INET_ADDRSTRLEN);
        		size_t recv_size = recvfrom(micro_socket, buffer, sizeof(buffer), 0, micro_info->ai_addr, &micro_info->ai_addrlen);
        		printf("\n\033[1;32m[Success] \033[0mReceived (%ld bytes)\033[0m\n", recv_size);
				close(sockfd);
				write(new_socket, buffer, strlen(buffer));
				bzero(buffer,256);
			}

			else if (client_message[0] == 't' && client_message[1] == 'w')
			{
				puts("Option 2 Selected -> Currency Convertor");
				write(new_socket, client_message, strlen(client_message));
			}
			else
			{
				puts("Option 3 Selected -> Voting");
				write(new_socket, client_message, strlen(client_message));
			}
		}
	}
	return 0;
}