//Sammaritani Gloria 0000723439
//Stream Client

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#define DIM_BUFF 100
#define LEN 30

int main(int argc, char *argv[]){
	int sd, nread, nrecv, fd_file;
	char c, ok, nome_file[LEN], buff[DIM_BUFF], richiesta[LEN];
	struct hostent *host;
	struct sockaddr_in servaddr;  

    //controllo arg
	if(argc!=3){
		printf("[STREAM CLIENT] Error:%s serverAddress serverPort\n", argv[0]);
		exit(1);
	}
	printf("[STREAM CLIENT] Client avviato\n");

    //peparazione indirizzo sever
	memset((char *)&servaddr, 0, sizeof(struct sockaddr_in));
	servaddr.sin_family = AF_INET;
	host = gethostbyname(argv[1]);
	if (host == NULL){
		printf("[STREAM CLIENT] %s not found in /etc/hosts\n", argv[1]);
		exit(2);
	}
	servaddr.sin_addr.s_addr=((struct in_addr*) (host->h_addr))->s_addr;
	servaddr.sin_port = htons(atoi(argv[2]));

    //creazione connessione socket (non necessito bind)
	sd=socket(AF_INET, SOCK_STREAM, 0);
	if (sd <0){perror("apertura socket "); exit(3);}
	printf("[STREAM CLIENT] Creata la socket sd=%d\n", sd);

	if (connect(sd,(struct sockaddr *) &servaddr, sizeof(struct sockaddr))<0)
	{perror("Errore in connect"); exit(4);}
	printf("[STREAM CLIENT] Connect ok\n");

	/* CORPO DEL CLIENT: */
	printf("Nome del file da richiedere: ");

	while (gets(richiesta)){
    //invio il nome del file
		if (write(sd, richiesta , sizeof(richiesta))<0){
			perror("write");
			break;
		}
		printf("Richiesta del file %s inviata... \n", richiesta);
    
    /*
		if (read(sd, &ok, 1)<0){
			perror("read");      
			break;
		}*/

    if(read(sd, nome_file, sizeof(nome_file))<0){
        perror("read");
        break;
    }
		printf("Ricevo il file %s\n", nome_file);

    fd_file = open(nome_file, O_WRONLY | O_CREAT, 0555);
    if(fd_file < 0){
            printf("Impossibile aprire il file richiesto\n");
    } 
    else{
            printf("Ricevo il file:\n");
            //leggo il numero di file inviati
            while(read(sd, &nrecv, sizeof(int)) && nrecv > 0){
                    if(read(sd, buff, nrecv) > 0){
                            if(write(fd_file, buff, nrecv) < 0){
                                    printf("Impossibile scrivere su file %s\n",nome_file);
                            }
                            write(1, buff, nrecv);
                    }else {
                            perror("read");
                            break;
                    }
            }//fine ricezione
            close(fd_file);
            if(nrecv == -1)
                  printf("File inesistente nel Server\n");
    }//else

    printf("Nome del file da richiedere: \n");
  }//while

  printf("\n[STREAM CLIENT] Client: termino...\n");
  close(sd);
  exit(0);
}

