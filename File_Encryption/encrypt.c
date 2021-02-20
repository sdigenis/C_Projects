#include "mysystem_check.c"

int main(int argc, char *argv[]){
	
	int i,password_lenght=0;
	char password[50];
	char encypted_char, non_encypted_char;
	long int result;
	
	if (argc!=2){
		printf("Not enough parameters!\n");\
		exit(EXIT_FAILURE);
	}
	
	strcpy(password, argv[1]);
    password_lenght=strlen(password);
	i=0;
	do{
		result=my_read(STDIN_FILENO,&non_encypted_char,sizeof(char), __LINE__);
		if(result==0){
			break;
			
		}
        encypted_char=non_encypted_char^password[i];
		my_write(STDOUT_FILENO,&encypted_char,1, __LINE__);
		i++;
		if(i==password_lenght){
			i=0;
		}
	}
	while(1);
	return 0;
}
