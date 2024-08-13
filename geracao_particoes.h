//
// Created by Vanessa Braganholo on 16/09/2018.
// Updated by Raphael Oliveira on 18/09/2021.

#ifndef GERACAO_PARTICOES_H
#define GERACAO_PARTICOES_H

#include "cliente.h"
#include "nomes.h"

// Executa o algoritmo de geracao de particoes por Classificacao Interna
// nome_arquivo_entrada: nome do arquivo de entrada
// nome_arquivo_saida: lista encadeada com nomes de arquivos de saida
// M: tamanho do array em memoria para manipula��o dos registros
void classificacao_interna(char *nome_arquivo_entrada, Nomes *nome_arquivos_saida, int M);

// Executa o algoritmo de geracao de particoes por Selecao com Substituicao
// nome_arquivo_entrada: nome do arquivo de entrada
// nome_arquivo_saida: lista encadeada com nomes de arquivos de saida
// M: tamanho do array em memoria para manipula��o dos registros
void selecao_com_substituicao(char *nome_arquivo_entrada, Nomes *nome_arquivos_saida, int M);

void guarda_no_arquivo(Cliente **v, int i, FILE *p);
//void cria_particao(char *nome_particao, Nomes *nome_arquivos_saida, Cliente **v, FILE *p, int requisita_funcao, int menor, int i);
void carrega_registros(Cliente *v[], FILE *arq, int M);

// Executa o algoritmo de geracao de particoes por Selecao Natural
// nome_arquivo_entrada: nome do arquivo de entrada
// nome_arquivo_saida: lista encadeada com nomes de arquivos de saida
// M: tamanho do array em memoria para manipula��o dos registros
// n: tamanho do reservatorio
void selecao_natural(char *nome_arquivo_entrada, Nomes *nome_arquivos_saida, int M, int n);

#endif