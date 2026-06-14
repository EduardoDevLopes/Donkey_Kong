#ifndef CONSTANTS_H
#define CONSTANTS_H

// Estados principais do jogo para navegação entre telas
typedef enum GameState {
    STATE_MENU,      // Menu inicial com opções
    STATE_PLAYING,   // Jogo em andamento
    STATE_PAUSE,     // Menu de pausa
    STATE_RANKING,   // Tela de ranking
    STATE_EXIT       // Encerrar aplicação
} GameState;

// DIMENSÕES E CONFIGURAÇÕES DE TELA

#define SCREEN_WIDTH 600   // Largura da janela em pixels
#define SCREEN_HEIGHT 600  // Altura da janela em pixels
#define TILE_SIZE 20       // Tamanho de cada quadrado da grid (afeta escala visual e colisões)
#define TARGET_FPS 60      // Alvo de frames por segundo para fluidez do jogo

// CONFIGURAÇÕES DO MAPA

// Dimensões fixas da matriz de tiles
#define MAP_ROWS 30
#define MAP_COLS 30

// VELOCIDADES DOS PERSONAGENS

// Velocidade do jogador em pixels por segundo 
#define PLAYER_SPEED 150.0f  

// Velocidade dos inimigos em pixels por segundo
#define ENEMY_SPEED 170.0f 

// LIMITES DE DADOS

// Comprimento máximo do nome do jogador no ranking (deve ser menor que tamanho da struct)
#define MAX_PLAYER_NAME 20

// Número máximo de inimigos que podem estar em um mapa simultaneamente
#define MAX_ENEMIES 50

// Número de posições no ranking 
#define RANKING_SIZE 10

#endif