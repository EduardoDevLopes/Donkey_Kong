// include/map.h
#ifndef MAP_H
#define MAP_H

#include "constants.h"
#include "player.h"

// Estrutura que encapsula a matriz de caracteres do cenário
typedef struct Map {
    char tiles[MAP_ROWS][MAP_COLS];
} Map;

// Protótipos das funções modulares
void LoadMap(Map *map, const char *filename, Player *player);
void DrawMap(Map map);

#endif