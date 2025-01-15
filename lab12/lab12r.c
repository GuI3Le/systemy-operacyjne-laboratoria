#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main(int argc, char *argv[]){
	if(argc>1){
		key_t key = atoi(argv[1]);
		const char *filename = argv[2];
		FILE *file = fopen(filename, "wb");
		int shmid = shmget(key,101, 0666);
		char *shared_memory = (char *)shmat(shmid,NULL,0);
		shared_memory[0]=1;
	}
	return 0;
}

