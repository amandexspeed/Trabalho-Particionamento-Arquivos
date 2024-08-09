//
// Created by Vanessa Braganholo on 16/09/2018.
// Updated by Raphael Oliveira on 18/09/2021.

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include "geracao_particoes.h"
#include "nomes.h"
#include "cliente.h"
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
        for (int i = 0; i < M; i++)
        {
          salva_cliente(v[i], p);
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

// Guarda no arquvio os dados do vetor e o zera
void guarda_no_arquivo(Cliente **v, int i, FILE *p)
{
  int menor = procuraMenor(v, i);
  while (menor != INT_MAX)
  {
    salva_cliente(v[menor], p);
    v[menor] = cliente(INT_MAX, "");
    menor = procuraMenor(v, i);
  }
}

void cria_particao(char *nome_particao, Nomes *nome_arquivos_saida, Cliente **v, FILE *p, int requisita_funcao, int menor, int i)
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

// carregar vetor com os M registros
void carrega_registros(Cliente *v[], FILE *arq, int M)
{
  Cliente *cin = le_cliente(arq);
  Cliente *v[M];
  int i = 0;
  while (!feof(arq) && i < M)
  {
    v[i] = cin;
    cin = le_cliente(arq);
    i++;
  }

  if (i != M)
    M = i;
}

void selecao_com_substituicao(char *nome_arquivo_entrada, Nomes *nome_arquivos_saida, int M)
{
  // TODO: Inserir aqui o codigo do algoritmo de geracao de particoes
}

void selecao_natural(char *nome_arquivo_entrada, Nomes *nome_arquivos_saida, int M, int n)
{
  // TODO: Inserir aqui o codigo do algoritmo de geracao de particoes
}
