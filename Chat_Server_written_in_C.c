*
 ============================================================================
 Name        : chatServer.c
 Author      : Emin Yuce 120060023
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <malloc.h>
#include <semaphore.h>
#include <sched.h>
#include <fcntl.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <inttypes.h>

#define PORT 5000
#define MAX_LINE 1024
#define ERR_OPEN_1 11
#define ERR_READ 13
#define ERR_WRITE 14
#define CHAT 2
#define IS_ACCEPTED 1
#define EXIT 3
#define CHAT_END 6
#define CHAT_BEGIN 5

void * Thread_Run();

typedef struct Thr_Data {
	int connected;
	pthread_t th;
} thdata;

struct stack_ {

	int id;
	char * name;
	char * password;
	char * aktif;
	int del;
	struct stack_ *nextPtr;

};
pthread_mutex_t job_queue_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t job_queue_delete = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t conversation = PTHREAD_MUTEX_INITIALIZER;

typedef struct stack_ stack;

int number_Of_Conservation = 0;

stack *header;
char * messageDelimeters = ";";

char A[MAX_LINE][MAX_LINE]; //register book

void push(int id, char * name, char * password) {
	stack *newNode;
	//bellekten bi yer alıyoruz
	newNode = (stack *) malloc(sizeof(stack));
	//İçine sayımızı koyouruz
	newNode->id = id;
	newNode->name = name;
	newNode->password = password;
	newNode->del = 0;
	newNode->aktif = 0;
	//Stackın ilk pointeri bi şey tutuyo mu diye bakıyoruz
	if (header == NULL) {
		//tutmuyosa ilk elemanı tutsun
		header = newNode;
		newNode->nextPtr = NULL;
	} else {
		//tutyosa yeni eklenen bi sonrakine eklensin ve sonra diğerini tutsun
		newNode->nextPtr = header;
		header = newNode;
	}
}
int isEmpty() {
	if (header == NULL)
		return 1;
	else
		return 0;
}
int pop() {
	int k;

	stack *holder;

	if (isEmpty()) {
		printf("The stack is not formed\n");
	}

	holder = header;
	header = header->nextPtr;
	k = holder->id;
	free(holder);
	return k;

}
void deleteNode(int num) {

	stack *hold, *taken;
	hold = header;
	int flag = 0;
	if (!isExist(num)) {
		printf("The number you entered is not found\n");
		fflush(stdout);
		flag = 2;
	}
	if (flag == 0) {
		if (header->id == num) {
			header->del = 1;
			flag = 1;

		}

		while (header != NULL && flag == 0) {
			if (header->nextPtr != NULL) {
				if (header->nextPtr->id != num) {
					header = header->nextPtr;
				} else {
					taken = header->nextPtr;
					if (header->nextPtr->nextPtr != NULL) {
						header->nextPtr = header->nextPtr->nextPtr;
						taken->del = 1;
						break;
					} else {
						taken->del = 1;
						break;
					}
				}
			}
		}
	}
}
int isExist(int num) {

	stack *hold;
	hold = header;

	while (header != NULL) {

		if (header->id != num) {
			header = header->nextPtr;

		} else {
			header = hold;
			return 1;

		}
	}

	header = hold;

	return 0;
}
int howMany() {

	stack *hold;
	hold = header;
	int i = 0;
	while (header != NULL) {
		//printf("socket id %d name %s\n",header->id,header->name);
		header = header->nextPtr;
		i++;
	}

	header = hold;

	return i;
}
int SendOnlineUser() {

	stack * hold;
	hold = header;
	char str[5];
	char a[250];
	int i = 0;
	while (header != NULL) {
		if (header->del == 0) {
			sprintf(str, "%d", header->id);
			strcat(a, str);
			strcat(a, "-");
			strcat(a, header->name);
			strcat(a, "-");
			strcat(a, header->password);
			strcat(a, "/");
		} else {
			close(header->id);
		}
		header = header->nextPtr;
	}
	header = hold;
	printf("Online olanlar %s\n", a);
	fflush(stdout);

	BroadCastMessage(a);

	return i;
}
int BroadCastMessage(char * send_data) {
	stack * hold;
	hold = header;
	while (header != NULL) {
		sendMessage(header->id, send_data, "4", "clients", "server");
		header = header->nextPtr;
	}
	header = hold;
	return 0;
}
stack * SearchSocket(int socket) {
	stack * hold, *temp;
	hold = header;
	while (header != NULL) {

		if (header->id == socket) {
			temp = header;
		}
		header = header->nextPtr;
	}
	header = hold;

	return temp;
}
int f_err_report(int mode) {
	if (mode == ERR_OPEN_1)
		printf("error opening file1n");
	else if (mode == ERR_READ)
		printf("error reading from file");
	return 0;
}
int makeargv(const char *s, const char *delimiters, char ***argvp) {
	int error;
	int i;
	int numtokens;
	const char *snew;
	char *t;

	if ((s == NULL) || (delimiters == NULL) || (argvp == NULL)) {
		errno = EINVAL;
		return -1;
	}
	*argvp = NULL;
	snew = s + strspn(s, delimiters);
	if ((t = malloc(strlen(snew) + 1)) == NULL)
		return -1;
	strcpy(t, snew);
	numtokens = 0;
	if (strtok(t, delimiters) != NULL)
		for (numtokens = 1; strtok(NULL, delimiters) != NULL; numtokens++)
			;

	if ((*argvp = calloc(1, ((numtokens + 1) * sizeof(char *)))) == NULL) {
		error = errno;
		free(t);
		errno = error;
		return -1;
	}

	if (numtokens == 0)
		free(t);
	else {
		strcpy(t, snew);
		**argvp = strtok(t, delimiters);
		for (i = 1; i < numtokens; i++)
			*((*argvp) + i) = strtok(NULL, delimiters);
	}
	*((*argvp) + numtokens) = NULL;
	return numtokens;
}
int searchUserAndPassword(char * user, char * password, char *ip) {

	char ch;
	FILE *fp;
	char * text;
	char ** args;
	char delimeters[] = " \n\t";
	char delim[] = ",";
	char ** args1;
	char ** args2;
	int result = -1;
	int i = 0, j = 0;
	int len = 0, len2 = 0;

	if ((fp = fopen("kayit.txt", "r")) == NULL) {
		f_err_report(ERR_OPEN_1);
	}
	if (fp != NULL) {
		while (!feof(fp)) {
			ch = getc(fp);
			i++;
			if (ch == '\n') {
				j++;
			}
		}

		text = malloc(sizeof(char) * i);
		i = 0;

		if ((fp = fopen("kayit.txt", "r")) == NULL) {
			f_err_report(ERR_OPEN_1);
		}

		while (!feof(fp)) {
			ch = getc(fp);
			text[i++] = ch;

			if (ferror(fp))
				f_err_report(ERR_READ);
		}

		len = makeargv(text, delimeters, &args1);

		for (i = 0; i < len; i++) {
			len2 = makeargv(args1[i], delim, &args2);

			if ((strcmp(args2[0], user) == 0)) {
				if ((strcmp(args2[1], password) == 0)) {
					if ((strcmp(args2[2], ip) == 0)) {
						printf("Aranan Kişi bulundu");
						fflush(stdout);
						result = 1; //result 2 olmuşsa client sisteme kayıtlı demektir.
					}
				}
			}

		}

		fclose(fp);
	}
	return result;
}
int main() {
	int sock, true = 1;

	struct sockaddr_in server_addr, client_addr;
	int sin_size;

	pthread_t th;
	int rc;
	char str[5];

	/*
	 * Get a socket to work with.  This socket will
	 * be in the INET domain, and will be a
	 * stream socket.
	 */
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("ERROR opening socket");
		exit(1);
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.s_addr = INADDR_ANY;
	bzero((char *) &(server_addr.sin_zero), sizeof(server_addr.sin_zero));

	if (bind(sock, (struct sockaddr *) &server_addr, sizeof(struct sockaddr))
			== -1) {
		perror("ERROR on binding");
		exit(1);
	}
	/*
	 * Listen on the socket.
	 */
	if (listen(sock, 5) == -1) {
		perror("ERROR on listenning");
		exit(1);
	}

	printf("\nTCP Server Waiting for client on port %d", PORT);
	fflush(stdout);


	while (1) {
		int connected;
		sin_size = sizeof(struct sockaddr_in);
		/*
		 * Accept connections.  When we accept one, connected
		 * will be connected to the client.  client_addr will
		 * contain the address of the client.
		 */
		connected = accept(sock, (struct sockaddr *) &client_addr, &sin_size);

		rc = pthread_create(&th, NULL, Thread_Run, (void *) &connected);


		if (rc) {
			printf("ERROR; return code from pthread_create() is %d\n", rc);
		}
	}
	/*
	 * We can simply use close() to terminate the
	 * connection, since we're done with both sides.
	 */
	close(sock);
	return 0;
}
void * Thread_Run(void * args) {

	int * connected;
	//thdata * data;
	int goOn = 1;
	//data = (thdata *) args;
	connected = (int *) args;

	printf("Thread Created \n");
	while (goOn) {

		readMessage(connected, &goOn);

	}
	close(*connected);
}
int readMessage(int * connected, int *goON) {
	int socket_Fd = 0;
	socket_Fd = *connected;
	stack * alan, *veren;
	char recv_data[MAX_LINE];
	int bytes_recieved;

	bzero(recv_data, strlen(recv_data));
	bytes_recieved = read(socket_Fd, recv_data, MAX_LINE-1);

	char delim[] = ";";
	char ** args;
	int kill_Flag = 0;

	int len = 0;
	int messageType = 0;

	if (bytes_recieved != -1) {
		recv_data[bytes_recieved] = '\0';

		len = strlen(recv_data);
		printf("length %d socket numarası %d bytes_recieved %d\n",len,socket_Fd,bytes_recieved);
		fflush(stdout);
		if (len > 0) {
			len = makeargv(recv_data, delim, &args);

			printf("alici  %s\n", args[0]);
			fflush(stdout);
			printf("message type  %s\n", args[1]);
			fflush(stdout);
			printf("yazi  %s\n", args[2]);
			fflush(stdout);
			printf("yollayan  %s\n", args[3]);
			fflush(stdout);

			messageType = atoi(args[1]);
			switch (messageType) {

			case IS_ACCEPTED: {

				if (searchUserAndPassword(args[3], args[2], args[0]) == 1) {
					printf("Accepted %d\n ", howMany());
					fflush(stdout);

					//pthread_mutex_lock(&job_queue_mutex);
					push(socket_Fd, args[3], args[2]);
					SendOnlineUser();
					//pthread_mutex_unlock(&job_queue_mutex);

				} else {

					printf("Not Accepted\n ");
					fflush(stdout);
					sendMessage(socket_Fd, "onaylanmadi", "7", "alici",
							"server");//Not Accepted Leave

					*goON = 0;
					kill_Flag = 1;
				}
				break;
			}
			case CHAT: {

				printf("CHAT \n");
				fflush(stdout);

				alan = SearchSocket(atoi(args[0]));
				//veren = SearchSocket(atoi(args[3]));

				sendMessage(atoi(args[0]), args[2], "2", args[0], args[3]);

				break;
			}
			case CHAT_BEGIN: {

				printf("Chat Begin\n");
				fflush(stdout);

				sendMessage(atoi(args[0]), args[2], "5", args[0], args[3]);
				fflush(stdout);

				break;
			}
			case CHAT_END: {

				printf("Chat End\n");
				fflush(stdout);

				sendMessage(atoi(args[0]), args[2], "6", args[0], args[3]);

				break;
			}
			case EXIT: {

				printf("Exit\n");
				fflush(stdout);
				socket_Fd = atoi(args[2]);
				//pthread_mutex_lock(&job_queue_delete);
				deleteNode(socket_Fd);
				kill_Flag = 1;
				*goON = 0;
				SendOnlineUser();
				//pthread_mutex_unlock(&job_queue_delete);
				break;
			}


			}

		}
	} else {

	}
	return 0;
}
int sendMessage(int target_Fd, char * send_data, char * tipi, char * alici,
		char *yollayan) {
	char a[MAX_LINE];
	int len = 0;
	int n = 1;
	strcat(a, alici);
	strcat(a, messageDelimeters);
	strcat(a, tipi);
	strcat(a, messageDelimeters);
	strcat(a, send_data);
	strcat(a, messageDelimeters);
	strcat(a, yollayan);
	n = write(target_Fd, a, strlen(a));
	if (n < 0) {
		printf("ERROR writing to socket\n");
		fflush(stdout);
	}
	bzero(a, strlen(a));
	return 0;
}

char * create_File(char *alici, char * yollayan) {
	struct tm *Sys_T = NULL;
	time_t Tval = 0;
	char a[15];
	char b[15];
	char c[15];
	char d[15];
	char e[15];
	char f[15];
	int month, day, year, min, hour, sec;
	char * path;

	path = malloc(sizeof(char) * 250);

	Tval = time(NULL);

	Sys_T = localtime(&Tval);

	month = (int) Sys_T->tm_mon + 1;
	day = (int) Sys_T->tm_mday;
	year = (int) Sys_T->tm_year + 1900;
	min = (int) Sys_T->tm_min;
	hour = (int) Sys_T->tm_hour;
	sec = (int) Sys_T->tm_sec;
	//ahmet_mehmet_year_month_day_hour_minutes_seconds.txt

	strcat(path, alici);
	strcat(path, "_");
	strcat(path, yollayan);
	strcat(path, "_");

	sprintf(a, "%d", year);
	strcat(path, a);
	strcat(path, "_");

	sprintf(b, "%d", month);
	strcat(path, b);
	strcat(path, "_");

	sprintf(c, "%d", day);
	strcat(path, c);
	strcat(path, "_");

	sprintf(d, "%d", hour);
	strcat(path, d);
	strcat(path, "_");

	sprintf(e, "%d", min);
	strcat(path, e);
	sprintf(f, "%d", sec);
	strcat(path, ".txt");

	printf("Dosya ismi= %s\n", path);
	return path;
}
int record_Conservation(char * alici, char * yollayan, char * yazi, char * path) {

	FILE * fileid;
	int len = 0, i = 0;

	fileid = fopen(path, "w");
	if (fileid == NULL) {
		printf("Error: open file\n");
		return -1;
	}
	printf("create fileid: %d", fileid);

	strcat(yollayan, ":");
	strcat(yollayan, yazi);
	len = strlen(yollayan);

	for (i = 0; i < len; i++) {
		fprintf(fileid, "%f", yollayan[i]);
	}

	fclose(fileid);
	printf("close fileid: %d\n", fileid);

}