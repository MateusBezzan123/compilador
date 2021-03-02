#include <stdio.h>
#include <stdlib.h>

void main()
{
    char nomeArquivo[] = "teste.txt";
    char conteudoLinha;
    int numeroLinha = 1;
    int ascii;

    FILE *arquivo;
    arquivo = fopen(nomeArquivo, "r");

    if (arquivo == NULL)
    {
        printf("Arquivo %s não existe nesse trabalho.\n", nomeArquivo);
        exit(0);
    }

    while ((conteudoLinha = fgetc(arquivo)) != EOF)
    {  
        ascii = (int) conteudoLinha;

        printf("Linha (%d) => (%d) (%c) \n", numeroLinha, ascii, conteudoLinha);
        if ((ascii == 10) || (ascii == 13)) { 
          numeroLinha++;
        }
    }
}