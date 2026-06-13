#include <phase.h>
#include <stdio.h>

// IMPLEMENTAÇÃO DE GERENCIAMENTO DE FASES

bool AdvancePhase(int *currentPhase, Player *player, Map *map, Enemy enemies[], int *enemyCount) {
    // Incrementa o número da fase atual
    (*currentPhase)++;
    
    // Constrói o nome do arquivo da próxima fase
    // Exemplo: fase 1 = "mapas/mapa1.txt", fase 2 = "mapas/mapa2.txt"
    char proximoMapa[30];
    sprintf(proximoMapa, "mapas/mapa%d.txt", *currentPhase);

    // Verifica se o arquivo de próxima fase existe
    if (FileExists(proximoMapa)) {
        // Arquivo existe - reinicia jogador, carrega novo mapa e inimigos
        InitPlayer(player);
        LoadMap(map, proximoMapa, player);
        InitEnemies(enemies, enemyCount, *map);
        return true; // Fase carregada com sucesso
    } else {
        // Arquivo não existe - fim do jogo (jogador venceu todas as fases)
        return false; // Indicar que não há próxima fase
    }
}