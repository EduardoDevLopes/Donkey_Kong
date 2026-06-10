#ifndef PLAYER_H
#define PLAYER_H

#include <constants.h>

typedef struct Posi {
    int row;
    int col;
} Posi;

typedef struct Player {
    float x;
    float y;
    Posi pos;
} Player;

// Forward declaration: avisa o compilador que a struct Map existe,
// evitando dependência cíclica entre player.h e map.h
typedef struct Map Map;

void InitPlayer(Player *player);
void UpdatePlayer(Player *player, Map map); // Agora recebe o mapa para testar as colisões
void DrawPlayer(Player player);

#endif