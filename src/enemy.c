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
                
            }
        }
    }
}

void UpdateEnemies(Enemy enemies[], int enemyCount, Map map) {
    float deltaTime = GetFrameTime();

    for (int i = 0; i < enemyCount; i++) {
        if (!enemies[i].active) continue;

        // Calcula a posição futura baseada na velocidade e no tempo do frame
        float nextX = enemies[i].x + (enemies[i].dirX * ENEMY_SPEED * deltaTime);
        int currentRow = enemies[i].pos.row;
        int nextCol;

        // Determina qual coluna testar à frente dependendo da direção
        if (enemies[i].dirX == 1) {
            // Lado direito: testa a extremidade direita do retângulo do inimigo
            nextCol = (int)((nextX + TILE_SIZE) / TILE_SIZE);
        } else {
            // Lado esquerdo: testa a extremidade esquerda (origem x).
            // O truncamento automático do (int) deteta a invasão no bloco da esquerda imediatamente
            nextCol = (int)(nextX / TILE_SIZE);
        }

        bool turnAround = false;

        // 1. Verificação de limites das bordas do mapa
        if (nextCol < 0 || nextCol >= MAP_COLS) {
            turnAround = true;
        } else {
            // 2. Colisão com o obstáculo sólido 'Z' na mesma linha
            if (map.tiles[currentRow][nextCol] == 'Z') {
                turnAround = true;
            }

            // 3. Sensor de queda: Evita que o inimigo caminhe para fora da plataforma
            int floorRow = currentRow + 1;
            if (floorRow < MAP_ROWS) {
                char floorTile = map.tiles[floorRow][nextCol];
                // Se o próximo bloco abaixo não for chão ou escada válida, ele deve voltar
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
            
            // Alinhamento forçado (Snap to Grid): Garante que o inimigo não passe da borda
            // reposicionando-o exatamente no início do bloco lógico atual em que ele já estava estável
            enemies[i].x = enemies[i].pos.col * TILE_SIZE;
        } else {
            enemies[i].x = nextX; // Movimento seguro aprovado
        }

        // Atualiza a posição lógica da grade struct (coluna e linha na matriz)
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

        DrawTriangle(v1, v2, v3, RED);
        DrawTriangleLines(v1, v2, v3, MAROON);
    }
}