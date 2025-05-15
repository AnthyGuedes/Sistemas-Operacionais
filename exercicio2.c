// Anthony e Matheus, teste feito em uma maquina virtual ubunto pelo cmd
//2 – Desenvolva um algoritmo multithreading para fazer a multiplicação de uma matriz NxN por um vetor de tamanho N.

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// variaveis globais (alocadas dinamicamente)
int **matriz;
int *vetor;
int *resultado;
int N;  // ordem da matriz e tamanho do vetor

// estrutura para a thread
typedef struct {
    int linha;
} ThreadData;

// função multiplicar das thread 
void* multiplica_linha(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    int i = data->linha;
    resultado[i] = 0;

    for (int j = 0; j < N; j++) {
        resultado[i] += matriz[i][j] * vetor[j];
    }

    pthread_exit(NULL);
}

// funções auxiliares
void gerar_matriz_aleatoria() {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            matriz[i][j] = rand() % 10;
}

void gerar_vetor_aleatorio() {
    for (int i = 0; i < N; i++)
        vetor[i] = rand() % 10;
}

void imprimir_matriz() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++)
            printf("%d ", matriz[i][j]);
        printf("\n");
    }
}

void imprimir_vetor(int* v) {
    for (int i = 0; i < N; i++)
        printf("%d ", v[i]);
    printf("\n");
}

int main() {
    srand(time(NULL));
    
    // dimensão da matriz e do vetor
    printf("Informe o valor de N (ordem da matriz NxN): ");
    scanf("%d", &N);

    // aloca a matriz, vetor e resultado
    matriz = malloc(N * sizeof(int*));
    for (int i = 0; i < N; i++)
        matriz[i] = malloc(N * sizeof(int));

    vetor = malloc(N * sizeof(int));
    resultado = malloc(N * sizeof(int));

    // gera dados aleatórios
    gerar_matriz_aleatoria();
    gerar_vetor_aleatorio();

    // exibe os dados gerados
    printf("\nMatriz:\n");
    imprimir_matriz();

    printf("\nVetor:\n");
    imprimir_vetor(vetor);

    // criação das threads
    pthread_t* threads = malloc(N * sizeof(pthread_t));
    ThreadData* dados = malloc(N * sizeof(ThreadData));

    for (int i = 0; i < N; i++) {
        dados[i].linha = i;
        if (pthread_create(&threads[i], NULL, multiplica_linha, &dados[i]) != 0) {
            perror("Erro ao criar thread");
            return 1;
        }
    }

    // espera as threads terminarem
    for (int i = 0; i < N; i++) {
        pthread_join(threads[i], NULL);
    }

    // mostra o resultado
    printf("\nResultado da multiplicacao (vetor resultado):\n");
    imprimir_vetor(resultado);

    // libera memória
    for (int i = 0; i < N; i++)
        free(matriz[i]);
    free(matriz);
    free(vetor);
    free(resultado);
    free(threads);
    free(dados);

    return 0;
}
