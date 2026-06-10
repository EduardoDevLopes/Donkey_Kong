#include <ranking.h>
#include <stdio.h>
#include <string.h>

void CarregarPlacar(TIPO_PLACAR placar[]) {
    for (int i = 0; i < 10; i++) {
        strcpy(placar[i].nome, "---");
        placar[i].time = 999999; // Tempo alto indica posição vaga/não preenchida
    }

    FILE *arq = fopen("placar/placar.bin", "rb");
    if (arq != NULL) {
        fread(placar, sizeof(TIPO_PLACAR), 10, arq);
        fclose(arq);
    }
}

void VerificarESalvarPlacar(const char *nome, int tempoTotal) {
    TIPO_PLACAR placar[10];
    CarregarPlacar(placar); // Carrega o estado atual do disco

    int posicaoInsercao = -1;

    // Procura o local correto de inserção (menor tempo fica no topo)
    for (int i = 0; i < 10; i++) {
        if (tempoTotal < placar[i].time) {
            posicaoInsercao = i;
            break;
        }
    }

    // Se o jogador entrou para o TOP 10 melhores tempos
    if (posicaoInsercao != -1) {
        // Algoritmo de Shift: empurra elementos para baixo abrindo a vaga
        for (int i = 9; i > posicaoInsercao; i--) {
            placar[i] = placar[i - 1];
        }

        // Armazena as informações do novo recordista
        strncpy(placar[posicaoInsercao].nome, nome, 19);
        placar[posicaoInsercao].nome[19] = '\0'; // Garante terminação nula segura
        placar[posicaoInsercao].time = tempoTotal;

        // Sobrescreve as 10 structs atualizadas no arquivo binário
        FILE *arq = fopen("placar/placar.bin", "wb");
        if (arq != NULL) {
            fwrite(placar, sizeof(TIPO_PLACAR), 10, arq);
            fclose(arq);
        }
    }
}