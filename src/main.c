// src/main.c
#include <raylib.h>
#include <common.h>
#include <menu.h>
#include <constants.h>

int main(void) {
    // 1. Configuração para Tela Cheia usando a resolução nativa do sistema
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Donkey Kong INF"); // Passar 0, 0 faz a Raylib abrir na resolução do sistema
    SetTargetFPS(TARGET_FPS);            // Trava o FPS em 60

    GameState estadoAtual = STATE_MENU;
    int opcaoSelecionadaMenu = 0;

    // Loop principal do jogo
    while (estadoAtual != STATE_EXIT && !WindowShouldClose()) {
        
        // --- PASSO 1: Atualizar a lógica dependendo do Estado ---
        switch (estadoAtual) {
            case STATE_MENU:
                UpdateMenu(&estadoAtual, &opcaoSelecionadaMenu);
                break;
            case STATE_PLAYING:
                if (IsKeyPressed(KEY_TAB)) estadoAtual = STATE_PAUSE; 
                break;
            case STATE_PAUSE:
                if (IsKeyPressed(KEY_TAB)) estadoAtual = STATE_PLAYING;
                break;
            case STATE_RANKING:
                if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_ENTER)) estadoAtual = STATE_MENU;
                break;
            default:
                break;
        }

        // --- PASSO 2: Desenhar os elementos na tela ---
        BeginDrawing();
        
        // Armar os pontos centrais para telas de estado genéricas
        int centroX = GetScreenWidth() / 2;
        int centroY = GetScreenHeight() / 2;

        switch (estadoAtual) {
            case STATE_MENU:
                DrawMenu(opcaoSelecionadaMenu);
                break;
            case STATE_PLAYING:
                ClearBackground(BLUE);
                DrawText("JOGO EM EXECUCAO", centroX - MeasureText("JOGO EM EXECUCAO", 24) / 2, centroY - 30, 24, WHITE);
                DrawText("Pressione TAB para pausar", centroX - MeasureText("Pressione TAB para pausar", 18) / 2, centroY + 20, 18, LIGHTGRAY);
                break;
            case STATE_PAUSE:
                ClearBackground(DARKGRAY);
                DrawText("JOGO PAUSADO", centroX - MeasureText("JOGO PAUSADO", 24) / 2, centroY, 24, YELLOW);
                break;
            case STATE_RANKING:
                ClearBackground(BLACK);
                DrawText("RANKING - TOP 10", centroX - MeasureText("RANKING - TOP 10", 30) / 2, centroY - 100, 30, GOLD);
                DrawText("Pressione ENTER para voltar", centroX - MeasureText("Pressione ENTER para voltar", 18) / 2, centroY + 150, 18, GRAY);
                break;
            default:
                break;
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}