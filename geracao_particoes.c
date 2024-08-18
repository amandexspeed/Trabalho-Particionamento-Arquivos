//
// Created by Vanessa Braganholo on 16/09/2018.
// Updated by Raphael Oliveira on 18/09/2021.

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "geracao_particoes.h"
/*#include "nomes.h"
#include "cliente.h"
*/
#include "limits.h"

void classificacao_interna(char *nome_arquivo_entrada, Nomes *nome_arquivos_saida, int M)
{
  int fim = 0; // variável de controle para saber se arquivo de entrada terminou
  FILE *arq;   // declara ponteiro para arquivo

  // abre arquivo para leitura
  if ((arq = fopen(nome_arquivo_entrada, "rb")) == NULL)
  {
    printf("Erro ao abrir arquivo de entrada\n");
  }
  else
  {

    // le primeiro cliente
    Cliente *cin = le_cliente(arq);

    while (!(fim))
    {
      // le o arquivo e coloca no vetor
      Cliente *v[M];

      int i = 0;
      while (!feof(arq) && i < M)
      {
        v[i] = cin;
        cin = le_cliente(arq);
        i++;
      }

      // ajusta tamanho M caso arquivo de entrada tenha terminado antes do vetor
      if (i != M)
      {
        M = i;
      }

      // faz ordenacao
      for (int j = 1; j < M; j++)
      {
        Cliente *c = v[j];
        i = j - 1;
        while ((i >= 0) && (v[i]->cod_cliente > c->cod_cliente))
        {
          v[i + 1] = v[i];
          i = i - 1;
        }
        v[i + 1] = c;
      }

      // cria arquivo de particao e faz gravacao
      char *nome_particao = nome_arquivos_saida->nome;
      nome_arquivos_saida = nome_arquivos_saida->prox;

      FILE *p;
      if ((p = fopen(nome_particao, "wb")) == NULL)
      {
        printf("Erro criar arquivo de saida\n");
      }
      else
      {
        for (int j = 0; j < M; j++)
        {
          salva_cliente(v[j], p);
        }
        fclose(p);
      }
      if (feof(arq))
      {
        fim = 1;
      }
    }
  }
}

int verifica_congelado(int vetor[], int *tam, int cod_atual) {
    for (int i = 0; i < *tam; i++)
        if (vetor[i] == cod_atual)
            return 1;
    return 0;
}

//Para seleção natural
int menor_valor(Cliente *clientes[], int quantidade)
{
    //Verificação se a quantidade é invalida ou vetor de clientes vazio.
    if (quantidade <= 0 || clientes == NULL) {
        return -1;
    }

    //Usa-se o primeiro indice do vetor de clientes para haver comparação
    int indice_menor = 0;
    if(clientes != NULL)
    for (int i = 1; i < quantidade; i++) {
        if (clientes[i] != NULL && clientes[i]->cod_cliente < clientes[indice_menor]->cod_cliente) {
            indice_menor = i;
        }
    }
    return indice_menor;
}

// Guarda no arquvio os dados do vetor e o zera
void guarda_no_arquivo(Cliente *v[], int i, FILE *p)
{
  int menor = menor_valor(v, i);
  for(int j = 0; j < i; j++)
  {
    salva_cliente(v[menor], p);
    v[menor] = cliente(INT_MAX, "");
    menor = menor_valor(v, i);
  }
}

/*void cria_particao(char *nome_particao, Nomes *nome_arquivos_saida, Cliente **v, FILE *p, int requisita_funcao, int menor, int i)
{
  if ((p = fopen(nome_particao, "wb")) == NULL)
  {
    printf("Erro criar arquivo de saida\n");
  }
  else
  {
    if (requisita_funcao == 1)
      guarda_no_arquivo(v, i, p);

    fclose(p);
  }
}
*/



//Cria apenas a partição
FILE* cria_particao(char *nome_particao,Nomes *nome_arquivos_saida)
{

  // cria arquivo de particao e faz gravacao
  if(nome_arquivos_saida == NULL)
  {
    return NULL;
  }
  
  nome_arquivos_saida = nome_arquivos_saida->prox;
  nome_particao = nome_arquivos_saida->nome;

  FILE *p = NULL;
  if(nome_particao!=NULL)
    if ((p = fopen(nome_particao, "wb")) == NULL)
    {
      printf("Erro criar arquivo de saida\n");
    }

  return p;

}

//Despeja o que está no vetor no ponteiro p e vê se precisa criar outra partição
FILE* termina_e_cria_particao(FILE *p, char *nome_particao, Nomes *nome_arquivos_saida, Cliente *v[], int i)
{

  guarda_no_arquivo(v, i, p);
  fclose(p);

  // cria arquivo de particao e faz gravacao
  if(nome_arquivos_saida == NULL)
  {
    return NULL;
  }
  nome_arquivos_saida = nome_arquivos_saida->prox;
  nome_particao = nome_arquivos_saida->nome;

  if(nome_arquivos_saida == NULL)
  {
    return NULL;
  }

  if ((p = fopen(nome_particao, "wb")) == NULL)
  {
    printf("Erro criar arquivo de saida\n");
  }

  return p;

}

//Cria uma nova partição pra despejar a memória dentro dela
FILE* cria_particao_nova(char *nome_particao, Nomes *nome_arquivos_saida, Cliente *v[], int i)
{
  if(nome_arquivos_saida != NULL)
  {
  // cria arquivo de particao e faz gravacao
    nome_arquivos_saida = nome_arquivos_saida->prox;
    if(nome_arquivos_saida != NULL)
    {
      nome_particao = nome_arquivos_saida->nome;
    }
  }

  FILE *p = NULL;
  if(nome_particao!=NULL){
    if ((p = fopen(nome_particao, "wb")) == NULL)
    {
      printf("Erro criar arquivo de saida\n");
    }
    else
    {
        guarda_no_arquivo(v, i, p);
        fclose(p);
    }
  }
  
  return p;

}


// carregar vetor com os M registros
int carrega_registros(Cliente *v[], FILE *arq, int M)
{
  Cliente *cin = le_cliente(arq);
  int i = 0;
  while (!feof(arq) && i < M)
  {
    v[i] = cin;
    cin = le_cliente(arq);
    i++;
  }

  return i;

}


//Para seleção com substituição
int menor_valor_cong(Cliente *clientes[], int quantidadeClientes, int congelados[], int quantidadeCong)
{
    //Verificação se a quantidade é invalida ou vetor de clientes vazio.
    if (quantidadeClientes <= 0 || clientes == NULL) {
        return -1;
    }

    //Usa-se o primeiro indice do vetor de clientes para haver comparação
    int indice_menor = 0;

    for (int i = 1; i < quantidadeClientes; i++) {
        if (clientes[i] != NULL && clientes[i]->cod_cliente < clientes[indice_menor]->cod_cliente) {
            //Verificação se o indice está congelado
            
                if(!verifica_congelado(congelados, &quantidadeCong, i))
                    
                    indice_menor = i;
           
        }
    }
    return indice_menor;

}
//Atualização seleção natural
void gerir_reservatorio(Cliente *clientes[], FILE *nome_arquivo_entrada, FILE *nome_arquivo_saida, FILE *reservatorio, int M, Nomes **nome_arquivos_saida) 
{
    int capacidade_reservatorio = 0;
    int i;
    Cliente *auxiliar;
    while (nome_arquivos_saida != NULL) {
        while (1) {
            // Encontra o índice do menor cliente no vetor novo_cliente
            int indice_menor = menor_valor(clientes, M);
            
            if (indice_menor == -1) {
                break; // Se o índice for inválido, interrompe o loop
            }

            // Salva o cliente de menor valor no arquivo de saída
            salva_cliente(clientes[indice_menor], nome_arquivo_saida);

            auxiliar = clientes[indice_menor];

            //Lê o próximo cliente do arquivo principal
            clientes[indice_menor] = le_cliente(nome_arquivo_entrada);

            //Lógica para guardar no reservatório
            if (clientes[indice_menor]->cod_cliente < auxiliar->cod_cliente && capacidade_reservatorio < M-1) {
                    salva_cliente(clientes[indice_menor], reservatorio);
                    clientes[indice_menor] = le_cliente(nome_arquivo_entrada); //Faz a leitura do próximo cliente no arquivo de entrada
                    capacidade_reservatorio++;
            }

            //Se o reservatório chegar ao seu limite termina o loop da lógica do reservatório
            if (capacidade_reservatorio == M - 1){
                break;
            }
        }

        // Aplicação do bubblesort para organizar os clientes que estão na memória antes de serem colocados na partição
        for (i = 0; i < M - 1; i++) {
            for (int j = i + 1; j < M; j++) {
                if (clientes[i] != NULL && clientes[j] != NULL && 
                    clientes[i]->cod_cliente > clientes[j]->cod_cliente) {
                    // Troca os clientes de posição
                    Cliente *temp = clientes[i];
                    clientes[i] = clientes[j];
                    clientes[j] = temp;
                }
            }
        }

        // Guarda os clientes no arquivo de saída
        guarda_no_arquivo(clientes, M, nome_arquivo_saida);

        //Fecha o atual arquivo de saida
        fclose(nome_arquivo_saida);

        // Muda o nome da partição para a próxima partição
        *nome_arquivos_saida = (*nome_arquivos_saida)->prox;

        //Carrega o próximo arquivo de saída
        nome_arquivo_saida = fopen((*nome_arquivos_saida)->nome, "wb");

        // Carregar registros do reservatório
        rewind(reservatorio); //Como o cursor foi andando conforme foi sendo escrito, é necessário voltar
        carrega_registros(clientes, reservatorio, M);

        // Limpa o reservatório para ser usado novamente
        freopen("reservatorio.dat", "wb+", reservatorio);
    }
}


/*------Congelamento------*/
/*
    -> Recebe o vetor onde estão sendo guardados os elementos que vão ser ignorados;
    -> Recebe a posição do dado que vai ser congelado;
    -> Recebe a posição e o tamanho do vetor;
    -> Verifica se a posição não ultrapassa o tamanho máximo do vetor;
    -> Caso seja menor, guarda no vetor, na posição indicada, o dado;
    -> Caso não, retorna 0.
*/
int congela(int vetor[], int pos, int *tam, int max) {
    if (*tam < max)
    {
        vetor[(*tam)++] = pos;
        return 1;
    }
    return 0;
}

/*------Verifica se a posição está congelada------*/
/*
    -> Recebe o vetor onde estão sendo guardados os elementos que vão ser ignorados;
    -> Recebe a posição e o tamanho do vetor;
    -> Recebe o código atual do loop exterior;
    -> Verifica se código está congelado;
    -> Caso esteja, retorna 1;
    -> Caso não, retorna 0.
*/


/*------Comparação do último registro com o próximo dado do arquivo------*/
/*
    -> Recebe o dado do arquivo e compara com o último dado inserido na partição;
    -> Basicamente, é retornado um inteiro que indica se o dado é menor ou não;
    -> Esse método pode ser utilizado tanto na etapa antes do congelamento que é caso
    do seleção com substituição, quanto para inserir no reservatório no caso da seleção naturol.
*/
int compara_arq_memoria(int dado, int memoria) {
    return dado < memoria ? 1 : 0;
  
}


//------------------------FUNÇÕES PRINCIPAIS------------------------//

void selecao_com_substituicao(char *nome_arquivo_entrada, Nomes *nome_arquivos_saida, int M)
{    
  FILE *arqEnt, *arqPart;   // declara ponteiro para arquivo
  char *nome_particao = nome_arquivos_saida->nome;
  if ((arqEnt = fopen(nome_arquivo_entrada, "rb")) == NULL)
  {
    printf("Erro ao abrir arquivo de entrada\n");
  }
  else
  {
    Cliente *vetorCliente[M];
    int carregados = carrega_registros(vetorCliente, arqEnt, M);

    Cliente *dadoArq = le_cliente(arqEnt);

    if(dadoArq==NULL){

      fclose(arqEnt);
      cria_particao_nova(nome_particao, nome_arquivos_saida, vetorCliente, carregados);
      return;
    }
    Cliente *memoria = cliente(INT_MAX, "");

    int vetorCong[M], posCong = 0;

    arqPart = cria_particao(nome_particao, nome_arquivos_saida);

    if (arqPart == NULL)
    {
      printf("Erro ao abrir arquivo de particao\n");
    }
    else
    {

      while(dadoArq != NULL){
        
        int posicaoMenor = menor_valor_cong(vetorCliente, M, vetorCong, posCong);

        memoria = vetorCliente[posicaoMenor];
        salva_cliente(memoria, arqPart);
        vetorCliente[posicaoMenor] = dadoArq;

        if (compara_arq_memoria(dadoArq ->cod_cliente, memoria->cod_cliente))
        {
            if(congela(vetorCong, posicaoMenor, &posCong, M)){

              if(posCong == M-1){

                fclose(arqPart);
                arqPart = cria_particao(nome_particao, nome_arquivos_saida);
                if(arqPart==NULL){
                  break;
                }
                posCong = 0;
              }

            }else{

              printf("Erro ao congelar\n");

            }
        }

        dadoArq = le_cliente(arqEnt);
        if(dadoArq == NULL){

          Cliente ** despejo = NULL;
          int quantReg;

          if(posCong==0){

            quantReg = M - 1;
            despejo = (Cliente**) malloc(sizeof(Cliente*)*quantReg);
            int j=0;
            for(int i=0; i<M; i++){
              if(i!=posicaoMenor)
                despejo[j++] = vetorCliente[i];

            }

          }
          else{

            quantReg = posCong;
            despejo = (Cliente**) malloc(sizeof(Cliente*)*posCong);

            for(int i=0; i<posCong; i++){

              despejo[i] = vetorCliente[vetorCong[i]];
              vetorCliente[vetorCong[i]] = NULL;

            }
          }

          termina_e_cria_particao(arqPart , nome_particao, nome_arquivos_saida, despejo, quantReg);

        }

      }
    }
  }
}

void selecao_natural(char *nome_arquivo_entrada, Nomes *nome_arquivos_saida, int M, int n)
{
	  Cliente *clientes[M];
    FILE * reservatorio = fopen("reservatorio.dat", "wb+");
    FILE *entrada = fopen(nome_arquivo_entrada, "rb");
    FILE *saida = fopen(nome_arquivos_saida->nome, "wb");
    

    if (reservatorio == NULL || entrada == NULL) {
        printf("Erro ao abrir arquivos.\n");
        return;
    }

    int carregados = carrega_registros(clientes, entrada, M);

    if(carregados<M){

      fclose(entrada);
      cria_particao_nova(nome_arquivos_saida->nome, nome_arquivos_saida, clientes, carregados);
      return;
    }

    gerir_reservatorio(clientes, entrada, saida, reservatorio, n, &nome_arquivos_saida);

    fclose(reservatorio);
    fclose(entrada);
}

