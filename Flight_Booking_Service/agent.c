#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/un.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<sys/sem.h>


#define SIZE 5
#define EXIT_FAILURE 1

typedef struct airticket{
    char airline[3];
    char departure[4];
    char destination[4];
    int stops;
    int seats;
}AirTicket;

int main(int argc, char *argv[]){
    
    int sockfd,i, number, number_structs=0;
    struct sockaddr_un addr;
    long int pidid;
    key_t keym, keys;
    int shmid, control, semid;
    AirTicket *shm_P, client_s;
    char option;
    struct sembuf operation;
    
        /*server's socket connection*/
    sockfd=socket(AF_UNIX, SOCK_STREAM, 0);
    if(sockfd<0){
        fprintf(stderr, "Error in socket: %s\n" , strerror(errno));
        exit(EXIT_FAILURE);
    }
    addr.sun_family=AF_UNIX;
    strcpy(addr.sun_path, argv[1]);
    while(connect(sockfd, (struct sockaddr *) &addr, sizeof(addr))==-1){
        if(errno==ENOENT){
            sleep(1);
            continue;
        }
        fprintf(stderr,"Error in connect: %s", strerror(errno));
        exit(EXIT_FAILURE);
    }
        /*connected with server's socket*/
        /*server's share memory connection*/
    keym=ftok(".", 'a');
    shmid=shmget(keym,0,0);
    if(shmid==-1){
        fprintf(stderr,"Error in shmget: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    shm_P=(AirTicket *)shmat(shmid,NULL,0);
        /*connected with server's share memory*/
        /*server's semaphores connection*/
    keys=ftok(",", 'b');
    if((semid=semget(keys,1,S_IRWXU))==-1){
        fprintf(stderr,"Error in semget: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
        /*connected with server's semaphores*/
        /*interaction with server*/
    pidid=getpid();
    printf("client id = %ld\n", pidid);
    if(send(sockfd, &pidid, sizeof(int), 0)==-1){
        fprintf(stderr,"Error in send: %s\n",strerror(errno));
        exit(EXIT_FAILURE);
    }
    if(recv(sockfd, &number, sizeof(int), 0)==-1){
        fprintf(stderr,"Error in recv: %s\n",strerror(errno));
        exit(EXIT_FAILURE);
    }
    if(number<0){
        printf("Too many clients at the moment try again later !\n");
        exit(EXIT_FAILURE);
    }
    if(recv(sockfd, &number_structs, sizeof(int), 0)==-1){
        fprintf(stderr,"Error in send: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    while(1){
        printf("FIND RESERVE EXIT\n");
        scanf(" %c", &option);
        if(option=='e' )
            break;
        switch(option){
            case 'f': {
                scanf("%s %s %d", client_s.departure, client_s.destination, &client_s.seats);
                for(i=0, control=0; i<number_structs; i++){
                    if((strcmp(client_s.departure, shm_P[i].departure)==0) && (strcmp(client_s.destination, shm_P[i].destination)==0) && client_s.seats<= shm_P[i].seats){
                        printf("%s %s %s %d %d\n", shm_P[i].airline, shm_P[i].departure, shm_P[i].destination, shm_P[i].stops, shm_P[i].seats);
                        control++;
                    }
                }
                if(control==0){
                    printf("No flights or not enough tickets for requested flight!\n");
                }
                break;
            }
            case 'r':{
                operation.sem_num= 0; 
                operation.sem_op= -1; 
                operation.sem_flg= SEM_UNDO;
                if(semop(semid,&operation,1)==-1){
                    fprintf(stderr,"Error semop: %s\n",strerror(errno));
                    exit(EXIT_FAILURE);
                }
                scanf("%s %s %s %d",client_s.departure, client_s.destination, client_s.airline,  &client_s.seats);
                for(i=0, control=0; i<number_structs; i++){
                    if((strcmp(client_s.airline, shm_P[i].airline)==0) && (strcmp(client_s.departure, shm_P[i].departure)==0) && (strcmp(client_s.destination, shm_P[i].destination)==0) && client_s.seats<=shm_P[i].seats){
						
                        shm_P[i].seats=shm_P[i].seats-client_s.seats;
                        printf("Reservation completed !\n");
                        control++;
                        if(send(sockfd, &option, sizeof(char),0)==-1){
                            fprintf(stderr,"Error in send: %s\n", strerror(errno));
                            exit(EXIT_FAILURE);
                        }
                        
                        if(send(sockfd, &client_s.seats, sizeof(int),0)==-1){
                            fprintf(stderr,"Error in send: %s\n", strerror(errno));
                            exit(EXIT_FAILURE);
                        }
                        break;
                    }
                }
                if(control==0){
                    printf("No flights or not enough tickets for requested flight!\n");
                }
                operation.sem_num= 0; 
                operation.sem_op= 1; 
                operation.sem_flg= SEM_UNDO;
                if(semop(semid,&operation,1)==-1){
                    fprintf(stderr,"Error semop: %s\n",strerror(errno));
                    exit(EXIT_FAILURE);
                }
                break;
            }
            case 'e':{
                if(send(sockfd, &option,sizeof(char),0)==-1){
                    fprintf(stderr,"Error in send: %s\n", strerror(errno));
                    exit(EXIT_FAILURE);
                }
                if(send(sockfd, &pidid, sizeof(int), 0)==-1){
                    fprintf(stderr,"Error in send: %s\n",strerror(errno));
                    exit(EXIT_FAILURE);
                }
                break;
            }
            default:{
                printf("The options are Find(f), Reservation(r) and Exit(e)!\nPlease choose one of these.\n");
                break;
            }
        }
    }
    
    
    close(sockfd);
    return 0; 
}
