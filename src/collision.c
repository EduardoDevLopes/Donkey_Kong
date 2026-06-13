#include <collision.h>
#include <raylib.h>

// IMPLEMENTAÇÃO DE VERIFICAÇÃO DE COLISÕES
bool CheckPlayerEnemyCollision(Player *player, Enemy enemies[], int enemyCount) {

    // Cria retângulo representando o jogador na tela
    // Usa x e y em pixels para comparação com coordenadas do Raylib
    Rectangle playerRec = { player->x, player->y, TILE_SIZE, TILE_SIZE };

    // Verifica colisão com cada inimigo
    for (int i = 0; i < enemyCount; i++) {
        // Ignora inimigos inativos (não presentes no jogo)
        if (!enemies[i].active) continue;

        // Cria retângulo representando o inimigo
        Rectangle enemyRec = { enemies[i].x, enemies[i].y, TILE_SIZE, TILE_SIZE };

        // Raylib verifica se dois retângulos se sobrepõem
        // Se houver sobreposição, há colisão
        if (CheckCollisionRecs(playerRec, enemyRec)) {
            return true; // Colisão detectada - jogador foi capturado
        }
    }

    // Nenhuma colisão detectada
    return false;
}