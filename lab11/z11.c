#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define MAX_LINES 370200

char **words;
int    words_cnt;

int main(int argc, char *argv[]) {
	words = malloc(sizeof(char *) * MAX_LINES);
	FILE *f = fopen("words_alpha.txt", "r");
	assert(f != NULL);
	char line[255];
	for(words_cnt = 0; words_cnt < MAX_LINES; words_cnt++)
		if(fgets(line, sizeof(line), f)){
			words[words_cnt] = strdup(line);
		//	printf("%s", line);
		}
		else
			break;
	fclose(f);
	printf("lines in file: %d\n", words_cnt);
	unsigned int w =  atoi(argv[1]);
	if(w==0){
	}
	else if(w<=16){
	}
	printf("%d\n",w);
	for(int i = 0; i < words_cnt; i++)
		free(words[i]);
	free(words);
	return 0;
}
