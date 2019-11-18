enum format { SMS, TXT, COM, JPG};
enum command {REGS, SEND, RECV};
enum errors {
			ololo, lolol
};

struct packet create_packet(char* buf);
char* packet_info(struct packet pack);
struct packet create_pack(enum command com, int n, int end, unsigned char signature,enum format f_t, char* info, char* id_in, char* id_out);

static int MESSAGE_SIZE = 305;
static char *formats[] = {
							"SMS",
							"TXT",
							"COM",
							"JPG"
						};

static char *commands[] = {
							"REGS",
							"SEND",
							"RECV"
						};

static char* errors[] = {

};

struct packet
{
	enum command com;
	int number;
	int end;
	enum format format_t;
	//unsigned long signature;
	// int signature;//unsigned char 128	char* info
	// int id_in;// char* 32
	// int id_out;// char*
	char* info;
	unsigned char signature;
	char* log_in;
	char* log_out;
};
struct info_list
{
	int number;
	char* part;
	struct info_list* next;
};