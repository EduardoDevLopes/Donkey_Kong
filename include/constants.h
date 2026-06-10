// include/constants.h
#ifndef CONSTANTS_H
#define CONSTANTS_H

typedef enum GameState {
    STATE_MENU,
    STATE_PLAYING,
    STATE_PAUSE,
    STATE_RANKING,
    STATE_EXIT
} GameState;

#define SCREEN_WIDTH 600   
#define SCREEN_HEIGHT 600  
#define TILE_SIZE 20
#define TARGET_FPS 60

// Definições da matriz do mapa
#define MAP_ROWS 30
#define MAP_COLS 30

#define PLAYER_SPEED 150.0f  

#endif