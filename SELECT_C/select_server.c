//Sammaritani Gloria 0000723439
//Select Server

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#define DIM_BUFF 100
#define LENGTH_FILE_NAME 20
#define max(a,b) ((a) > (b) ? (a) : (b))
#define NUM 6
#define LEN 30

int conta_file (char *name){
	DIR *dir;
	struct dirent * dd;
	int count = 0;
	dir = opendir (name);
	while ((dd = readdir(dir)) != NULL){
		printf("Trovato il file %s\n", dd-> d_name);
		count++;
	}
	printf("Numero totale di file %d\n", count);
	closedir (dir);
	return count;
}
void gestore(int signo){
	int stato;
	printf("esecuzione gestore di SIGCHLD\n");
	wait(&stato);
}

int main(int argc, char **argv){
	int  listenfd, connfd, udpfd, fd_file, nready, maxfdp1;
	const int on = 1;
	char zero=0, buff[DIM_BUFF], nome_file[LEN], nome_dir[LEN], richiesta[LEN];
	fd_set rset;
	int len, nread, nwrite, num, ris, port;
	struct sockaddr_in cliaddr, servaddr;

    //controllo arg
	if(argc!=2){
		printf("Error: %s port\n", argv[0]);
		exit(1);
	}
	else port = atoi(argv[1]);
	printf("[SELECT SERVER] Server avviato\n");

    //creazione socket TCP
	listenfd=socket(AF_INET, SOCK_STREAM, 0);
	if (listenfd <0)
	{perror("apertura socket TCP "); exit(1);}
	printf("[SELECT SERVER] Creata la socket TCP d'ascolto, fd=%d\n", listenfd);

    //inzializzazione indirizzo
	memset ((char *)&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(port);

	if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on))<0)
	{perror("set opzioni socket TCP"); exit(2);}
printf("[SELECT SERVER] Set opzioni socket TCP ok\n");

	if (bind(listenfd,(struct sockaddr *) &servaddr, sizeof(servaddr))<0)
	{perror("bind socket TCP"); exit(3);}
	printf("[SELECT SERVER] Bind socket TCP ok\n");

	if (listen(listenfd, 5)<0)
	{perror("listen"); exit(4);}
	printf("[SELECT SERVER] Listen ok\n");

    //iniziallizazione indirizzo
	udpfd=socket(AF_INET, SOCK_DGRAM, 0);
	if(udpfd <0)
	{perror("apertura socket UDP"); exit(5);}
	printf("[SELECT SERVER] Creata la socket UDP, fd=%d\n", udpfd);

    //inizializzazione indirizzo e bind
	memset ((char *)&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(port);

	if(setsockopt(udpfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on))<0)
	{perror("set opzioni socket UDP"); exit(6);}
	printf("[SELECT SERVER] Set opzioni socket UDP ok\n");

	if(bind(udpfd,(struct sockaddr *) &servaddr, sizeof(servaddr))<0)
	{perror("bind socket UDP"); exit(7);}
	printf("[SELECT SERVER] Bind socket UDP ok\n");

	signal(SIGCHLD, gestore);

	FD_ZERO(&rset);
	maxfdp1=max(listenfd, udpfd)+1;

	for(;;){
		FD_SET(listenfd, &rset);
		FD_SET(udpfd, &rset);

		if ((nready=select(maxfdp1, &rset, NULL, NULL, NULL))<0){
			if (errno==EINTR) continue;
			else {perror("select"); exit(8);}
		}

		if (FD_ISSET(listenfd, &rset)){
			printf("Ricevuta richiesta di get di un file\n");
			len = sizeof(struct sockaddr_in);
			if((connfd = accept(listenfd,(struct sockaddr *)&cliaddr,&len))<0){
				if (errno==EINTR) continue;
				else {perror("accept"); exit(9);}
			}

			if (fork()==0){
				close(listenfd);
				printf("Dentro il figlio, pid=%i\n", getpid());

				for (;;){
          //leggo il nomefile
					if (read(connfd, &richiesta, sizeof(richiesta))<=0)
					{ perror("read"); break; }
					printf("Richiesto file %s\n", richiesta);

          strcpy(nome_file, richiesta);
          if(write(connfd, nome_file, sizeof(nome_file))<0){
              perror("write");
              break;
          }
          
          //apro il file dal leggere
					fd_file = open(nome_file, O_RDONLY);
          printf("fd %d\n",fd_file);
					if (fd_file < 0){
						printf("File inesistente\n"); 
            nread = -1;
						/*write(connfd, "N", 1);*/
					} else {
            //se l'aperutra va a buon fine
						/*write(connfd, "S", 1);*/
            printf("Leggo e invio il file richiesto\n");

            //leggo il numero di byte dal file
            while((nread = read(fd_file, buff, sizeof(buff))) > 0){
                    write(connfd,&nread,sizeof(int));
                    if ((nwrite = write(connfd, buff, nread)) < 0){
                            perror("write"); 
                            break;
                    }
            }

            nread = -2;
            printf("Terminato invio file\n");

            //invio teminazione
            close(fd_file);
          }//else
          write (connfd,&nread,sizeof(int));
        }//for
        printf("Figlio %i: chiudo connessione e termino\n", getpid());
        close(connfd);
        exit(0);
      }//figlio
      close(connfd);
    } 


    //gestione servizio UDP
    if (FD_ISSET(udpfd, &rset)){

            printf("Server: ricevuta richiesta\n");
            len=sizeof(struct sockaddr_in);
            if (recvfrom(udpfd, &nome_dir, sizeof(nome_dir), 0, (struct sockaddr *)&cliaddr, &len)<0)
            {perror("recvfrom"); continue;}

            printf("Richiesto %s\n", nome_dir);
            num = conta_file(nome_dir);
            printf("Risultato : %i\n", num);

            ris=htonl(num);

            if (sendto(udpfd, &ris, sizeof(ris), 0, (struct sockaddr *)&cliaddr, len)<0)
            {perror("sendto"); continue;}

    } 
  } /* ciclo for della select */
}
