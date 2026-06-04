#include <raylib.h>
#include <common.h>
#include <menu.h>
#include <constants.h>
#include <pause.h>

int main(void) {
    // Inicialização da janela padrão fixa (600 x 600)
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Donkey Kong INF");
    SetTargetFPS(TARGET_FPS);

    GameState estadoAtual = STATE_MENU;
    int opcaoSelecionadaMenu = 0;
    int opcaoSelecionadaPausa = 0;

    // Loop principal do jogo
    while (estadoAtual != STATE_EXIT && !WindowShouldClose()) {
        
        // --- PASSO 1: Atualizar a lógica dependendo do Estado ---
        switch (estadoAtual) {

            case STATE_MENU:
                UpdateMenu(&estadoAtual, &opcaoSelecionadaMenu);
                break;

            case STATE_PLAYING:
                if (IsKeyPressed(KEY_TAB)) {
                    opcaoSelecionadaPausa = 0; //Sempre define a pausa na primeira opção
                    estadoAtual = STATE_PAUSE; 
                }
                break;

            case STATE_PAUSE:
                UpdatePauseMenu(&estadoAtual, &opcaoSelecionadaPausa);
                break;

            case STATE_RANKING:
                if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_ENTER)) 
                    estadoAtual = STATE_MENU;
                break;

            default:
                break;
        }

        // Renderização Gráfica
        BeginDrawing();
        ClearBackground(BLACK);

        switch (estadoAtual) {
            case STATE_MENU:
                DrawMenu(opcaoSelecionadaMenu);
                break;
                
            case STATE_PLAYING:
                // Representação visual da área útil do jogo rodando ao fundo
                ClearBackground(BLUE);
                DrawText("JOGO EM EXECUCAO", SCREEN_WIDTH / 2 - MeasureText("JOGO EM EXECUCAO", 24) / 2, 250, 24, WHITE);
                DrawText("Pressione TAB para pausar", SCREEN_WIDTH / 2 - MeasureText("Pressione TAB para pausar", 18) / 2, 300, 18, LIGHTGRAY);
                break;
                    
            case STATE_PAUSE:
                // Mantém o cenário do jogo desenhado embaixo e plota o menu de pausa por cima
                ClearBackground(BLUE); 
                DrawText("JOGO EM EXECUCAO", SCREEN_WIDTH / 2 - MeasureText("JOGO EM EXECUCAO", 24) / 2, 250, 24, WHITE);
                    
                // Desenha a máscara e opções da pausa por cima
                DrawPauseMenu(opcaoSelecionadaPausa);
                break;
                    
            case STATE_RANKING:
                DrawText("RANKING - TOP 10", SCREEN_WIDTH / 2 - MeasureText("RANKING - TOP 10", 30) / 2, 100, 30, GOLD);
                DrawText("Pressione ENTER para voltar", SCREEN_WIDTH / 2 - MeasureText("Pressione ENTER para voltar", 18) / 2, 500, 18, GRAY);
                break;
            default:
                break;
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}