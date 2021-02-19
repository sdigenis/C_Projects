#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#define SIZE 25
#define ANSI_COLOR_BLUE    "\x1b[34m"
char game[SIZE][SIZE];
char display[SIZE][SIZE];

int decl(){
	int i, j;
	for (i=0; i<SIZE; i++){
		for(j=0; j<SIZE; j++){
			game[i][j]='0';
			display[i][j]='-';
		}
	}
	return 0;
}

int check(){
	int i, j;
	for(i=0; i<SIZE; i++){
		for(j=0; j<SIZE; j++){
			if(game[i][j]=='*'){
				if(game[i][j]==display[i][j]){
					break;
				}
				break;
			}
			
		}
	}
	return 0;
}

int showE(){
	int k, a, b ;
	
	printf("   ");
		for(k=1; k<=6; k++){
			printf(" %d ", k);
		}
		for(a=1; a<=6; a++){
			printf("\n");
			printf(" %d ", a);
			for(b=1; b<=6; b++){
				printf(" %c ", display[a][b]);
			}
		}
	printf("\n");
	return 0;
}

int showM(){
	int k, a, b ;
	
		printf("   ");
	for(k=1; k<=12; k++){
		if(k<10){
		printf(" %d ", k);
		}
		else{
			printf("%d ", k);
		}
	}
	for(a=1; a<=12; a++){
		printf("\n");
		if(a<10){
			printf(" %d ", a);
		}
		else{
			printf("%d ", a);
		}
		for(b=1; b<=12; b++){
			printf(" %c ", display[a][b]);
		}
	}
	printf("\n");
	return 0;
}

int showX(){
	int k, a, b;
	printf("   ");
	for(k=1; k<=24; k++){
		if(k<10){
			printf(" %d ", k);
		}
		else{
			printf("%d ", k);
		}
	}
	for(a=1; a<=24; a++){
		printf("\n");
		if(a<10){
			printf(" %d ", a);
		}
		else{
			printf("%d ", a);
		}
		for(b=1; b<=24; b++){
			printf(" %c ", display[a][b]);
		}
	}
	printf("\n");
	return 0;
}

int solutionE(){
	int k, i, j;
	
	for(k=1; k<=6; k++){
		printf(" %d ", k);
	}
	for(i=1; i<=6; i++){
		printf("\n");
		printf(" %d ", i);
		for(j=1; j<=6; j++){
			printf(" %c ", game[i][j]);
		}
	}
	return 0;
}

int solutionM(){
	int k, i, j;
	
	for(k=1; k<=12; k++){
		if(k<10){
			printf(" %d ", k);
		}
		else {
			printf("%d ", k);
		}
	}
	for(i=1; i<=12; i++){
		printf("\n");
		if(i<10){
			printf(" %d ", i);
		}
		else{
			printf("%d ", i);
		}
		for(j=1; j<=12; j++){
			printf(" %c ", game[i][j]);
		}
	}
	return 0;
}

int solutionX(){
	int k, i, j;
	for(k=1; k<=24; k++){
		if(k<10){
			printf(" %d ", k);
		}
		else {
			printf("%d ", k);
		}
	}
	for(i=1; i<=24; i++){
		printf("\n");
		if(i<10){
			printf(" %d ", i);
		}
		else{
			printf("%d ", i);
		}
		for(j=1; j<=24; j++){
			printf(" %c ", game[i][j]);
		}
	}
	return 0;
}

int main(int argc, char *argv[]){
	
	int k, i, j, mines, r, c, a;
	char level, pali;
	
	srand(time(NULL));
	while(3){
		while(1){
			decl();
			printf("1. EASY (E,e)\n2. MEDIUM (M,m)\n3. EXPERT (X,x)\n");
			printf("Choose difficulty level: ");
			scanf(" %c", &level);
			printf("   ");
			if(level=='E' || level=='e'){
				for(k=1; k<=6; k++){
					printf(" %d ", k);
				}
				for(i=1; i<=6; i++){
					printf("\n");
					printf(" %d ", i);
					for(j=1; j<=6; j++){
						printf(" %c ", display[i][j]);
					}
				}
				mines=6*6*20/100;
				break;
			}
			else if(level=='M' || level=='m'){
				for(k=1; k<=12; k++){
					if(k<10){
						printf(" %d ", k);
					}
					else {
						printf("%d ", k);
					}
				}
				for(i=1; i<=12; i++){
					printf("\n");
					if(i<10){
						printf(" %d ", i);
					}
					else{
						printf("%d ", i);
					}
					for(j=1; j<=12; j++){
						printf(" %c ", display[i][j]);
					}
				}
				mines=12*12*20/100;
				break;
			}
			else if(level=='X' || level=='x'){
				for(k=1; k<=24; k++){
					if(k<10){
						printf(" %d ", k);
					}
					else {
						printf("%d ", k);
					}
				}
				for(i=1; i<=24; i++){
					printf("\n");
					if(i<10){
						printf(" %d ", i);
					}
						else{
							printf("%d ", i);
						}
					for(j=1; j<=24; j++){
							printf(" %c ", display[i][j]);
					}
				}
					mines=24*24*20/100;
					break;
				}
				else {
					printf("Try again!!\n");
				}
		}
		if(level=='E' || level=='e'){
			a=0;
			for(i=0; i<100; i++){
				r=(rand()%6+1);
				c=(rand()%6+1);
				game[r][c]='*';
				if(game[r][c]=='*'){
					a++;
				}
				else{
					continue;
				}
				if(a==mines){
					break;
				}
			}
		}
		if(level=='M' || level=='m'){
			a=0;
			for(i=0; i<100; i++){
				r=(rand()%12+1);
				c=(rand()%12+1);
				game[r][c]='*';
				if(game[r][c]=='*'){
					a++;
				}
				else{
					continue;
				}
				if(a==mines){
					break;
				}
			}
		}
		if(level=='X' || level=='x'){
			a=0;
			for(i=0; i<mines; i++){
				r=(rand()%24+1);
				c=(rand()%24+1);
				game[r][c]='*';
				if(game[r][c]=='*'){
					a++;
				}
				else{
					continue;
				}
				if(a==mines){
					break;
				}
			}
		}
		if(level=='E' || level=='e'){
			for(i=0; i<=6; i++){
				for(j=0; j<=6; j++){
					if(game[i-1][j-1]=='*' && game[i][j]!='*' && i!=0 && j!=0){
						game[i][j]++;
					}
					if(game[i][j-1]=='*' && game[i][j]!='*' && i!=0){
						game[i][j]++;
					}
					if(game[i][j+1]=='*' && game[i][j]!='*'){
						game[i][j]++;
					}
					if(game[i-1][j]=='*' && game[i][j]!='*' && j!=0){
						game[i][j]++;
						}
					if(game[i+1][j]=='*' && game[i][j]!='*'){
						game[i][j]++;
					}
					if(game[i+1][j+1]=='*' && game[i][j]!='*'){
							game[i][j]++;
					}
					if(game[i+1][j-1]=='*' && game[i][j]!='*' && i!=0){
						game[i][j]++;
					}
					if(game[i-1][j+1]=='*' && game[i][j]!='*' && j!=0){
							game[i][j]++;
					}
				}
			}
		}
		if(level=='M' || level=='m'){
			for(i=0; i<=12; i++){
				for(j=0; j<=12; j++){
					if(game[i-1][j-1]=='*' && game[i][j]!='*' && i!=0 && j!=0){
						game[i][j]++;
					}
					if(game[i][j-1]=='*' && game[i][j]!='*' && i!=0){
						game[i][j]++;
					}
					if(game[i][j+1]=='*' && game[i][j]!='*'){
						game[i][j]++;
					}
					if(game[i-1][j]=='*' && game[i][j]!='*' && j!=0){
						game[i][j]++;
					}
					if(game[i+1][j]=='*' && game[i][j]!='*'){
						game[i][j]++;
					}
					if(game[i+1][j+1]=='*' && game[i][j]!='*'){
						game[i][j]++;
					}
					if(game[i+1][j-1]=='*' && game[i][j]!='*' && i!=0){
						game[i][j]++;
					}
					if(game[i-1][j+1]=='*' && game[i][j]!='*' && j!=0){
						game[i][j]++;
				}
				}
			}
			}
		if(level=='X' || level=='x'){
			for(i=0; i<=24; i++){
				for(j=0; j<=24; j++){
					if(game[i-1][j-1]=='*' && game[i][j]!='*' && i!=0 && j!=0){
						game[i][j]++;
					}
					if(game[i][j-1]=='*' && game[i][j]!='*' && i!=0){
						game[i][j]++;
					}
					if(game[i][j+1]=='*' && game[i][j]!='*'){
						game[i][j]++;
					}
					if(game[i-1][j]=='*' && game[i][j]!='*' && j!=0){
						game[i][j]++;
						}
					if(game[i+1][j]=='*' && game[i][j]!='*'){
						game[i][j]++;
					}
					if(game[i+1][j+1]=='*' && game[i][j]!='*'){
						game[i][j]++;
					}
					if(game[i+1][j-1]=='*' && game[i][j]!='*' && i!=0){
						game[i][j]++;
					}
					if(game[i-1][j+1]=='*' && game[i][j]!='*' && j!=0){
						game[i][j]++;
					}
				}
			}
		}
		printf("\n   ");
		printf("\n\n");
		if(level=='E' || level=='e'){
			for(i=0; i<6*6-mines; i++){
				printf("\033[22;34mRow= \033[0m");
				scanf(" %d", &r);
				printf("\033[22;34mCol= \033[0m");
				scanf( " %d", &c);
				if(r>6 || r<1){
					printf("Invalid %d. Try again!\n", r );
				}
				if(c>6 || c<1){
				printf("Invalid %d. Try again!\n", c);
				}
				display[r][c]=game[r][c];
				if(display[r][c]=='*'){
						printf("Oops! You have selected a mine.\n");
						break;
				}
				showE();
				printf("\n");
			}
		}
		if(level=='M' || level=='m'){
			for(i=0; i<12*12-mines-1; i++){
				printf("\033[22;34mRow= \033[0m");
				scanf(" %d", &r);
				printf("\033[22;34mCol= \033[0m");
				scanf( " %d", &c);
				if(r>12 || r<1){
					printf("Invalid %d. Try again!\n", r );
				}
				if(c>12 || c<1){
					printf("Invalid %d. Try again!\n", c);
				}
				display[r][c]=game[r][c];
				if(display[r][c]=='*'){
						printf("Oops! You have selected a mine.\n");
						break;
				}
				check();
				showM();
				printf("\n");
			}
		}
		if(level=='X' || level=='x'){
			for(i=0; i<24*24-mines-1; i++){
				printf("\033[22;34mRow= \033[0m");
				scanf(" %d", &r);
				printf("\033[22;34mCol= \033[0m");
				scanf( " %d", &c);
				if(r>24 || r<1){
					printf("Invalid %d. Try again!\n", r );
				}
				if(c>24 || c<1){
				printf("Invalid %d. Try again!\n", c);
				}
				display[r][c]=game[r][c];
				if(display[r][c]=='*'){
						printf("Oops! You have selected a mine.\n");
						break;
				}
				check();
				showX();
				printf("\n");
			}
		}
		if((level=='E' || level=='e') && i==6*6-mines){
			printf("Congratulations! You have solved the puzzle! ");
		}
		if((level=='M' || level=='m') && i==12*12-mines){
			printf("Congratulations! You have solved the puzzle! ");
		}
		if((level=='X' || level=='x') && i==24*24-mines){
			printf("Congratulations! You have solved the puzzle! ");
		}
		printf("\n");
		printf("   ");
		if(level=='E' || level=='e'){
			solutionE();
		}
		else if (level=='M' || level=='m'){
			solutionM();
		}
		else {
			solutionX();
		}
		printf("\n\n");
		while(2){
			printf("Play again? (y/n) ");
			scanf(" %c", &pali);
			if(pali=='y'){
				break;
			}
			else if(pali=='n'){
				break;
			}
			else{
				printf("Try again!\n");
			}
		}
		if(pali=='n'){
				break;
		}
		if(pali=='y'){
			continue;
		}
	}
	printf("\n");
	return 0;
}