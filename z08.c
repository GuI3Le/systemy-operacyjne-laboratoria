#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
int main(int argc, char *argv[]){
	//printf("argc: %d\n",argc);
	if(argc ==1){
		fprintf(stderr, "error, program called without arguments\n");
		return 201;
	}
		for(int i=1;i<argc;i++){
			if(!isdigit(*argv[i])){
				fprintf(stderr,"nie jest liczba");
				return 202;
			}
			if(*argv[i]>100 || *argv[i]<0){
				fprintf(stderr,"number out of range [0,100]");
				return 202;
			}
		}
	if(argc==2) {
		//printf("%s\n",argv[1]);
		return atoi(argv[1]);
	}
	if(argc==3){
		//printf("%d\n",atoi(argv[2]));
		if(atoi(argv[1])>=atoi(argv[2])){
			//printf("%s\n",argv[1]);
			return atoi(argv[1]);
		}
		//printf("%s\n",argv[2]);
		return atoi(argv[2]);
	}
	//if((argc-1)%2==0){
		//printf("argc: %d",argc);
		int size1 = argc/2+2;
		int size2 = argc/2+2;
		if(argc%2==0){
			//printf("nie parzysta");
			size1-=1;
		}
		char *array1[size1];
		char *array2[size2];
		array1[0]=argv[0];
		array2[0]=argv[0];
		//size1-=1;
		//size2-=1;
		for(int i=1;i<size1-1;i++){
			array1[i]=argv[i];
			array2[i]=argv[size1+i-2];
			//printf("arr2: %s\n",array2[i]);

		}
		if(argc%2==0){
			array2[size2-2]=argv[argc-1];
		}
		array1[size1-1]=NULL;
		array2[size2-1]=NULL;

		/*
		fork();
		int cpid1 = getpid();
		fork();
		int cpid2 = getpid();
        	int flag = execvp(argv[1],array1);
		*/
		pid_t cpid1 = fork();
		/*
		if(cpid1<0||cpid2<0){
			printf("fork failed");
			return 203;
		}*/
		if(cpid1==0){
			// child1
			//printf("c1\n");
			execv(argv[0],array1);
		}
		pid_t cpid2 = fork();

		if(cpid2==0){
			// child2
			//printf("c2\n");
			execv(argv[0],array2);
		}
		if(cpid1>0 && cpid2>0){
			// parent
			int status1;
			int status2;
			waitpid(cpid1,&status1,0);
			waitpid(cpid2,&status2,0);
			//printf("%d\t%d\t%d\t%s\n",getpid(),cpid1,flag,*array1);
			printf("\n%d\t%d\t%d | ",getpid(),cpid1,WEXITSTATUS(status1));
			for(int i=1;i<size1-1;i++){
				printf("%s ",array1[i]);
			}
			printf("\n%d\t%d\t%d | ",getpid(),cpid2,WEXITSTATUS(status2));
			for(int i=1;i<size2-1;i++){
				printf("%s ",array2[i]);
			}
			printf("\n");
			
			int return_code =0;
			if(status1>status2){
				return_code = WEXITSTATUS(status1);
			}else{
				return_code = WEXITSTATUS(status2);
			}
			printf("%d\t\t%d\n",getpid(),return_code);
			return return_code;
		}
	//}
	return 0;
}
