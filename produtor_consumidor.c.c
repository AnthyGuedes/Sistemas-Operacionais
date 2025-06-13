
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define TAMANHO_BUFFER 5
#define TOTAL_PRODUCOES 10

int buffer[TAMANHO_BUFFER];
int inicio = 0;
int fim = 0;
int quantidade = 0;

pthread_mutex_t trava = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t pode_produzir = PTHREAD_COND_INITIALIZER;
pthread_cond_t pode_consumir = PTHREAD_COND_INITIALIZER;

void* produtor(void* arg) {
    for (int i = 0; i < TOTAL_PRODUCOES; i++) {
        int item = rand() % 100;

        pthread_mutex_lock(&trava);

        while (quantidade == TAMANHO_BUFFER) {
            printf("Produtor esperando (buffer cheio)\n");
            pthread_cond_wait(&pode_produzir, &trava);
        }

        buffer[inicio] = item;
        inicio = (inicio + 1) % TAMANHO_BUFFER;
        quantidade++;
        printf("Produtor produziu: %d\n", item);

        pthread_cond_signal(&pode_consumir);
        pthread_mutex_unlock(&trava);

        sleep(1);
    }
    pthread_exit(NULL);
}

void* consumidor(void* arg) {
    for (int i = 0; i < TOTAL_PRODUCOES; i++) {
        pthread_mutex_lock(&trava);

        while (quantidade == 0) {
            printf("Consumidor esperando (buffer vazio)\n");
            pthread_cond_wait(&pode_consumir, &trava);
        }

        int item = buffer[fim];
        fim = (fim + 1) % TAMANHO_BUFFER;
        quantidade--;
        printf("Consumidor consumiu: %d\n", item);

        pthread_cond_signal(&pode_produzir);
        pthread_mutex_unlock(&trava);

        sleep(2);
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t th_produtor, th_consumidor;

    pthread_create(&th_produtor, NULL, produtor, NULL);
    pthread_create(&th_consumidor, NULL, consumidor, NULL);

    pthread_join(th_produtor, NULL);
    pthread_join(th_consumidor, NULL);

    printf("\nFim da produção e consumo!\n");
    return 0;
}
