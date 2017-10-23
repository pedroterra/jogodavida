//jogo da vida

/*
  [X] 	REQ01: O sistema deve permitir ao usuário a definição do tamanho do mundo
	(mínimo 50 e máximo 100)

  [X]	REQ02: O sistema deve permitir ao usuário selecionar os seres vivos da primeira
	geração da simulação

  []	REQ03: O sistema deverá permitir ao usuário limpar todas as células do mundo (mapa)

  []	REQ04: O sistema deverá permitir ao usuário iniciar a simulação

  []	REQ05: O sistema deverá permitir ao usuário definir a velocidade da simulação das
	gerações. Isso no caso do usuário determinar a quantidade de gerações que serão
	simuladas (requisito 6)

  [X]	REQ06: O sistema deverá permitir ao usuário simular um determinado conjunto de
	gerações previamente definido

  []	REQ07: O sistema deverá permitir ao usuário simular passo-a-passo as gerações

  []	REQ08: O sistema deverá permitir ao usuário selecionar alguns padrões de imagens do
	seres vivos.

  []	REQ09: O sistema deverá permitir ao usuário salvar uma geração em arquivo

  []	REQ10: O sistema deverá permitir ao usuário carregar uma geração previamente
	armazenada em arquivo.

*/
#include<stdlib.h>
#include<stdio.h>

//assinatura das funções
void imprime_mapa(int **matriz, int tamanho);
int conta_vizinhos(int **matriz, int linha, int coluna, int tamanho);
void copia_matriz(int **m_copia, int **m_original, int tamanho);
void limpa_matriz(int **matriz, int tamanho);
// int interface_comandos();
void rodar_simulacao(int **mapa, int **mapa_next_gen, int tamanho, int num_gen);
//void escolha_tam_mat(int *n);

int main(){
	
	//informações importantes: tamanho mapa, numero de gerações, limpar
	
	int num_seres = 100000, i, j, vizinhos, num_gen, k, ser;
	int n=0;
	//int *n;
	int **mapa ;
	int **mapa_next_gen;
	int *tam_mat;
	
	//escolha_tam_mat(int *n);

	while (n < 10 || n > 100){
		printf("\nEscolha o tamanho da matriz a ser criada: \n");
		scanf("%d", &n);

		if (n< 10 || n >100 ){
	    	    printf("\nTamanho inválido, repita o processo");
		} else {
	    	    printf("tamanho: %d \n", n);

		}
	}
	n = n+2;
	
	printf("Escolha o número de gerações a serem rodadas:\n ");
	scanf("%d", &num_gen);
	
	mapa  = (int **)malloc( n * sizeof(int *));
	for (i=0; i<n;i++){
		mapa [i] = (int *)malloc( n * sizeof(int));
	}

	mapa_next_gen = (int **)malloc( n * sizeof(int *));
	for (i=0; i<n;i++){
		mapa_next_gen [i] = (int *)malloc( n * sizeof(int));
	}

	// coloca zeros no mapa
	limpa_matriz(mapa, n);

	copia_matriz(mapa_next_gen, mapa, n);

	printf("\nMAPA\n");
	imprime_mapa(mapa, n);

	// coloca ser vivo
	int metade = n/2;
	
	
	printf("Escolha o ser\n\n ");
	printf("ser 1:\n");
	printf("o o o o o o o o o o \n\n");
	printf("ser 2:\n");
	printf("  o \n    o \no o o \n\n");
	printf("ser 3:\n");
	printf("  o \no o o \no   o \n  o \n\n");
	
	
	
	scanf("%d", &ser);
	
	if(ser == 1){
		mapa[metade][metade - 5] = 1;
		mapa[metade][metade - 4] = 1;
		mapa[metade][metade - 3] = 1;
		mapa[metade][metade - 2] = 1;
		mapa[metade][metade - 1] = 1;
		mapa[metade][metade] = 1;
		mapa[metade][metade + 1] = 1;
		mapa[metade][metade + 2] = 1;
		mapa[metade][metade + 3] = 1;
		mapa[metade][metade + 4] = 1;
	}
	if (ser == 2){
		mapa[metade+1][metade-1] = 1;
		mapa[metade+1][metade] = 1;
		mapa[metade+1][metade+1] = 1;
		mapa[metade][metade+1] = 1;
		mapa[metade-1][metade] = 1;
	}
	if (ser == 3){
		mapa[metade+1][metade] = 1;
		mapa[metade][metade-1] = 1;
		mapa[metade][metade+1] = 1;
		mapa[metade-1][metade-1] = 1;
		mapa[metade-1][metade] = 1;
		mapa[metade-1][metade+1] = 1;
		mapa[metade-2][metade] = 1;
	}

	

	// imprime a matriz
	printf("\nMAPA\n");
	imprime_mapa(mapa, n);
    
    // while (num_seres > n*n)
//     {
//
//         printf("\nEscolha o numero de seres vivos para a primeira geração: \n");
//         scanf("%d", &num_seres);
//
//         if (num_seres > n*n )
//         {
//             printf("\nNumero invalido, repita o processo");
//         } else{
//         	printf("num seres: %d", num_seres);
//         }
//     }
	
	int ini_sim;
	printf("\n> Iniciar Simulação? (sim = 1 e  nao = 0)");
	scanf("%d", &ini_sim);
	if (ini_sim == 1){
		rodar_simulacao(mapa, mapa_next_gen, n,num_gen);
		
	}

	return 0;
}

////////////
void imprime_mapa(int **matriz, int tamanho){
	int i,j;
	
	for (i = 0; i < tamanho; ++i){
            for (j = 0; j < tamanho; ++j){
		    if(i == 0 || i == tamanho-1){
			    printf(". ");
		    } else if( j == 0 ){
			    printf(". ");
		    } else if (j == tamanho-1){
		    	printf(".");
		    } else if (matriz [i][j] == 1){
			    printf("o ");
		    } else{
			    printf("  ");
		    }
            }
            printf("\n");
        }
	
}

int conta_vizinhos(int **matriz, int linha, int coluna, int tamanho){
	int total_vizinhos = 0;
	int v1, v2, v3, v4, v5, v6, v7, v8;
	
	v1 = matriz[linha-1][coluna -1];
	v2 = matriz[linha -1][coluna];
	v3 = matriz[linha-1][coluna+1];
	v4 = matriz[linha][coluna-1];
	v5 = matriz[linha][coluna+1];
	v6 = matriz[linha+1][coluna-1];
	v7 = matriz[linha+1][coluna];
	v8 = matriz[linha+1][coluna+1];
	
	if (linha == 0){
		printf("\nl = 0");
		v1 = 0;
		v2 = 0;
		v3 = 0;
	}
		
	if (coluna == 0 ){
		v1 = 0;
		v4 = 0;
		v6 = 0;
	}
	
	if (coluna == tamanho){
		v3 = 0;
		v5 = 0;
		v8 = 0;
	}
	
	if (linha == tamanho){
		v6 = 0;
		v7 = 0;
		v8 = 0;
	}
	
	total_vizinhos = v1+v2+v3+v4+v5+v6+v7+v8;
	return total_vizinhos;
}

void copia_matriz(int **m_copia, int **m_original, int tamanho){
	int i, j;
	for (i = 0; i < tamanho; ++i){
		for (j = 0; j < tamanho; ++j){
			m_copia[i][j] = m_original [i][j];
		}
	}
}

void limpa_matriz(int **matriz, int tamanho){
	int i,j;
	
	for (i = 0; i < tamanho; ++i){
		for (j = 0; j < tamanho; ++j){
	    	    matriz [i][j] = 0;
		}
	}	
}

void rodar_simulacao(int **mapa, int **mapa_next_gen, int tamanho, int num_gen){
	int vizinhos, i, j, k;
	
        for (k=0;k<num_gen;k++){
		for(i = 1; i< tamanho-1; i++){
			for (j=1; j<tamanho-1; j++){
				vizinhos = conta_vizinhos(mapa, i, j, tamanho);
				if (vizinhos == 3){
					mapa_next_gen[i][j]=1;
				} else if (vizinhos == 2){
					mapa_next_gen[i][j] = mapa[i][j];
				}else if (vizinhos < 2){
					mapa_next_gen[i][j] = 0;
				} else if (vizinhos > 3){
					mapa_next_gen[i][j] = 0;
				}
			}
		}
		copia_matriz(mapa,  mapa_next_gen, tamanho);
		
		printf("\ngeracao %d \n", k+1);
		imprime_mapa(mapa, tamanho);
	}
}

// void escolha_tam_mat(int *n){
//
// 	do{
// 		printf("\nEscolha o tamanho da matriz a ser criada: \n");
// 		scanf("%d", &n);
//
// 		if (n< 10 || n >100 ){
// 	    	    printf("\nTamanho inválido, repita o processo");
// 		} else {
// 	    	    printf("tamanho: %d \n", n);
//
// 		}
// 	}
// 	while (n< 10 || n > 100);
//
// }


// int escolha_num_gen(){
// 	int num_gen;
// 	printf("Escolha o número de gerações a serem rodadas:\n ");
// 	scanf("%d", &num_gen);
// 	return
// }
//
// int interface_comandos(int **matriz, int tamanho){
// 	int comando;
// 	printf("O que deseja fazer:");
// 	printf("1 - Limpar mapa");
// 	printf("2 - Escolher numero de gerações");
// 	printf("3 - Iniciar Simulação");
// 	printf("4 - modificar tamanho do mapa");
//
// 	scanf("%d", &comando);
//
// 	if (comando == 1){
// 		limpa_matriz(matriz, tamanho);
// 	} else if (comando == 2){
//
// 	}
// }










