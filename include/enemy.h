#ifndef ENEMY_H
#define ENEMY_H

#include <raylib.h>
#include <map.h>

typedef struct {
    float x, y;
    struct {
        int row;
        int col;
    } pos;
    int dirX;       // -1 = Esquerda, 1 = Direita
    bool active;    // Para controle se o inimigo está no jogo
} Enemy;

// Inicializa a lista de inimigos procurando os caracteres 'E' no mapa
void InitEnemies(Enemy enemies[], int *enemyCount, Map map);

// Atualiza a posição horizontal e trata a inversão de marcha nas bordas/paredes 
void UpdateEnemies(Enemy enemies[], int enemyCount, Map map);

// Desenha os inimigos na tela 
void DrawEnemies(Enemy enemies[], int enemyCount);

#endif