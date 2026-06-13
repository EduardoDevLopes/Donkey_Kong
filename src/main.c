#include <stdio.h>
#include <raylib.h>
#include <constants.h>
#include <menu.h>
#include <pause.h>
#include <player.h>
#include <map.h>
#include <enemy.h>
#include <ranking.h>
#include <collision.h>
#include <phase.h>

// FUNÇÃO PRINCIPAL DO JOGO
int main(void) {
    // INICIALIZAÇÃO DA JANELA
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Donkey Kong INF");
    
    // Define a taxa de quadros por segundo
    SetTargetFPS(TARGET_FPS);

    // INICIALIZAÇÃO DE VARIÁVEIS DE ESTADO
    // Estado atual do jogo (começa no menu)
    GameState estadoAtual = STATE_MENU;
    
    // Opções selecionadas em cada menu
    int opcaoSelecionadaMenu = 0;
    int opcaoSelecionadaPausa = 0;

    // INICIALIZAÇÃO DE OBJETOS DO JOGO
    // Estrutura do jogador (posição, etc)
    Player jogador;
    
    // Estrutura do mapa (matriz 30x30)
    Map mapa;
    
    // Array de inimigos (até 100 simultaneamente)
    Enemy enemies[MAX_ENEMIES];
    int enemyCount = 0;

    // VARIÁVEIS DE PROGRESSO DA SESSÃO
    // Rastreia qual fase o jogador está (0 = mapa0, 1 = mapa1, etc)
    int faseAtual = 0;
    
    // Tempo decorrido desde o início da partida em segundos
    float tempoTotal = 0.0f;

    // VARIÁVEIS DE INTERFACE / RANKING

    // Buffer para armazenar nome do jogador durante entrada
    char nomeJogador[MAX_PLAYER_NAME] = "\0";
    
    // Conta quantas letras o jogador digitou
    int letrasCount = 0;
    
    // Flag: jogador está digitando nome para salvar recorde?
    bool gravandoRecorde = false;
    
    // Flag: exibir tela de Game Over antes do ranking?
    bool exibindoGameOver = false;

    // Array que armazena os 10 melhores tempos/pontuações
    TIPO_PLACAR ranking[RANKING_SIZE]; 

    // INICIALIZAÇÃO PADRÃO
    // Configura jogador na posição inicial
    InitPlayer(&jogador);
    
    // Carrega o primeiro mapa (mapa0.txt) e posiciona jogador no 'P'
    LoadMap(&mapa, "mapas/mapa0.txt", &jogador);
    
    // Inicializa inimigos encontrando todos os 'E' no mapa
    InitEnemies(enemies, &enemyCount, mapa);

    // LOOP PRINCIPAL DO JOGO
    while (estadoAtual != STATE_EXIT && !WindowShouldClose()) {
        
        // LÓGICA DE ATUALIZAÇÃO
        // Processa entrada do jogador, atualiza física, verifica colisões
        
        switch (estadoAtual) {
            // ESTADO: MENU INICIAL
            case STATE_MENU: {
                // Processa entrada no menu (setas, Enter)
                UpdateMenu(&estadoAtual, &opcaoSelecionadaMenu);
                
                // Se jogador selecionou "Novo Jogo", inicializa uma nova partida
                if (estadoAtual == STATE_PLAYING) {
                    // Reset absoluto de todas as variáveis para nova partida
                    faseAtual = 2;              // Volta para fase 0
                    tempoTotal = 0.0f;          // Zera o relógio
                    nomeJogador[0] = '\0';      // Limpa nome anterior
                    letrasCount = 0;            // Zera contador de letras
                    gravandoRecorde = false;    // Não está gravando recorde ainda
                    exibindoGameOver = false;   // Não mostra tela de game over

                    // Reinicializa todos os objetos do jogo
                    InitPlayer(&jogador);
                    LoadMap(&mapa, "mapas/mapa0.txt", &jogador); 
                    InitEnemies(enemies, &enemyCount, mapa);
                }
                break;
            }
            
            // ESTADO: JOGO EM ANDAMENTO 
            case STATE_PLAYING: {
                // Acumula tempo a cada frame
                tempoTotal += GetFrameTime();

                // Atualiza posição do jogador baseado em entrada de teclado
                UpdatePlayer(&jogador, mapa);
                
                // Atualiza posição de todos os inimigos
                UpdateEnemies(enemies, enemyCount, mapa);
                
                // Verifica se jogador colidiu com algum inimigo
                if (CheckPlayerEnemyCollision(&jogador, enemies, enemyCount)) {
                    // Colisão detectada - jogador foi capturado
                    estadoAtual = STATE_RANKING;
                    exibindoGameOver = true; // Mostrar tela de Game Over
                }

                // Verifica se jogador atingiu a porta de saída (F)
                if (estadoAtual == STATE_PLAYING && mapa.tiles[jogador.pos.row][jogador.pos.col] == 'F') {
                    // Tenta avançar para próxima fase
                    if (!AdvancePhase(&faseAtual, &jogador, &mapa, enemies, &enemyCount)) {
                        // Não há próxima fase - Vitória Total!
                        estadoAtual = STATE_RANKING;
                        gravandoRecorde = true;
                        exibindoGameOver = false;
                    }
                }
                
                // Verifica se jogador pressionou TAB (pausa)
                if (IsKeyPressed(KEY_TAB)) {
                    opcaoSelecionadaPausa = 0; // Reset seleção do menu de pausa
                    estadoAtual = STATE_PAUSE;
                }
                break;
            }
            
            // ESTADO: MENU DE PAUSA
            case STATE_PAUSE: {
                // Processa entrada no menu de pausa (setas, Enter, TAB)
                UpdatePauseMenu(&estadoAtual, &opcaoSelecionadaPausa);
                break;
            }
            
            // ESTADO: RANKING / FIM DE JOGO
            case STATE_RANKING: {
                // Lógica de Game Over (mostrar antes do ranking)
                if (exibindoGameOver) {
                    // Tela estática de Game Over aguardando ENTER
                    if (IsKeyPressed(KEY_ENTER)) {
                        exibindoGameOver = false; // Desativa Game Over, mostra ranking
                    }
                }
                // Lógica de entrada de nome para novo recorde
                else if (gravandoRecorde) {
                    // Captura caracteres digitados
                    int tecla = GetCharPressed();
                    while (tecla > 0) {
                        // Adiciona caractere se for imprimível e houver espaço
                        if ((tecla >= 32) && (tecla <= 125) && (letrasCount < MAX_PLAYER_NAME - 1)) {
                            nomeJogador[letrasCount] = (char)tecla;
                            nomeJogador[letrasCount + 1] = '\0';
                            letrasCount++;
                        }
                        tecla = GetCharPressed();
                    }

                    // Detecta Backspace para deletar caracteres
                    if (IsKeyPressed(KEY_BACKSPACE)) {
                        letrasCount--;
                        if (letrasCount < 0) letrasCount = 0;
                        nomeJogador[letrasCount] = '\0';
                    }

                    // Detecta ENTER para confirmar nome e salvar
                    if (IsKeyPressed(KEY_ENTER) && letrasCount > 0) {
                        // Converte tempo em float para milissegundos inteiros
                        // Exemplo: 12.345 segundos = 12345 milissegundos
                        VerificarESalvarPlacar(nomeJogador, (int)(tempoTotal * 1000));
                        gravandoRecorde = false; // Termina entrada de nome
                    }
                } 
                // Lógica de exibição do ranking
                else {
                    // Carrega os 10 melhores tempos do arquivo
                    CarregarPlacar(ranking);

                    // Volta ao menu se pressionar ENTER ou ESC
                    if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_ENTER)) {
                        estadoAtual = STATE_MENU;
                    }
                }
                break;
            }
            
            default:
                break;
        }

        // PASSO 2: RENDERIZAÇÃO GRÁFICA
        // Desenha todos os elementos na tela
        
        BeginDrawing();
        ClearBackground(BLACK); // Fundo preto

        switch (estadoAtual) {
            // RENDERIZAR: MENU INICIAL
            case STATE_MENU:
                DrawMenu(opcaoSelecionadaMenu);
                break;
            
            // RENDERIZAR: JOGO EM ANDAMENTO
            case STATE_PLAYING: {
                DrawMap(mapa);       // Desenha mapa (plataformas, escadas)
                DrawPlayer(jogador); // Desenha jogador
                DrawEnemies(enemies, enemyCount); // Desenha inimigos

                // Exibe tempo no canto superior direito
                // Formata como: TEMPO: MM.MSSs (minutos.milissegundos segundos)
                {
                    int seg = (int)tempoTotal;
                    int mil = (int)((tempoTotal - seg) * 1000);
                    DrawText(TextFormat("TEMPO: %02d.%03ds", seg, mil), 
                             SCREEN_WIDTH - 190, 15, 20, WHITE);
                }
                
                // Exibe número da fase atual no canto superior esquerdo
                DrawText(TextFormat("FASE: %d", faseAtual), 20, 15, 20, GREEN);
                break;
            }
            
            // RENDERIZAR: MENU DE PAUSA 
            case STATE_PAUSE:
                // Renderiza jogo ao fundo
                DrawMap(mapa);
                DrawPlayer(jogador);
                DrawEnemies(enemies, enemyCount);
                
                // Renderiza menu de pausa 
                DrawPauseMenu(opcaoSelecionadaPausa);
                break;
            
            // RENDERIZAR: RANKING / FIM DE JOGO 
            case STATE_RANKING: {
                if (exibindoGameOver) {
                    // Tela de Game Over
                    int seg = (int)tempoTotal;
                    int mil = (int)((tempoTotal - seg) * 1000);

                    DrawText("GAME OVER!", 
                             SCREEN_WIDTH / 2 - MeasureText("GAME OVER!", 36) / 2, 140, 36, RED);
                    DrawText("Você foi capturado por um inimigo!", 
                             SCREEN_WIDTH / 2 - MeasureText("Você foi capturado por um inimigo!", 20) / 2, 210, 20, WHITE);
                    DrawText(TextFormat("Tempo de sobrevivência: %02d.%03d segundos", seg, mil), 
                             SCREEN_WIDTH / 2 - MeasureText(TextFormat("Tempo de sobrevivência: %02d.%03d segundos", seg, mil), 20) / 2, 260, 20, LIGHTGRAY);
                    
                    DrawText("Pressione ENTER para ir ao Ranking", 
                             SCREEN_WIDTH / 2 - MeasureText("Pressione ENTER para ir ao Ranking", 16) / 2, 420, 16, GRAY);
                }
                else if (gravandoRecorde) {
                    // Tela de entrada de nome para novo recorde
                    int seg = (int)tempoTotal;
                    int mil = (int)((tempoTotal - seg) * 1000);

                    DrawText("PARABÉNS! VOCÊ VENCEU O JOGO!", 
                             SCREEN_WIDTH / 2 - MeasureText("PARABÉNS! VOCÊ VENCEU O JOGO!", 26) / 2, 120, 26, GOLD);
                    DrawText(TextFormat("Tempo Final Total: %02d.%03d segundos", seg, mil), 
                             SCREEN_WIDTH / 2 - MeasureText(TextFormat("Tempo Final Total: %02d.%03d segundos", seg, mil), 20) / 2, 180, 20, WHITE);
                    DrawText("Insira seu nome para o Placar:", 
                             SCREEN_WIDTH / 2 - MeasureText("Insira seu nome para o Placar:", 20) / 2, 260, 20, LIGHTGRAY);
                    
                    // Caixa de entrada de texto
                    DrawRectangle(SCREEN_WIDTH / 2 - 150, 310, 300, 50, DARKGRAY);
                    DrawRectangleLines(SCREEN_WIDTH / 2 - 150, 310, 300, 50, MAROON);
                    DrawText(nomeJogador, 
                             SCREEN_WIDTH / 2 - MeasureText(nomeJogador, 22) / 2, 323, 22, RAYWHITE);
                    
                    DrawText("Pressione ENTER para Salvar", 
                             SCREEN_WIDTH / 2 - MeasureText("Pressione ENTER para Salvar", 16) / 2, 400, 16, GRAY);
                } 
                else {
                    // Tela de exibição do ranking (TOP 10)
                    DrawText("RANKING - TOP 10 MELHORES TEMPOS", 
                             SCREEN_WIDTH / 2 - MeasureText("RANKING - TOP 10 MELHORES TEMPOS", 24) / 2, 50, 24, GOLD);
                    
                    // Desenha cada entrada do ranking
                    for (int i = 0; i < RANKING_SIZE; i++) {
                    // Cor diferente para os 3 primeiros lugares encadeada corretamente
                    Color corLinha = (i == 0) ? GOLD :                 // 1º lugar = Ouro
                        (i == 1) ? GetColor(0x81a1c1ff) : // 2º lugar = Prata
                        (i == 2) ? GetColor(0xcd7f32ff) : LIGHTGRAY; // 3º lugar = Bronze | restro cinza-claro                        // 4º ao 10º = Cinza claro
                        char textoPlacar[50];
                        if (ranking[i].time == 999999) {
                            // Posição vaga (nunca foi preenchida)
                            sprintf(textoPlacar, "%02d.  %-15s  ---", i + 1, ranking[i].nome);
                        } else {
                            // Converte milissegundos de volta para segundos.milissegundos para exibição
                            int rSeg = ranking[i].time / 1000;
                            int rMil = ranking[i].time % 1000;
                            sprintf(textoPlacar, "%02d.  %-15s  %02d.%03ds", i + 1, ranking[i].nome, rSeg, rMil);
                        }
                        
                        // Desenha a linha do ranking
                        DrawText(textoPlacar, SCREEN_WIDTH / 2 - 160, 120 + (i * 32), 20, corLinha);
                    }
                    
                    // Instrução para voltar ao menu
                    DrawText("Pressione ENTER para voltar ao Menu", 
                             SCREEN_WIDTH / 2 - MeasureText("Pressione ENTER para voltar ao Menu", 18) / 2, 500, 18, GRAY);
                }
                break;
            }
            
            default:
                break;
        }

        EndDrawing(); // Finaliza renderização deste frame
    }

    CloseWindow();
    
    return 0;
}