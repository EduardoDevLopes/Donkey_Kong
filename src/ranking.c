#include <ranking.h>
#include <stdio.h>
#include <string.h>

// CARREGAMENTO DO RANKING
void CarregarPlacar(TIPO_PLACAR placar[]) {
    // INICIALIZAÇÃO DE VALORES PADRÃO 
    // Preenche todas as 10 posições com entradas vazias
    for (int i = 0; i < 10; i++) {
    strcpy(placar[i].nome, "---");      // Nome vazio "---"
        placar[i].time = 999999;            // Tempo alto indica posição não preenchida
    }

    // LEITURA DO ARQUIVO 
    FILE *arq = fopen("placar/placar.bin", "rb");
    if (arq != NULL) {
        // Arquivo existe - lê as 10 estruturas TIPO_PLACAR do arquivo
        fread(placar, sizeof(TIPO_PLACAR), 10, arq);
        fclose(arq);
    }
    // Se arquivo não existe, mantém os valores padrão inicializados acima
}

// VERIFICAÇÃO E SALVAMENTO DE RANKING
void VerificarESalvarPlacar(const char *nome, int tempoTotal) {
    // CARREGAMENTO DO RANKING ATUAL
    TIPO_PLACAR placar[10];
    CarregarPlacar(placar); // Carrega estado atual do disco

    int posicaoInsercao = -1;// Assume que o jogador não entrou no ranking

    // BUSCA DA POSIÇÃO CORRETA
    // Procura por onde inserir o novo tempo
    // O ranking é mantido em ordem crescente (menor tempo no topo)
    for (int i = 0; i < 10; i++) {
        if (tempoTotal < placar[i].time) { // Novo tempo é melhor que o tempo na posição i
            posicaoInsercao = i; // Encontrou o lugar correto
            break;
        }
    }

    // INSERÇÃO DO NOVO RECORDISTA 
    if (posicaoInsercao != -1) {
        // O jogador entrou no TOP 10 - insere na posição correta 
        // Algoritmo de shift: empurra todos os elementos para baixo, abrindo espaço
        // Começa pelo final para não sobrescrever dados
        for (int i = 9; i > posicaoInsercao; i--){
            placar[i] = placar[i - 1];
        }

        // Insere os dados do novo recordista na posição correta
        strncpy(placar[posicaoInsercao].nome, nome, 19); // Copia no máximo 19 caracteres
        placar[posicaoInsercao].nome[19] = '\0';          // Garante terminação nula segura
        placar[posicaoInsercao].time = tempoTotal;        // Armazena o tempo

        // --- SALVAMENTO NO ARQUIVO ---
        // Abre arquivo para escrita (sobrescreve dados anteriores)
        FILE *arq = fopen("placar/placar.bin", "wb");
        if (arq != NULL) {
            // Escreve as 10 estruturas atualizadas no arquivo
            fwrite(placar, sizeof(TIPO_PLACAR), 10, arq);
            fclose(arq);
        }
    }
}