//include/pause.h
#ifndef PAUSE_H
#define PAUSE_H
#include <constants.h>

//Protótipos das funções do menu de pausa
void DrawPauseMenu(int selectedOption);
void UpdatePauseMenu(GameState *currentState, int *selectedOption);

#endif