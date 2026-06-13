#ifndef RANKING_H
#define RANKING_H

// ESTRUTURA DE DADOS DO RANKING
// Armazena um nome e seu tempo para o ranking
typedef struct tipo_placar {
    char nome[20];  // Nome do jogador (máximo 19 caracteres + terminador nulo)
    int time;       // Tempo em milissegundos (convertido para display em apresentação)
} TIPO_PLACAR;

// FUNÇÕES DO MÓDULO RANKING
// Carrega o ranking do arquivo placar.bin
// Se arquivo não existir, inicializa com posições vazias
void CarregarPlacar(TIPO_PLACAR placar[]);

// Verifica se o tempo do jogador atual está no TOP 10
// Se sim, insere na posição correta e salva no arquivo
void VerificarESalvarPlacar(const char *nome, int tempoTotal);

#endif