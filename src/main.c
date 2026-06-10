// src/main.c
#include <stdio.h>
#include <raylib.h>
#include <constants.h>
#include <menu.h>
#include <pause.h>
#include <player.h>
#include <map.h>
#include <enemy.h>
#include <ranking.h>

#define MAX_ENEMIES 100

// CORREÇÃO: Adicionado o ponteiro 'exibindoGameOver' para que a colisão saiba ativar a tela correta
void CheckPlayerEnemyCollisions(Player *player, Enemy enemies[], int enemyCount, GameState *estadoAtual, bool *exibindoGameOver) {
    Rectangle playerRec = { player->x, player->y, TILE_SIZE, TILE_SIZE };

    for (int i = 0; i < enemyCount; i++) {
        if (!enemies[i].active) continue;

        Rectangle enemyRec = { enemies[i].x, enemies[i].y, TILE_SIZE, TILE_SIZE };

        if (CheckCollisionRecs(playerRec, enemyRec)) {
            // Modifica o estado do jogo e ativa explicitamente a flag visual de Game Over
            *estadoAtual = STATE_RANKING;
            *exibindoGameOver = true; 
            break;
        }
    }
}

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Donkey Kong INF");
    SetTargetFPS(TARGET_FPS);

    GameState estadoAtual = STATE_MENU;
    int opcaoSelecionadaMenu = 0;
    int opcaoSelecionadaPausa = 0;

    Player jogador;
    Map mapa;
    Enemy enemies[MAX_ENEMIES];
    int enemyCount = 0;

    // Variáveis de progresso da sessão ativa
    int faseAtual = 0;
    float tempoTotal = 0.0f; // Mantido em float para precisão contínua da Raylib

    // Flags de controle visual para as telas de fim de jogo
    char nomeJogador[20] = "\0";
    int letrasCount = 0;
    bool gravandoRecorde = false;
    bool exibindoGameOver = false;

    TIPO_PLACAR ranking[10]; 

    // Inicialização padrão do sistema
    InitPlayer(&jogador);
    LoadMap(&mapa, "mapas/mapa2.txt", &jogador);
    InitEnemies(enemies, &enemyCount, mapa);

    while (estadoAtual != STATE_EXIT && !WindowShouldClose()) {
        
        switch (estadoAtual) {
            case STATE_MENU: {
                UpdateMenu(&estadoAtual, &opcaoSelecionadaMenu);
                
                // Quando entra no jogo vindo do Menu, força o reset absoluto de todas as marcas
                if (estadoAtual == STATE_PLAYING) {
                    faseAtual = 0;
                    tempoTotal = 0.0f;
                    nomeJogador[0] = '\0';
                    letrasCount = 0;
                    gravandoRecorde = false;
                    exibindoGameOver = false;

                    InitPlayer(&jogador);
                    LoadMap(&mapa, "mapas/mapa0.txt", &jogador); 
                    InitEnemies(enemies, &enemyCount, mapa);
                }
                break;
            }
                
            case STATE_PLAYING:
                // Atualiza o tempo acumulando os segundos e frações de cada frame
                tempoTotal += GetFrameTime();

                UpdatePlayer(&jogador, mapa);
                UpdateEnemies(enemies, enemyCount, mapa); 
                
                // CORREÇÃO: Passando a flag de controle para a função de colisões
                CheckPlayerEnemyCollisions(&jogador, enemies, enemyCount, &estadoAtual, &exibindoGameOver);

                // Condição de vitória da fase (Colisão com a Porta 'F')
                if (estadoAtual == STATE_PLAYING && mapa.tiles[jogador.pos.row][jogador.pos.col] == 'F') {
                    faseAtual++;
                    char proximoMapa[30];
                    
                    // CORREÇÃO: Como o 'faseAtual++' já aconteceu acima, o próximo mapa é exatamente o valor de 'faseAtual'
                    sprintf(proximoMapa, "mapas/mapa%d.txt", faseAtual); 

                    // Procura de forma autônoma modificações ou novas adições de mapas
                    if (FileExists(proximoMapa)) {
                        InitPlayer(&jogador);
                        LoadMap(&mapa, proximoMapa, &jogador);
                        InitEnemies(enemies, &enemyCount, mapa);
                    } else {
                        // Se não encontrar o próximo arquivo, vitória absoluta alcançada!
                        estadoAtual = STATE_RANKING;
                        gravandoRecorde = true;
                        exibindoGameOver = false;
                    }
                }
                if (IsKeyPressed(KEY_TAB)) {
                    opcaoSelecionadaPausa = 0; 
                    estadoAtual = STATE_PAUSE;
                }
                break;
             
            case STATE_PAUSE:
                UpdatePauseMenu(&estadoAtual, &opcaoSelecionadaPausa);
                break;
                
            case STATE_RANKING:
                 if (exibindoGameOver) {
                    // Tela de Game Over estática: aguarda confirmação para ir à tabela
                    if (IsKeyPressed(KEY_ENTER)) {
                        exibindoGameOver = false; // Desliga a tela de Game Over e expõe o ranking
                    }
                }
                else if (gravandoRecorde) {
                    int tecla = GetCharPressed();
                    while (tecla > 0) {
                        if ((tecla >= 32) && (tecla <= 125) && (letrasCount < 19)) {
                            nomeJogador[letrasCount] = (char)tecla;
                            nomeJogador[letrasCount + 1] = '\0';
                            letrasCount++;
                        }
                        tecla = GetCharPressed();
                    }

                    if (IsKeyPressed(KEY_BACKSPACE)) {
                        letrasCount--;
                        if (letrasCount < 0) letrasCount = 0;
                        nomeJogador[letrasCount] = '\0';
                    }

                    if (IsKeyPressed(KEY_ENTER) && letrasCount > 0) {
                        // CORREÇÃO: Envia o tempo convertido para milissegundos inteiros (Ex: 12.345s vira 12345)
                        VerificarESalvarPlacar(nomeJogador, (int)(tempoTotal * 1000));
                        gravandoRecorde = false;
                    }
                } else {
                    CarregarPlacar(ranking);

                    if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_ENTER)) {
                        estadoAtual = STATE_MENU;
                    }
                }
                break;
            default:
                break;
        }

        // --- PASSO 2: RENDERIZAÇÃO GRÁFICA ---
        BeginDrawing();
        ClearBackground(BLACK); 

        switch (estadoAtual) {
            case STATE_MENU:
                DrawMenu(opcaoSelecionadaMenu);
                break;
                
            case STATE_PLAYING:
                DrawMap(mapa);       
                DrawPlayer(jogador); 
                DrawEnemies(enemies, enemyCount); 
                
                // CORREÇÃO VISUAL: Tempo formatado exibindo Segundos . Milésimos
                {
                    int seg = (int)tempoTotal;
                    int mil = (int)((tempoTotal - seg) * 1000);
                    DrawText(TextFormat("TEMPO: %02d.%03ds", seg, mil), SCREEN_WIDTH - 190, 15, 20, WHITE);
                }
                DrawText(TextFormat("FASE: %d", faseAtual), 20, 15, 20, GREEN);
                break;
                
            case STATE_PAUSE:
                DrawMap(mapa);      
                DrawPlayer(jogador); 
                DrawEnemies(enemies, enemyCount); 
                DrawPauseMenu(opcaoSelecionadaPausa); 
                break;
                
            case STATE_RANKING:
                if (exibindoGameOver) {
                    // CORREÇÃO: Exibição correta da tela com dados de segundos e milésimos
                    int seg = (int)tempoTotal;
                    int mil = (int)((tempoTotal - seg) * 1000);

                    DrawText("GAME OVER!", SCREEN_WIDTH / 2 - MeasureText("GAME OVER!", 36) / 2, 140, 36, RED);
                    DrawText("Você foi capturado por um inimigo!", SCREEN_WIDTH / 2 - MeasureText("Você foi capturado por um inimigo!", 20) / 2, 210, 20, WHITE);
                    DrawText(TextFormat("Tempo de sobrevivência: %02d.%03d segundos", seg, mil), SCREEN_WIDTH / 2 - MeasureText(TextFormat("Tempo de sobrevivência: %02d.%03d segundos", seg, mil), 20) / 2, 260, 20, LIGHTGRAY);
                    
                    DrawText("Pressione ENTER para ir ao Ranking", SCREEN_WIDTH / 2 - MeasureText("Pressione ENTER para ir ao Ranking", 16) / 2, 420, 16, GRAY);
                }
                else if (gravandoRecorde) {
                    int seg = (int)tempoTotal;
                    int mil = (int)((tempoTotal - seg) * 1000);

                    DrawText("PARABÉNS! VOCÊ VENCEU O JOGO!", SCREEN_WIDTH / 2 - MeasureText("PARABÉNS! VOCÊ VENCEU O JOGO!", 26) / 2, 120, 26, GOLD);
                    DrawText(TextFormat("Tempo Final Total: %02d.%03d segundos", seg, mil), SCREEN_WIDTH / 2 - MeasureText(TextFormat("Tempo Final Total: %02d.%03d segundos", seg, mil), 20) / 2, 180, 20, WHITE);
                    DrawText("Insira seu nome para o Placar:", SCREEN_WIDTH / 2 - MeasureText("Insira seu nome para o Placar:", 20) / 2, 260, 20, LIGHTGRAY);
                    
                    DrawRectangle(SCREEN_WIDTH / 2 - 150, 310, 300, 50, DARKGRAY);
                    DrawRectangleLines(SCREEN_WIDTH / 2 - 150, 310, 300, 50, MAROON);
                    DrawText(nomeJogador, SCREEN_WIDTH / 2 - MeasureText(nomeJogador, 22) / 2, 323, 22, RAYWHITE);
                    DrawText("Pressione ENTER para Salvar", SCREEN_WIDTH / 2 - MeasureText("Pressione ENTER para Salvar", 16) / 2, 400, 16, GRAY);
                } else {
                    DrawText("RANKING - TOP 10 MELHORES TEMPOS", SCREEN_WIDTH / 2 - MeasureText("RANKING - TOP 10 MELHORES TEMPOS", 24) / 2, 50, 24, GOLD);
                    
                    for (int i = 0; i < 10; i++) {
                        Color corLinha = (i == 0) ? GOLD : (i < 3 ? GetColor(0x81a1c1ff) : LIGHTGRAY);
                        
                        char textoPlacar[50];
                        if (ranking[i].time == 999999) {
                            sprintf(textoPlacar, "%02d.  %-15s  ---", i + 1, ranking[i].nome);
                        } else {
                            // CORREÇÃO: Converte os milissegundos inteiros guardados de volta para segundos e milésimos
                            int rSeg = ranking[i].time / 1000;
                            int rMil = ranking[i].time % 1000;
                            sprintf(textoPlacar, "%02d.  %-15s  %02d.%03ds", i + 1, ranking[i].nome, rSeg, rMil);
                        }
                        
                        DrawText(textoPlacar, SCREEN_WIDTH / 2 - 160, 120 + (i * 32), 20, corLinha);
                    }
                    
                    DrawText("Pressione ENTER para voltar ao Menu", SCREEN_WIDTH / 2 - MeasureText("Pressione ENTER para voltar ao Menu", 18) / 2, 500, 18, GRAY);
                }
                break;
            default:
                break;
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}