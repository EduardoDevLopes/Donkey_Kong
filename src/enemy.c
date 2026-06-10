#include <enemy.h>

void InitEnemies(Enemy enemies[], int *enemyCount, Map map) {
    *enemyCount = 0;

    // Varre a matriz do mapa procurando o caractere 'E'
    for (int r = 0; r < MAP_ROWS; r++) {
        for (int c = 0; c < MAP_COLS; c++) {
            if (map.tiles[r][c] == 'E') {
                // Configura o inimigo na posição correspondente da grade
                enemies[*enemyCount].x = c * TILE_SIZE;
                enemies[*enemyCount].y = r * TILE_SIZE;
                enemies[*enemyCount].pos.row = r;
                enemies[*enemyCount].pos.col = c;
                enemies[*enemyCount].dirX = 1; // Começa se movendo para a direita
                enemies[*enemyCount].active = true;
                
                (*enemyCount)++;
                
                // Opcional: Se não quiser que o caractere 'E' interfira na renderização lógica do mapa,
                // você pode deixar como está, pois o switch do mapa apenas ignorará ou desenhará o fundo.
            }
        }
    }
}

void UpdateEnemies(Enemy enemies[], int enemyCount, Map map) {
    float deltaTime = GetFrameTime();

    for (int i = 0; i < enemyCount; i++) {
        if (!enemies[i].active) continue;

        float nextX = enemies[i].x + (enemies[i].dirX * ENEMY_SPEED * deltaTime);
        int currentRow = enemies[i].pos.row;

        // Determina qual será a coluna limite à frente dependendo da direção
        int nextCol = (enemies[i].dirX == 1) 
                      ? (int)((nextX + TILE_SIZE - 1) / TILE_SIZE) 
                      : (int)(nextX / TILE_SIZE);

        bool turnAround = false;

        // 1. Verificação de limites do mapa (paredes externas)
        if (nextCol < 0 || nextCol >= MAP_COLS) {
            turnAround = true;
        } else {
            // 2. Colisão com obstáculo sólido 'Z' na mesma linha [cite: 29, 93]
            if (map.tiles[currentRow][nextCol] == 'Z') {
                turnAround = true;
            }

            // 3. Sensor de queda: Evita que o inimigo caminhe para fora da plataforma 
            int floorRow = currentRow + 1;
            if (floorRow < MAP_ROWS) {
                char floorTile = map.tiles[floorRow][nextCol];
                // Se o próximo bloco abaixo não for chão/escada válido, ele deve voltar [cite: 29]
                if (floorTile != 'Z' && floorTile != 'H' && floorTile != 'S' && floorTile != 'D') {
                    turnAround = true;
                }
            } else {
                turnAround = true; // Fim do mapa abaixo
            }
        }

        // Se encontrou parede ou fim de plataforma, inverte o sentido 
        if (turnAround) {
            enemies[i].dirX *= -1; 
        } else {
            enemies[i].x = nextX; // Aplica o movimento
        }

        // Atualiza a posição lógica da grade struct [cite: 49]
        enemies[i].pos.col = (int)((enemies[i].x + TILE_SIZE / 2) / TILE_SIZE);
        enemies[i].pos.row = (int)((enemies[i].y + TILE_SIZE / 2) / TILE_SIZE);
    }
}

void DrawEnemies(Enemy enemies[], int enemyCount) {
    for (int i = 0; i < enemyCount; i++) {
        if (!enemies[i].active) continue;

        // Exemplo usando triângulos brancos/vermelhos conforme sugerido no documento 
        Vector2 v1 = { enemies[i].x + TILE_SIZE / 2, enemies[i].y };
        Vector2 v2 = { enemies[i].x, enemies[i].y + TILE_SIZE };
        Vector2 v3 = { enemies[i].x + TILE_SIZE, enemies[i].y + TILE_SIZE };

        DrawTriangle(v1, v2, v3, RED); // Use RED ou WHITE para destacar do jogador azul 
        DrawTriangleLines(v1, v2, v3, MAROON);
    }
}