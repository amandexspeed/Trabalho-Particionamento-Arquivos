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
int menor_valor(Cliente *clientes[], int M)
{
  //Verificação se a quantidade é invalida ou vetor de clientes vazio.
    if (M <= 0 || clientes == NULL) {
        return -1;
    }

    //Usa-se o primeiro indice do vetor de clientes para haver comparação
    int indice_menor = 0;
    if(clientes != NULL)
    for (int i = 1; i < M; i++) {
      if(clientes[i] == NULL)
      {
        break;
      }
      if (clientes[i]->cod_cliente < clientes[indice_menor]->cod_cliente) {
          indice_menor = i;
      }
    }
    return indice_menor;
}

void bubblesort(Cliente **clientes, int M) {
    int i, j;
    for (i = 0; i < M - 1; i++) {
        for (j = 0; j < M - i - 1; j++) {
            if (clientes[j]->cod_cliente > clientes[j + 1]->cod_cliente) {
                Cliente *temp = clientes[j];
                clientes[j] = clientes[j + 1];
                clientes[j + 1] = temp;
            }
        }
    }
}

// Guarda no arquvio os dados do vetor e o zera
void guarda_no_arquivo(Cliente *v[], int i, FILE *p)
{
  int j = 0;
  int menor = menor_valor(v, i);
  while (j<i)
  {
    salva_cliente(v[menor], p);
    if(v[menor]->cod_cliente == INT_MAX)
    {
      break;
    }
    v[menor] -> cod_cliente = INT_MAX;
    menor = menor_valor(v, i);
    j++;
    if(j == i)
    {
      if (v[menor]->cod_cliente != INT_MAX){
        salva_cliente(v[menor], p);
      }
    }
  }

}

//Cria apenas a partição
FILE* cria_particao(char *nome_particao,Nomes *nome_arquivos_saida)
{

  // cria arquivo de particao e faz gravacao
  if(nome_arquivos_saida == NULL)
  {
    return NULL;
  }
  nome_particao = nome_arquivos_saida->nome;
  nome_arquivos_saida = nome_arquivos_saida->prox;

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

  if ((p = fopen(nome_particao, "wb")) == NULL)
  {
    printf("Erro criar arquivo de saida\n");
  }

  return p;

}

//Cria uma nova partição pra despejar a memória dentro dela
FILE* cria_particao_nova(char *nome_particao, Nomes *nome_arquivos_saida, Cliente *v[], int i)
{
  // cria arquivo de particao e faz gravacao
    if(nome_arquivos_saida != NULL)
    {
      nome_particao = nome_arquivos_saida->nome;
      nome_arquivos_saida = nome_arquivos_saida->prox;
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
  Cliente *cin = NULL;
  int i = 0;
  while (!feof(arq) && i < M)
  {
    cin = le_cliente(arq);
    v[i] = cin;
    i++;
    
  }

  if(i<M)
    i = i-1; //Pra compensar a leitura do nulo

  return i;

}

//Para seleção com substituição
int menor_valor_cong(Cliente *clientes[], int quantidadeClientes, int congelados[], int quantidadeCong)
{
    //Verificação se a quantidade é invalida ou vetor de clientes vazio.
    if (quantidadeClientes <= 0 || clientes == NULL) {
        return -1;
    }

    int indice_menor = 0;
    while(verifica_congelado(congelados, &quantidadeCong, indice_menor))
    {
        indice_menor++;
    }


    for (int i = indice_menor + 1; i < quantidadeClientes; i++) {
        if (clientes[i] != NULL && clientes[i]->cod_cliente < clientes[indice_menor]->cod_cliente) {
            //Verificação se o indice está congelado
            
                if(!verifica_congelado(congelados, &quantidadeCong, i))
                    
                    indice_menor = i;
           
        }
    }
    return indice_menor;

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
    nome_arquivos_saida = nome_arquivos_saida->prox;

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

              if(posCong == M){

                fclose(arqPart);
                arqPart = cria_particao(nome_particao, nome_arquivos_saida);
                nome_arquivos_saida = nome_arquivos_saida->prox;
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

            quantReg = M;
            despejo = vetorCliente;

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
  Cliente **clientes = (Cliente**) malloc(sizeof(Cliente*)*M); //Lista de ponteiros de clientes.
  char *nome_arquivo_saida = nome_arquivos_saida->nome; //Nome do arquivo de saída, passado como parametro.

  FILE *entrada = fopen(nome_arquivo_entrada, "rb"); //Abertura para leitura binária do arquivo de entrada.
  FILE *saida = fopen(nome_arquivo_saida, "wb"); //Abertura para escrita binária no arquivo de saída.
  FILE *reservatorio = fopen("reservatorio.dat", "wb+"); //Abertura para escrita binária no arquivo de saída e sempre apagando quando for escrever.

  //Verificação se os clientes ou os arquivos são nulos.
  if(clientes == NULL || entrada == NULL ||  saida == NULL){
      printf("Erro ao alocar memória");
      return;
  }

  //Definição do tamanho do reservatório
  int tamanhoReservatorio = 0;

  //Variável de controle para fazer a leitura dos clientes e guarda-los no vetor de ponteiros de clientes até chegar no limite de memória.
  int i = 0;
  while(i < M && (clientes[i] = le_cliente(entrada)) != NULL)
  {
    i++;
  }

  //Ajuste no limite de memória para ser compatível com a quantidade de clientes, caso estes sejam menores que o limite.
  M = i;

  //Loop onde irá ocorrer todo o processo de seleção natural.
  while(1)
  {
      int menor = -1; //É definido -1 para caso de falha, pois se falhar em pegar o menor valor, então permanecerá no caso de falha.
        
        //Loop para fazer a verificação se todos os clientes forem NULL, ele encontre algum cliente que satisfaça a condição, prossegue.
        for(int j = 0; j < M; j++){
            if(clientes[j] != NULL){
                menor = j;
                break;
            }
        }

        //Caso ele falhe no primeiro loop, o loop principal irá ser encerrado.
        if(menor == -1){
            break;
        }

        //Verificação se alguma casa em específica é NULL e compara os códigos de clientes na casa atual do loop com o código do menor cliente até então, assim, definindo o menor.
        for(int j = menor + 1; j < M; j++){
            if(clientes[j] != NULL && clientes[j]->cod_cliente < clientes[menor]->cod_cliente)
                menor = j;
        }

      //Chave que irá receber o código do menor cliente.
      int key = clientes[menor]->cod_cliente;

      //Guarda o cliente com o menor código na partição de saída.
      salva_cliente(clientes[menor], saida);

      //Como key já possui o valor do menor código no vetor de clientes, e o cliente já foi guardado na partição de saída, então, é dado um free neste cliente.
      free(clientes[menor]);

      //É lido o próximo cliente e guardado na casa do antigo menor.
      clientes[menor] = le_cliente(entrada);

      //Loop para fazer o controle do reservatório
      while(clientes[menor] != NULL && clientes[menor]->cod_cliente < key)
      {
        salva_cliente(clientes[menor], reservatorio);
        tamanhoReservatorio++;

        if(tamanhoReservatorio == n)
        {
          break;
        }
        free(clientes[menor]);
        clientes[menor] = le_cliente(entrada);
      }

      //Caso o tamanho do reservatório seja igual ao número de registros.
      if(tamanhoReservatorio == n)
      {
        //É aplicado um bubblesort nos que estão ainda na memória.
        bubblesort(clientes, M);

        //É salvo o restante na partição de saída.
        for(i = 1; i < M; i++)
        {
          salva_cliente(clientes[i], saida);
          free(clientes[i]);
        }

        //Fecha o arquivo de saída
        fclose(saida);
        //Fecha o reservatório
        fclose(reservatorio);

        //Abre o reservatório para que seja puxado os clientes que estão nele para a memória.
        reservatorio = fopen("reservatorio.dat", "rb+");

        i = 0;
        while(i < M && (clientes[i] = le_cliente(reservatorio)) != NULL)
        {
          i++;
        }
        //Atualiza o tamanho da memória com o do reservatório para a próxima chamada do loop.
        M = tamanhoReservatorio;

        //Fecha o reservatório
        fclose(reservatorio);

        //Abre novamente o reservatório porém no modo de leitura e escrita, onde ele irá apagar os dados que estão no arquivo, de modo que seja possível usa-lo novamente.
        reservatorio = fopen("reservatorio.dat", "wb+");
        //Atualização do tamanho do reservatório
        tamanhoReservatorio = 0;

        //Muda para o próximo arquivo de saída (partição).
        nome_arquivos_saida = nome_arquivos_saida->prox;
        nome_arquivo_saida = nome_arquivos_saida->nome;
        //Abre para a nova partição para saida.
        saida = fopen(nome_arquivo_saida, "wb");
      }
  }
  fclose(entrada);
  fclose(reservatorio);
  fclose(saida);
}

