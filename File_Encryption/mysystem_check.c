/*
 *  PROG2_HW2
 *  1.4.2018
 * 
 *  DIGENIS_2388
 *  CHATZIGEORGIOU_2480
 *  
 *  PROGRAMMA POU PRAGMATOPOIEI OLOUS
 *  TOUS APARAITHTOUS ELAGXOUS TWN 
 *  SYSTHMATWN POY XRHSIMOPOIOYME STO
 *  GENIKO MAS PROGRAMMA
 */

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <utime.h>
#include <sys/time.h>
#include <errno.h>
#include <time.h>
#include <sys/wait.h>

/*
 * SYNARTHSH POY KANEI KLISI TOU SYSTHMATOS WRITE
 * KAI TAYTOXRONA PRAGMATOPOIEI OLOUS TOUS ELEGXOUS
 * POY PREPEI NA GINONTAI SE KATHE KLISI TOU SYSTHMATOS
 * WRITE GIA THN TIMI POY EPISTREFEI
 */
int my_write(int fd , void *buf , size_t nbytes, int line){
	int bytes=0,new=0;
	
	bytes=write(fd,buf,nbytes);
	if(bytes==-1){
		fprintf(stderr, "Error in my write:%s in line %d\n",strerror(errno),  line );
		close(fd);
		exit(1);
	}
	
	if (bytes == 0) {
		return bytes;
	}
	new = bytes;
	while(bytes < nbytes){
		nbytes = nbytes - new;
		buf=buf+bytes;
		bytes = write(fd, buf + bytes, nbytes);
		new = bytes;
		if(bytes < 0){
			fprintf(stderr, "Error in my write:%s in line %d\n",strerror(errno),  line );
			close(fd);
			exit(1);  
		}
	}
	return(bytes);
}
/*
 * SYNARTHSH POY KANEI KLISI TOU SYSTHMATOS LSEEK
 * KAI TAYTOXRONA PRAGMATOPOIEI OLOUS TOUS ELEGXOUS
 * POY PREPEI NA GINONTAI SE KATHE KLISI TOU SYSTHMATOS
 * LSEEK GIA THN TIMI POY EPISTREFEI
 */
int my_lseek(int fd, off_t pos, int whence, int line){
    off_t file_position;
    
    file_position = lseek(fd, (int)pos, whence);
    if(file_position == -1){
        fprintf(stderr,"ERROR detected in my_lseek on line %d: %s\n", line,strerror(errno));
        exit (1); 
    }
    return fd;
}
/*
 * SYNARTHSH POY KANEI KLISI TOU SYSTHMATOS READ
 * KAI TAYTOXRONA PRAGMATOPOIEI OLOUS TOUS ELEGXOUS
 * POY PREPEI NA GINONTAI SE KATHE KLISI TOU SYSTHMATOS
 * READ GIA THN TIMI POY EPISTREFEI
 */
int my_read(int fd , void *buf , size_t nbytes, int line){
    int bytes=0,pos=0,total_bytes_read=0;
    
    bytes=read(fd,buf,nbytes);
    if(bytes==-1){
        fprintf(stderr,"ERROR detected in my_read on line %d: %s [%d]\n", line,strerror(errno),errno);
        close(fd);
        exit(1);
    }
    
    if (bytes == 0) {
        return bytes;
    }
    
    total_bytes_read=bytes;
    while(total_bytes_read<nbytes){
        pos=total_bytes_read;
		buf=buf+pos;
        bytes=read(fd,buf,nbytes-total_bytes_read);
        if(bytes==-1){
            fprintf(stderr,"ERROR detected in my_read on line %d: %s [%d]\n", line,strerror(errno),errno);
            close(fd);
            exit(1);
        }
        total_bytes_read=total_bytes_read+bytes;
    }
    return(total_bytes_read);
}

/*
 * SYNARTHSH POY KANEI KLISI TOU SYSTHMATOS READ
 * KAI TAYTOXRONA PRAGMATOPOIEI OLOUS TOUS ELEGXOUS
 * POY PREPEI NA GINONTAI SE KATHE KLISI TOU SYSTHMATOS
 * READ GIA THN TIMI POY EPISTREFEI
 */
void my_dup(int oldfd, int fd_new, int line){
    int dup_return;
    
    dup_return=dup2(oldfd,fd_new);
    if(dup_return==-1){
        fprintf(stderr,"ERROR detected in my_dup on line %d: %s\n", __LINE__,strerror(errno));
        exit(1);
    }
    return;
}


