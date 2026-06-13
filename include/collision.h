#ifndef COLLISION_H
#define COLLISION_H

#include <raylib.h>
#include <player.h>
#include <enemy.h>
#include <constants.h>

/* VERIFICAÇÃO DE COLISÕES
Verifica colisão entre jogador e inimigos
Se houver colisão, retorna true (jogador morreu)*/
bool CheckPlayerEnemyCollision(Player *player, Enemy enemies[], int enemyCount);

#endif