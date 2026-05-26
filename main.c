#include "raylib.h"

int main(void) {
    // Inicializa uma janela de 600x600 pixels com o título do jogo
    InitWindow(600, 600, "Donkey Kong INF - Teste");
    SetTargetFPS(60);

    // Loop principal do jogo (roda a 60 frames por segundo)
    while (!WindowShouldClose()) {
        // Começa a desenhar na tela
        BeginDrawing();
            ClearBackground(BLACK); // Fundo preto
            
            // Desenha um texto na tela: Texto, Posição X, Posição Y, Tamanho da Fonte, Cor
            DrawText("Raylib funcionando no Linux Mint!", 100, 280, 20, RAYWHITE);
        EndDrawing();
    }

    // Fecha a janela ao sair do loop
    CloseWindow();
    return 0;
}