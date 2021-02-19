#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/time.h>
#include <signal.h>
#include <sys/wait.h>
#include <signal.h>

#define MAX_NAME 100
#define MAX_PROGRAMS 10
#define MAX_ORIGINS 20

struct execute {
    pid_t id;
    char test_name[MAX_NAME];
    char *origins[MAX_ORIGINS];
    int n_origins;
    int onoroff;
    struct execute *prev;
    struct execute *next;
};
struct execute *head;

struct execute *search_who_runs(){
    
    struct execute *program;  
    
    if(head->next==head){
        return NULL;
    }
    program=head->next;
    
    while(program->onoroff!=1){
        program=program->next;
    }
    return program;
}

void alarm_handler(int sig){
    struct execute *program;
    write(STDOUT_FILENO,"BEEP!\n", 6);
    
    program=search_who_runs();
    if(program==NULL){
        return;
    }
    program->onoroff=0;
    if(kill(program->id,SIGSTOP)==-1){
        fprintf(stderr,"Error in 1stkill: %s %d\n",strerror(errno), __LINE__-1);
        exit (1);
    }
    if(program->next!=head){
        program=program->next;
    }
    else{
        program=program->next->next;
    }
    program->onoroff=1;
    if(kill(program->id,SIGCONT)==-1){
        fprintf(stderr,"Error in 2ndkill: %s %d\n",strerror(errno), __LINE__-1);
        exit (1);
    }
}

void delete_program(pid_t pid){
    struct execute *deleted_program;
    
    deleted_program=head->next;
    while(1){
        if(deleted_program->id==pid){
            deleted_program->next->prev=deleted_program->prev;
            deleted_program->prev->next=deleted_program->next;
            free(deleted_program);
            printf("Program finished!\n");
            break;
        }
        if(deleted_program==head){
            printf("This program wasnt found!\n");
            break;
        }
        deleted_program=deleted_program->next;
    }
}

struct execute *find_program(pid_t id){
    struct execute *searching_program;
    
    searching_program=head->next;
    
    while(searching_program!=head){
        if(searching_program->id==id){
            printf("program found\n");
            return searching_program;
        }
        searching_program=searching_program->next;
    }
    printf("This program wasnt found!\n");
    return NULL;
}

void child_handler(int sig){
    
    struct execute *program, *program_who_runs;
    pid_t pid;
    
    pid=waitpid(-1, NULL, WNOHANG);
    if(pid==-1){
        fprintf(stderr,"Error in waitpid: %s\n", strerror(errno));
        exit(1);
    }
    
    if(pid>0){
        program_who_runs=search_who_runs();
        program=find_program(pid);
        if(program==program_who_runs){
            if(program->next!=head){
                program_who_runs=program->next;
            }
            else{
                program_who_runs=head->next;
                if(program_who_runs==program){
                    delete_program(head->prev->id);
                    return;
                }
            }
            program_who_runs->onoroff=1;
            if(kill(program_who_runs->id,SIGCONT)==-1){
                fprintf(stderr,"Error in kill: %s %d\n",strerror(errno), __LINE__-1);
                exit (1);
            }
            
        }
        delete_program(pid);
    }
    
    
//     if(WSTOPSIG(status)){
//     }
//     else{
//         pid=waitpid(-1, &status, 0);
//         if(pid==-1){
//             fprintf(stderr,"Error in waitpid: %s\n", strerror(errno));
//             exit(1);
//         }
//         program=search_who_runs(pid);
//         if(program->next==head){
//             program=head->next;
//             delete_program(head->prev->id);
//             if(head->next!=head){
//                 program->onoroff=1;
//                 if(kill(program->id,SIGCONT)==-1){
//                     fprintf(stderr,"Error in kill: %s %d\n",strerror(errno), __LINE__-1);
//                     exit (1);
//                 }
//             }
//         }
//         else{
//             program=program->next;
//             program->onoroff=1;
//             if(kill(program->id,SIGCONT)==-1){
//                 fprintf(stderr,"Error in kill: %s %d\n",strerror(errno), __LINE__-1);
//                 exit (1);
//             }
//             delete_program(program->prev->id);
//         }
//     }
}

void initiate_list(){
    head=(struct execute *)malloc(sizeof(struct execute));
    if(head==NULL){
        perror("malloc");
    }
    head->next=head;
    head->prev=head;
}

void print_programs(){
    struct execute *program;
    int i;
    program=head->prev;
    printf("\tExecutables programs\n");
    while(1){
        if(program==head){
            break;
        }
        printf("pid: %ld, name:( %s", (long int )program->id, program->test_name);
        for(i=1; program->origins[i]!=NULL; i++){
            printf(", %s", program->origins[i]);
        }
        printf(")");
        if(program->onoroff==1){
            printf("(R)");
        }
        printf("\n");
        program=program->prev;
    }
}

void quit(){
    struct execute *program;
    int i;
    if(head->next!=head){
        for(program=head->next; program!=head; program=program->next){
            if(kill(program->id,SIGTERM)==-1){
                fprintf(stderr,"Error in kill: %s %d\n", strerror(errno), __LINE__);
                exit(1);
            }
            program->next->prev=program->prev;
            program->prev->next=program->next;
            for(i=0; program->origins[i]!=NULL; i++){
                free(program->origins[i]);
            }
        }
    }
    free(head);
}

void exec(sigset_t set){
    char program[MAX_NAME]={'\0'}, executable[MAX_NAME];
    char *word_start, *space_p;
    int i;
    int signum;
    pid_t pid;
    struct execute *new_program;
    
    new_program=(struct execute *)malloc(sizeof(struct execute));
    if(new_program==NULL){
        printf("Not enough memory!\n");
        exit(1);
    }
    new_program->onoroff=0;
    new_program->next=head->next;
    new_program->prev=head;
    
    new_program->next->prev=new_program;
    new_program->prev->next=new_program;
    
    fgets(program, MAX_NAME-1, stdin);
    strcpy(executable, "./");
    word_start=program+1;
    
    for(i=0; ; i++, word_start=space_p+1){
        space_p=strchr(word_start, ' ');
        if(space_p!=NULL){
            *space_p='\0';
        }
        if(i==0){
            strcat(executable, word_start);
            strcpy(new_program->test_name, executable);
            new_program->origins[i]=(char *)malloc(sizeof(word_start+1));
            strcpy(new_program->origins[i],word_start);
        }
        else{
            new_program->origins[i]=(char *)malloc(sizeof(word_start+1));
            strcpy(new_program->origins[i],word_start);
        }
        if(space_p==NULL){
            new_program->origins[i+1]=NULL;
            break;
        }
    }
    new_program->n_origins=i+1;
    pid=fork();
    if(pid==0){
        if(sigwait(&set, &signum)==-1){
            fprintf(stderr,"Error in sigwait: %s\n", strerror(errno));
            exit(1);
        }
        execvp(new_program->test_name,new_program->origins);
        fprintf(stderr,"Error in execvp: %s\n", strerror(errno));
        delete_program(pid);
        exit (1);
    }
    if(pid>0){
        new_program->id=pid;  
        if(new_program->next==head){
            new_program->onoroff=1;
            if(kill(new_program->id, SIGCONT)==-1){
                fprintf(stderr,"Error in kill: %s %d\n", strerror(errno), __LINE__-1);
                exit(1);
            }
        }
    }
}
void term(){
    pid_t id;
    struct execute *program;
    
    scanf(" %ld", (long int *)&id);
    program=find_program(id);
    if(program!=NULL){
        if(kill(id,SIGTERM)==-1){
            fprintf(stderr,"Error in kill sending SIGUSR1: %s\n", strerror(errno));
            exit (1);
        }
    }
}
void sig(){
    pid_t id;
    struct execute *program;
    
    scanf(" %ld", (long int*)&id);
    program=find_program(id);
    if(program!=NULL){
        if(kill(id,SIGUSR1)==-1){
            fprintf(stderr,"Error in kill sending SIGUSR1: %s\n", strerror(errno));
            exit (1);
        }
    }
}

int main(int argc, char *argv[]){
    
    char command[5];
    struct itimerval time={{0}};
    struct sigaction act1={{0}}, act2={{0}}, act3={{0}};
    sigset_t set;
    
    sigemptyset(&set);
    sigaddset(&set, SIGCONT);
    if(sigprocmask(SIG_BLOCK, &set, NULL)==-1){
        fprintf(stderr,"Error in sigprocmask: %s\n",strerror(errno));
        exit(1);
    }
    
    act1.sa_handler=alarm_handler;
    act1.sa_flags=SA_RESTART;
    if(sigaction(SIGALRM, &act1, NULL)==-1){
        fprintf(stderr,"Error in sigaction: %s\n", strerror(errno));
        exit (1);
    }
    
    time.it_value.tv_sec=20;
    time.it_value.tv_usec=0;
    time.it_interval.tv_sec=20;
    time.it_interval.tv_usec=0;
    if((setitimer(ITIMER_REAL, &time, NULL))==-1){
        fprintf(stderr,"Error in setitimer: %s\n", strerror(errno));
        exit (1);
    }
    act2.sa_handler=SIG_IGN;
    act2.sa_flags=SA_RESTART;
    if(sigaction(SIGUSR1, &act2, NULL)==-1){
        fprintf(stderr,"Error in sigaction: %s\n", strerror(errno));
        exit(1);
    }
    act3.sa_handler=child_handler;
    act3.sa_flags=SA_RESTART;
    if(sigaction(SIGCHLD, &act3, NULL)==-1){
        fprintf(stderr, "Error in sigaction: %s\n", strerror(errno));
        exit(1);
    }
    
    initiate_list();
    
    while(1){
        scanf(" %s", command);
        
        if(strcmp(command,"exec")==0){
            exec(set);
        }
        else if(strcmp(command,"term")==0){
            term();
        }        
        else if(strcmp(command, "sig")==0){
            sig();
        }
        else if(strcmp(command, "list")==0){
            print_programs();
        }
        else if(strcmp(command, "quit")==0){
            quit();
            break;
        }
        else{
            printf("\tChoose between the options:\nexec, term, sig, list or quit\n");
        }
    }
    return 0;
}


