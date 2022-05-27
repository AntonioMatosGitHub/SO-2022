#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define READ_END 0
#define WRITE_END 1
#define MAXC 1024      
#define NPTRS   2

void replaceWordInText(const char *text, const char *oldWord, const char *newWord) {
   int i = 0, cnt = 0;
   int len1 = strlen(newWord);
   int len2 = strlen(oldWord);
   for (i = 0; text[i] != '\0'; i++) {
      if (strstr(&text[i], oldWord) == &text[i]) {
         cnt++;
         i += len2 - 1;
      }
   }
   char *newString = (char *)malloc(i + cnt * (len1 - len2) + 1);
   i = 0;
   while (*text) {
      if (strstr(text, oldWord) == text) {
         strcpy(&newString[i], newWord);
         i += len1;
         text += len2;
      }
      else
      newString[i++] = *text++;
   }
   printf("New String: %s\n", newString);
}

int main(int argc[], int argv[]){
    
    char buf[MAXC],                 /* fixed buffer to read each line */
        **lines = NULL;             /* pointer to pointer to hold collection of lines */
    size_t  nptrs = NPTRS,          /* number of pointers available */
            used = 0;               /* number of pointers used */
    
    FILE *fp;
    fp = fopen("test.txt","r");
    
    if (!fp) {  /* validate file open for reading */
        perror ("file open failed");
        return 1;
    }
    
    /* allocate/validate block holding initial nptrs pointers */
    if ((lines = malloc (nptrs * sizeof *lines)) == NULL) {
        perror ("malloc-lines");
        exit (EXIT_FAILURE);
    }
    
    while (fgets (buf, MAXC, fp)) {                 /* read each line into buf */
        size_t len;
        buf[(len = strcspn (buf, "\n"))] = 0;       /* trim \n, save length */
        
        if (used == nptrs) {    /* check if realloc of lines needed */
            /* always realloc using temporary pointer (doubling no. of pointers) */
            void *tmp = realloc (lines, (2 * nptrs) * sizeof *lines);
            if (!tmp) {                             /* validate reallocation */
                perror ("realloc-lines");
                break;                              /* don't exit, lines still good */
            }
            lines = tmp;                            /* assign reallocated block to lines */
            nptrs *= 2;                             /* update no. of pointers allocatd */
            /* (optionally) zero all newly allocated memory here */
        }
        
        /* allocate/validate storage for line */
        if (!(lines[used] = malloc (len + 1))) {
            perror ("malloc-lines[used]");
            break;
        }
        memcpy (lines[used], buf, len + 1);         /* copy line from buf to lines[used] */
        
        used += 1;                                  /* increment used pointer count */
    }
    /* (optionally) realloc to 'used' pointers to size no. of pointers exactly here */
    
    if (fp != stdin)   /* close file if not stdin */
        fclose (fp);

    /* use lines as needed (simply outputting here) */
    for (size_t i = 0; i < used; i++) {
        printf (lines[i]);
        free (lines[i]);                            /* free line storage when done */
    }
    
    free (lines);       /* free pointers when done */

    /*int pipe1[2];
    int id = fork();


    if(id > 0){
    close(pipe1[READ_END]);
    write(pipe1[WRITE_END],fp,strlen(input_str) + 1);
    close(pipe1[WRITE_END]);
    wait(NULL);
 
    }
    else{

    }
*/
    return 0;
}

