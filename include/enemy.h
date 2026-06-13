#ifndef ENEMY_H
#define ENEMY_H
#include <raylib.h>
#include <map.h>


// ESTRUTURA DE DADOS DO INIMIGO

// Representa um inimigo no jogo
// Inimigos se movem horizontalmente sobre plataformas
typedef struct {
    float x, y;         // Posição em pixels na tela, mudam continuamente
    struct {
        int row;        // Linha na grid (0-29)
        int col;        // Coluna na grid (0-29)
    } pos;
    int dirX;           // Direção do movimento: -1 = esquerda, 1 = direita
    bool active;        // Controla se o inimigo está presente no jogo
} Enemy;

// FUNÇÕES DO MÓDULO ENEMY

// Inicializa inimigos procurando os caracteres 'E' no mapa
// Cada 'E' encontrado se torna um inimigo que inicia movendo para a direita
void InitEnemies(Enemy enemies[], int *enemyCount, Map map);

// Atualiza posição dos inimigos a cada frame
// Inimigos se movem horizontalmente e invertem direção ao atingir paredes ou bordas
void UpdateEnemies(Enemy enemies[], int enemyCount, Map map);

// Desenha todos os inimigos na tela como triângulos vermelhos
void DrawEnemies(Enemy enemies[], int enemyCount);

#endif