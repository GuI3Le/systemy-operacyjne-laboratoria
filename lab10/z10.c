#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <pthread.h>

#define MAX_LINES 370200

char **words;
int    words_cnt;
int letters_cnt[26]={0};
pthread_mutex_t lock ;

typedef struct Params {
    unsigned int start;
    unsigned int end;
}Params;

void print_letters_cnt() {
    for (int i = 0; i < 26; i++) {
        printf("%c : %d\t",i+97, letters_cnt[i]);
        if ((i+1)%5==0) {
            printf("\n");
        }
    }
    printf("\n");
}

void* thread_counting(void* param) {
    Params* params = (Params*) param;
    int thread_letters_cnt[26]={0};
    unsigned long thread_id = pthread_self();
    printf("Thread #%lu started start=%u, end=%u\n",thread_id,params->start,params->end);
    for(int i=params->start; i<=params->end;i++) {
        for (int j=0;words[i][j]!=10;j++) {
            thread_letters_cnt[words[i][j]-97]++;
        }
    }
    pthread_mutex_lock(&lock);
    for (int i = 0; i < 26; ++i) {
        letters_cnt[i]+=thread_letters_cnt[i];
    }
    pthread_mutex_unlock(&lock);
    printf("Thread #%lu stopped\n",thread_id);
}

int main(int argc, char *argv[]) {
    unsigned int w =  atoi(argv[1]);
    if (w<=16) {
    words = malloc(sizeof(char *) * MAX_LINES);
    FILE *f = fopen("words_alpha.txt", "r");
    assert(f != NULL);
    char line[255];
    for(words_cnt = 0; words_cnt < MAX_LINES; words_cnt++)
        if(fgets(line, sizeof(line), f)){
            words[words_cnt] = strdup(line);
        }
        else
            break;
    fclose(f);
    printf("lines in file: %d\n", words_cnt);
    struct timespec t1;
    struct timespec t2;
        if(w==0){
            clock_gettime(CLOCK_REALTIME, &t1);

            for(int i=0; i<words_cnt;i++) {
                for (int j=0;words[i][j]!='\r';j++) {
                    letters_cnt[words[i][j]-97]++;
                }
            }
            clock_gettime(CLOCK_REALTIME, &t2);
        }
        else {
            Params params[w]={};
            pthread_t threads[w];
            unsigned int offset = words_cnt / w;
            unsigned int start = 0;
            unsigned int end = offset-1;
            clock_gettime(CLOCK_REALTIME,&t1);
            for (int i = 0; i < w-1; ++i) {
                params[i].start = start;
                params[i].end = end;
                pthread_create(&threads[i],NULL,thread_counting,&params[i]);
                start +=offset;
                end+=offset;
            }

            int mod = words_cnt % w;
            if (mod  != 0) {
                end += mod;
            }
            params[w-1].start = start;
            params[w-1].end = end;
            pthread_create(&threads[w-1],NULL,thread_counting,&params[w-1]);
            params[w-1].start = start;
            params[w-1].end = end;
            for (int i = 0; i < w; ++i) {
                pthread_join(threads[i],NULL);
            }
            clock_gettime(CLOCK_REALTIME,&t2);
        }
        double t3 = (t2.tv_sec - t1.tv_sec)+(t2.tv_nsec-t1.tv_nsec)/1e9;
        printf("Execution time: %f s\n", t3);
        print_letters_cnt();
        for(int i = 0; i < words_cnt; i++)
            free(words[i]);
        free(words);
    }
    return 0;
}
