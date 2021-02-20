/*
 *  PROG2_HW1
 *  11.3.2018
 * 
 *  DIGENIS_2388
 *  CHATZIGEORGIOU_2480
 *  
 *  PROGRAMMA POU DIAXEIRIZETAI MIA VASI
 *  PSITHIAKWN ANTIKEIMENWN KAI DINEI THN
 *  DINATOTHTA STON XRHSTH NA KANEI EISAGWGH,
 *  ANAZHTHSH, EKSAGWGH KATHWS KAI DIAGRAFH
 *  ARXIWN STHN VASI AYTH.
 * 
 */

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>

#define ONOMA_SIZE 250
#define BIT_BUFFER 512

int my_write(int fd , void *buf , size_t nbytes);
int my_open(char *fname);
int my_lseek(int fd, off_t pos, int whence);
void my_close(int fd);
int size_of_file(int fd_file);
int searchSuccess(int fd, char file_name[]);
void ffind(int fd, char file_name[]);
int fimport(int fd, char *file_name);
void fexport(int fd, char *file_name, char *destination);
void closef(int fd);
void fdelete(int fd, char file_name[]);

/*
 * SYNARTHSH POY KANEI KLISI TOU SYSTHMATOS WRITE
 * KAI TAYTOXRONA PRAGMATOPOIEI OLOUS TOUS ELEGXOUS
 * POY PREPEI NA GINONTAI SE KATHE KLISI TOU SYSTHMATOS
 * WRITE GIA THN TIMI POY EPISTREFEI
 */
int my_write(int fd , void *buf , size_t nbytes){
    int bytes;
    
    bytes = write(fd,buf,nbytes);
    if(bytes==-1){
        fprintf(stderr,"write error: %s\n",strerror(errno));
        close(fd);
        exit(1);
    }
    else if(bytes == 0){
        printf("Nothing was written.\n");
    }
    return(bytes);
}

/*
 * SYNARTHSH POY KANEI KLISI TOU SYSTHMATOS OPEN
 * KAI TAYTOXRONA PRAGMATOPOIEI OLOUS TOUS ELEGXOUS
 * POY PREPEI NA GINONTAI SE KATHE KLISI TOU SYSTHMATOS
 * OPEN GIA THN TIMI POY EPISTREFEI
 */
int my_open(char *fname){
    int fd;
    fd = open(fname, O_RDWR | O_EXCL | O_CREAT,0);
    if(fd == -1){
        fprintf(stderr,"open error: %s\n",strerror(errno));
        exit(1);
    }
    return(fd);
}


/*
 * SYNARTHSH POY KANEI KLISI TOU SYSTHMATOS LSEEK
 * KAI TAYTOXRONA PRAGMATOPOIEI OLOUS TOUS ELEGXOUS
 * POY PREPEI NA GINONTAI SE KATHE KLISI TOU SYSTHMATOS
 * LSEEK GIA THN TIMI POY EPISTREFEI
 */
int my_lseek(int fd, off_t pos, int whence){
    off_t file_position;
    
    file_position = lseek(fd, (int)pos, whence);
    if(file_position == -1){
        fprintf(stderr,"lseek erron: %s\n",strerror(errno));
        exit (1); 
    }
    return fd;
}

/*
 * SYNARTHSH POY KANEI KLISI TOU SYSTHMATOS CLOSE
 * KAI TAYTOXRONA PRAGMATOPOIEI OLOUS TOUS ELEGXOUS
 * POY PREPEI NA GINONTAI SE KATHE KLISI TOU SYSTHMATOS
 * CLOSE GIA THN TIMI POY EPISTREFEI
 */
void my_close(int fd){
    int file_end;
    
    file_end = close(fd);
    if(file_end == -1){
        fprintf(stderr,"close error: %s\n",strerror(errno));
        exit (1); 
    }
    return;
}

int size_of_file(int fd_file){
    int file_size;
    
    file_size=lseek(fd_file,0,SEEK_END);
    lseek(fd_file,0,SEEK_SET);
    
    return file_size;
}

/*
 *  SYNARTHSH POY PRAGMATOPOIEI ANAZHTHSH
 *  ENOS AREIOY STHN VASI OPWS H FFIND
 *  ME TH DIAFORA OTI KANEI ANAZHTHSH
 *  MONO TOU ONOMATOS
 */
int searchSuccess(int fd, char file_name[]){
    
    int current_file_name_size=0;
    int current_file_size=0;
    char current_file_name[ONOMA_SIZE] = {'\0'};
    int bytes_in_db=0;
    int bytes_read=4;
    
    bytes_in_db=size_of_file(fd);
    my_lseek(fd,+4,SEEK_SET);
    
    do{
        read(fd,&current_file_name_size,sizeof(int));
        read(fd,current_file_name,current_file_name_size);
        current_file_name[current_file_name_size]='\0';
        if(strcmp(current_file_name,file_name)==0){
            return 0;
        }
        read(fd,&current_file_size,sizeof(int));
        my_lseek(fd,+current_file_size,SEEK_CUR);
        bytes_read = bytes_read + current_file_name_size + 2 * sizeof(int) + current_file_size;
    }
    while(bytes_in_db > bytes_read);
    return 1;
}

/*
 *  SYNARTHSH POY PRAGMATOPOIEI ANAZHTHSH
 *  ENOS AREIOY STHN VASI
 */
void ffind(int fd, char file_name[]){
    
    int current_file_name_size;
    int current_file_size;
    char current_file_name[ONOMA_SIZE] = {'\0'};
    int bytes_in_db;
    int bytes_read=4;
    bytes_in_db=size_of_file(fd);
    my_lseek(fd,+4,SEEK_SET);
    
    do{
        read(fd,&current_file_name_size,sizeof(int));
        read(fd,current_file_name,current_file_name_size);
        current_file_name[current_file_name_size]='\0';
        if(strstr(current_file_name,file_name)!=NULL || strcmp(file_name,"*")==0){
            printf("\t%s\n", current_file_name);
        }
        read(fd,&current_file_size,sizeof(int));
        my_lseek(fd,+current_file_size,SEEK_CUR);
        bytes_read=bytes_read+current_file_name_size+2*sizeof(int)+current_file_size;
    }
    while(bytes_in_db>bytes_read);
}

/*
 *  SYNARTHSH POY PRAGMATOPOIEI EISAGWGH
 *  ENOS AREIOY STHN VASI
 */
int fimport(int fd, char *file_name){
    int i,name_lenght;
    int fd_file;
    char unsigned buffer[BIT_BUFFER] = {'\0'};
    int file_size;
    
    name_lenght=strlen(file_name);   
    fd_file = open(file_name,O_RDONLY,S_IRWXU);
    
    if(fd_file<0){
        printf("This file doesn't exist!\n");
        return(-1);
    }
    
    if(searchSuccess(fd,file_name) == 0){
        printf("This file already exists!\n");
        
        exit(0);
    }
    else{
        file_size=size_of_file(fd_file);
        my_lseek(fd,0,SEEK_END);
        my_write(fd,&name_lenght,sizeof(int));
        my_write(fd,file_name,name_lenght*sizeof(char));
        my_write(fd,&file_size,sizeof(int));
        fsync(fd);
        
        for(i=0; i<=file_size; i=i+BIT_BUFFER){
            if(i+BIT_BUFFER>file_size){
                read(fd_file,buffer,file_size-i);
                my_write(fd,buffer,file_size-i);
                fsync(fd);
            }
            
            if(i+BIT_BUFFER<file_size){
                read(fd_file,buffer,BIT_BUFFER);
                my_write(fd,buffer,BIT_BUFFER);
                fsync(fd);
            }   
        }
    }
    return 1;
}

/*
 *  SYNARTHSH POY PRAGMATOPOIEI EKSAGWGH
 *  ENOS AREIOY APO THN VASI SE ENA ALLO ARXEIO
 */
void fexport(int fd, char *file_name, char *destination){
    
    int fd_file, i;
    char buffer[BIT_BUFFER] = {'\0'};
    int file_size;
    
    if(searchSuccess(fd, file_name)==1){
        printf("This file doesn't exist in the database! \n");
        return;
    }
    else{
        fd_file=open(destination,O_RDWR | O_EXCL | O_CREAT, S_IRWXU);
        if(fd_file<0){
            fd_file=open(destination,O_RDWR, S_IRWXU);
            my_lseek(fd_file, 0, SEEK_END);
        }
        
        else{
            my_lseek(fd_file,0,SEEK_SET);
        }
        read(fd,&file_size,sizeof(int));
        
        for(i=0; i<file_size; i=i+BIT_BUFFER){
            if(i+BIT_BUFFER>file_size){
                read(fd,buffer,file_size-i);
                my_write(fd_file,buffer,file_size-i);
                fsync(fd);
            }
            if(i+BIT_BUFFER<file_size){
                read(fd,buffer,BIT_BUFFER);
                my_write(fd_file,buffer,BIT_BUFFER);
                fsync(fd);
            }
        }
    }
}

/*
 *  SYNARTHSH POY PRAGMATOPOIEI DIAGRAFH
 *  ENOS AREIOY APO THN VASI
 */
void fdelete(int fd, char file_name[ONOMA_SIZE]){
    
    int i,file_size, name_lenght,rest_bytes_in_db,bytes_in_db;
    char buffer[BIT_BUFFER];
    int bytes_bf_dlt_file,thesi, telos;
    
    name_lenght=strlen(file_name);
    bytes_in_db=size_of_file(fd);
    
    if(searchSuccess(fd,file_name)==1){
        printf("This file doesn't exist in the database! \n");
        return;
    }
    else{
        read(fd,&file_size,sizeof(int));
        my_lseek(fd,+file_size,SEEK_CUR);
        thesi=lseek(fd,0,SEEK_CUR);
        telos=lseek(fd,0,SEEK_END);
        rest_bytes_in_db=telos-thesi;
        my_lseek(fd,-rest_bytes_in_db ,SEEK_END);
        printf("%d\n", rest_bytes_in_db);
        bytes_bf_dlt_file=bytes_in_db-(sizeof(int)+name_lenght*sizeof(char)+sizeof(int)+file_size)-rest_bytes_in_db;
        //kanei diagrafh tou teleutaiou arxeiou apo th vasi mas...
        if(lseek(fd,0,SEEK_CUR)==lseek(fd,0,SEEK_END)){
            my_lseek(fd,-(sizeof(int)+name_lenght*sizeof(char)+sizeof(int)+file_size),SEEK_END);
            ftruncate(fd,bytes_in_db-(sizeof(int)+name_lenght*sizeof(char)+sizeof(int)+file_size));
            return;
        }
        //kanei diagrafh se opoiodipote shmeio ths vasis
        else{
            for(i=0; i<rest_bytes_in_db; i=i+BIT_BUFFER){
                if(i+BIT_BUFFER>rest_bytes_in_db){
                    my_lseek(fd,bytes_bf_dlt_file+2*sizeof(int)+name_lenght+file_size+i,SEEK_SET);
                    read(fd,buffer,rest_bytes_in_db-i);
                    my_lseek(fd,bytes_bf_dlt_file+i,SEEK_SET);
                    write(fd,buffer,rest_bytes_in_db-i);
                    fsync(fd);
                }
                if(i+BIT_BUFFER<rest_bytes_in_db){
                    my_lseek(fd,bytes_bf_dlt_file+2*sizeof(int)+name_lenght+file_size+i,SEEK_SET);
                    read(fd,buffer,BIT_BUFFER);
                    my_lseek(fd,bytes_bf_dlt_file+i,SEEK_SET);
                    write(fd,buffer,BIT_BUFFER);
                    fsync(fd);
                }
            }
        }
        ftruncate(fd,bytes_in_db-(name_lenght*sizeof(char)+2*sizeof(int)+file_size));
        my_lseek(fd,-(name_lenght*sizeof(char)+2*sizeof(int)+file_size),SEEK_CUR);
    }
    
}

//PRAGMATOPOIEI KLEISIMO THS VASIS
void closef(int fd){
    fsync(fd);
    my_close(fd);
}

int main(int argc, char *argv[]){
    int fd;
    int mgn;
    char option;
    char str[5] = {'\0'};
    char file_name[ONOMA_SIZE] = {'\0'};
    char destination[ONOMA_SIZE] = {'\0'};
    char format_string[20];
    
    sprintf(format_string,"%%%ds",ONOMA_SIZE - 1);
    
    //elegxos an mpainei sto arxeio
    if(argc != 2){
        printf("Give a correct database to enter ! \n");
        return 1;
    }
    
    fd = open(argv[1], O_RDWR | O_EXCL | O_CREAT, S_IRWXU);
    
    if(fd<0){
        fd = open(argv[1],O_RDWR,S_IRWXU);
        my_lseek(fd,0,SEEK_SET);
        read(fd,str,4);
        mgn=strcmp("rose",str);
        if(mgn != 0){
            printf("You cannot open this database !\n");
            return 0;
        }
    }
    if(fd > 0){
        my_lseek(fd,0, SEEK_SET);
        my_write(fd,"rose",4);
    }
    
    while(1){
        printf("1.import(i)​\n2.find(f)\n3.export(e)\n4.delete(d)\n5.quit(q)\n");
        printf("Select an option: ");
        scanf(" %c" , &option);
        switch(option){
            case('i'):
                printf("Enter the name of the file you want to import to the base: ");
                scanf(format_string,file_name);
                fimport(fd,file_name);
                break;
            case('f'):
                printf("Enter the name of the file you want to search in the base: ");
                scanf(format_string,file_name);
                ffind(fd,file_name);
                break;
            case('e'):
                printf("Enter the name of the file you want to export from the base: ");
                scanf(format_string,file_name);
                printf("Enter the name of the file you want to paste the exported file: ");
                scanf(format_string,destination);
                fexport(fd,file_name,destination);
                break;
            case('d'): 
                printf("Enter the name of the file you want to delete from the base: ");
                scanf(format_string,file_name);
                fdelete(fd,file_name);
                break;
            case('q'):
                closef(fd);
                return 0;
            default: 
                printf("\tPlease enter a valid option!\n");
                //return 0;
        }
    }
    return 0; 
}
