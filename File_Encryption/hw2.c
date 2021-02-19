/*
 *  PROG2_HW2
 *  1.4.2018
 * 
 *  DIGENIS_2388
 *  CHATZIGEORGIOU_2480
 * 
 *  To programma apoteleitai apo epimerous mikrotera 
 *  programmata pou eketlountai sundiasmana kai parexei 
 *  sto xrhsth tis dynatothtes
 *  na apo8hkeusei ta dedomena enos katalogou se ena
 *  arxeio, kwdikopoihmeno, kai epeita ,eisagwntas to
 *  katallhlo kleidi na to apokwdikopoihsei kai na 
 *  eksagei ta dedomena se ena neo katalogo.
 */

#include "mysystem_check.c"
#define MAX_NAME 100
#define MAX_PASSWORD 20

int main (int argc, char *argv[]){
	
	int dir_arch_pipe[2], arch_encr_pipe[2], decr_unarch_pipe[2],fd_file;
	char directory[MAX_NAME]={'\0'}, password[MAX_PASSWORD]={'\0'};
	char archived_file[MAX_NAME]={'\0'}, pathname[MAX_NAME]={'\0'};
	char mgn_valid[12]={'\0'};
	pid_t pid_1,pid_2, pid_3;
	static int WRITE_END=1;
	static int READ_END=0;
	int i;
	
	strcpy(directory, argv[2]);
	strcpy(pathname,directory);
	strcpy(password, argv[3]);
	strcpy(archived_file, argv[4]);
	strcat(pathname, "/");
	strcat(pathname,archived_file);
	
	if(argc!=5){
		
		/*  
		 * 	otan dinete san orisma apo thn grammh entolwn
		 *  to '-E' tote ola ta dedomena enos katalogou 
		 *  apo8hkeuontai se ena arxeio archive
		 */
		printf("Wrong number of arguments.\n");
		exit(EXIT_FAILURE);
	}
	if(strcmp(argv[1],"-E")==0){
		fd_file=open(archived_file, O_RDWR | O_EXCL | O_CREAT, S_IRWXU);
		if(fd_file<0){
			if(errno==EEXIST){
				printf("This file already exists!\n");
				return 1;
			}
			else{
				fprintf(stderr,"Error in open: %s in line: %d\n",strerror(errno),__LINE__);
				return 1;
			}
		}
		my_write(fd_file,"P2CRYPTAR",8*sizeof(char)+sizeof(int),__LINE__);
		
		if(pipe(dir_arch_pipe)==-1){
			fprintf(stderr,"Error in pipe: %s in line: %d\n",strerror(errno),__LINE__);
			exit(EXIT_FAILURE);
		}
		if(pipe(arch_encr_pipe)==-1){
			fprintf(stderr,"Error in pipe: %s in line: %d\n",strerror(errno),__LINE__);
			exit(EXIT_FAILURE);
		}
		
		pid_1=fork();
		if(pid_1==-1){
			fprintf(stderr,"Error in fork: %s in line: %d\n",strerror(errno),__LINE__);
			exit(EXIT_FAILURE);
		}
		if(pid_1==0){
// 			lseek(fd_file,9,SEEK_SET);
			dup2(dir_arch_pipe[WRITE_END],STDOUT_FILENO);
			close(dir_arch_pipe[READ_END]);
			close(dir_arch_pipe[WRITE_END]);
			execlp("./dirlist","dirlist",directory,NULL);
			fprintf(stderr,"Error in execlp: %s in line: %d\n",strerror(errno),__LINE__);
			return -1; 
		}
		pid_2=fork();
		if(pid_2==-1){
			fprintf(stderr,"Error in fork: %s in line: %d\n",strerror(errno),__LINE__);
			exit(EXIT_FAILURE);
		}
		if(pid_2==0){
			dup2(dir_arch_pipe[READ_END],STDIN_FILENO);
			dup2(arch_encr_pipe[WRITE_END],STDOUT_FILENO);
			close(dir_arch_pipe[WRITE_END]);
			close(arch_encr_pipe[READ_END]);
			close(arch_encr_pipe[WRITE_END]);
			execlp("./archive","archive",NULL);
			fprintf(stderr,"Error in execlp: %s in line: %d\n",strerror(errno),__LINE__);
			return -1;
		}
		close(dir_arch_pipe[READ_END]);
		close(dir_arch_pipe[WRITE_END]);
		pid_3=fork();
		if(pid_3==-1){
			fprintf(stderr,"Error in fork: %s in line: %d\n",strerror(errno),__LINE__);
			exit(EXIT_FAILURE);
		}
		if(pid_3==0){
			dup2(arch_encr_pipe[READ_END],STDIN_FILENO);
			close(arch_encr_pipe[READ_END]);
			close(arch_encr_pipe[WRITE_END]);
			dup2(fd_file, STDOUT_FILENO);
			execlp("./encrypt","encrypt",password,NULL);
			fprintf(stderr,"Error in execlp: %s in line: %d\n",strerror(errno),__LINE__);
			return -1;
		}	
		close(arch_encr_pipe[WRITE_END]);
		close(arch_encr_pipe[READ_END]);
		/*perimenei na termatistoun oles oi diergasies paidia ,gia na mhn meinoun zombie.*/
		for(i=0;i<3;i++){
			if(waitpid(-1,NULL,0)==-1){
				fprintf(stderr,"waitpid error in line %d: %s",__LINE__,strerror(errno));
				return 1;
			}
		}
	}
	
	/*  otan dinete san orisma apo thn grammh entolwn to '-D',
	 *  tote ola ta dedomena tou arxeiou archive mesw ths 
	 *  crypt apokruptografountai kai mesw ths unarchive 
	 *  dhmiourgeitai enas neos katalogos me arxeia ta dedomena
	 *  pou htan apo8hkeumena sto arxeio archive.
	 */
	if(strcmp(argv[1],"-D")==0){
		fd_file=open(archived_file, O_RDWR, S_IRWXU);
		if(fd_file==-1){
			if(errno==ENOENT){
				printf("This file does not exist.\n");
				return 1;
			}
			else{
				fprintf(stderr,"open error: %s\n",strerror(errno));
				return 1;
			}
		}
		my_read(fd_file,mgn_valid,8*sizeof(char)+sizeof(int),__LINE__);
// 		printf("mgn_valid:%s\n",mgn_valid);
		if(strcmp(mgn_valid,"P2CRYPTAR")!=0){
			fprintf(stderr,"Cannot access this file!\n");
			exit(EXIT_FAILURE);
		}
		else{
			if(pipe(decr_unarch_pipe)==-1){
				fprintf(stderr,"Error in pipe: %s in line: %d\n",strerror(errno),__LINE__);
				exit(EXIT_FAILURE);
			}
			pid_1=fork();
			if(pid_1<0){
				fprintf(stderr,"Error in fork: %s in line: %d\n",strerror(errno),__LINE__);
				exit(EXIT_FAILURE);
			}
			if(pid_1==0){
				dup2(fd_file,STDIN_FILENO);
				close(fd_file);
				dup2(decr_unarch_pipe[WRITE_END],STDOUT_FILENO);
				close(decr_unarch_pipe[WRITE_END]);
 				close(decr_unarch_pipe[READ_END]);
				execlp("./encrypt","encrypt", password,NULL);
				fprintf(stderr,"Error in execlp: %s in line: %d\n",strerror(errno),__LINE__);
				return -1;
			}
			pid_2=fork();
			if(pid_2<0){
				fprintf(stderr,"Error in fork: %s in line: %d\n",strerror(errno),__LINE__);
				exit(EXIT_FAILURE);
			}
			
			if(pid_2==0){
				close(fd_file);
				dup2(decr_unarch_pipe[READ_END],STDIN_FILENO);
 				close(decr_unarch_pipe[WRITE_END]);
				close(decr_unarch_pipe[READ_END]);
				execlp("./unarchive","unarchive",directory,NULL);
				fprintf(stderr,"Error in execlp: %s in line: %d\n",strerror(errno),__LINE__);
				return -1;
			}
			close(fd_file);
			close(decr_unarch_pipe[WRITE_END]);
			close(decr_unarch_pipe[READ_END]);
			for(i=0;i<2;i++){
				if(waitpid(-1,NULL,0)==-1){
					fprintf(stderr,"waitpid error in line %d: %s\n",__LINE__,strerror(errno));
					return 1;
				}
			}
		}
	}
	return 0;	
}
