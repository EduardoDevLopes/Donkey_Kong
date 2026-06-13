#ifndef MAP_H
#define MAP_H
#include <constants.h>
#include <player.h>

// ESTRUTURA DE DADOS DO MAPA
typedef struct Map {
    char tiles[MAP_ROWS][MAP_COLS];// Matriz 30x30, representa o mapa
} Map;

// FUNÇÕES DO MÓDULO MAP
// Carrega um mapa de um arquivo texto e inicializa o jogador
void LoadMap(Map *map, const char *filename, Player *player); 

// Renderiza o mapa na tela, desenhando cada tile conforme seu tipo
void DrawMap(Map map);

#endif