#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

int main(int argc, char* argv[]) {

    FILE *file;
    char c;
    int phrases = 1;
    file = fopen(argv[1], "r");
    bool flag = false;

    if(argc == 3){
        if (strcmp(argv[2],"-l")==0){
            printf("[%d] ", phrases);
            while((c = getc(file))!=EOF){
                if (flag && c != '\n'){
                    phrases++;
                    printf("\n[%d] ", phrases);
                }
                if (c != '\n') printf("%c", c);
                if(c == '.' || c == '?' || c == '!'){
                    flag = true;
                } else if (c != '\n'){
                    flag = false;
                }
            }
        }
    }

    else {
        while((c = getc(file))!=EOF){
                if (flag && c != '\n'){
                    phrases++;
                }
                if(c == '.' || c == '?' || c == '!'){
                    flag = true;
                } else if (c != '\n'){
                    flag = false;
                }
            }
        printf("%d\n",phrases);
    }
    
    fclose(file);

    return 0;    

}

