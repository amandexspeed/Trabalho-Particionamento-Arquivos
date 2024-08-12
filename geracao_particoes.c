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

void classificacao_interna(char *nome_arquivo_entrada, Nomes *nome_arquivos_saida, int M)
{
	int fim = 0; //variável de controle para saber se arquivo de entrada terminou
    FILE *arq; //declara ponteiro para arquivo

    //abre arquivo para leitura
    if ((arq = fopen(nome_arquivo_entrada, "rb")) == NULL) {
        printf("Erro ao abrir arquivo de entrada\n");
    } else {

        //le primeiro cliente
        Cliente *cin = le_cliente(arq);

        while (!(fim)) {
            //le o arquivo e coloca no vetor
            Cliente *v[M];

            int i = 0;
            while (!feof(arq) && i < M) {
                v[i] = cin;
                cin = le_cliente(arq);
                i++;
            }

            //ajusta tamanho M caso arquivo de entrada tenha terminado antes do vetor
            if (i != M) {
                M = i;
            }

            //faz ordenacao
            for (int j = 1; j < M; j++) {
                Cliente *c = v[j];
                i = j - 1;
                while ((i >= 0) && (v[i]->cod_cliente > c->cod_cliente)) {
                    v[i + 1] = v[i];
                    i = i - 1;
                }
                v[i + 1] = c;
            }

            //cria arquivo de particao e faz gravacao
            char *nome_particao = nome_arquivos_saida->nome;
            nome_arquivos_saida = nome_arquivos_saida->prox;

            FILE *p;
            if ((p = fopen(nome_particao, "wb")) == NULL) {
                printf("Erro criar arquivo de saida\n");
            } else {
                for (int i = 0; i < M; i++) {
					salva_cliente(v[i], p);
                }
                fclose(p);
            }
            if (feof(arq)) {
                fim = 1;
            }
        }
    }
}

int menor_valor(Cliente **clientes, int quantidade)
{
    //Verificação se a quantidade é invalida ou vetor de clientes vazio.
    if (quantidade <= 0 || clientes == NULL) {
        return -1;
    }

    //Usa-se o primeiro indice do vetor de clientes para haver comparação
    int indice_menor = 0;
    for (int i = 1; i < quantidade; i++) {
        if (clientes[i] != NULL && clientes[i]->cod_cliente < clientes[indice_menor]->cod_cliente) {
            indice_menor = i;
        }
    }
    return indice_menor;
}

void gerir_reservatorio(Cliente **clientes, int quantidade, FILE *nome_arquivo_entrada, FILE *nome_arquivo_saida) 
{
    int i;
    Cliente *novo_cliente;

    while (1) {
        // Encontra o índice do menor cliente no vetor novo_cliente
        int indice_menor = menor_valor(clientes, quantidade);
        
        if (indice_menor == -1) {
            break; // Se o índice for inválido, interrompe o loop
        }

        // Salva o cliente de menor valor no arquivo de saída
        salva_cliente(clientes[indice_menor], nome_arquivo_saida);

        //Lê o próximo cliente do arquivo principal (levando em consideração que o curso já passou dos clientes lidos)
        novo_cliente = le_cliente(nome_arquivo_entrada);

        //Se não houver mais clientes no arquivo, interrompe o loop
        if (novo_cliente == NULL) { 
            break;
        }

        // Compara o código do novo cliente com o cliente de menor valor, caso o código do cliente do arquivo seja maior do que o código de vetor, substitue
        if (novo_cliente->cod_cliente > clientes[indice_menor]->cod_cliente) {
            free(clientes[indice_menor]); // Libera o espaço de memória do cliente anterior
            clientes[indice_menor] = novo_cliente;
        } else {
            free(novo_cliente); // Libera a memória do novo cliente já que ele não será usado
        }
    }
    
    // Aquele bubble sort de leve pra ordenar os clientes restantes
    for (i = 0; i < quantidade - 1; i++) {
        for (int j = i + 1; j < quantidade; j++) {
            if (clientes[i] != NULL && clientes[j] != NULL && 
                clientes[i]->cod_cliente > clientes[j]->cod_cliente) {
                // Troca os clientes de posição
                Cliente *temp = clientes[i];
                clientes[i] = clientes[j];
                clientes[j] = temp;
            }
        }
    }

    // Salva os clientes restantes no arquivo de saída
    for (i = 0; i < quantidade; i++) {
        if (clientes[i] != NULL) {
            salva_cliente(clientes[i], nome_arquivo_saida);
            free(clientes[i]); // Libera a memória do cliente
        }
    }
/*------Congelamento------*/
/*
    -> Recebe o vetor onde estão sendo guardados os elementos que vão ser ignorados;
    -> Recebe o dado que vai ser congelado;
    -> Recebe a posição e o tamanho do vetor;
    -> Verifica se a posição não ultrapassa o tamanho máximo do vetor;
    -> Caso seja menor, guarda no vetor, na posição indicada, o dado;
    -> Caso não, retorna 0.
*/
int congela(int *vetor, int dado, int *tam, int max) {
    if (tam < max)
    {
        vetor[(*tam)++] = dado;
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
int verifica_congelado(int *vetor, int *tam, int cod_atual) {
    for (int i = 0; i < tam; i++)
        if (vetor[i] == cod_atual)
            return 1;
    return 0;
}

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

void selecao_com_substituicao(char *nome_arquivo_entrada, Nomes *nome_arquivos_saida, int M)
{    
    int dado, memoria;
    int *vetor, tam;
    vetor = (int*) malloc(sizeof(int)*M);
    // uso
    if (compara_arq_memoria(dado, memoria))
    {
        congela(vetor, dado, &tam, M);
    }
        
	//TODO: Inserir aqui o codigo do algoritmo de geracao de particoes
}

void selecao_natural(char *nome_arquivo_entrada, Nomes *nome_arquivos_saida, int M, int n)
{
	//TODO: Inserir aqui o codigo do algoritmo de geracao de particoes
}

