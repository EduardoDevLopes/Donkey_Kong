#include <raylib.h>
#include <common.h>
#include <menu.h>
#include <constants.h>
#include <pause.h>
#include <player.h>

int main(void) {
    // Inicialização da janela padrão fixa (600 x 600)
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Donkey Kong");
    SetTargetFPS(TARGET_FPS);

    GameState estadoAtual = STATE_MENU;
    int opcaoSelecionadaMenu = 0;
    int opcaoSelecionadaPausa = 0;

    //Declaração da instância do jogador estruturado
    Player jogador;
    InitPlayer(&jogador);


    // Loop principal do jogo
    while (estadoAtual != STATE_EXIT && !WindowShouldClose()) {
        
        // Atualiza a lógica dependendo do Estado
        switch (estadoAtual) {

            case STATE_MENU: {
                GameState estadoAnterior = estadoAtual;
                UpdateMenu(&estadoAtual, &opcaoSelecionadaMenu);
               
                //Se o usuário entrou no jogo pelo menu inicializa o personagem
                if (estadoAnterior == STATE_MENU && estadoAtual == STATE_PLAYING){
                    InitPlayer(&jogador);
                }    
            }
                
                break;

            case STATE_PLAYING:
                // Atualiza a lógica de movimentação e teclas do jogador
                UpdatePlayer(&jogador);

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
                // Simulação do fundo de jogo limpo de 600x600 pixels
                DrawText("Pressione TAB para pausar", 20, 20, 16, LIGHTGRAY);
                
                // Desenha o bloco do jogador na tela nas coordenadas calculadas
                DrawPlayer(jogador);
                break;
                    
            case STATE_PAUSE:
                // Mantém o jogador visível ao fundo de forma estática enquanto pausado
                DrawPlayer(jogador);
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