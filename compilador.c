#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int isCondicaoParada(int ascii);
void limparString(char vetor[]);
int isPalavraReservada(char *palavra);
void tratamentoError(int numeroLinha, int tipoError, char *conteudoError);
int isVerificarLiteral(int ascii);

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
       tratamentoError(0,0,nomeArquivo);
    }

    while ((conteudoLinha = fgetc(arquivo)) != EOF)
    {  
        ascii = (int) conteudoLinha;
        if(!isVerificarLiteral(ascii)){
         // tratamentoError(numeroLinha,2,conteudoLinha);
         printf("Linha (%d) =>  Literal invalido (%c)\n",numeroLinha, conteudoLinha);
         exit(0);
        }

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
                    tratamentoError(numeroLinha,1,acumalador);
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

void tratamentoError(int numeroLinha, int tipoError, char *conteudoError) {
    switch (tipoError)
    {
    case 0:
         printf("Arquivo %s não existe nesse trabalho.\n", conteudoError);
         exit(0);
        break;
    
    case 1:
         printf("Linha (%d) =>  Palavra invalida (%s)\n",numeroLinha, conteudoError);
         exit(0);
         break;
    case 2:
         printf("Linha (%d) =>  Literal invalido (%s)\n",numeroLinha, conteudoError);
         exit(0);
         break;
    
    default:
        break;
    }
}

int isVerificarLiteral(int ascii) {
    if(
        (ascii>=97 && ascii<=122) || // a - z 
        (ascii>=65 && ascii<=90) || // A - Z 
        (ascii == 10) || // \0 -> 10
        (ascii == 9) || // tab -> 32
        (ascii == 32) || // espaco -> 32
        (ascii == 40) || // ( -> 40 
        (ascii == 41) ||  // ) -> 41
        (ascii == 123) || // { -> 123
        (ascii == 125) || // } -> 125
        (ascii == 59) || // ; -> 59
        (ascii == 44) || // , -> 44
        (ascii == 33) || // ! -> 33
        (ascii == 34) || // " -> 34
        (ascii == 60) || // < : 60
        (ascii == 61) || // = : 61
        (ascii == 62) || // > : 62
        (ascii == 43) || // + : 43
        (ascii == 10) || // Line Feed - LF (Windows) -> 10
        (ascii == 13) || // Enter - CR (Unix) -> 13
        (ascii == 42) || // *
        (ascii == 37) || // %
        (ascii == 95) || // _
        (ascii == 94) || // ^
        (ascii == 45) || // -
        (ascii == 46) || // .
        (ascii == 47)  // /	
    ){

        return 1;
    }
    return 0;
}