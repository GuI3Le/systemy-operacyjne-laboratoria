#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main(int argc, char *argv[]){
	if(argc>0){
	//key_t key;
	const char* filename = argv[1];
	char *shared_memory;
	//printf("%s\n",file);
	int id = 'S';
	key_t key = ftok(filename,id);
	int shmid = shmget(key, 101, 0666 | IPC_CREAT);
	shared_memory  = (char *)shmat(shmid,NULL,0);

	printf("shared memory attached, key=%ld\n",key);
	FILE *file=fopen(filename, "rb");
	if(file = NULL){
		perror("Cannot open file");
		return -1;
	}
	printf("waiting for reciver...\n");
	while(shared_memory[0] == 0){
	}
	printf("ready to write\n");
	}
	return 0;
}
