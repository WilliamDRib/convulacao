//Ass: William Dutra Ribeiro
//Relatório: https://docs.google.com/document/d/1pj-JLuykbh5XXLhGlJO0apoULGb9LDASJcFFsMHYGnc/edit?usp=sharing
//Video: https://www.youtube.com/watch?v=adunlWQR6Ms


#include <stdio.h>
#include <stdlib.h>
#define tamfiltro 3
#define TAM 50


int ** alocar(int tam){
	int ** vet = (int**) calloc(tam, sizeof(int *));
	for(int i = 0; i < tam;i++){ 
		vet[i] = (int*) calloc(tam, sizeof(int));
	}
	return vet;	
}//Função para alocar a matriz dinamicamente;

void testar(int ** vet){
	if(vet == NULL){
		printf("Memoria insuficiente");
		exit(0);
	}
}//Verificando se a matriz foi realmente alocada;

void preencher(int ** vet,int tam){
	for(int i = 0; i < tam; i++){
		for(int j = 0; j < tam; j++){
			printf("Qual o valor deseja nessa posição? (%d x %d)", i, j);
			int valor;
			scanf("%d", &valor);
			vet[i][j] = valor;
		}
	}
}//Função utilizada para inserir valores nas matrizes;

void mostrar(int ** vet,int tam, char texto[]){
	printf("Valores do %s\n",texto);
	for(int i = 0; i < tam; i++){
		printf(" |");
		for(int j = 0; j < tam; j++){
			printf(" %d |",vet[i][j]);
		}
		printf("\n");
	}
}//Função para imprimir a matriz;
 
int ** convulacao(int ** vet, int ** filtro, int ** conv,int tam){
	for(int i = 0; i < tam; i++){
		for(int j = 0; j < tam; j++){
			int cont = 0;
			for(int li = -1; li <= 1; li++){
				for(int col = -1; col <= 1; col++){
					if(i+li >= 0 && i+li < tam){
						if(j+col >= 0 && j+col < tam){
							cont += vet[i+li][j+col] * filtro[li+1][col+1];
						}
					}					
				}
			}
			conv[i][j] = cont;
		}
	}
	return conv;
}//Função responsavel para realizar a convolucao de duas matrizes;

void desalocar(int ** vet,int tam){
	for(int i = 0; i < tam;i++){
		free(vet[i]);
	}
	free(vet);
}//Função de desalocamento das matrizes;


typedef struct {
	int num_princ[TAM][TAM];
	int num_filtro[TAM][TAM];
	int num_conv[TAM][TAM];
	int tamanho;
}Dados;//Setando o typedef dos dados para um bkp;

void bkp(int ** vet_princ, int ** vet_filtro, int ** vet_conv, Dados * d,int cont, int tamanho){

	for(int p = 0; p < tamanho; p++){
		for(int j = 0; j < tamanho; j++){
			d[cont].num_princ[p][j] = vet_princ[p][j];
			d[cont].num_conv[p][j] = vet_conv[p][j];
		}
	}

	for(int p = 0; p < 3; p++){
		for(int j = 0; j < 3; j++){
			d[cont].num_filtro[p][j] = vet_filtro[p][j];
		}
	}

	d[cont].tamanho = tamanho;
}//Função responsavel em guardar os valores;

void mostrar_bkp(Dados * d, int cont){
	for(int pos = 0; pos <= cont; pos ++){
		printf("Matriz origem do caso %d\n", pos + 1);
		for(int i = 0; i < d[pos].tamanho; i++){
			printf(" |");                                     //Origem
			for(int j = 0; j < d[pos].tamanho; j++){
				printf(" %d |",d[pos].num_princ[i][j]);
			}
			printf("\n");
		}
		printf("\nMatriz filtro do caso %d\n", pos + 1);
		for(int i = 0; i < 3; i++){
			printf(" |");                                     //Filtro
			for(int j = 0; j < 3; j++){
				printf(" %d |",d[pos].num_filtro[i][j]);
			}
			printf("\n");
		}
		printf("\nMatriz convulada do caso %d\n", pos + 1);
		for(int i = 0; i < d[pos].tamanho; i++){
			printf(" |"); 																		//Convulada
			for(int j = 0; j < d[pos].tamanho; j++){
				printf(" %d |",d[pos].num_conv[i][j]);
			}
			printf("\n");
		}
		printf("=====================\n");
	}	
}//Função para mostrar os dados armazenados em bkp;


int main(void) {
	int loop = 1;
	int cont = 0;

	int ** matriz_filtro  = alocar(tamfiltro);	//alocação do filtro;
	testar(matriz_filtro);

	Dados * dados = (Dados *) malloc(50 * sizeof(Dados)); //bkp;

	while(loop != 0){

		int tamanho=0;
		while(tamanho<=0){
			printf("\nQual o tamanho da matriz deseja (obs: numero positivo)  ");
			scanf("%d", &tamanho);
		}

		int ** matriz_origem = alocar(tamanho);
		printf("Agora prencha o essa matriz\n");
		testar(matriz_origem);											//alocando, verificando e preenchendo;
		preencher(matriz_origem, tamanho);

		printf("Certo, agora prencha o filtro\n");
		preencher(matriz_filtro, tamfiltro);
		
		int ** matriz_conv = alocar(tamanho);
		testar(matriz_conv);												// Convolucionando;
		matriz_conv = convulacao(matriz_origem, matriz_filtro, matriz_conv, tamanho);
		mostrar(matriz_conv,tamanho,"Tabela Convulada");		

		bkp(matriz_origem,matriz_filtro,matriz_conv,dados,cont,tamanho);

		printf("Deseja sair do programa? (0 = Sim) (1 = Nao) (2 = backup)");
		scanf("%d", &loop);		

		if(loop == 0){
			desalocar(matriz_origem,tamanho);
			desalocar(matriz_filtro,tamfiltro); //desalocando;
			desalocar(matriz_conv,tamanho);
			free(dados);
		}else if(loop == 2){
			printf("Estes são todas os casos que ocorreram!\n");
			mostrar_bkp(dados,cont);
		}
		cont ++;
	}
}
