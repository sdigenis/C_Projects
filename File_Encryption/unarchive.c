#include "mysystem_check.c"
#define BIT_BUFFER 512
#define MAX_NAME 100

int main(int argc, char *argv[]){
	
	int i=0, name_size=0, fd=0, file_length=0;
	char pathname[MAX_NAME]={'\0'}, filename[MAX_NAME]={'\0'};
	char buffer[512];
	mode_t permissions=0;
	time_t access, modification;
	off_t file_size=0;
	struct timeval times[2];
	
	if (argc!=2){
		printf("Not enough parameters!\n");\
		exit(EXIT_FAILURE);
	}
	
	if(mkdir(argv[1], 0700)==-1){
		fprintf(stderr, "Error in mkdir: %s in line %d\n",strerror(errno), __LINE__ );
		exit(EXIT_FAILURE);
	}
	
	while(1){
		for(i=0; i<MAX_NAME; i++){
		pathname[i]='\0';
		}
		if(my_read(STDIN_FILENO, &name_size, sizeof(int),__LINE__)==0){
			return 0;
		}
		my_read(STDIN_FILENO, filename, name_size * sizeof(char),__LINE__);
		
		file_length=strlen(argv[1]);
		strcpy(pathname, argv[1]);
		strcat(pathname, "/");
		strcat(pathname,filename);
		pathname[name_size+1+file_length]='\0';
// 		fprintf(stderr, "[P2UNARCHIVE] name_size: %d\n", name_size);
// 		fprintf(stderr, "[P2UNARCHIVE] filename: %s\n", filename);
//  	fprintf(stderr, "[P2UNARCHIVE] pathname: %s\n", pathname);
				
		fd=open(pathname,O_RDWR | O_CREAT, S_IRWXU);
		if(fd<0){
			fprintf(stderr, "Error in open: %s in line %d\n",strerror(errno), __LINE__ );
			exit(EXIT_FAILURE);
		}
		else{			
			my_read(STDIN_FILENO, &access, sizeof(struct timespec), __LINE__);
			my_read(STDIN_FILENO, &modification, sizeof(struct timespec), __LINE__);
			my_read(STDIN_FILENO, &permissions, sizeof(mode_t),__LINE__);
			my_read(STDIN_FILENO, &file_size, sizeof(off_t),__LINE__);
			
			times[0].tv_sec=access;
            times[0].tv_usec=0;
			times[1].tv_sec=modification;
            times[1].tv_usec=0;
						
			if(chmod(pathname,permissions )==-1){
				fprintf(stderr, "Error in chmod: %s in line %d\n",strerror(errno),  __LINE__ );
				exit(EXIT_FAILURE);
			}			
			for(i=0; i<file_size; i=i+BIT_BUFFER){
				if(i+BIT_BUFFER>file_size){
					my_read(STDIN_FILENO,buffer,file_size-i,__LINE__);
					my_write(fd,buffer,file_size-i,__LINE__);
				}
				
				if(i+BIT_BUFFER<=file_size){
					my_read(STDIN_FILENO,buffer,BIT_BUFFER,__LINE__);
					my_write(fd,buffer,BIT_BUFFER,__LINE__);
				}  
			}
			
			if(utimes(pathname, times)==-1){
				fprintf(stderr, "Error in utimes: %s in line %d\n",strerror(errno), __LINE__);
				exit(EXIT_FAILURE);
			}
		}
		close(fd);
	}
	
	return 0; 
}
