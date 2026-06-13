#ifndef PHASE_H
#define PHASE_H
#include <player.h>
#include <map.h>
#include <enemy.h>
#include <constants.h>

// GERENCIAMENTO DE FASES
// Avança para a próxima fase
// Retorna true se conseguiu carregar próxima fase, false se chegou ao final do jogo
bool AdvancePhase(int *currentPhase, Player *player, Map *map, Enemy enemies[], int *enemyCount);

#endif