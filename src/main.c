// src/main.c
#include <raylib.h>
#include <constants.h>
#include <menu.h>
#include <pause.h>
#include <player.h>
#include <map.h>
#include <enemy.h>

#define MAX_ENEMIES 100

// Função auxiliar para verificar colisão por formato retangular e acionar o Game Over
void CheckPlayerEnemyCollisions(Player *player, Enemy enemies[], int enemyCount, GameState *estadoAtual, Map *mapa, int *enemyCountRef) {
    // Caixa de colisão retangular do jogador
    Rectangle playerRec = { player->x, player->y, TILE_SIZE, TILE_SIZE };

    for (int i = 0; i < enemyCount; i++) {
        if (!enemies[i].active) continue;

        // O inimigo ('E') é tratado utilizando a lógica de colisão retangular
        Rectangle enemyRec = { enemies[i].x, enemies[i].y, TILE_SIZE, TILE_SIZE };

        // Se as caixas de colisão se sobrepuserem (mesma posição efetiva)
        if (CheckCollisionRecs(playerRec, enemyRec)) {
            // Critério do PDF: Ao morrer, o estado muda para exibir a tela apropriada (Game Over/Ranking)
            *estadoAtual = STATE_RANKING;
            
            // Reinicia o jogador e recarrega o mapa/inimigos do arquivo texto para a próxima tentativa
            InitPlayer(player); 
            LoadMap(mapa, "mapa0.txt", player); 
            InitEnemies(enemies, enemyCountRef, *mapa);
            break;
        }
    }
}

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Donkey Kong");
    SetTargetFPS(TARGET_FPS);

    GameState estadoAtual = STATE_MENU;
    int opcaoSelecionadaMenu = 0;
    int opcaoSelecionadaPausa = 0;

    Player jogador;
    Map mapa;
    
    // Vetor e contador para gerenciar a lógica dos inimigos do mapa
    Enemy enemies[MAX_ENEMIES];
    int enemyCount = 0;

    // Inicialização segura inicial
    InitPlayer(&jogador);
    LoadMap(&mapa, "mapa0.txt", &jogador);
    InitEnemies(enemies, &enemyCount, mapa);

    while (estadoAtual != STATE_EXIT && !WindowShouldClose()) {
        // --- PASSO 1: ATUALIZAÇÃO DA LÓGICA ---
        switch (estadoAtual) {
            case STATE_MENU: {
                GameState estadoAnterior = estadoAtual;
                UpdateMenu(&estadoAtual, &opcaoSelecionadaMenu);
                
                // Quando o jogo inicia de fato, recarrega o mapa do arquivo de texto e os inimigos
                if (estadoAnterior == STATE_MENU && estadoAtual == STATE_PLAYING) {
                    InitPlayer(&jogador);
                    LoadMap(&mapa, "mapa0.txt", &jogador); // Lê o arquivo e move o jogador para a posição 'P'
                    InitEnemies(enemies, &enemyCount, mapa);
                }
                break;
            }
                
            case STATE_PLAYING:
                UpdatePlayer(&jogador, mapa);
                UpdateEnemies(enemies, enemyCount, mapa); // Atualiza o comportamento de vai-e-vem dos inimigos
                CheckPlayerEnemyCollisions(&jogador, enemies, enemyCount, &estadoAtual, &mapa, &enemyCount); // Processa colisões e Game Over

                if (IsKeyPressed(KEY_TAB)) {
                    opcaoSelecionadaPausa = 0; 
                    estadoAtual = STATE_PAUSE;
                }
                break;
             
            case STATE_PAUSE:
                UpdatePauseMenu(&estadoAtual, &opcaoSelecionadaPausa);
                break;
                
            case STATE_RANKING:
                // Critério do PDF: Após a tela de fim de jogo/ranking, leva o jogador de volta ao menu
                if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_ENTER)) {
                    estadoAtual = STATE_MENU;
                }
                break;
            default:
                break;
        }

        // --- PASSO 2: RENDERIZAÇÃO GRÁFICA ---
        BeginDrawing();
        ClearBackground(BLACK); // Fundo preto do espaço sideral/fase do Donkey Kong

        switch (estadoAtual) {
            case STATE_MENU:
                DrawMenu(opcaoSelecionadaMenu);
                break;
                
            case STATE_PLAYING:
                DrawMap(mapa);       // Desenha todo o cenário estático mapeado do arquivo texto
                DrawPlayer(jogador); // Desenha o quadrado dinâmico azul por cima do cenário
                DrawEnemies(enemies, enemyCount); // Desenha os inimigos retangulares na tela
                break;
                
            case STATE_PAUSE:
                DrawMap(mapa);      
                DrawPlayer(jogador); 
                DrawEnemies(enemies, enemyCount); // Mantém os inimigos visíveis sob a máscara de pausa
                DrawPauseMenu(opcaoSelecionadaPausa); // Aplica a máscara escura de pausa sobreposta
                break;
                
            case STATE_RANKING:
                // Tela de fim de jogo integrada ao sistema de ranking conforme o fluxo exigido
                DrawText("GAME OVER", SCREEN_WIDTH / 2 - MeasureText("GAME OVER", 40) / 2, 150, 40, RED);
                DrawText("RANKING - TOP 10", SCREEN_WIDTH / 2 - MeasureText("RANKING - TOP 10", 30) / 2, 240, 30, GOLD);
                DrawText("Pressione ENTER para voltar ao Menu", SCREEN_WIDTH / 2 - MeasureText("Pressione ENTER para voltar ao Menu", 18) / 2, 500, 18, GRAY);
                break;
            default:
                break;
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}