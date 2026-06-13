#ifndef PAUSE_H
#define PAUSE_H
#include <constants.h>

// PROTÓTIPOS DAS FUNÇÕES DO MENU DE PAUSA

// Renderiza o menu de pausa na tela
void DrawPauseMenu(int selectedOption);

// Processa entrada do usuário no menu de pausa
void UpdatePauseMenu(GameState *currentState, int *selectedOption);

#endif