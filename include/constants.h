#ifndef CONSTANTS_H
#define CONSTANTS_H

// Definição dos estados da Máquina de Estados do jogo
typedef enum GameState {
    STATE_MENU,
    STATE_PLAYING,
    STATE_RANKING,
    STATE_PAUSE,
    STATE_GAME_OVER,
    STATE_EXIT
} GameState;

// Dimensões baseadas no mapa de 30x30 tiles
#define TILE_SIZE 20
#define SCREEN_WIDTH 600   // 30 * 20
#define SCREEN_HEIGHT 600  // 30 * 20
#define TARGET_FPS 60
#define PLAYER_SPEED 150.0f //Velocidade em pixels por segundo

#endif