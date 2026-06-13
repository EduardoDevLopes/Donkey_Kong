#ifndef PLAYER_H
#define PLAYER_H
#include <constants.h>


// ESTRUTURAS DE DADOS DO JOGADOR
// Representa uma posição na grid de 30x30
// Usado para lógica de colisão e verificação de tiles
typedef struct Posi {
    int row;  // Linha na matriz (0-29)
    int col;  // Coluna na matriz (0-29)
} Posi;

// Representa o jogador do jogo
// Armazena tanto posição em pixels (para renderização suave) quanto em grid (para colisões)
typedef struct Player {
    float x;    // Posição X em pixels (muda continuamente durante movimento)
    float y;    // Posição Y em pixels (muda continuamente durante movimento)
    Posi pos;   // Posição na grid 30x30 (sincronizada com x e y)
} Player;

// Forward declaration: avisa o compilador que a struct Map existe,
// evitando dependência cíclica entre player.h e map.h
typedef struct Map Map;

// FUNÇÕES DO MÓDULO PLAYER
// Inicializa o jogador com valores padrão na posição (0, 0)
void InitPlayer(Player *player);

// Atualiza a posição do jogador baseado em entrada de teclado e colisões com mapa
// Parâmetros: player = jogador a atualizar, map = mapa para testar colisões
void UpdatePlayer(Player *player, Map map);

// Desenha o jogador na tela usando Raylib
void DrawPlayer(Player player);

#endif