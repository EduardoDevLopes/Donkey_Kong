#include <raylib.h>
#include <pause.h>

void DrawPauseMenu(int selectedOption) {
    // Escurece a tela de jogo que ficou ao fundo (efeito semi-transparente)
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(BLACK, 0.75f));

    // Título centralizado
    DrawText("JOGO PAUSADO", SCREEN_WIDTH / 2 - MeasureText("JOGO PAUSADO", 32) / 2, 180, 32, YELLOW);

    // Configuração de cores com base na seleção por condicionais
    Color corOpcao1 = (selectedOption == 0) ? YELLOW : WHITE;
    Color corOpcao2 = (selectedOption == 1) ? YELLOW : WHITE;
    Color corOpcao3 = (selectedOption == 2) ? YELLOW : WHITE;

    // Renderização das 3 opções solicitadas
    DrawText("1. CONTINUAR", SCREEN_WIDTH / 2 - MeasureText("1. CONTINUAR", 20) / 2, 280, 20, corOpcao1);
    DrawText("2. SAIR PARA O MENU", SCREEN_WIDTH / 2 - MeasureText("2. SAIR PARA O MENU", 20) / 2, 330, 20, corOpcao2);
    DrawText("3. SAIR DO JOGO", SCREEN_WIDTH / 2 - MeasureText("3. SAIR DO JOGO", 20) / 2, 380, 20, corOpcao3);
    
    DrawText("Use W/S ou Setas para navegar e ENTER para confirmar", SCREEN_WIDTH / 2 - MeasureText("Use W/S ou Setas para navegar e ENTER para confirmar", 14) / 2, 480, 14, GRAY);
}

void UpdatePauseMenu(GameState *currentState, int *selectedOption) {
    // Navegação vertical pelas opções
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
        *selectedOption = *selectedOption + 1;
        if (*selectedOption > 2) {
            *selectedOption = 0;
        }
    }

    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
        *selectedOption = *selectedOption - 1;
        if (*selectedOption < 0) {
            *selectedOption = 2;
        }
    }

    // Execução da opção escolhida ao pressionar ENTER
    if (IsKeyPressed(KEY_ENTER)) {
        switch (*selectedOption) {
            case 0: 
                *currentState = STATE_PLAYING; 
                break;
            case 1: 
                *currentState = STATE_MENU; 
                break;
            case 2: 
                *currentState = STATE_EXIT; 
                break;
        }
    }

    // Atalho clássico de UX: se apertar TAB novamente, o jogo despausa e continua
    if (IsKeyPressed(KEY_TAB)) {
        *currentState = STATE_PLAYING;
    }
}