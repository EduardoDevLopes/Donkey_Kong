#ifndef PLAYER_H
#define PLAYER_H

#include <constants.h>

// Estrutura de posição aninhada, conforme sugerido nas dicas do enunciado
typedef struct Position {
    int row;    // Linha na matriz (0 a 29)
    int col;    // Coluna na matriz (0 a 29)
} Position;

typedef enum PlayerState {
    PLAYER_ALIVE,
    PLAYER_DEAD
} PlayerState;

// Estrutura principal do jogador com os dados obrigatórios do enunciado
typedef struct Player {
    Position pos;       // Localização (linha, coluna)
    float x;
    float y;
    PlayerState state;  // Estado (ativo ou morto)
    int score;          // Pontuação (baseada no menor de execução)
} Player;

// Protótipos das funções de subprogramação modular
void InitPlayer(Player *player);
void UpdatePlayer(Player *player);
void DrawPlayer(Player player);

#endif