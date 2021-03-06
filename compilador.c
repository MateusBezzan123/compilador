#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int isCondicaoParada(int ascii);
void limparString(char vetor[]);
int isPalavraReservada(char *palavra);

void main()
{
    char nomeArquivo[] = "teste.txt";
    char conteudoLinha;
    int numeroLinha = 1;
    int ascii;
    char acumalador[500];
    int contAcumalador = 0;

    limparString(acumalador);

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

        if(!isCondicaoParada(ascii)){
            acumalador[contAcumalador] = conteudoLinha;
            contAcumalador++;
            printf("Linha (%d) => (%d) (%c) - (%s)\n", numeroLinha, ascii, conteudoLinha, acumalador);
        } else{
            if (strlen(acumalador) > 0) {
                printf("Linha (%d)  Encontrou uma condição de parada => (%d) (%c) - (%s)\n", numeroLinha, ascii, conteudoLinha, acumalador);
                if(isPalavraReservada(acumalador)){
                    printf("Linha (%d) => Palavra Reservada encontrada (%s)\n", numeroLinha, acumalador);
                    limparString(acumalador);
                    contAcumalador = 0;
                } else{
                    printf("Linha (%d) =>  Palavra invalida (%s)\n",numeroLinha, acumalador);
                }
            }
         
        }

        // Proxima linha
        if ((ascii == 10) || (ascii == 13)) { 
          numeroLinha++;
          limparString(acumalador);
          contAcumalador = 0; 
        }
    }
}

int isCondicaoParada(int ascii) {

    if (
        (ascii != 10) && // \0 -> 10
        (ascii != 9) && // tab -> 32
        (ascii != 32) && // espaco -> 32
        (ascii != 40) && // ( -> 40 
        (ascii != 41) && // ) -> 41
        (ascii != 59) && // ; -> 59
        (ascii != 123) && // { -> 123
        (ascii != 125) && // } -> 125
        (ascii != 44) && // , -> 44
        (ascii != 33) && // ! -> 33
        (ascii != 34) && // " -> 34
        (ascii != 60) && // < : 60
        (ascii != 61) && // = : 61
        (ascii != 62) && // > : 62
        (ascii != 43) && // + : 43
        (ascii != 10) && // Line Feed - LF (Windows) -> 10
        (ascii != 13) // Enter - CR (Unix) -> 13

    ) {
        return 0;
    }
    return 1;
}

void limparString(char vetor[]) {
	int i = 0;

	while (i <= 500) {
		vetor[i] = '\0';
		i ++;
	}
}


int isPalavraReservada(char *palavra) {
    int i;
    char palavrasReservadas[10][9]={
        "acordar", 
        "dormir", 
        "pensar", 
        "falar", 
        "tentar", 
        "outra_vez",
        "tarefa",
        "liquido",
        "solido",
        "gasoso"
    };

    for (i = 0; i <= 10; i++) {
		if (strcmp(palavra, palavrasReservadas[i]) == 0) {
			return 1;
		}		
	}
    return 0;
}