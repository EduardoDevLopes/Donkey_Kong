#include <raylib.h>
#include <menu.h>
#include <constants.h>

// RENDERIZAÇÃO DO MENU
void DrawMenu(int selectedOption) {
    // LIMPEZA E FUNDO
    // Define fundo preto para o menu
    ClearBackground(BLACK);

    // CENTRALIZAÇÃO HORIZONTAL
    // Calcula o centro da tela para desenhar elementos alinhados
    int centroX = GetScreenWidth() / 2;
    int centroY = GetScreenHeight() / 2;

    // TÍTULO DO JOGO
    DrawText("DONKEY KONG INF", centroX - MeasureText("DONKEY KONG INF", 40) / 2, centroY - 150, 40, RED);

    // CORES DAS OPÇÕES
    // Opção selecionada fica amarela, outras ficam brancas
    Color corOpcao1 = (selectedOption == 0) ? YELLOW : WHITE;
    Color corOpcao2 = (selectedOption == 1) ? YELLOW : WHITE;
    Color corOpcao3 = (selectedOption == 2) ? YELLOW : WHITE;

    // DESENHO  OPÇÕES DO MENU
    DrawText("1. NOVO JOGO", centroX - MeasureText("1. NOVO JOGO", 24) / 2, centroY - 20, 24, corOpcao1);
    DrawText("2. RANKING",   centroX - MeasureText("2. RANKING", 24) / 2,   centroY + 30, 24, corOpcao2);
    DrawText("3. SAIR",      centroX - MeasureText("3. SAIR", 24) / 2,      centroY + 80, 24, corOpcao3);
    
    // INSTRUÇÕES 
    DrawText("Use W/S ou Setas para navegar e ENTER para selecionar", 
             centroX - MeasureText("Use W/S ou Setas para navegar e ENTER para selecionar", 16) / 2, 
             centroY + 200, 16, GRAY);
}

// ATUALIZAÇÃO DO MENU

void UpdateMenu(GameState *currentState, int *selectedOption) {
    // NAVEGAÇÃO PARA BAIXO 
    // Se pressionou seta para baixo ou 'S', passa para próxima opção
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
        *selectedOption = *selectedOption + 1;
        if (*selectedOption > 2) *selectedOption = 0; // Volta para primeira opção quando chega no final
    }

    // NAVEGAÇÃO PARA CIMA
    // Se pressionou seta para cima ou 'W', volta para opção anterior
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
        *selectedOption = *selectedOption - 1;
        if (*selectedOption < 0) *selectedOption = 2; // Volta para última opção quando chega no início
    }

    // EXECUÇÃO DA OPÇÃO
    // Se pressionou ENTER, executa a opção selecionada
    if (IsKeyPressed(KEY_ENTER)) {
        switch (*selectedOption) {
            case 0: 
                *currentState = STATE_PLAYING;  // Inicia novo jogo
                break;
            case 1: 
                *currentState = STATE_RANKING;  // Vai para tela de ranking
                break;
            case 2: 
                *currentState = STATE_EXIT;     // Encerra o programa
                break;
        }
    }
}