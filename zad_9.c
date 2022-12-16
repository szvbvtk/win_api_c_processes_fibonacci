#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){
    setvbuf(stdout, NULL, _IOLBF, BUFSIZ);

    if(argc != 2){ // nazwa (która jest zawsze pierwszym argumentem programu) + jeden argument
        fprintf(stderr, "Nieprawidłowa ilość argumentów\n"); // błąd na strumień stderr
        return 1;
    }

    char *endptr;
    int argument = strtol(argv[1], &endptr, 10); // jeśli argument1 (argv[1]) nie jest liczbą to do wskaźnika endptr zostanie przekazane miejsce, w którym pojawił się znak argumentu inny niż cyfra
    // w przeciwnym wypadku endptr = koniec tablicy znaków (argv[1] to tablica znaków) czyli '\0'
    
    if(*endptr != '\0'){ // jeśli funkcja strtol napotkała problem przy próbie konwersji
        fprintf(stderr, "Podan argument nie jest liczbą\n");
        return 2;
    }
    
    if(argument < 1 || argument > 13){
        fprintf(stderr, "Argument nie jest liczbą z przedziału <1,13>\n");
        return 3;
    }

    if(argument == 1 || argument == 2){
        return 1; // wynika z zasady działania ciągu fibonacciego
    }
        	STARTUPINFO si;
	        PROCESS_INFORMATION pi[2];

            char argline1[50];
            char argline2[50];

            memset(&si, 0, sizeof(si));
	        memset(&pi, 0, sizeof(pi));	
	        si.cb = sizeof(si);

            sprintf(argline1, "%s %d", argv[0], (argument - 1));
            sprintf(argline2, "%s %d", argv[0], (argument - 2));

            CreateProcessA(NULL, argline1, NULL, NULL, 0, 0, NULL, NULL, &si, pi + 0);
            CreateProcessA(NULL, argline2, NULL, NULL, 0, 0, NULL, NULL, &si, pi + 1);

            HANDLE children_handle[2] = {pi[0].hProcess, pi[1].hProcess};
	        WaitForMultipleObjects(2, children_handle, 1, INFINITE);

            DWORD child1_exit_code = 0;
            DWORD child2_exit_code = 0;
            GetExitCodeProcess(pi[0].hProcess, &child1_exit_code);
            GetExitCodeProcess(pi[1].hProcess, &child2_exit_code);

            DWORD processId = GetCurrentProcessId();
            DWORD child1_processId = pi[0].dwProcessId;
            DWORD child2_processId = pi[1].dwProcessId;

            fprintf(stdout, "%lu %lu %lu %lu\n", processId, child1_processId, argument - 1, child1_exit_code);
            fprintf(stdout, "%lu %lu %lu %lu\n", processId, child2_processId, argument - 2, child2_exit_code);
            fprintf(stdout, "%lu %lu\n\n", processId, child1_exit_code + child2_exit_code);

            CloseHandle( pi[1].hProcess );
		    CloseHandle( pi[1].hThread );
            CloseHandle( pi[0].hProcess );
		    CloseHandle( pi[0].hThread );


            return child1_exit_code + child2_exit_code;

}