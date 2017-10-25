//jogo da vida

/*
  [ ] 	permitir ao usuário a redefinir tamanho do mundo

  [ ]  colocar limites mínimo 50 e máximo 100

  [X]	REQ02: O sistema deve permitir ao usuário desenhar os seres vivos da primeira geração da simulação

  [ ]	melhorar sistema escolha seres vivos da primeira geração da simulação

  [ ]	REQ07: O sistema deverá permitir ao usuário simular passo-a-passo as gerações

  [ ]	aumentar seleção alguns padrões de imagens do seres vivos.

  [ ]	REQ09: O sistema deverá permitir ao usuário salvar uma geração em arquivo

  [ ]	REQ10: O sistema deverá permitir ao usuário carregar uma geração previamente armazenada em arquivo.

*/
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>

//assinatura das funções
void imprime_mapa(int **matriz, int tamanho);
int conta_vizinhos(int **matriz, int linha, int coluna);
void copia_matriz(int **m_copia, int **m_original, int tamanho);
void limpa_matriz(int **matriz, int tamanho);
int interface_comandos(int **matriz, int tamanho, int **mapa_next_gen, int num_gen[], int tipo_sim[], int vel_sim[]);
void rodar_simulacao(int **mapa, int **mapa_next_gen, int tamanho, int num_gen[], int tipo_sim[], int vel_sim[]);
void selecionar_seres(int **mapa, int tamanho);
void seleciona_num_gen(int *num_gen);
void pressione_enter();
void escolher_tamanho(int tam, int **mapa, int **mapa_next_gen);
void desenha_ser(int **matriz, int tamanho);

int main(){
	
	//informações importantes: tamanho mapa, numero de gerações, limpar
	int i, quit;

	//int tam = 0;	
	int **mapa;
	int **mapa_next_gen;
	int tam_mat;
	int num_gen[1];
	int tipo_sim[1];
	int vel_sim[1];
	
	int tam = 0;
	
	// int tam[1];
// 	tam = 0;
	
	tipo_sim[0]=1;
	vel_sim[0]=0;
	
	// Escolha do tamanho da matriz
	//escolher_tamanho(tam, mapa,mapa_next_gen);
	
	while (tam < 20 || tam > 100){
		printf("\n> Escolha o tamanho da matriz a ser criada: \n");
		scanf("%d", &tam);

		if (tam < 10 || tam >100 ){
	    	    printf("\n> Tamanho inválido, repita o processo");
		}
	}
	tam = tam +2;

	//cria mapa do tamanho correto
	mapa  = (int **)malloc( tam * sizeof(int *));
	for (i=0; i< tam ;i++){
		mapa [i] = (int *)malloc( tam * sizeof(int));
	}

	//cria mapa da próxima geração do tamanho correto
	mapa_next_gen = (int **)malloc( tam * sizeof(int *));
	for (i=0; i< tam ;i++){
		mapa_next_gen [i] = (int *)malloc( tam * sizeof(int));
	}
	
	// coloca zeros no mapa e no mapa next gen
	limpa_matriz(mapa, tam);
	copia_matriz(mapa_next_gen, mapa, tam);
	
	//seleciona  o numero de gerações a ser rodado
	seleciona_num_gen(num_gen);

	//imprime mapa inicial
	printf("\nMAPA\n");
	imprime_mapa(mapa, tam);
	
	// printf("desenhe os seres: \n");
// 	desenha_ser(mapa, tam);

	// coloca ser vivo
	selecionar_seres(mapa, tam);

	// imprime a matriz
	printf("\nMAPA\n");
	imprime_mapa(mapa, tam);
	
	//roda interface até que usuário marque para sair do programa
	while(quit != 9){
		quit = interface_comandos(mapa, tam, mapa_next_gen, num_gen, tipo_sim, vel_sim);
	}

	return 0;
}

////////////
void imprime_mapa(int **matriz, int tamanho){
	// função para imprimir matriz em seu estado atual.
	
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

int conta_vizinhos(int **matriz, int linha, int coluna){
	// conta o número de vizinhos de uma célula e devolve esse valor
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
	
	total_vizinhos = v1+v2+v3+v4+v5+v6+v7+v8;
	return total_vizinhos;
}

void copia_matriz(int **m_copia, int **m_original, int tamanho){
	//copia valores de uma matriz para outra
	int i, j;
	for (i = 0; i < tamanho; ++i){
		for (j = 0; j < tamanho; ++j){
			m_copia[i][j] = m_original [i][j];
		}
	}
}

void limpa_matriz(int **matriz, int tamanho){
	//coloca zero em todas as posições de uma matriz
	int i,j;
	for (i = 0; i < tamanho; ++i){
		for (j = 0; j < tamanho; ++j){
	    	    matriz [i][j] = 0;
		}
	}	
}

void rodar_simulacao(int **mapa, int **mapa_next_gen, int tamanho, int num_gen[], int tipo_sim[], int vel_sim[]){
	//roda a simulacao 
	int vizinhos, i, j, k;
	printf("Iniciando Simulacao\n");
        for (k=0;k<num_gen[0];k++){
		
		if (tipo_sim[0] == 2){
			printf("Press Enter to Continue");
			while( getchar() != '\n' );
		} else {
			sleep(vel_sim[0]);
		}
		
		for(i = 1; i< tamanho-1; i++){
			for (j=1; j< tamanho-1; j++){
				vizinhos = conta_vizinhos(mapa, i, j);
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

int interface_comandos(int **matriz, int tamanho, int **mapa_next_gen, int num_gen[], int tipo_sim[], int vel_sim[]){
	int comando;
	printf("\n> O que deseja fazer:\n");
	printf("0 - Refinir tamanho mapa\n");
	printf("1 - Selecionar seres vivos\n");
	printf("2 - limpar Mapa\n");
	printf("3 - Determinar numero de geracoes a ser rodadas\n");
	printf("4 - Escolha tipo de simulacao\n");
	printf("5 - Rodar simulacao\n");
	printf("6 - Salvar última geração\n");
	printf("7 - Importar arquivo geração\n");
	printf("8 - vizualizar configuracoes atuais\n");
	printf("9 - Sair do jogo\n");
	
	scanf("%d", &comando);

	if (comando == 0){
		//printf("Refinir tamanho mapa\n");
		printf("Funcao ainda não implementada\n");
	
	}else if (comando == 1){
		desenha_ser(matriz, tamanho);
		selecionar_seres(matriz, tamanho);
		imprime_mapa(matriz, tamanho);
	
	} else if (comando == 2){
		printf("Limpando a Matriz\n");
		limpa_matriz(matriz, tamanho);
		imprime_mapa(matriz, tamanho);
	
	} else if (comando == 3){
		seleciona_num_gen(num_gen);
		printf("Serão rodadas %d geracoes \n", num_gen[0]);
	
	} else if (comando == 4){
		printf("    1 - Simulacao automatica\n");
		printf("    2 - Simulacao manual\n");
		scanf("	    %d", &tipo_sim[0]);
		if (tipo_sim[0] == 1){
			printf("> Escolha a velocidade da simulação em segundos:\n");
			scanf("%d", &vel_sim[0]);
		}	
	
	} else if (comando ==5){
		rodar_simulacao(matriz, mapa_next_gen, tamanho, num_gen, tipo_sim, vel_sim);
	
	} else if (comando == 6){
		//printf("6 - Salvar Ultima Geração\n");
		printf("Funcao Ainda Não Implementada\n");
	} else if (comando == 7){
		//printf("7 - Importar Arquivo Geração\n");
		printf("Funcao ainda não implementada\n");
	} else if (comando == 8){
		printf("\nConfiguracao Atual\n");
		printf("-Mapa atual:\n");
		imprime_mapa(matriz, tamanho);
		printf("-Tamanho lateral mapa: %d \n", tamanho -2);
		printf("-Numero de gerações a ser rodadas: %d \n", num_gen[0]);
		if (tipo_sim[0] == 1){
			printf("-Tipo de simulação: Automática\n");
			printf("-Velocidade da simulação: %d \n", vel_sim[0]);
			//pressione_enter();
		} else{
			printf("-Tipo de simulação: Manual\n");
			printf("-Velocidade da simulação: NA \n");
			//pressione_enter();
		}
		//pressione_enter();
	
	} else if (comando == 9){
		printf("Tchau!\n");
		
	}
	return comando;
}


void selecionar_seres(int **matriz, int tamanho){
	int metade = tamanho/2;
	int ser;
	
	printf("> Escolha o ser que quer adicionar ao mapa:\n\n ");
	printf("Ser 1:\n\n");
	printf("o o o o o o o o o o \n\n");
	printf("------------------------\n\n");
	printf("Ser 2:\n\n");
	printf("  o \n");
	printf("    o \n");
	printf("o o o \n\n");
	printf("------------------------\n\n");
	printf("Ser 3:\n\n");
	printf("  o \n");
	printf("o o o \n");
	printf("o   o \n");
	printf("  o   \n\n");
	printf("------------------------\n\n");
	
	printf(" Ou digite 0 para não colocar nenhum ser no momento\n");
	
	scanf("%d", &ser);
	
	if(ser == 1){
		matriz[metade][metade - 5] = 1;
		matriz[metade][metade - 4] = 1;
		matriz[metade][metade - 3] = 1;
		matriz[metade][metade - 2] = 1;
		matriz[metade][metade - 1] = 1;
		matriz[metade][metade] = 1;
		matriz[metade][metade + 1] = 1;
		matriz[metade][metade + 2] = 1;
		matriz[metade][metade + 3] = 1;
		matriz[metade][metade + 4] = 1;
	}
	if (ser == 2){
		matriz[metade+1][metade-1] = 1;
		matriz[metade+1][metade] = 1;
		matriz[metade+1][metade+1] = 1;
		matriz[metade][metade+1] = 1;
		matriz[metade-1][metade] = 1;
	}
	if (ser == 3){
		matriz[metade+1][metade] = 1;
		matriz[metade][metade-1] = 1;
		matriz[metade][metade+1] = 1;
		matriz[metade-1][metade-1] = 1;
		matriz[metade-1][metade] = 1;
		matriz[metade-1][metade+1] = 1;
		matriz[metade-2][metade] = 1;
	}
	
}

void seleciona_num_gen(int num_gen[]){
	printf("> Escolha o número de gerações a serem rodadas:\n");
	scanf("%d", &num_gen[0]);
}

void pressione_enter(){
	printf("\nPress Enter to Continue\n");
	while( getchar() != '\n' );
}

void desenha_ser(int **matriz, int tamanho){
	int linha = -1, coluna= -1, sair = 1;
	
	do{
		while (linha < 0 || linha > tamanho-2 || coluna <0 || coluna > tamanho-2){
			printf("digite o numero de uma linha (de 0 a %d):\n", tamanho-2);
			scanf ("%d", &linha);
			printf("digite o numero da coluna (de 0 a %d):\n", tamanho-2);
			scanf ("%d", &coluna);
			
			if (linha < 0 || linha > tamanho-2 || coluna <0 || coluna > tamanho-2){
				printf("ponto inválido, tende novamente");
			}
		}
		
	
		matriz[linha+1][coluna+1] = 1;
	
		imprime_mapa(matriz, tamanho);
		
		printf("Deseja adicinar outro ponto?\n");
		printf("Digite 1 para sim e 0 para não");
		scanf("%d", &sair);
		
		linha = -1;
		coluna= -1;
		
	} while (sair == 1);
	
}


// void escolher_tamanho(int tam, int **mapa, int **mapa_next_gen){
// 	// Escolha do tamanho da matriz
// 	//tam= 0;
// 	int i;
//
// 	while (tam < 20 || tam > 100);{
// 		printf("\n> Escolha o tamanho da matriz a ser criada: \n");
// 		scanf("%d", &tam);
// 		//printf("%d", tam);
//
// 		if (tam < 10 || tam >100 ){
// 	    	    printf("\n> Tamanho inválido, repita o processo");
// 		}
// 	}
// 	tam = tam +2;
//
// 	//cria mapa do tamanho correto
// 	mapa  = (int **)malloc( tam * sizeof(int *));
// 	for (i=0; i< tam ;i++){
// 		mapa [i] = (int *)malloc( tam * sizeof(int));
// 	}
//
// 	//cria mapa da próxima geração do tamanho correto
// 	mapa_next_gen = (int **)malloc( tam * sizeof(int *));
// 	for (i=0; i< tam ;i++){
// 		mapa_next_gen [i] = (int *)malloc( tam * sizeof(int));
// 	}
//
// }








