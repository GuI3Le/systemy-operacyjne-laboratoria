#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <fcntl.h>

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
		//printf("%c\n",shared_memory[200]);
		printf("shared memory attached, key=%ld\n",key);
		int fd=open(filename, O_RDONLY);
		if(fd < 0){
			perror("Cannot open file");
			return -1;
		}
		printf("waiting for reciver...\n");
		

		//ssize_t bytes_read = read(fd, shared_memory,100);
		//printf("%d byte(s) sent..\n", bytes_read);
		while(shared_memory[100]!=101){
		}
		ssize_t bytes_read=100;
		while(bytes_read==100){
			bytes_read = read(fd, shared_memory,100);
			printf("%d byte(s) sent..\n", bytes_read);
			while(shared_memory[100]!=103){}
		}
		
		close(fd);
		shmdt(shared_memory);
		printf("shared memory detached\n");
		shmctl(shmid, IPC_RMID, NULL);
		//printf("ready to write\n");
	}
	return 0;
}
