#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>

volatile sig_atomic_t counter;

static void handler(int signum){
    printf("got sigusr1\n");
    counter=0;
}


int main (int argc, char *argv[]){
    
    
    int maxrepeat, block;
    long int id;
    struct sigaction act={{0}};
    sigset_t s1;

    if(argc!=5){
        printf("Not enough arguments\n");
        exit(EXIT_FAILURE);
    }
    if((strcmp(argv[1], "-m"))!=0){
        printf("Wrong argument -m\n");
        exit(EXIT_FAILURE);
    }
    if((strcmp(argv[3], "-b"))!=0){
        printf("Wrong argument -b\n");
        exit(EXIT_FAILURE);
    }
    
    maxrepeat=atoi(argv[2]);
    block=atoi(argv[4]);
    if(maxrepeat<=0){
        printf("Wrong argument for repeats\n");
        exit(EXIT_FAILURE);
    } 
    if(block!=0 && block!=1){
        printf("Wrong blocking argument\n");
        exit(EXIT_FAILURE);
    }
    if(block==0){
        sigemptyset(&s1);
        sigaddset(&s1, SIGUSR1);
        act.sa_handler=handler;
        sigaction(SIGUSR1, &act, NULL);
        for(counter=1; counter<=maxrepeat; counter++){
            id=getpid();
            printf("%ld, %d , %d\n",id, counter, maxrepeat);
            if(counter==maxrepeat){
                break;
            }
            sleep(5);
            
        }
    }
    if(block==1){
        sigemptyset(&s1);
        sigaddset(&s1, SIGUSR1);
        act.sa_handler=handler;
        sigaction(SIGUSR1, &act, NULL);
        for(counter=1; counter<=maxrepeat; counter++){
            if(counter<maxrepeat/2){
                sigprocmask(SIG_BLOCK, &s1, NULL);
            }
            if(counter>maxrepeat/2){
                sigprocmask(SIG_UNBLOCK, &s1, NULL);
            }
            id=getpid();
            printf("%ld, %d, %d\n", id, counter, maxrepeat);
            sleep(5);
        }
    }
 return 0;   
}
