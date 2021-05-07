#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int MEMORIA_CONSUMIDA = 0;
int MAX_MEMORIA_CONSUMIDA = 128000;

int isCondicaoParada(int ascii);
void limparString(char vetor[]);
int isPalavraReservada(char *palavra);
void tratamentoError(int numeroLinha, int tipoError, char *conteudoError);
int isVerificarLiteral(int ascii);

// Baseado no site: https://programacaodescomplicada.wordpress.com/complementar/
struct balaceamento{
    char caracter;
};

//Definição do tipo Pilha
struct elemento{
    struct balaceamento dados;
    struct elemento *prox;
};

typedef struct elemento Elem;

typedef struct elemento* Pilha;

Pilha* cria_Pilha();
int insere_Pilha(Pilha* pi, struct balaceamento bala);
int remove_Pilha(Pilha* pi);
int tamanho_Pilha(Pilha* pi);
void imprime_Pilha(Pilha* pi); // Esse item para debuggar

// Baseado no site: https://programacaodescomplicada.wordpress.com/complementar/
struct simbolo{
   char tipoDado [50];
   char nomeVariavel [500];
   char possivelValor [500];
   char modulo [500]; 
};

//Definição do tipo lista
struct elementoSimbolo{
    struct simbolo dados;
    struct elementoSimbolo *prox;
};
typedef struct elementoSimbolo ElemSim;

typedef struct elementoSimbolo* Lista;

Lista* cria_lista();
void libera_lista(Lista* li);
int insere_lista_final(Lista* li, struct simbolo sim);
int tamanho_lista(Lista* li);
void imprime_lista(Lista* li); // Esse item para debuggar

void somarMemoriaConsumida(int memoria);
void mostrarConsumoMemoria();

int verificarAcordarValido(char *token, int nuLinha);
void removerQuebraLinha(char *token);
void removerTabulacao(char* token);
void removerCaracterEspaco(char *token);

void main()
{ 
    int linhaPossuiAcordar = 0;
    int existeAcordar = 0;
    char nomeArquivo[] = "teste.txt";
    char conteudoLinha;
    int numeroLinha = 1;
    int ascii;
    char acumalador[500];
    int contAcumalador = 0;
    char linhaCompleta[500];
    int contLinhaCompleta = 0;
    
    
    Pilha* piColchete = cria_Pilha();
    Pilha* piParentese = cria_Pilha();
    Pilha* piChaves = cria_Pilha();
    Pilha* piAspas = cria_Pilha();

    limparString(acumalador);
    limparString(linhaCompleta);
    

    FILE *arquivo;
    arquivo = fopen(nomeArquivo, "r");

    somarMemoriaConsumida(sizeof(contLinhaCompleta));
    somarMemoriaConsumida(sizeof(linhaCompleta));
    somarMemoriaConsumida(sizeof(nomeArquivo));
    somarMemoriaConsumida(sizeof(conteudoLinha));
    somarMemoriaConsumida(sizeof(ascii));
    somarMemoriaConsumida(sizeof(acumalador));
    somarMemoriaConsumida(sizeof(contAcumalador));
    somarMemoriaConsumida(sizeof(piColchete));
    somarMemoriaConsumida(sizeof(piParentese));
    somarMemoriaConsumida(sizeof(piChaves));
    somarMemoriaConsumida(sizeof(piAspas));
    somarMemoriaConsumida(sizeof(arquivo));
    

    if (arquivo == NULL)
    {
       tratamentoError(0,0,nomeArquivo);
    }

    while ((conteudoLinha = fgetc(arquivo)) != EOF)
    {  
        ascii = (int) conteudoLinha;
        linhaCompleta[contLinhaCompleta] = conteudoLinha;
        contLinhaCompleta++;

        // puts(linhaCompleta);

        somarMemoriaConsumida(sizeof(ascii));

        if(!isVerificarLiteral(ascii)){
         // tratamentoError(numeroLinha,2,conteudoLinha);
         printf("Linha (%d) =>  Literal invalido (%c)\n",numeroLinha, conteudoLinha);
         exit(0);
        }

        if (ascii == 40) { // 40 = (
            struct balaceamento novoBalaceamento;
            novoBalaceamento.caracter = conteudoLinha;
           insere_Pilha(piParentese, novoBalaceamento);
        }
        
        if (ascii == 41) { // 41 = )
            if (tamanho_Pilha(piParentese) == 0) {
                tratamentoError(numeroLinha,7,"");
            }
            remove_Pilha(piParentese);
        }

        if (ascii == 123) { // 123 = {
            struct balaceamento novoBalaceamento;
            novoBalaceamento.caracter = conteudoLinha;
           insere_Pilha(piChaves, novoBalaceamento);
        }
        
        if (ascii == 125) { // 125 = }
            if (tamanho_Pilha(piChaves) == 0) {
                tratamentoError(numeroLinha,3,"");
            }
            remove_Pilha(piChaves);
        }

        if (ascii == 91) { // 91 = [
            struct balaceamento novoBalaceamento;
            novoBalaceamento.caracter = conteudoLinha;
           insere_Pilha(piColchete, novoBalaceamento);
        }
        
        if (ascii == 93) { // 93 = ]
            if (tamanho_Pilha(piColchete) == 0) {
                tratamentoError(numeroLinha,5,"");
            }
            remove_Pilha(piColchete);
        }

        if(!isCondicaoParada(ascii)){
            acumalador[contAcumalador] = conteudoLinha;
            contAcumalador++;
            // printf("Linha (%d) => (%d) (%c) - (%s)\n", numeroLinha, ascii, conteudoLinha, acumalador);
        } else{
            if (strlen(acumalador) > 0) {
                // printf("Linha (%d)  Encontrou uma condição de parada => (%d) (%c) - (%s)\n", numeroLinha, ascii, conteudoLinha, acumalador);
                if(isPalavraReservada(acumalador)){
                    // printf("Linha (%d) => Palavra Reservada encontrada (%s)\n", numeroLinha, acumalador);

                     
		            if (strcmp("acordar", acumalador) == 0) {
			            existeAcordar++;
                        linhaPossuiAcordar = 1;
		            }

                    limparString(acumalador);
                    contAcumalador = 0;
                } else{
                    tratamentoError(numeroLinha,1,acumalador);
                }
            }
         
        }

        // Proxima linha
        if ((ascii == 10) || (ascii == 13)) { 
            
            if (existeAcordar > 1){
                tratamentoError(0,10,"");
            }

            if (linhaPossuiAcordar == 1){
                 verificarAcordarValido(linhaCompleta, numeroLinha);
            }

            if (tamanho_Pilha(piColchete) != 0) {
                tratamentoError(numeroLinha,6,"");
            }

            if (tamanho_Pilha(piParentese) != 0) {
                tratamentoError(numeroLinha,8,"");
            }
            numeroLinha++;
            limparString(acumalador);
            contAcumalador = 0; 
            contLinhaCompleta = 0;
            limparString(linhaCompleta);
            linhaPossuiAcordar = 0;
        }
    }

    if (tamanho_Pilha(piChaves) != 0) {
        tratamentoError(0,4,"");
    }

    if (existeAcordar == 0){
        tratamentoError(0,9,"");
    }


    imprime_Pilha(piChaves);
    mostrarConsumoMemoria();
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
        (ascii != 91) && // [	
        (ascii != 93) && // ]
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
    case 3:
         printf("Linha (%d) => Esta fechando chaves sem abrir \n",numeroLinha);
         exit(0);
         break;
    case 4:
         printf("Exite  abertura de chaves sem fechamento \n");
         exit(0);
         break;
    case 5:
         printf("Linha (%d) => Esta fechando colchete sem abrir \n",numeroLinha);
         exit(0);
         break;
    case 6:
         printf("Linha (%d) => Exite  abertura de colchete sem fechamento \n",numeroLinha);
         exit(0);
         break;
    case 7:
         printf("Linha (%d) => Esta fechando  parentese sem abrir \n",numeroLinha);
         exit(0);
         break;
    case 8:
         printf("Linha (%d) => Exite  abertura de parentese sem fechamento \n",numeroLinha);
         exit(0);
         break;
    case 9:
         printf("Nao foi encontrado a funcao 'acordar()' no arquivo!!!\n");
         exit(0);
         break;
    case 10:
         printf("Ha mais de uma funcao 'acordar()' no arquivo!!!\n");
         exit(0);
         break;
    case 11:
         removerQuebraLinha(conteudoError);
         printf("Linha (%d) => declaracao de acordar esta invalida: (%s)\n", numeroLinha, conteudoError);
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
        (ascii == 47) || // /	
        (ascii == 91) || // [	
        (ascii == 93)  // ]
    ){

        return 1;
    }
    return 0;

}

Pilha* cria_Pilha(){
    Pilha* pi = (Pilha*) malloc(sizeof(Pilha));
    if(pi != NULL)
        *pi = NULL;
    return pi;
}

int insere_Pilha(Pilha* pi, struct balaceamento bala){
    if(pi == NULL)
        return 0;
    Elem* no;
    no = (Elem*) malloc(sizeof(Elem));
    if(no == NULL)
        return 0;
    no->dados = bala;
    no->prox = (*pi);
    *pi = no;
    return 1;
}

int remove_Pilha(Pilha* pi){
    if(pi == NULL)
        return 0;
    if((*pi) == NULL)
        return 0;
    Elem *no = *pi;
    *pi = no->prox;
    free(no);
    return 1;
}

int tamanho_Pilha(Pilha* pi){
    if(pi == NULL)
        return 0;
    int cont = 0;
    Elem* no = *pi;
    while(no != NULL){
        cont++;
        no = no->prox;
    }
    return cont;
}

void imprime_Pilha(Pilha* pi){
    if(pi == NULL)
        return;
    Elem* no = *pi;
    while(no != NULL){
        printf("Caracter: '%d' - '%c'\n",no->dados.caracter, (char) no->dados.caracter);
        printf("-------------------------------\n");
        no = no->prox;
    }
}

Lista* cria_lista(){
    Lista* li = (Lista*) malloc(sizeof(Lista));
    if(li != NULL)
        *li = NULL;
    return li;
}

void libera_lista(Lista* li){
    if(li != NULL){
        ElemSim* no;
        while((*li) != NULL){
            no = *li;
            *li = (*li)->prox;
            free(no);
        }
        free(li);
    }
}

int insere_lista_final(Lista* li, struct simbolo sim){
    if(li == NULL)
        return 0;
    ElemSim *no;
    no = (ElemSim*) malloc(sizeof(ElemSim));
    if(no == NULL)
        return 0;
    no->dados = sim;
    no->prox = NULL;
    if((*li) == NULL){//lista vazia: insere início
        *li = no;
    }else{
        ElemSim *aux;
        aux = *li;
        while(aux->prox != NULL){
            aux = aux->prox;
        }
        aux->prox = no;
    }
    return 1;
}

int tamanho_lista(Lista* li){
    if(li == NULL)
        return 0;
    int cont = 0;
    ElemSim* no = *li;
    while(no != NULL){
        cont++;
        no = no->prox;
    }
    return cont;
}

void imprime_lista(Lista* li){
    if(li == NULL)
        return;
    ElemSim* no = *li;
    while(no != NULL){
        printf("Tipo de Dado: %s\n",no->dados.tipoDado);
        printf("Nome Variavel: %s\n",no->dados.nomeVariavel);
        printf("Possivel Valor: %s\n",no->dados.possivelValor);
        printf("Modulo: %s\n",no->dados.modulo);
        printf("-------------------------------\n");

        no = no->prox;
    }
}

void somarMemoriaConsumida(int memoria){
    MEMORIA_CONSUMIDA = MEMORIA_CONSUMIDA + memoria;

    float porcentagem = 0;
	if (MAX_MEMORIA_CONSUMIDA > 0) {
		porcentagem = (MEMORIA_CONSUMIDA * 100) / MAX_MEMORIA_CONSUMIDA;
	}

	if (porcentagem > 90 && porcentagem < 99) {
		printf("A MEMORIA ATUAL ESTA ENTRE 90 %% A 99 %%, MEMORIA ATUAL: %.2f %%\n\n", porcentagem);
	}

	if (MEMORIA_CONSUMIDA > MAX_MEMORIA_CONSUMIDA) {
		printf ("MEMORIA ATINGIU O LIMITE PARAMETRIZADO.\n");
		mostrarConsumoMemoria();
		exit(0);
	}
}

void mostrarConsumoMemoria() {
	printf("\n---------------------------------------------------------------------\n");
	printf("\nMEMORIA PARAMETRIZADA: %d bytes\n", MAX_MEMORIA_CONSUMIDA);
	float porcentagem = 0;
	if (MAX_MEMORIA_CONSUMIDA > 0) {
		porcentagem = (MEMORIA_CONSUMIDA * 100) / MAX_MEMORIA_CONSUMIDA;
	}

	printf("PORCENTAGEM DE MEMORIA => %.2f %% de %i bytes\n\n" , porcentagem, MEMORIA_CONSUMIDA);
}

int verificarAcordarValido(char *token, int nuLinha){
    int i, ascii, count = 0, isInValido = 0;
	char palavraTmp[500];
	char palavraProcessada[500];
	limparString(palavraTmp);
	limparString(palavraProcessada);

	strcpy(palavraProcessada, token);

	removerQuebraLinha(palavraProcessada);
	removerTabulacao(palavraProcessada);
	removerCaracterEspaco(palavraProcessada);

	for (i = 0; i < strlen(palavraProcessada); i++) {
		ascii = (int) palavraProcessada[i];
		palavraTmp[count] = (char) ascii;
		count++;

		if (strcmp(palavraTmp, "acordar(){") == 0) {
			isInValido = 1;
			break;
		}
	}

	if (!isInValido) {
		tratamentoError(nuLinha, 11, token);
	}
    return 1;
}

void removerQuebraLinha(char* token) {
	int i, ascii, count = 0;
	char tokenTemp[500];
	limparString(tokenTemp);

	for (i = 0; i < strlen(token); i++) {
		ascii = (int) token[i];

		if ((ascii != 13) && (ascii != 10)) {
			tokenTemp[count] = token[i];
			count++;
		}
	}

	strcpy(token, tokenTemp);
}

void removerTabulacao(char* token) {

	int i, ascii, count = 0;
	char tokenTemp[500];
	limparString(tokenTemp);

	for (i = 0; i < strlen(token); i++) {
		ascii = (int) token[i];
		// Tab => 9
		if (ascii != 9) {
			tokenTemp[count] = token[i];
			count++;
		}
	}

	strcpy(token, tokenTemp);
}

void removerCaracterEspaco(char *token) {
	int i, ascii, count = 0;
	char tokenTemp[500];
	limparString(tokenTemp);

	for (i = 0; i < strlen(token); i++) {
		ascii = (int) token[i];
		// Espaco => 32
		if (ascii != 32) {
			tokenTemp[count] = token[i];
			count++;
		}
	}

	strcpy(token, tokenTemp);
} 
