/*
 *  PROG2_HW3
 *  6.5.2018
 * 
 *  DIGENIS_2388
 *  CHATZIGEORGIOU_2480
 * 
 *  To programma ylopoiei ena systhma krathshs aeroporikwn 
 *  eisithsiwn to opoio ekteleitai se enan topiko upologisth.
 *  O server dexetai aithmata apo praktores gia prosvasi se 
 *  koinorhsth mnhmh pou perilamvanei plirofories ptisewn 
 *  kai diathesimwn thesewn,kathws kai enhmervseis pou 
 *  dinoun oi diaforoi praktores. 
 */
#include <stdio.h> 
#include <string.h>  
#include <stdlib.h> 
#include <errno.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <fcntl.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/un.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include<sys/sem.h>


#define N 100
#define SIZE 500
#define PATH_NAME 128
#define BIT_BUFFER
#define MAX_AGENTS 100
#define EXIT_FAILURE 1

typedef  struct Agent{
    int sd;
    long int id;
    int seats;
}Agent;


typedef struct airticket{
	char airline[3];
	char departure[4];
	char destination[4];
	int stops;
	int seats;
}AirTicket;

off_t size_of_file(char pathname[]){
	
	int fdfile; 
	off_t file_size;
	struct stat info;
	
	fdfile=open(pathname, O_RDONLY, 00700);
	if(fdfile<0){
		fprintf(stderr,"Error in open: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	if(stat(pathname, &info)==-1){
		fprintf(stderr, "Error in stat: %s \n",strerror(errno));
		exit(EXIT_FAILURE);
	}
	file_size=info.st_size;
	return file_size; 
}

void share_to_fd(int fd, AirTicket *shm_P, int number){
	
	int i=0;
	lseek(fd, 0, SEEK_SET);
	dup2(fd,STDOUT_FILENO);
	for(i=0; i<number; i++){
		fprintf(stdout,"%s %s %s %d %d\n", shm_P[i].airline, shm_P[i].departure, shm_P[i].destination, shm_P[i].stops, shm_P[i].seats);
	}
}


int main(int argc , char *argv[]){ 
	
	int master_socket , addrlen , new_socket , client_socket[30] , 
	max_clients = 30 , activity, i , valrecv , sd, s[SIZE], k;  
	int max_sd, end=0;  
	struct sockaddr_un address;  
	
	fd_set readfds;  
	int fdfile;
	off_t file_size;
	int shmid, semid;
	AirTicket *shm_P;
	char pathname[PATH_NAME];
	int number_structs;
	long int client_id;
	int seats_booked;
	char option/*, end2*/;
	int newfile, number;
	int j, online=0;
	key_t keys, keym;
	Agent agent[MAX_AGENTS];
    
    
	if(argc!=4){
		printf("Not enough arguments!\n");
		exit(EXIT_FAILURE);
	}
	
	max_clients=atoi(argv[1]);
	strcpy(pathname, argv[2]);
	
	for (i = 0; i < max_clients; i++){  
		agent[i].sd = 0;
        agent[i].id=0;
        agent[i].seats=0;
	}  
	
	file_size=size_of_file(pathname);
	fdfile=open(pathname,O_RDWR, 00400);
    
        /*share memory cration*/
	dup2(STDIN_FILENO, 1);
	dup2(fdfile,STDIN_FILENO);
	keym=ftok(".", 'a');
	number_structs=(file_size / sizeof(struct airticket) +1)  ;
	shmid=shmget(keym,number_structs*sizeof(AirTicket), IPC_CREAT|IPC_EXCL|S_IRWXU);
	if(shmid==-1){
		shmdt(shm_P);
		shmctl(shmid,IPC_RMID,NULL);
		perror("shmget");
		exit(EXIT_FAILURE);
	}
	shm_P=(struct airticket *)shmat(shmid,NULL,0);
	j=0;
	while(1){
		if(fscanf(stdin,"%s %s %s %d %d\n", shm_P[j].airline, shm_P[j].departure, shm_P[j].destination, &shm_P[j].stops, &shm_P[j].seats)==EOF)
			break;
		j++;
	}
        /*share memory created */
        /*socket creation*/
	if( (master_socket = socket(AF_UNIX , SOCK_STREAM , 0)) == 0){  
		fprintf(stderr,"Error in socket: %s\n", strerror(errno));  
		exit(EXIT_FAILURE);  
	}  
	
	address.sun_family = AF_UNIX;  
	strcpy(address.sun_path,argv[3]);
	if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0){  
		fprintf(stderr,"Error in bind: %s\n", strerror(errno));  
		exit(EXIT_FAILURE);  
	}  
	
	if (listen(master_socket, max_clients) ==-1){  
		fprintf(stderr,"Error in listen: %s\n", strerror(errno)); 
		exit(EXIT_FAILURE);  
	}
	
	addrlen = sizeof(address);  
	dup2(1,STDIN_FILENO);
        /*socket created*/
        /*semaphores creation*/
    keys=ftok(",", 'b');
    if((semid=semget(keys,1, IPC_CREAT | S_IRWXU))==-1){
        fprintf(stderr,"Error in semget: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    if(semctl(semid,0, SETVAL, 1)==-1){
        fprintf(stderr,"Error in semctl: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
        /*semaphores created*/
        /*interaction with clients*/
    for(i=0; i<max_clients; i++){
        s[i]=0;
    }
	while(1){  
		if(option=='Q'){
			break;
		}
		FD_ZERO(&readfds);  
		FD_SET(master_socket, &readfds);  
		max_sd = master_socket;  
		
		for ( i = 0 ; i < max_clients ; i++){  
			sd = client_socket[i];  
			
			if(sd > 0)  
				FD_SET( sd , &readfds);  
			
			if(sd > max_sd)  
				max_sd = sd;  
		}  
		
		activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);  
		
		if ((activity ==-1)){  
			fprintf(stderr,"Error in select: %s\n", strerror(errno));  
		}  
		
		if (FD_ISSET(master_socket, &readfds)){  
			if ((new_socket = accept(master_socket,(struct sockaddr *)&address, (socklen_t*)&addrlen))==-1){  
				fprintf(stderr,"Error in accept: %s\n", strerror(errno));  
				exit(EXIT_FAILURE);  
			}  
			
			if(recv(new_socket, &client_id, sizeof(long int), 0)==-1){
				fprintf(stderr,"Error in recv: %s\n", strerror(errno));
				exit(EXIT_FAILURE);
			}
			online++;
			
			if(online>max_clients){
				number=-1;
				if(send(new_socket, &number, sizeof(int), 0)==-1){
					fprintf(stderr,"Error in send: %s\n", strerror(errno));
					exit(EXIT_FAILURE);
				}
				close(new_socket);
			}
			else{
				number=1;
				if(send(new_socket, &number, sizeof(int), 0)==-1){
					fprintf(stderr,"Error in send: %s\n", strerror(errno));
					exit(EXIT_FAILURE);
				}
				if(send(new_socket, &number_structs, sizeof(int), 0)==-1){
					fprintf(stderr,"Error in send: %s\n", strerror(errno));
					exit(EXIT_FAILURE);
				}
				printf("Agent: %ld connected\n", client_id);
			}
			for (i = 0; i < max_clients; i++){  
				if( client_socket[i] == 0 ){  
					agent[i].sd = new_socket;
                    s[i]=new_socket;
                    agent[i].id=client_id;
                    client_socket[i]=new_socket;
					break;  
				}  
			}  
		}  
		
		for (i = 0; i < max_clients; i++){  
			sd = client_socket[i];  
			if(option=='Q'){
               
                end=1;
				break;
			}
			for(k=0;k<max_clients;k++){
                if(online==s[k]){
                    client_socket[k]=s[k];
                    break;
                }
            }
			
			if (FD_ISSET( sd , &readfds)){ 
                if(end==1)
                    break;
				if ((valrecv = recv( sd , &option, sizeof(char), 0)) == 0){  
					option='e';
					printf("Agent: %ld disconnected having booked %d tickets\n", agent[i].id, agent[i].seats);
					close( sd );  
					client_socket[i] = 0;
                    agent[i].sd=0;
                    agent[i].id=0;
                    agent[i].seats=0;
                    s[i]=-1;
					online--;
                    if(online==0){
                        end=1;
                        break;
                    }
				}  
				
				if(option=='r'){
					if(recv(sd, &seats_booked, sizeof(int), 0)==-1){
						fprintf(stderr,"Error in recv: %s\n", strerror(errno));
						exit(EXIT_FAILURE);
					}
					agent[i].seats=agent[i].seats+seats_booked;
					printf("Agent: %ld booked %d tickets\n", agent[i].id, seats_booked);
				}
				if(option=='Q'){
					continue;
				}
				
			} 
			/*if(sd==STDIN_FILENO){
                scanf(" %c", &end2);
                if(end2=='Q'){
                    for(k=0;i<max_clients;k++){
                        if(agent[i].sd !=0 && agent[i].id > 0){
                            printf("Client with pid=%ld got %d total tickets\n",agent[i].id,agent[i].seats);
                        close(agent[i].sd);
                    }
                }
                    end=1;
                    break;
                }
            }*/
			 if(end==1)
                break;
		} 
		 if(end==1){
             for(i=0; i<max_clients; i++){
                 if(agent[i].sd!=0){
                    printf("Agent: %ld bought %d tickets\n", agent[i].id, agent[i].seats);
                 }
             }
            printf("No more agents connected in the server!\n");
            break;
         }
	}  
	
	
	newfile=open(pathname, O_RDWR, 00700);
	if(newfile==-1){
		fprintf(stderr,"Error in open: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	share_to_fd(newfile, shm_P, number_structs);
	shmdt(&shm_P);
    semctl(semid,0,IPC_RMID);
	shmctl(shmid,IPC_RMID,NULL);
	close(master_socket);
	unlink(argv[3]);
	
	return 0;  
}  
