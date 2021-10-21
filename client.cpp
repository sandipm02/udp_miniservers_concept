#include <stdio.h>
#include <string.h> //strlen
#include <stdlib.h> //strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>    //write
#include <iostream>

using namespace std;

//FUNCTION DECLAR.
void createSocket();
void menu();

//GLOBAL VARS
int socket_desc;
struct sockaddr_in server;
char *serverResp[2000];
string inputOption;

int main(int argc, char *argv[])
{
    while (1)
    {
        // Init Program
        menu();
        printf("\033[0mPlease Input a Menu Option: ");
        cin >> inputOption;
        while (inputOption != "1" && inputOption != "2" && inputOption != "3" && inputOption != "4")
        {
            printf("\033[0mInvalid Menu Option: ");
            cin >> inputOption;
        }

        // Options
        if (inputOption == "4")
        {
            exit(1);
        }


        if (inputOption == "1")
        {
            string word;
            string num = "o";
            printf("\nEnter an English word: ");
            cin >> word;
            word = num + word;
            const char *str = word.c_str();
            createSocket();
            if (send(socket_desc, str, strlen(str), 0) < 0)
            {
                puts("\033[1;31mRequest Send Failed\033[0m\n");
                exit(1);
            }
            else
            {
                puts("\033[1;32mRequest Sent Succesfully\033[0m");
            }
            if (recv(socket_desc, serverResp, 2000, 0) < 0)
            {
                exit(1);
            }
            printf("French translation: %s\n", serverResp);
        }



        if (inputOption == "2")
        {
            char *two = "2";
            createSocket();
            if (send(socket_desc, two, strlen(two), 0) < 0)
            {
                puts("\033[1;31mRequest Send Failed\033[0m\n");
                exit(1);
            }
            else
            {
                puts("\033[1;32mRequest Sent Succesfully\033[0m");
            }
            if (recv(socket_desc, serverResp, 2000, 0) < 0)
            {
                exit(1);
            }
            puts("Reply received\n");
            printf("%s", serverResp);
        }



        if (inputOption == "3")
        {
            char *thr = "thr";
            createSocket();
            if (send(socket_desc, thr, strlen(thr), 0) < 0)
            {
                puts("\033[1;31mRequest Send Failed\033[0m\n");
                exit(1);
            }
            else
            {
                puts("\033[1;32mRequest Sent Succesfully\033[0m");
            }
            if (recv(socket_desc, serverResp, 2000, 0) < 0)
            {
                exit(1);
            }
            puts("Reply received\n");
            printf("%s", serverResp);
        }
    }

    
    return 0;
}

void createSocket()
{
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(8888);
    if (connect(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        exit(1);
    }
}

void menu()
{
    printf("\n\033[1;36m+++++++ \033[1;37mServices \033[1;36m+++++++\n");
    printf("\033[0m(1) \033[1;32mTranslator\033[0m\n");
    printf("\033[0m(2) \033[1;32mCurrency Convertor\033[0m\n");
    printf("\033[0m(3) \033[1;32mVoting\033[0m\n");
    printf("\033[0m(4) \033[1;31mExit\033[0m\n\n");
}