#include "mysystem_check.c"

int main(int argc, char *argv[]){
	DIR *dir;
    char pathname[100];
	struct dirent *dir_read;
	
	if (argc!=2){
		printf("Enter a correct file name of the directory!\n");
		exit(EXIT_FAILURE);
	}
	dir=opendir(argv[1]);
	if(dir==NULL){
		fprintf(stderr, "Error in opendir: %s \n",strerror(errno));
		exit(EXIT_FAILURE);
	}
	
	while((dir_read=readdir(dir))!=NULL){
		if(dir_read->d_type == DT_REG){
            strcpy(pathname, argv[1]);
            strcat(pathname, "/");
            strcat(pathname, dir_read->d_name);
			printf("%s\n", pathname);
		}
	}
	if(closedir(dir)==-1){
		fprintf(stderr, "Error in closedir: %s \n",strerror(errno));
		exit(EXIT_FAILURE);
	}
	
	return 0;
}
