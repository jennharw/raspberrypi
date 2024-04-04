#include <time.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/stat.h>
#include <wiringPi.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define LED 1

static void *clnt_connection(void *arg);
int sendData(int fd, FILE *fp, char *file_name);
void sendOk(FILE *fp);
void sendError(FILE *fp);

void ledControl(int gpio, int state){
fprintf(stderr, "n\n\nmonononononononONONO\n\n\n");	
    digitalWrite(gpio, state);
}

int main(int argc, char **argv){
    int serv_sock;
    pthread_t tid;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_size;

    if(argc != 2){
        fprintf(stderr, "usage port %s", argv[0]);
        fprintf(stderr, "usage default port 8000");
        
    }
    if (wiringPiSetup() == -1){
        perror("wiring setup error");
    }
    pinMode(LED, OUTPUT);
    serv_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (serv_sock < 0){
        perror("socket");
        return -1;
    }
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(8000);

    if(bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
        perror("bind");
        return -1;
    }

    if(listen(serv_sock, 10) < 0){
        perror("listen");
        return -1;
    }
    while(1){
        int clnt_sock;
        clnt_addr_size = sizeof(clnt_addr);
        clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
        fprintf(stderr, "Cleint Ip %s:%d\n", inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));

        pthread_create(&tid, NULL, clnt_connection, (void *)&clnt_sock);
        pthread_detach(tid);
    }
    return 0;
}
void *clnt_connection(void *arg){
    fprintf(stderr, "start connect");
	int clnt_sock = *((int *)arg), clnt_fd;
    FILE *clnt_read, *clnt_write;

    char reg_line[BUFSIZ], reg_buf[BUFSIZ];
    char method[10];
    char file_name[256];

    clnt_read = fdopen(clnt_sock, "r");
    clnt_write = fdopen(dup(clnt_sock), "w");
    clnt_fd = clnt_sock;

    fgets(reg_line, BUFSIZ, clnt_read);
    fputs(reg_line, stdout);

    strcpy(method, strtok(reg_line, " /"));
    if(strcmp(method, "POST") == 0){
	fprintf(stderr, "asdfasdf\n POSTclint \n");
        sendOk(clnt_write);
        fclose(clnt_read);
        fclose(clnt_write);
	fprintf(stderr, "\n\nfclose errrrrrr\n\n");
        return NULL;
    }
    else if(strcmp(method, "GET") != 0){
        sendError(clnt_write);
        fclose(clnt_read);
        fclose(clnt_write);
        return NULL;
    }
    strcpy(file_name, strtok(NULL, "/"));
   // fprintf(stderr, "\nGET why\nfile name %s\n", file_name);
    if (strstr(file_name, "?")!= NULL){
	fprintf(stderr, "HERERHAEREREA ");
        char opt[8]={0}, var[8]={0};
        strtok(file_name,"?");
	//fprintf(stderr, "file \n");
	//char *ptr1, *ptr2;        

//char *token = strtok_r(NULL, "=", &ptr1);
//if (token != NULL) {
  //  strcpy(opt, token);
//}

//token = strtok_r(NULL, "=", &ptr2);
//if (token != NULL) {
 //   strcpy(var, token);
//}
	strcpy(opt, strtok(NULL, "="));
        strcpy(var, strtok(NULL, "= "));
	
//	char opt[8], var[8];
 //   char *question_mark_pos = strstr(file_name, "?");
  //  *question_mark_pos = '\0'; 
   // sscanf(question_mark_pos + 1, "%7[^=]=%7s", opt, var);  
    fprintf(stderr, "\n\n\n%s-%s\n", opt, var);
//fprintf(stderr, "\n\n\n%s-%s\n", opt, var); 
//        fprintf(stderr, "file name%s", file_name);
	fprintf(stderr,"\nVVValue LED %s %d\n",opt, strcmp(opt, "led") );
//	fprintf(stderr, "kk%c%c%c", var[0], var[1], var[2]);
//	fprintf(stderr, "%d", var[2]);
//	fprintf(stderr, "=1=");
//	fprintf(stderr, "%s", var);
//	//fprintf(stderr, "=======");
//	//fprintf(stderr, "%d", 'n');
//	char p2[4] = "On ";
	fprintf(stderr, "\nVvvvvalue %s-%d\n",var, strcmp(var, "On"));
//fprintf(stderr, "\nVvvvvalue %s-%d\n",var, strcmp(var, "On "));
//fprintf(stderr, "\nVvvvvalue %s-%d\n",var, strcmp(var, 'On '));
        if(!strcmp(opt, "led") && !strcmp(var, "On")){
            fprintf(stderr, "LED ONqqqq");
            ledControl(LED, 1);
        }else if (!strcmp(opt, "led") && !strcmp(var, "Off")){
            fprintf(stderr, "LED OFFFFFF");
            ledControl(LED, 0);
        }
    }
   do {
		fgets(reg_line, BUFSIZ, clnt_read);
		fputs(reg_line, stdout);
		strcpy(reg_buf, reg_line);
        }  while(strncmp(reg_line, "\r\n", 2));

	sendData(clnt_fd, clnt_write, file_name);
        fclose(clnt_read);
	fclose(clnt_write);
	return NULL;
    
}

int sendData(int fd, FILE *fp, char *file_name){
    fprintf(stderr,"send data");
	char protocol[] = "HTTP/1.1 200 OK\r\n";
    char server[] = "Server:Netscape-Enterpris\6.0\r\n";
    char cnt_type[] = "Content-type:text/html\r\n";
    char end[] = "\r\n";
    char buf[BUFSIZ];
    int len;

    fputs(protocol, fp);
    fputs(server, fp);
    fputs(cnt_type, fp);
    fputs(end, fp);

    fflush(fp);
//    fprintf(stderr,"open fp ordwr file name\n", file_name);
    
    fd= open("control.html", O_RDONLY);
    //fprintf(stderr, "??%d", fd);
    do {
        len = read(fd, buf, BUFSIZ);
        fwrite(buf, len, sizeof(char), fp);
    }while(len == BUFSIZ);
    fflush(fp);
    close(fd);
    return 0;
}

void sendOk(FILE *fp){
    fprintf(stderr, "\nsendOK\n");
    char protocol[] = "HTTP/1.1 200 OK\r\n";
    char server[] = "Server: Netscape-Enterprise/6.0\r\n\r\n";
    fputs(protocol, fp);
    fputs(server, fp);
    fflush(fp);
    fprintf(stderr, "\nsendok complete\n");

}

void sendError(FILE *fp){
    char protocol[] = "HTTP/1.1 200 OK\r\n";
    char server[] = "Server:Netscape-Enterprise\6.0\r\n";
    char cnt_len[] = "Content-Length:1024\r\n";
    char cnt_type[] = "Content-type:text/html\r\n";
    char content1[] = "<html><head><title>Bad connection</title></head>";
    char content2[] = "<body><font size=+5>Bad Request </font></body></html>";

    fprintf(stderr, "send error\n");
    fputs(protocol, fp);
    fputs(server, fp);
    fputs(cnt_len, fp);
    fputs(content1, fp);
    fputs(content2, fp);
    fflush(fp);
    
}

