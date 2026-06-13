#ifndef MENU_H
#define MENU_H
#include <constants.h>

// PROTÓTIPOS DAS FUNÇÕES DO MENU
// Renderiza o menu inicial na tela
void DrawMenu(int selectedOption);

// Processa entrada do usuário no menu e atualiza estado
void UpdateMenu(GameState *currentState, int *selectedOption);

#endif