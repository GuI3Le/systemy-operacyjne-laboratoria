#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <windows.h>

#define MAX_LINES 370200

char **words;
int    words_cnt;
int letters_cnt[26]={0};
HANDLE hMutex;

typedef struct Params {
    int start;
    int end;
}Params;

void print_letters_cnt() {
    for (int i = 0; i < 26; i++) {
        printf("%c : %d\t",i+97, letters_cnt[i]);
        if ((i+1)%5==0) {
            printf("\n");
        }
    }
}

DWORD WINAPI thread_counting(LPVOID lpParam) {
    Params *thread_params = (Params *)lpParam;
    int thread_letters_cnt[26]={0};
    //int thread_word_cnt = thread_params->end - thread_params->start+1;
    printf("Thread %lu started start=%u, end=%u\n",GetCurrentThreadId(),thread_params->start, thread_params->end);
    for(int i=thread_params->start; i<=thread_params->end;i++) {
        for (int j=0;words[i][j]!=10;j++) {
            thread_letters_cnt[words[i][j]-97]++;
            //printf("%c", words[i][j]);
        }
    }
    WaitForSingleObject(hMutex,INFINITE);
    for(int i=0;i<26;i++) {
        letters_cnt[i]+=thread_letters_cnt[i];
    }
    ReleaseMutex(hMutex);
    return 0;
}


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
    clock_t t1 = 0;
    clock_t t2 = 0;
    if(w==0){
        t1 = clock();
        for(int i=0; i<words_cnt;i++) {
            for (int j=0;words[i][j]!=10;j++) {
                letters_cnt[words[i][j]-97]++;
                //printf("%c", words[i][j]);
            }
        }
        t2 = clock();
    }
    else if(w<=16){
        struct Params params[w];
        unsigned int offset = (words_cnt / w);
        unsigned int start = 0;
        unsigned int end = offset-1;
        //end--;
        //offset--;
        //struct Params params = { start, end }; // nie bedzie dzialac?
        HANDLE hThread[w];
        DWORD dwThreadId[w];
        hMutex = CreateMutex(NULL,FALSE,NULL);
        t1 = clock();
        for (int i = 0; i < w-1; ++i) {
            params[i].start = start;
            params[i].end = end;
            hThread[i] = CreateThread(
                NULL,
                0,
                thread_counting,
                &params[i],
                0,
                &dwThreadId[i]
            );
            start += offset;
            //start=start + i;
            end += offset;
            //end++;
        }
        int mod = words_cnt % w;
        if (mod  != 0) {
            end += mod;
        }
        params[w-1].start = start;
        params[w-1].end = end;
        hThread[w-1] = CreateThread(
            NULL,
            0,
            thread_counting,
            &params[w-1],
            0,
            &dwThreadId[w-1]
        );

        WaitForMultipleObjects(w, hThread,TRUE, INFINITE);
        t2 = clock();
        for (int i = 0; i < w; ++i) {
            CloseHandle(hThread[i]);
        }

    }
    //clock_t t2 = clock();
    double t3 = (double)(t2-t1)/CLOCKS_PER_SEC;
    printf("Execution time: %f seconds\n", t3);
    print_letters_cnt();
    CloseHandle(hMutex);
    for(int i = 0; i < words_cnt; i++)
        free(words[i]);
    free(words);
    return 0;
}
