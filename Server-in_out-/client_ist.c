#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h> 

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "packet.h"

char mess[] = "Hello there!\n";
char buf[512];

char* send_client(char* message,int mess_size, char* log_in, char* log_out);
char* receive_client(char* info, char* log_in);
char* registrate_client(char* user_data);
void open_sock(int* sock);
void close_sock(int* sock);

int main()
{
  //  int * sock = malloc(sizeof(int*));
   // open_sock(sock);
    char* buf; int fd;


    //send_client("Hello, dude\n","in login\0", "out login\0");
    if((fd=open("file",O_RDWR)) == 0)
    {
        printf("File error\n");
        return -1;
    }

    int file_size = lseek(fd, 0L, SEEK_END);
    lseek(fd, 0, SEEK_SET);
    buf = malloc(file_size*sizeof(char*));
    read(fd, buf, file_size);
    printf("Filesize = %d\n", file_size);
    buf = send_client(buf, file_size,//"Bye, dude\n",
        "in login\0", "out login\0");
    printf("%s\n",(create_packet(buf)).info);
    buf = receive_client(buf,//"pleeeeeeee\n", 
        "uset_id_in");
    printf("%s\n",(create_packet(buf)).info);
    return 0;
}

char* send_client(char* message, int size_mess, char* log_in, char* log_out)  //char* log_in , char* log_out
{
    int* sock = malloc(sizeof(int*));
    open_sock(sock);
    printf("send function\n");
    struct packet message_pack;
    char* buf = malloc(MESSAGE_SIZE*sizeof(char*));
    int start = 0; int i;
    //int size = strlen(message);
    printf("message size %d\n", size_mess);
    printf("bufer message created\n");
    for(i=0; (start+MESSAGE_SIZE)<size_mess; i++,start+=MESSAGE_SIZE )
    {
        memcpy(buf, message+start, MESSAGE_SIZE);
        message_pack = create_pack(SEND, i, 0, '1', TXT, buf, log_in, log_out );
        send(*sock, packet_info(message_pack), 512, 0);
         printf("bufer message created and sent\n");
    }
    
    memcpy(buf, message+start, size_mess-i*MESSAGE_SIZE);
    message_pack = create_pack(SEND, i, 1, '1', TXT, message, log_in, log_out );
    send(*sock, packet_info(message_pack), 512, 0);
    char* return_buf = malloc(512*sizeof(char*));
    recv(*sock, return_buf, 512, 0);
    close_sock(sock);
    free(sock);
    return return_buf;
}

char* receive_client(char* info, char* log_in)
{
    int* sock = malloc(sizeof(int*));
    open_sock(sock);

    struct packet message_pack;
    message_pack = create_pack(RECV, 0, 1, '1', TXT, "\n", log_in, "\n" );
    send(*sock, packet_info(message_pack), 512, 0);
    char* return_buf = malloc(512*sizeof(char*));
    recv(*sock, return_buf, 512, 0);
    close_sock(sock);
    free(sock);
    return return_buf;
}

char* registrate_client(char* user_data)
{
    int* sock = malloc(sizeof(int*));
    open_sock(sock);
    struct packet message_pack;
    message_pack = create_pack(REGS, 0, 1, '1', TXT, user_data, "111\n", "111\n" );
    send(*sock, packet_info(message_pack), 512, 0);

    char* return_buf = malloc(512*sizeof(char*));
    recv(*sock, return_buf, 512, 0);
    close_sock(sock);
    free(sock);
    return return_buf;
}

void open_sock(int* sock)
{
    struct sockaddr_in addr;

    *sock = socket(AF_INET, SOCK_STREAM, 0);
    if(*sock < 0)
    {
        perror("socket");
        exit(1);
    }
    addr.sin_family = AF_INET;
    addr.sin_port = htons(3425); // или любой другой порт...
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    if(connect(*sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("connect");
        exit(2);
    }
}

void close_sock(int* sock)
{
    close(*sock);
}