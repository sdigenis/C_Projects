#include "mysystem_check.c"
#define MAX_NAME 100
#define BIT_BUFFER 512

int main(int argc, char *argv[]){
    
    int fd, i, name_size=0;
    char pathname[MAX_NAME], filename[MAX_NAME];
    char *path_to_file;
    char buffer[BIT_BUFFER];
    struct stat info;
    
    do{
        if(scanf("%s", pathname)==EOF){
            return(0);
        }
        if((path_to_file=strrchr(pathname, '/'))!=NULL){
            strcpy(filename,path_to_file+1);
        }
        else{
            strcpy(filename,pathname);
        }
        fd=open(pathname, O_RDWR, S_IRWXU);
        if(fd<0){
            fprintf(stderr, "Error in open: %s in line: %d\n",strerror(errno), __LINE__);
            exit(EXIT_FAILURE);
        }
        else{
            name_size=strlen(filename);
            my_write(STDOUT_FILENO, &name_size, sizeof(int), __LINE__);
            my_write(STDOUT_FILENO, filename, name_size*sizeof(char), __LINE__);
//              fprintf(stderr,"name_size: %d  filename: %s\n",name_size, filename);
            if(stat(pathname, &info)==-1){
                fprintf(stderr, "Error in stat: %s \n",strerror(errno));
                exit(EXIT_FAILURE);
            }
//  			fprintf(stderr,"Last access %s: ", ctime(&info.st_atime));
//  			fprintf(stderr,"Last modification %s: ", ctime(&info.st_mtime));
            my_write(STDOUT_FILENO, &info.st_atime, sizeof(struct timespec), __LINE__);
            my_write(STDOUT_FILENO, &info.st_mtime, sizeof(struct timespec), __LINE__);
            my_write(STDOUT_FILENO, &info.st_mode, sizeof(mode_t), __LINE__);
            my_write(STDOUT_FILENO, &info.st_size, sizeof(off_t), __LINE__);
            
            for(i=0; i<info.st_size; i=i+BIT_BUFFER){
                if(i+BIT_BUFFER>info.st_size){
                    my_read(fd,buffer,info.st_size-i, __LINE__);
                    my_write(STDOUT_FILENO,buffer,info.st_size-i, __LINE__);
                }
                
                if(i+BIT_BUFFER<=info.st_size){
                    my_read(fd,buffer,BIT_BUFFER, __LINE__);
                    my_write(STDOUT_FILENO,buffer,BIT_BUFFER, __LINE__);
                }  
            }
        }
    }
    while(1);
  
    return 0;
}
