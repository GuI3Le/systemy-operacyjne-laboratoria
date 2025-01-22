#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <fcntl.h>


int main(int argc, char *argv[]){
	if(argc>0){
		const char* filename = argv[1];
		unsigned char *shared_memory;
		int id = 'S';
		key_t key = ftok(filename,id);
		int shmid = shmget(key, 101, 0666 | IPC_CREAT);
		shared_memory  = (unsigned char *)shmat(shmid,NULL,0);
		printf("shared memory attached, key=%ld\n",key);
		int fd=open(filename, O_RDONLY);
		if(fd < 0){
			perror("Cannot open file");
			return -1;
		}
		printf("waiting for reciver...\n");
		shared_memory[0] = 0;
		ssize_t bytes_read = 0;
		while(shared_memory[0]!=101){}
		do{
                bytes_read = read(fd, shared_memory+1,100);
                shared_memory[0]=bytes_read;
         		printf("%ld byte(s) sent...\n", bytes_read);
         		while(shared_memory[0]!=102){}
		}while(bytes_read==100);
		shared_memory[0]=200;
		close(fd);
		shmdt(shared_memory);
		printf("shared memory detached\n");
		shmctl(shmid, IPC_RMID, NULL);
	}
	return 0;
}
