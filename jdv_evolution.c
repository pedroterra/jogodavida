//jogo da vida

/*

[ ] colocar limites mínimo 50 e máximo 100

[ ] aumentar seleção alguns padrões de imagens do seres vivos.

[ ] fazer biblioteca do jogodavida

[ ] fazer interface grafica

[ ] arrumar tamanho letras

[ ] setinhas

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
void desenha_ser(int **matriz, int tamanho);
void salvar_geracao(int **matriz, int tamanho);
void importa_geracao(int **matriz, int tamanho);
int contar_seres(int **matriz, int tamanho);
void gerar_pontos_aleatorios(int **matriz, int tamanho);
void auto_save(int **matriz, int colony_number[], int tamanho);
void import_for_reproduction(int colony_one, int colony_two, int **matriz_pai, int **matriz_mae, int tamanho, int directory[]);
void create_initial_colonies();
void create_initial_colonies(int **matriz, int **mapa_next_gen, int tamanho, int num_gen[], int tipo_sim[], int vel_sim[], int geracao_global[], int colony_number[]);
void get_top_ten(int top_10[]);
void reproduce(int **matriz_pai, int **matriz_mae, int **matriz_filha, int tamanho);
void auto_save2(int **matriz, int colony_number[], int tamanho);
void create_next_batch(int **mapa, int **mapa_next_gen, int **matriz_pai, int **matriz_mae, int **matriz_filha,int tamanho, int num_gen[], int tipo_sim[], int vel_sim[], int geracao_global[]);

int colony_number[1];
int survivors[100];
int top_10[10];
int num_ser;
int directory[1];

int main(){

  //informações importantes: tamanho mapa, numero de gerações, limpar
  int i, quit, j,k,l, m=0, n;
  int **mapa;
  int **mapa_next_gen;
  int **matriz_pai;
  int **matriz_mae;
  int **matriz_filha;
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
  colony_number[0]=0;
  directory[0]=2;

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

  matriz_pai  = (int **)malloc( tam * sizeof(int *));
  for (i=0; i< tam ;i++){
    matriz_pai [i] = (int *)malloc( tam * sizeof(int));
  }

  matriz_mae  = (int **)malloc( tam * sizeof(int *));
  for (i=0; i< tam ;i++){
    matriz_mae [i] = (int *)malloc( tam * sizeof(int));
  }

  matriz_filha  = (int **)malloc( tam * sizeof(int *));
  for (i=0; i< tam ;i++){
    matriz_filha [i] = (int *)malloc( tam * sizeof(int));
  }

  // coloca zeros no mapa e no mapa next gen
  limpa_matriz(mapa, tam);
  copia_matriz(mapa_next_gen, mapa, tam);

  create_initial_colonies(mapa, mapa_next_gen, tam, num_gen, tipo_sim,vel_sim,geracao_global,colony_number);
  printf("Start ten\n");
  get_top_ten(top_10);

  for (n = 0; n<3; n++){
    create_next_batch(mapa,mapa_next_gen, matriz_pai, matriz_mae, matriz_filha, tam, num_gen, tipo_sim, vel_sim, geracao_global);
    printf("Start ten\n");
    get_top_ten(top_10);
  }

  //roda interface até que usuário marque para sair do programa
  while(quit != 9){
    quit = interface_comandos(mapa, tam, mapa_next_gen, num_gen, tipo_sim, vel_sim, geracao_global);
  }

  return 0;
}

//funcoes
void imprime_mapa(int **matriz, int tamanho){
  // função para imprimir matriz em seu estado atual.
  int i,j;
  int seres = 0;

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
        //seres = seres + 1;
      } else{
        printf("  ");
      }
    }
    printf("\n");
  }
  seres = contar_seres(matriz, tamanho);
  printf("\n\nnumero de seres vivos: %d \n\n", seres);

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

  // printf("Iniciando Simulacao\n");
  // printf("\nGeracao global: %d \n", geracao_global[0]);
  // printf("Geracao local: 0\n");
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
    printf("\nGeração global: %d \n", geracao_global[0]);
    //printf("\nGeracao local: %d \n", k+1);
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
    printf("\t2 - Escolhendo de padroes prontos\n");
    printf("\t3 - Gerar pontos aleatorios\n\t>> ");
    scanf("%d", &subcomando);

    if (subcomando == 1) desenha_ser(matriz, tamanho);
    else if (subcomando == 2) {
      selecionar_seres(matriz, tamanho);
      imprime_mapa(matriz, tamanho);
    } else if(subcomando == 3){
      gerar_pontos_aleatorios(matriz, tamanho);
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
    //auto_save(matriz, colony_number,tamanho);

  } else if (comando == 7){
    importa_geracao(matriz,tamanho);

  } else if (comando == 8){
    printf("\nConfiguracao Atual\n");
    printf("-Mapa atual:\n");
    imprime_mapa(matriz, tamanho);
    printf("-Tamanho lateral mapa: %d \n", tamanho -2);
    printf("-Numero de gerações a ser rodadas: %d \n", num_gen[0]);
    if (tipo_sim[0] == 1){
      printf("-Tipo de simulação: Automática\n");
      printf("-Velocidade da simulação: %d \n", vel_sim[0]);

    } else{
      printf("-Tipo de simulação: Manual\n");
      printf("-Velocidade da simulação: NA \n");

    }
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
  char base_path[] = "/Users/pedrobirmann/Desktop/jdv/", extensao[] = ".txt";
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
      if(matriz [i][j] == 1){
        fprintf(fp,"%d\n",i);
        fprintf(fp,"%d\n\n",j);
      }
    }
  }
  fclose(fp);
  printf("\n\tArquivo Salvo com o nome %s.txt  \n", filename);
}


void importa_geracao(int **matriz, int tamanho){
  int i,j;
  char base_path[] = "/Users/pedrobirmann/Desktop/jdv/";
  char filename[50];
  char finalpath[100] = "";

  int aux;
  int dado;
  int lin, col, flag_lin =0, flag_col = 0;

  printf("\tEscreva o nome do seu arquivo: \n\t>> ");
  scanf("%s",filename);

  strcat(finalpath,base_path);
  strcat(finalpath,filename);

  FILE *fp;

  if((fp=fopen(finalpath, "rt"))==NULL) {
    printf("\t Erro, não foi possível abrir o seu arquivo\n");
    exit(1);
  }

  i = 0;
  while (!feof(fp)) {
    aux = fscanf(fp, "%d", &dado);

    if (flag_lin ==0){
      lin = dado;
      flag_lin = 1;
    } else  {
      col = dado;
      flag_col = 1;
    }


    if (flag_lin == 1 && flag_col == 1){
      flag_lin = 0;
      flag_col = 0;
      if (lin < tamanho - 1 || col < tamanho - 1){
        matriz[lin][col] = 1;
      }
    }
  }

  fclose(fp);
  printf("\n\tArquivo importado\n");

  imprime_mapa(matriz, tamanho);
}

int contar_seres(int **matriz, int tamanho){
  int i, j, seres = 0;
  for (i = 1; i < tamanho-1; i++){
    for (j=1; j < tamanho-1; j++){
      if (matriz[i][j] == 1){
        seres = seres + 1;
      }
    }
  }
  return seres;
}

void gerar_pontos_aleatorios(int **matriz, int tamanho){
  int linha, coluna, i, rodadas;

  rodadas = (tamanho*tamanho)/10;

  srand(time(NULL));
  for (i =0; i < rodadas; i++){
    linha = rand()%tamanho;
    coluna = rand()%tamanho;
    if(linha <0) linha = 0;
    if(coluna<0) coluna = 0;
    //printf("linha: %d \ncoluna: %d \n", linha, coluna);
    matriz[linha][coluna] = 1;
  }
  imprime_mapa(matriz, tamanho);

}

void auto_save(int **matriz, int colony_number[], int tamanho){
  int i,j;
  char base_path[] = "/Users/pedrobirmann/Desktop/jdv/", extensao[] = ".txt";
  //char filename[50];
  char finalpath[100] = "";
  char filename[3];

  sprintf(filename, "%d", colony_number[0]);

  strcat(finalpath,base_path);
  strcat(finalpath,"colonia");
  strcat(finalpath,filename);
  strcat(finalpath,extensao);

  //printf("\nabrindo arquivo\n");

  FILE *fp;

  if((fp=fopen(finalpath, "w"))==NULL) {
    printf("\t Erro, não foi possível abrir o seu arquivo\n");
    exit(1);
  }

  for (i = 0; i < tamanho; ++i){
    for (j = 0; j < tamanho; ++j){
      if(matriz [i][j] == 1){
        fprintf(fp,"%d\n",i);
        fprintf(fp,"%d\n\n",j);
      }
    }
  }
  fclose(fp);
  printf("\n\tArquivo Salvo com o nome colonia%s.txt  \n", filename);
  colony_number[0] = colony_number[0] + 1;
}

void auto_save2(int **matriz, int colony_number[], int tamanho){
  //printf("\nentrou save2\n");
  int i,j;
  char base_path[] = "/Users/pedrobirmann/Desktop/jdv2/", extensao[] = ".txt";
  char finalpath[100] = "";
  char filename[3];

  sprintf(filename, "%d", colony_number[0]);

  strcat(finalpath,base_path);
  strcat(finalpath,"colonia");
  strcat(finalpath,filename);
  strcat(finalpath,extensao);

  printf("\nabrindo arquivo\n");

  FILE *fp;

  if((fp=fopen(finalpath, "w"))==NULL) {
    printf("\t Erro, não foi possível abrir o seu arquivo\n");
    exit(1);
  }

  for (i = 0; i < tamanho; ++i){
    for (j = 0; j < tamanho; ++j){
      if(matriz [i][j] == 1){
        fprintf(fp,"%d\n",i);
        fprintf(fp,"%d\n\n",j);
      }
    }
  }
  fclose(fp);
  printf("\n\tArquivo Salvo com o nome colonia%s.txt  \n", filename);
  colony_number[0] = colony_number[0] + 1;
}

void import_for_reproduction(int colony_one, int colony_two, int **matriz_pai, int **matriz_mae, int tamanho, int directory[]){
  int i,j;

  char base_path[] = "/Users/pedrobirmann/Desktop/jdv/colonia";
  char base_path2[] = "/Users/pedrobirmann/Desktop/jdv2/colonia";

  char filename_pai[3];
  char filename_mae[3];
  char finalpath_pai[100] = "";
  char finalpath_mae[100] = "";

  limpa_matriz(matriz_pai, tamanho);
  limpa_matriz(matriz_mae, tamanho);

  sprintf(filename_pai, "%d", colony_one);

  int aux;
  int dado;
  int lin, col, flag_lin =0, flag_col = 0;
  if (directory[0] ==2 ){
    strcat(finalpath_pai,base_path);
  } else{
    strcat(finalpath_pai,base_path2);
  }

  strcat(finalpath_pai,filename_pai);
  strcat(finalpath_pai,".txt");

  FILE *fp;

  if((fp=fopen(finalpath_pai, "rt"))==NULL) {
    printf("\t Erro, não foi possível abrir o seu arquivo\n");
    exit(1);
  }

  i = 0;
  while (!feof(fp)) {
    aux = fscanf(fp, "%d", &dado);

    if (flag_lin ==0){
      lin = dado;
      flag_lin = 1;
    } else  {
      col = dado;
      flag_col = 1;
    }


    if (flag_lin == 1 && flag_col == 1){
      flag_lin = 0;
      flag_col = 0;
      if (lin < tamanho - 1 || col < tamanho - 1){
        matriz_pai[lin][col] = 1;
      }
    }
  }

  fclose(fp);

  sprintf(filename_mae, "%d", colony_two);

  if (directory[0] ==2 ){
    strcat(finalpath_mae,base_path);
  } else{
    strcat(finalpath_mae,base_path2);
  }

  strcat(finalpath_mae,filename_mae);
  strcat(finalpath_mae,".txt");

  if((fp=fopen(finalpath_mae, "rt"))==NULL) {
    printf("\t Erro, não foi possível abrir o seu arquivo\n");
    exit(1);
  }

  i = 0;
  while (!feof(fp)) {
    aux = fscanf(fp, "%d", &dado);

    if (flag_lin ==0){
      lin = dado;
      flag_lin = 1;
    } else  {
      col = dado;
      flag_col = 1;
    }

    if (flag_lin == 1 && flag_col == 1){
      flag_lin = 0;
      flag_col = 0;
      if (lin < tamanho - 1 || col < tamanho - 1){
        matriz_mae[lin][col] = 1;
      }

    }
  }

  fclose(fp);

  // printf("\n\tArquivos importado\n");
  printf("\nMatriz PAI\n");
  imprime_mapa(matriz_pai, tamanho);
  printf("\nMatriz MAE\n");
  imprime_mapa(matriz_mae, tamanho);

};

void reproduce(int **matriz_pai, int **matriz_mae, int **matriz_filha, int tamanho){
  int i, j;

  limpa_matriz(matriz_filha, tamanho);

  for (i=0; i< tamanho; i++){
    for (j=0; j<tamanho; j++){
      if (i < tamanho/2){
        matriz_filha[i][j] = matriz_pai[i][j];
      } else {
        matriz_filha[i][j] = matriz_mae[i][j];
      }
    }
  }
  imprime_mapa(matriz_filha, tamanho);
}

void create_initial_colonies(int **matriz, int **mapa_next_gen, int tamanho, int num_gen[], int tipo_sim[], int vel_sim[], int geracao_global[], int colony_number[]){
  int i,j, seres;

  for(i=0; i<100; i++){
    gerar_pontos_aleatorios(matriz,tamanho);
    auto_save(matriz,colony_number,tamanho);
    for (j=0; j<100; j++){
      rodar_simulacao(matriz, mapa_next_gen, tamanho, num_gen, tipo_sim, vel_sim, geracao_global);
    }
    seres = contar_seres(matriz,tamanho);
    survivors[i] = seres;
  }
  for(i = 0; i<100; i++){
    printf("surv: %d\n", survivors[i]);
  }

  // printf("Start ten\n");
  // get_top_ten(top_10);

}

void get_top_ten(int top_10[]){

  int i, j,k,l, count = 0;
  int max, idx;
  int temp_vec[100];

  printf("\nentr0u top ten");

  for (k=0; k<100; k++){
    temp_vec[k] = survivors[k];
    //printf(" %d-", temp_vec[k]);
  }

  while(count < 10){
    max = temp_vec[0];
    for (i=0; i<100 ;i++){
      if (temp_vec[i] > max){
        max = temp_vec[i];
        idx = i;
      }
    }
    //printf("max : %d, idx: %d", max, idx);
    top_10[count] = idx;
    temp_vec[idx] = 0;
    count = count + 1;
  }

  for (j=0;j<10;j++){
    printf("\ncolonia: %d, sobreviventes: %d", top_10[j], survivors[top_10[j]]);
  }
}

void create_next_batch(int **mapa, int **mapa_next_gen, int **matriz_pai, int **matriz_mae, int **matriz_filha,int tamanho, int num_gen[], int tipo_sim[], int vel_sim[], int geracao_global[]){

  int j,k,l;
  printf("entra next b\n" );
  colony_number[0] = 0;

  for (j=0; j<10; j++){
    for (k=0; k<10; k++){
      printf("\nimportando para reprodução");
      import_for_reproduction(top_10[j], top_10[k], matriz_pai, matriz_mae, tamanho, directory);
      printf("\nreproduzindo\n");
      reproduce(matriz_pai, matriz_mae, matriz_filha, tamanho);

      copia_matriz(mapa, matriz_filha, tamanho);
      printf("\nsalva filha\n");

      if (directory[0] == 2){
        auto_save2(mapa, colony_number, tamanho);
      } else{
        auto_save(mapa, colony_number, tamanho);
      }

      printf("\nroda filha 100 vezes\n");
      for (l=0; l<100; l++){
        //printf("\nrodada %d \n", l);
        rodar_simulacao(mapa, mapa_next_gen, tamanho, num_gen, tipo_sim, vel_sim, geracao_global);
      }
      num_ser = contar_seres(mapa, tamanho);
      survivors[(j*10)+k] = num_ser;

    }
  }

  if (directory[0] == 2){
    directory[0] = 1;
  } else {
    directory[0] = 2;
  }
}
