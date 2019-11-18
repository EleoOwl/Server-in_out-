#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h> 
#include "packet.h"

#define buf_len 512



int cont(char del, char* delimiters)
{
	while(*delimiters!='\0'){
		if (*delimiters==del) return 1;
		delimiters++;
	}
	return 0;}

char **split_n(char*str, char*delimiters, int* colv)
{
	char *pch;
	char** result;
	int n = strlen(str);
	int col = 1;
	for(int i=0; i<n; i++)
	{
		if( cont(str[i], delimiters)) col++;
	}
	result = malloc(col*sizeof(char*));
	n=0;
	*colv=col;
	pch = strtok (str, delimiters);
	while(pch!=NULL)
	{
		result[n] = pch;
		pch = strtok (NULL, delimiters);
		n++;
	}

	return result;}




 //create an packet for sending it to user
struct packet create_packet(char* buf)
{
	struct packet pack; //char* sp = "@";
	int n=0;
	char** buf_ch = split_n(buf, "@", &n);
	if(n<8) return pack;
	
	for(int i=0;i<3;i++) if(strcmp(buf_ch[0], commands[i])==0) pack.com = i;
	pack.number = atoi(buf_ch[1]);
	pack.end = atoi( buf_ch[2]);
	pack.signature = *buf_ch[3];//atoi(buf_ch[3]);

	//pack.format_t = buf_ch[4];
	for(int i=0;i<3;i++) if(strcmp(buf_ch[0], formats[i])==0) pack.format_t = i;
	pack.info = buf_ch[5];
	pack.log_in = buf_ch[6]; //atoi(buf_ch[6]);
	pack.log_out = buf_ch[7]; //atoi(buf_ch[7]);
	return pack;
}

struct packet create_pack(enum command com, int n, int end,
							 //int signature,
							 unsigned char signature,
							 enum format f_t, char* info, 
							// int id_in, 
							 char* log_in,
							 char* log_out
							// int id_out
							 )
{
	struct packet pack;
	pack.number = n;
	pack.end = end;
	pack.signature = signature;
	pack.com = com;
	pack.info = info;
	pack.format_t = f_t;
	//pack.id_in =  id_in;
	//pack.id_out =  id_out;
	pack.log_in = log_in; 
	pack.log_out = log_out;
	return pack;
}

char* packet_info(struct packet pack)
{
	char* info;
	info = (char *)malloc(buf_len * sizeof(char)); 
	sprintf(info, "%4.4s@%1d@%01d@%3.3s@%-128c@%-305.305s@%-32s@%-32s", commands[pack.com], pack.number, pack.end, formats[pack.format_t],
													   pack.signature,
													   pack.info, 
													   //pack.id_in, pack.id_out 
													   pack.log_in, pack.log_out
													   );

	return info;
}











