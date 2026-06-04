#include <raylib.h>
#include <menu.h>
#include <constants.h>

void DrawMenu(int selectedOption) {
    ClearBackground(BLACK);

    // Descobre o centro da tela cheia atual para desenhar o menu alinhado
    int centroX = GetScreenWidth() / 2;
    int centroY = GetScreenHeight() / 2;

    // Desenha os textos baseados no centro da tela
    DrawText("DONKEY KONG INF", centroX - MeasureText("DONKEY KONG INF", 40) / 2, centroY - 150, 40, RED);

    // Opções obrigatórias do enunciado centralizadas
    Color corOpcao1 = (selectedOption == 0) ? YELLOW : WHITE;
    Color corOpcao2 = (selectedOption == 1) ? YELLOW : WHITE;
    Color corOpcao3 = (selectedOption == 2) ? YELLOW : WHITE;

    DrawText("1. NOVO JOGO", centroX - MeasureText("1. NOVO JOGO", 24) / 2, centroY - 20, 24, corOpcao1);
    DrawText("2. RANKING",   centroX - MeasureText("2. RANKING", 24) / 2,   centroY + 30, 24, corOpcao2);
    DrawText("3. SAIR",      centroX - MeasureText("3. SAIR", 24) / 2,      centroY + 80, 24, corOpcao3);
    
    DrawText("Use W/S ou Setas para navegar e ENTER para selecionar", centroX - MeasureText("Use W/S ou Setas para navegar e ENTER para selecionar", 16) / 2, centroY + 200, 16, GRAY);
}

void UpdateMenu(GameState *currentState, int *selectedOption) {
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
        *selectedOption = *selectedOption + 1;
        if (*selectedOption > 2) *selectedOption = 0;
    }

    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
        *selectedOption = *selectedOption - 1;
        if (*selectedOption < 0) *selectedOption = 2;
    }

    if (IsKeyPressed(KEY_ENTER)) {
        switch (*selectedOption) {
            case 0: *currentState = STATE_PLAYING; break;
            case 1: *currentState = STATE_RANKING; break;
            case 2: *currentState = STATE_EXIT;    break;
        }
    }
}