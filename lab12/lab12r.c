#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]){
	if(argc>1){
		key_t key = atoi(argv[1]);
		const char *filename = argv[2];
		int fd = open(filename, O_WRONLY|O_TRUNC);
		int shmid = shmget(key,101, 0666);
		unsigned char *shared_memory = (unsigned char *)shmat(shmid,NULL,0);
		shared_memory[0]=101;
		while(shared_memory[0]!=200){
			while(shared_memory[0]>100 && shared_memory[0]!=200){
			}
			if (shared_memory[0]==200) break;
			write(fd, shared_memory+1, shared_memory[0]);
			printf("%d byte(s) received...\n", shared_memory[0]);
			shared_memory[0]=102;
		}
		printf("copying finished\n");
		close(fd);
		shmdt(shared_memory);
		printf("shared memory detached\n");
	}
	return 0;
}
