#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main(){
	float comprimento, largura;
	float area, perimetro;
  
    // Leitura das medidas do terreno
    printf("Digite o comprimento do terreno: ");
    scanf("%f", &comprimento);
    printf("Digite a largura do terreno: ");
    scanf("%f", &largura);

    // Criação do processo filho
    pid_t pid = fork();

    if (pid < 0) {
        // Caso ocorra um erro ao criar o processo filho
        perror("Erro ao criar o processo filho");
        exit(1);
    }

    if (pid == 0) {
        // Processo Filho - Calcula a área
        area = comprimento * largura;

        printf("Processo Filho: A área do terreno é %.2f m²\n", area);
        exit(0); // Finaliza o processo filho

        // Finaliza o processo filho
        return 0;
    } else {
        // Processo Pai - Calcula o perímetro
        perimetro = 2 * (comprimento + largura);



        // Espera o processo filho terminar
        wait(NULL);

        // Lê os resultados dos pipes
        read(pipe_area[0], &area, sizeof(area));
        read(pipe_perimetro[0], &perimetro, sizeof(perimetro));

        // Exibe os resultados
        printf("Área do terreno: %.2f\n", area);
        printf("Perímetro do terreno: %.2f\n", perimetro);
    }
	return 0;
}
