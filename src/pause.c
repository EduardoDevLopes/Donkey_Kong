#include <raylib.h>
#include <pause.h>

// RENDERIZAÇÃO DO MENU DE PAUSA

void DrawPauseMenu(int selectedOption) {
    // EFEITO DE ESCURECIMENTO
    // Desenha um retângulo semi-transparente sobre a tela de jogo
    // Isso cria efeito visual de pausa, deixando claro que o jogo está pausado
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(BLACK, 0.75f));

    // TÍTULO DO MENU 
    // Desenha "JOGO PAUSADO" em amarelo e grande, centralizado horizontalmente
    DrawText("JOGO PAUSADO", SCREEN_WIDTH / 2 - MeasureText("JOGO PAUSADO", 32) / 2, 180, 32, YELLOW);

    //CORES DAS OPÇÕES
    // Opção selecionada fica amarela, outras ficam brancas
    Color corOpcao1 = (selectedOption == 0) ? YELLOW : WHITE;
    Color corOpcao2 = (selectedOption == 1) ? YELLOW : WHITE;
    Color corOpcao3 = (selectedOption == 2) ? YELLOW : WHITE;

    // OPÇÕES DO MENU
    // Desenha as 3 opções de pausa especificadas no trabalho
    DrawText("1. CONTINUAR", SCREEN_WIDTH / 2 - MeasureText("1. CONTINUAR", 20) / 2, 280, 20, corOpcao1);
    DrawText("2. SAIR PARA O MENU", SCREEN_WIDTH / 2 - MeasureText("2. SAIR PARA O MENU", 20) / 2, 330, 20, corOpcao2);
    DrawText("3. SAIR DO JOGO", SCREEN_WIDTH / 2 - MeasureText("3. SAIR DO JOGO", 20) / 2, 380, 20, corOpcao3);
    
    // INSTRUÇÕES
    // Texto pequeno explicando controles
    DrawText("Use W/S ou Setas para navegar e ENTER para confirmar", 
             SCREEN_WIDTH / 2 - MeasureText("Use W/S ou Setas para navegar e ENTER para confirmar", 14) / 2, 
             480, 14, GRAY);
}

// ATUALIZAÇÃO DO MENU DE PAUSA
void UpdatePauseMenu(GameState *currentState, int *selectedOption) {
    // NAVEGAÇÃO PARA BAIXO
    // Se pressionou seta para baixo ou 'S', passa para próxima opção
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
        *selectedOption = *selectedOption + 1;
        if (*selectedOption > 2) {
            *selectedOption = 0; // Volta para primeira opção quando chega no final
        }
    }

    // NAVEGAÇÃO PARA CIMA
    // Se pressionou seta para cima ou 'W', volta para opção anterior
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
        *selectedOption = *selectedOption - 1;
        if (*selectedOption < 0) {
            *selectedOption = 2; // Volta para última opção quando chega no início
        }
    }

    // EXECUÇÃO DA OPÇÃO
    // Se pressionou ENTER, executa a opção selecionada
    if (IsKeyPressed(KEY_ENTER)) {
        switch (*selectedOption) {
            case 0: 
                // Continuar: volta ao jogo do ponto onde pausou
                *currentState = STATE_PLAYING; 
                break;
            case 1: 
                // Voltar ao menu: retorna à tela inicial
                *currentState = STATE_MENU; 
                break;
            case 2: 
                // Sair do jogo: encerra a aplicação
                *currentState = STATE_EXIT; 
                break;
        }
    }

    // ATALHO DE PAUSA
    // Se pressionou TAB novamente, despausa automaticamente
    if (IsKeyPressed(KEY_TAB)) {
        *currentState = STATE_PLAYING;
    }
}