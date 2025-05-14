// Anthony e Matheus, testes foram feitos numa maquina virtual ubunto pelo cmd
// 1 – Desenvolva um algoritmo multithreading para efetuar a soma de duas matrizes de ordem NxM.   

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// estrutura de dados para passar informações às threads
typedef struct {
    int **A;
    int **B;
    int **C;
    int linha_inicio;
    int linha_fim;
    int colunas;
} DadosThread;

// função executada por cada thread
void *soma_matrizes(void *arg) {
    DadosThread *dados = (DadosThread *) arg;

    for (int i = dados->linha_inicio; i < dados->linha_fim; i++) {
        for (int j = 0; j < dados->colunas; j++) {
            dados->C[i][j] = dados->A[i][j] + dados->B[i][j];
        }
    }

    pthread_exit(NULL);
}

//  alocar matriz dinamicamente
int **alocar_matriz(int linhas, int colunas) {
    int **matriz = malloc(linhas * sizeof(int *));
    for (int i = 0; i < linhas; i++) {
        matriz[i] = malloc(colunas * sizeof(int));
    }
    return matriz;
}

// liberar memória de matriz
void liberar_matriz(int **matriz, int linhas) {
    for (int i = 0; i < linhas; i++) {
        free(matriz[i]);
    }
    free(matriz);
}

// preencher matriz com valores digitados pelo usuário
void preencher_matriz(int **matriz, int linhas, int colunas, const char *nome) {
    printf("Digite os elementos da matriz %s:\n", nome);
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            printf("%s[%d][%d]: ", nome, i, j);
            scanf("%d", &matriz[i][j]);
        }
    }
}

// exibir matriz
void imprimir_matriz(const char *nome, int **matriz, int linhas, int colunas) {
    printf("Matriz %s:\n", nome);
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            printf("%4d ", matriz[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main() {
    int linhas, colunas, num_threads;

    
    printf("Informe o número de linhas: ");
    scanf("%d", &linhas);

    printf("Informe o número de colunas: ");
    scanf("%d", &colunas);

    printf("Informe o número de threads: ");
    scanf("%d", &num_threads);

    
    int **A = alocar_matriz(linhas, colunas);
    int **B = alocar_matriz(linhas, colunas);
    int **C = alocar_matriz(linhas, colunas);

    
    preencher_matriz(A, linhas, colunas, "A");
    preencher_matriz(B, linhas, colunas, "B");

    // criação das threads
    pthread_t threads[num_threads];
    DadosThread dados[num_threads];

    int linhas_por_thread = linhas / num_threads;
    int linha_atual = 0;

    for (int i = 0; i < num_threads; i++) {
        dados[i].A = A;
        dados[i].B = B;
        dados[i].C = C;
        dados[i].colunas = colunas;
        dados[i].linha_inicio = linha_atual;
        dados[i].linha_fim = (i == num_threads - 1) ? linhas : linha_atual + linhas_por_thread;

        pthread_create(&threads[i], NULL, soma_matrizes, &dados[i]);

        linha_atual = dados[i].linha_fim;
    }

    // espera o término de todas as threads
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    
    imprimir_matriz("A", A, linhas, colunas);
    imprimir_matriz("B", B, linhas, colunas);
    imprimir_matriz("C (A + B)", C, linhas, colunas);

    // liberação da memória
    liberar_matriz(A, linhas);
    liberar_matriz(B, linhas);
    liberar_matriz(C, linhas);

    return 0;
}
