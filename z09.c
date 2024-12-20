#include <stdio.h>
#include <windows.h>
#include <ctype.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        fprintf(stderr, "error, program called without arguments\n");
        return 201;
    }
    for (int i = 1; i < argc; i++)
    {
	char* argv_el = argv[i]; 
	while(*argv_el)
	{
        if (!isdigit((unsigned char)*argv_el))
        {
            fprintf(stderr, "not an uint\n");
            return 202;
        }
	argv_el+=1;
	}
	int number = atoi(argv[i]);
        if (number > 100)
        {
            fprintf(stderr, "number out of range [0,100]\n");
            return 202;
        }
		
    }
    if (argc == 2)
    {
        return atoi(argv[1]);
    }
    if (argc == 3)
    {
        if (atoi(argv[1]) >= atoi(argv[2]))
        {
            return atoi(argv[1]);
        }
        return atoi(argv[2]);
    }
    int size1 = argc / 2 + 2;
    int size2 = argc / 2 + 2;
    if (argc % 2 == 0)
    {
        size1 -= 1;
    }
    char *array1[size1];
    char *array2[size2];
    array1[0] = argv[0];
    array2[0] = argv[0];
    for (int i = 1; i < size1 - 1; i++)
    {
        array1[i] = argv[i];
        array2[i] = argv[size1 + i - 2];
    }
    if (argc % 2 == 0)
    {
        array2[size2 - 2] = argv[argc - 1];
    }
    char command1[1024];
    command1[0] = '\0';
    for(int i=0;i<size1-1;i++)
    {
        strcat(command1, array1[i]);
        if (i < size1 - 2)
        {
            strcat(command1, " ");
        }
    }
    printf("Command1: %s|\n",command1);
    char command2[1024];
    command2[0]='\0';
    for(int i=0;i<size2-1;i++)
    {
	    strcat(command2,array2[i]);
	    if(i<size2-2)
	    {
	    	strcat(command2," ");
		}
    }
    printf("Command2: %s|\n",command2);
    PROCESS_INFORMATION pi;
    STARTUPINFO si;
    ZeroMemory(&pi, sizeof(pi));
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    if (!CreateProcessA(
	NULL,
	command1,
	NULL,
	NULL,
	FALSE,
	0,
	NULL,
	NULL,
	&si,
	&pi
            ))
    {
	    printf("Process1 creation failed. Error: %d\n",GetLastError());
	    return 0;
    }
    // process 2
    PROCESS_INFORMATION pi2;
    STARTUPINFO si2;
    ZeroMemory(&pi2, sizeof(pi2));
    ZeroMemory(&si2, sizeof(si2));
    si2.cb = sizeof(si2);
    if (!CreateProcessA(
	NULL,
	command2,
	NULL,
	NULL,
	FALSE,
	0,
	NULL,
	NULL,
	&si2,
	&pi2
            ))
    {
	    printf("Process2 creation failed. Error: %d\n",GetLastError());
	    return 0;
    }

    HANDLE handles[2];
    handles[0]=pi.hProcess;
    handles[1]=pi2.hProcess;
    DWORD waitResult = WaitForMultipleObjects(
	2,
	handles,
	TRUE,
	INFINITE
	);
    DWORD c1ExitCode, c2ExitCode;
    GetExitCodeProcess(pi.hProcess,&c1ExitCode);
    GetExitCodeProcess(pi2.hProcess, &c2ExitCode);
    printf("\n%d\t%d\t%d | ", GetCurrentProcessId(), pi.dwProcessId, c1ExitCode);
    for(int i=1;i<size1-1;i++)
    {
	    printf("%s ", array1[i]);
    }
    printf("\n");
    printf("%d\t%d\t%d | ",GetCurrentProcessId(),pi2.dwProcessId,c2ExitCode);
    for(int i=1;i<size2-1;i++)
    {
	    printf("%s ", array2[i]);
    }
    printf("\n");
    int return_code = 0;
    if(c1ExitCode>c2ExitCode)
    {
	    return_code = c1ExitCode;
    }else
    {
	    return_code = c2ExitCode;
    }
    printf("%d\t\t%d\n",GetCurrentProcessId(),return_code);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    CloseHandle(pi2.hProcess);
    CloseHandle(pi2.hThread);
        return return_code;
}
