#include <iostream>     // For cerr and cout
#include <cstdlib>      // For atoi()
#include <sys/types.h>  // For data types
#include <sys/socket.h> // For socket(), connect(), send(), and recv()
#include <netdb.h>      // For gethostbyname()
#include <arpa/inet.h>  // For inet_addr()
#include <unistd.h>     // For close()
#include <netinet/in.h> // For sockaddr_in
#include <string.h>     // strcmp
#include <ctype.h>

#define BUFSIZE 256
using namespace std;

// GLOBAL VARIABLES
string frenchWords[5] = {"Bonjour", "Oui", "Non", "Temps", "Jour"};
string englishWords[5] = {"oHello", "oYes", "oNo", "oTime", "oDay"};
string englishWordsDisp[5] = {"Hello", "Yes", "No", "Time", "Day"};
string errorMessage = "No Matching Translation";

// FUNCTIONS
void print_buffer(string action, char buffer[], size_t bytes)
{
    printf("\033[1;36m%s: \033[1;36m%s (%ld bytes)\033[0m\n", action.c_str(), buffer, bytes);
}

/*
UDP Echo Server 
 */

int main(int argc, char const *argv[])
{
    // UDP SETUP
    addrinfo hnts, *micro_info;
    char buffer[BUFSIZE];
    memset(&hnts, 0, sizeof(addrinfo));
    hnts.ai_family = AF_INET;
    hnts.ai_socktype = SOCK_DGRAM;
    hnts.ai_flags = AI_PASSIVE;
    hnts.ai_protocol = 0;
    getaddrinfo(NULL, argv[1], &hnts, &micro_info);
    int micro_socket = socket(micro_info->ai_family, micro_info->ai_socktype, micro_info->ai_protocol);
    bind(micro_socket, micro_info->ai_addr, micro_info->ai_addrlen);
    printf("Translation Server Is \033[1;32m[Live]\033[0m on Port 3000\n");

    // UDP CONNECTION SETUP
    struct addrinfo hints, *server_info;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; // set to AF_INET6 to use IPv6
    hints.ai_socktype = SOCK_DGRAM;
    if ((getaddrinfo("127.0.0.1", "2000", &hints, &server_info)) != 0)
    {
        fprintf(stderr, "getaddrinfo() failed.\n");
        return 1;
    }

    // loop through all the results and make a socket
    int sockfd = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);

    size_t numbytes;

    while (1)
    {
        sockaddr_in *address = (sockaddr_in *)micro_info->ai_addr;
        char ip_str[INET_ADDRSTRLEN];
        inet_ntop(micro_info->ai_family, &(address->sin_addr), ip_str, INET_ADDRSTRLEN);
        memset(buffer, 0, sizeof(buffer));
        size_t recv_size = recvfrom(micro_socket, buffer, sizeof(buffer), 0, micro_info->ai_addr, &micro_info->ai_addrlen);
        printf("\n\033[1;32m[Success] \033[0mReceived (%ld bytes)\033[0m\n", recv_size);
        int switchV = 0;
        for (int i = 0; i < 5; i++)
        {
            if (buffer == englishWords[i])
            {
                printf("\n\033[1;33m[Match] \033[0mEnglish Word - \033[1;32m%s : \033[0mFrench Translation - \033[1;32m%s\033[0m\n", englishWordsDisp[i].c_str(), frenchWords[i].c_str());
                switchV = 1;
                if ((numbytes = sendto(sockfd, frenchWords[i].c_str(), strlen(frenchWords[i].c_str()), 0, server_info->ai_addr, server_info->ai_addrlen)) == -1)
                {
                    perror("sendto() failed");
                    exit(1);
                }
                printf("sent %d bytes to %s\n", numbytes, argv[1]);
                break;
            }
        }
        if (switchV != 1)
        {
            printf("\n\033[1;33m[No Match] \033[1;31mUnrecognized\033[0m\n");
            switchV = 0;
            if ((numbytes = sendto(sockfd, errorMessage.c_str(), strlen(errorMessage.c_str()), 0, server_info->ai_addr, server_info->ai_addrlen)) == -1)
            {
                perror("sendto() failed");
                exit(1);
            }
        }
    }
}
