
#include <stdio.h>

int main(int argc, char** argv) {

    if (argc != 3) {
        printf("Not enough / too much arguments given \n");
        printf("Follow this: 'gettftp host file' \n");
        
        return 1;   // End the program with an error
    }

    char *host = argv[1];
    char *file = argv[2];

    printf("%s", host);
    printf("%s", file);

    return 0;
}