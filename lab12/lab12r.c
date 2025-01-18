#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main(int argc, char *argv[]){
	if(argc>1){
		key_t key = atoi(argv[1]);
		const char *filename = argv[2];
		int fd = open(filename, O_READLY);
		int shmid = shmget(key,101, 0666);
		char *shared_memory = (char *)shmat(shmid,NULL,0);
		shared_memory[100]=101;
		while(true){
			if(shared_memory[100]==255) break;
			while(shared_memory[100]!=102){}
		}
		printf("copying finished\n");
		close(fd);
		shmdt(shared_memory);
		printf("shared memory detached\n");

		

	}
	return 0;
}

