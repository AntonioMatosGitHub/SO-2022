#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
    //abrir e ler os dois ficheiros
    char *infile1 = argv[1];
    char *infile2 = argv[2];

    FILE *fp;
    if ((fp = fopen(infile1, "r")) == NULL)
    {
        perror("cannot open file");
        exit(EXIT_FAILURE);
    }
    size_t str_size = 0;
    char *str = NULL;

    getline(&str, &str_size, fp); // vai buscar a primeira linha
    int rows = atoi(str);
    int cols = atoi(str + 2); // como quer int aponta para a posiçao str+2
    int matrix1[rows][cols];
    int i = 0, j = 0;
    while (getline(&str, &str_size, fp) > 0)
    {
        char *token = strtok(str, " ");
        while (token != NULL)
        {
            matrix1[i][j] = atoi(token);
            token = strtok(NULL, " ");
            j++;
        }
        i++;
        j = 0;
    }
    fclose(fp);
    

    if ((fp = fopen(infile2, "r")) == NULL)
    {
        perror("cannot open file");
        exit(EXIT_FAILURE);
    }
    str_size = 0;
    str = NULL;

    getline(&str, &str_size, fp); // vai buscar a primeira linha
    rows = atoi(str);
    cols = atoi(str + 2); // como quer int aponta para a posiçao str+2
    int matrix2[rows][cols];
    i = 0, j = 0;
    while (getline(&str, &str_size, fp) > 0)
    {
        char *token = strtok(str, " ");
        while (token != NULL)
        {
            matrix2[i][j] = atoi(token);
            token = strtok(NULL, " ");
            j++;
        }
        i++;
        j = 0;
    }
    fclose(fp);

    int nProcs = cols;

    // memoria partilhada
    int *matrixrR = mmap(NULL, rows * cols * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, 0, 0);

    if (matrixrR == MAP_FAILED)
    {
        perror("mmap");
        exit(EXIT_FAILURE);
    }
    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < cols; j++)
        {
            matrixrR[i * cols + j] = 0;
        }
    }
   
    printf("%dx%d\n",rows, cols);
    
    int id = fork();
    if (id > 0)
    {
        wait(NULL); // wait for everybody

        for (i = 0; i < rows; i++)
        {
            for (j = 0; j < cols; j++)
            {
                printf("%d ", matrixrR[i * cols + j]);
            }
            printf("\n");
        }
    }
    else
        for (int m = 0; m < nProcs - 1; m++)
        {
            int id = fork();
            if (id > 0)
            {
                int col = m;
                for (j = 0; j < rows; j++)
                {
                    matrixrR[j * cols + col] = matrix1[j][col] + matrix2[j][col];
                }
                wait(NULL);
                break;
            }
            else
            {
                if (m == nProcs - 2)
                {
                    int col = m + 1;
                    for (j = 0; j < rows; j++)
                    {
                        matrixrR[j * cols + col] = matrix1[j][col] + matrix2[j][col];
                    }
                    wait(NULL);
                }
            }

        }
        return 0;
}

