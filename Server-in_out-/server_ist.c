#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include "packet.h"
//#include "c0.h"

int listener_f();
char* writecommand_listener(struct packet pack, char* info, int* end_of_stream);
char* registrate(struct packet pack);
char* send_m(struct packet pack);
char* receive_m(struct packet pack);
int write_sending();
void addend_info(int num, char* info);


int port = 3425;

struct info_list* message;

 int main()
{
	listener_f();
}


int listener_f()
{
	int sock, listener;
	listener = socket(AF_INET, SOCK_STREAM, 0);
	if(listener < 0)
    {
        perror("socket");
        exit(1);
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port); //given number of port
    addr.sin_addr.s_addr = htonl(INADDR_ANY);  //address of local host
    //make listener  binded with addr address
    if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        exit(2);
    }
    //await for accepting
    listen(listener, 1);
    //variables fod user socket data
    	//void *addr, int *addrlen
    //accept requests all the time
    char buf[512];
    while(1)
    {
        sock = accept(listener, NULL, NULL);// addr, addrlen);
        printf("server, accept\n" );
        if(sock < 0)
        {
            perror("accept");
            exit(3);
        }
        int* end_of_stream; end_of_stream = malloc(sizeof(int*)); *end_of_stream = 0;//life=0
        char* info_from_command;
        struct packet pack;
        printf("%d\n", *end_of_stream);
        while(1&!(*end_of_stream))
        {
            recv(sock, buf, 512, 0);
            pack = create_packet(buf);
           	info_from_command = writecommand_listener(pack, info_from_command, end_of_stream);
            printf("new cycle yep %d\n",!( *end_of_stream));
        }
        int n;
        send(sock, packet_info( create_pack(SEND, 0,1, '1', TXT, info_from_command, "111\n", "111\n")), n, 0);
        close(sock);
    }

}


//return result of operation
char* writecommand_listener(struct packet pack, char* info, int* end_of_stream)
{
	 switch( pack.com )
            {
            	case REGS: 
            		*end_of_stream=1;
            		return registrate(pack);
            		break;
            	case SEND:
            		*end_of_stream=pack.end;
            		return send_m(pack);
            		break;
            	case RECV: 
            		*end_of_stream=1;
            		return receive_m(pack);//"$";
            		break;
            }

}


char* registrate(struct packet pack)
{
	printf("registrate\n");
   // __log("REGS + %s\n", pack.info);
	return "You've been registrated\n";
}

char* send_m(struct packet pack)
{

	if(pack.number==0) {
		struct info_list il;
		message = malloc(sizeof(struct info_list*));
		*message = il;
		(message->number )= 0; 
        message->part = malloc(sizeof(pack.info));
        strcpy((message->part),pack.info); 
        printf("wrong with 0 numb\n");
	}
	else
	{
        printf("will pe wrong with addend_info\n");
		addend_info(pack.number, pack.info);
        printf("wow, there are was no errors\n");
	}

	if (pack.end) 
	{
		printf("send %d\n", pack.number);
		printf("%s numb %d", message->part, message->number);
       // __log("SEND + %s\n", pack.info);
		if (write_sending())return "You've sent, its all right\n";
        else return "Smth went wrong\n";
	}
	else return "Not all parts was sent\n";
}

int write_sending()
{
    int counter=0;
    struct info_list* new = message;
    for(counter=0; new!=NULL; counter++)
    {
        if(counter!=new->number) return 0;
        // write_to_in, write_to_out message->part
        printf("%s\n", new->part);
        new = new->next;
    }
    return 1;
}

char* receive_m(struct packet pack)
{
	printf("receive\n");
    //__log("RCVV + %s\n", pack.info);

	//create returning info
    
	return "Receive what are you want \n";
}

void addend_info(int num, char* info)
{
	struct info_list* p; //= malloc(sizeof(struct info_list*));
	struct info_list* q = malloc(sizeof(struct info_list*));
	p = message;
	while(p->next!=NULL) p = p->next;
	q->number = num; q->part = info; q->next = NULL;
	p->next=q;
}
