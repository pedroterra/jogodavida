//jogo da vida

/*
  [ ] 	permitir ao usuário a redefinir tamanho do mundo

  [ ]  colocar limites mínimo 50 e máximo 100

  [X]	REQ02: O sistema deve permitir ao usuário desenhar os seres vivos da primeira geração da simulação

  [X]	melhorar sistema escolha seres vivos da primeira geração da simulação

  [ ]	REQ07: O sistema deverá permitir ao usuário simular passo-a-passo as gerações

  [ ]	aumentar seleção alguns padrões de imagens do seres vivos.

  [X]	REQ09: O sistema deverá permitir ao usuário salvar uma geração em arquivo

  [ ]	REQ10: O sistema deverá permitir ao usuário carregar uma geração previamente armazenada em arquivo.

*/
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include <time.h>

//assinatura das funções
void imprime_mapa(int **matriz, int tamanho);
int conta_vizinhos(int **matriz, int linha, int coluna);
void copia_matriz(int **m_copia, int **m_original, int tamanho);
void limpa_matriz(int **matriz, int tamanho);
int interface_comandos(int **matriz, int tamanho, int **mapa_next_gen, int num_gen[], int tipo_sim[], int vel_sim[], int geracao_global[]);
void rodar_simulacao(int **mapa, int **mapa_next_gen, int tamanho, int num_gen[], int tipo_sim[], int vel_sim[], int geracao_global[]);
void selecionar_seres(int **mapa, int tamanho);
void seleciona_num_gen(int *num_gen);
void pressione_enter();
void escolher_tamanho(int tam, int **mapa, int **mapa_next_gen);
void desenha_ser(int **matriz, int tamanho);
void salvar_geracao(int **matriz, int tamanho);

int main(){
	
	//informações importantes: tamanho mapa, numero de gerações, limpar
	int i, quit;

	int **mapa;
	int **mapa_next_gen;
	int tam_mat;
	int num_gen[1];
	int tipo_sim[1];
	int vel_sim[1];
	int geracao_global[1];
	
	int tam = 0;
	
	geracao_global[0] = 0;
	num_gen[0]=1;
	tipo_sim[0]=1;
	vel_sim[0]=0;
	
	// Escolha do tamanho da matriz
	
	while (tam < 20 || tam > 100){
		printf("\n-- Escolha o tamanho da matriz a ser criada: \n>> ");
		scanf("%d", &tam);

		if (tam < 20 || tam >100 ){
	    	    printf("\nTamanho inválido, repita o processo.");
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

	//imprime mapa inicial
	printf("\nMAPA\n");
	imprime_mapa(mapa, tam);
	
	//roda interface até que usuário marque para sair do programa
	while(quit != 9){
		quit = interface_comandos(mapa, tam, mapa_next_gen, num_gen, tipo_sim, vel_sim, geracao_global);
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

void rodar_simulacao(int **mapa, int **mapa_next_gen, int tamanho, int num_gen[], int tipo_sim[], int vel_sim[], int geracao_global[]){
	//roda a simulacao 
	int vizinhos, i, j, k;
	printf("Iniciando Simulacao\n");
	
	printf("\nGeracao global: %d \n", geracao_global[0]);
	printf("Geracao local: 0\n");
	imprime_mapa(mapa, tamanho);
	
        for (k=0;k<num_gen[0];k++){
		
		if (tipo_sim[0] == 2){
			printf("-- Pressione Enter para continuar");
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
		geracao_global[0] = geracao_global[0] +1;
		printf("\nGeração global: %d", geracao_global[0]);
		printf("\nGeracao local: %d \n", k+1);
		imprime_mapa(mapa, tamanho);
	}
}

int interface_comandos(int **matriz, int tamanho, int **mapa_next_gen, int num_gen[], int tipo_sim[], int vel_sim[], int geracao_global[]){
	int comando, subcomando;
	printf("\n-- Selecione um COMANDO:\n");
	printf(" 1 - Selecionar seres vivos\n");
	printf(" 2 - limpar Mapa\n");
	printf(" 3 - Determinar numero de geracoes a ser rodadas\n");
	printf(" 4 - Escolha tipo de simulacao\n");
	printf(" 5 - Rodar simulacao\n");
	printf(" 6 - Salvar última geração\n");
	printf(" 7 - Importar arquivo geração\n");
	printf(" 8 - vizualizar configuracoes atuais\n");
	printf(" 9 - Sair do jogo\n");
	printf(" >> ");
	scanf("%d", &comando);

	if (comando == 0){
		//printf("Refinir tamanho mapa\n");
		printf("Funcao ainda não implementada\n");
	
	}else if (comando == 1){
		printf("\n\t-- De qual maneira gostaria de inserir os seres?\n");
		printf("\t1 - Desenhando um ser customizado\n");
		printf("\t2 - Escolhendo de padroes prontos\n\t>> ");
		scanf("%d", &subcomando);
		
		if (subcomando == 1) desenha_ser(matriz, tamanho);
		else if (subcomando == 2) {
			selecionar_seres(matriz, tamanho);
			imprime_mapa(matriz, tamanho);
		} else printf("Comando inválido");
		
	
	} else if (comando == 2){
		printf("Limpando a Matriz\n");
		limpa_matriz(matriz, tamanho);
		imprime_mapa(matriz, tamanho);
	
	} else if (comando == 3){
		seleciona_num_gen(num_gen);
		printf("\tSerão rodadas %d geracoes \n", num_gen[0]);
	
	} else if (comando == 4){
		printf("\t1 - Simulacao automatica\n");
		printf("\t2 - Simulacao manual\n\t>>");
		scanf("%d", &tipo_sim[0]);
		if (tipo_sim[0] == 1){
			printf("\t\t-- Escolha a velocidade da simulação em segundos:\n\t\t");
			scanf("%d", &vel_sim[0]);
		}	
	
	} else if (comando ==5){
		rodar_simulacao(matriz, mapa_next_gen, tamanho, num_gen, tipo_sim, vel_sim, geracao_global);
	
	} else if (comando == 6){
		salvar_geracao(matriz, tamanho);
		
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
	
	printf("\t\t-- Escolha o ser que quer adicionar ao mapa:\n\n ");
	printf("\t\t\tSer 1:\n\n");
	printf("\t\t   o o o o o o o o o o \n\n");
	printf("\t\t------------------------\n\n");
	printf("\t\t\tSer 2:\n\n");
	printf("\t\t\t  o \n");
	printf("\t\t\t    o \n");
	printf("\t\t\to o o \n\n");
	printf("\t\t------------------------\n\n");
	printf("\t\t\tSer 3:\n\n");
	printf("\t\t\t  o \n");
	printf("\t\t\to o o \n");
	printf("\t\t\to   o \n");
	printf("\t\t\t  o   \n\n");
	printf("\t\t------------------------\n\n>> ");
	
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
	printf("\n\t-- Escolha o número de gerações a serem rodadas:\n\t>> ");
	scanf("%d", &num_gen[0]);
}

void pressione_enter(){
	printf("\nPress Enter to Continue\n");
	while( getchar() != '\n' );
}

void desenha_ser(int **matriz, int tamanho){
	int linha = -1, coluna= -1, sair = 1;
	printf("\n\t\t-- Escolha as coordenadas X e Y de cada ponto.\n");
	printf("\t\t(Seu Mapa tem tamanho %d por %d. Logo digite valores de 0 a %d)\n", tamanho-2, tamanho-2, tamanho-3);
	
	do{
		while (linha < 0 || linha > tamanho-3 || coluna <0 || coluna > tamanho-3){
			printf("\t\tEscolha o valor de X:\n\t\t>> ");
			scanf ("%d", &coluna);
			printf("\t\tEscolha o valor de Y:\n\t\t>> ");
			scanf ("%d", &linha);
			
			if (linha < 0 || linha > tamanho-3 || coluna <0 || coluna > tamanho-3){
				printf("\n\t\tPonto inválido! Tende novamente.\n\n");
			}
		}
		
		matriz[linha+1][coluna+1] = 1;
		imprime_mapa(matriz, tamanho);
		
		printf("\n-- Deseja adicinar outro ponto?\n");
		printf(" 1 - Sim \n 2 - Não\n>>");
		scanf("%d", &sair);
		
		linha = -1;
		coluna= -1;
		
	} while (sair == 1);
}

void salvar_geracao(int **matriz, int tamanho){
	int i,j;
	char base_path[] = "/Users/pedrobirmann/Desktop/", extensao[] = ".txt";
	char filename[50];
	char finalpath[100] = "";
		
	printf("\tEscolha o nome do seu arquivo: \n\t>> ");
	scanf("%s",filename);
	
	strcat(finalpath,base_path);
	strcat(finalpath,filename);
	strcat(finalpath,extensao);
	
        FILE *fp;

        if((fp=fopen(finalpath, "w"))==NULL) {
		printf("\t Erro, não foi possível abrir o seu arquivo\n");
		exit(1);
        } 
	
	for (i = 0; i < tamanho; ++i){
	        for (j = 0; j < tamanho; ++j){
			if(i == 0 || i == tamanho-1){
				fprintf(fp,". ");
			} else if( j == 0 ){
				fprintf(fp,". ");
			} else if (j == tamanho-1){
				fprintf(fp,".");
			} else if (matriz [i][j] == 1){
				fprintf(fp,"o ");
			} else{
				fprintf(fp,"  ");
			}
		}
		fprintf(fp,"\n");
	}
	fclose(fp);
	printf("\n\tArquivo Salvo com o nome %s.txt  \n", filename);
}
	
	
  
        














