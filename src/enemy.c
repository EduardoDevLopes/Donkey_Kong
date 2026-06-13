#include <enemy.h>

// INICIALIZAÇÃO DE INIMIGOS
void InitEnemies(Enemy enemies[], int *enemyCount, Map map) {
    // Começa com zero inimigos
    *enemyCount = 0;

    // VARREDURA DO MAPA
    // Procura por cada 'E' no mapa 
    for (int r = 0; r < MAP_ROWS; r++) {
        for (int c = 0; c < MAP_COLS; c++) {
            // Se encontrou um inimigo, configura suas propriedades iniciais
            if (map.tiles[r][c] == 'E') {
                
                // Converte posição da grid para pixels
                enemies[*enemyCount].x = c * TILE_SIZE;
                enemies[*enemyCount].y = r * TILE_SIZE;
                
                // Armazena posição na grid
                enemies[*enemyCount].pos.row = r;
                enemies[*enemyCount].pos.col = c;
                
                // Inimigo começa se movendo para a direita
                enemies[*enemyCount].dirX = 1;
                
                // Marca inimigo como ativo (presente no jogo)
                enemies[*enemyCount].active = true;
                
                // Passa para o próximo inimigo
                (*enemyCount)++;
            }
        }
    }
}

// ATUALIZAÇÃO DE INIMIGOS
void UpdateEnemies(Enemy enemies[], int enemyCount, Map map) {
    /* Calcula quanto tempo passou desde o último frame,
    Garante movimento suave independente da taxa de FPS*/
    float deltaTime = GetFrameTime();

    // Atualiza cada inimigo
    for (int i = 0; i < enemyCount; i++) {
        // Ignora inimigos inativos
        if (!enemies[i].active) continue;

        // CÁLCULO DE MOVIMENTO
        // Calcula a próxima posição baseado na velocidade, direção e tempo
        float nextX = enemies[i].x + (enemies[i].dirX * ENEMY_SPEED * deltaTime);
        int currentRow = enemies[i].pos.row;
        int nextCol;

        // Determina qual coluna verificar conforme a direção
        if (enemies[i].dirX == 1) {
            // Movendo para direita: testa a extremidade direita do inimigo
            nextCol = (int)((nextX + TILE_SIZE) / TILE_SIZE);
        } else {
            // Movendo para esquerda: testa a extremidade esquerda
            nextCol = (int)(nextX / TILE_SIZE);
        }

        bool turnAround = false;

        // VERIFICAÇÃO Limites do mapa
        if (nextCol < 0 || nextCol >= MAP_COLS) {
            turnAround = true; // Se chegou na borda do mapa, deve virar
        }

        else {
            // VERIFICAÇÃO Colisão com obstáculo sólido
            if (map.tiles[currentRow][nextCol] == 'Z') {
                turnAround = true; // Se há uma parede ('Z') no caminho, deve virar
            }

            // VERIFICAÇÃO Sensor de queda
            int floorRow = currentRow + 1; // Linha a frente do inimigo
           
            if (floorRow < MAP_ROWS) { //verifica se não é o fim do mapa
                char floorTile = map.tiles[floorRow][nextCol];

                // Se o próximo tile abaixo não for chão ou escada válida, inverte
                if (floorTile != 'Z' && floorTile != 'H' && floorTile != 'S' && floorTile != 'D') {
                    turnAround = true;
                }
            } else {
                // Fim do mapa abaixo = fim de plataforma = deve virar
                turnAround = true;
            }
        }

        // APLICA MOVIMENTO OU INVERSÃO
        if (turnAround) {
            // Inverte a direção 
            enemies[i].dirX *= -1; 
            
            /* Posiciona o inimigo no tile que estava antes de virar,
            Evita que ele atravesse a borda durante a inversão */
            enemies[i].x = enemies[i].pos.col * TILE_SIZE;
        } else {
            // Movimento seguro aprovado - atualiza posição
            enemies[i].x = nextX;
        }

        // SINCRONIZAÇÃO DA GRID
        // Atualiza a posição na grid para corresponder com pixels
       
        // Usa o centro do inimigo para determinar a coluna e linha correta
        enemies[i].pos.col = (int)((enemies[i].x + TILE_SIZE / 2) / TILE_SIZE);
        enemies[i].pos.row = (int)((enemies[i].y + TILE_SIZE / 2) / TILE_SIZE);
    }
}

// RENDERIZAÇÃO DE INIMIGOS
void DrawEnemies(Enemy enemies[], int enemyCount) {
    // Desenha cada inimigo como um triângulo
    for (int i = 0; i < enemyCount; i++) {
        // Ignora inimigos inativos
        if (!enemies[i].active) continue;

        // Define os três vértices de um triângulo apontando para cima
        // Vértice 1: Topo do triângulo (centro)
        Vector2 v1 = { enemies[i].x + TILE_SIZE / 2, enemies[i].y };
        
        // Vértice 2: Canto inferior esquerdo
        Vector2 v2 = { enemies[i].x, enemies[i].y + TILE_SIZE };
        
        // Vértice 3: Canto inferior direito
        Vector2 v3 = { enemies[i].x + TILE_SIZE, enemies[i].y + TILE_SIZE };

        // Desenha triângulo preenchido em vermelho
        DrawTriangle(v1, v2, v3, RED);
        
        // Desenha contorno do triângulo em branco para melhor visibilidade
        DrawTriangleLines(v1, v2, v3, WHITE);
    }
}