#ifndef MENU_H
#define MENU_H

#include <constants.h>

// Protótipos das funções do menu
void DrawMenu(int selectedOption);
void UpdateMenu(GameState *currentState, int *selectedOption);

#endif