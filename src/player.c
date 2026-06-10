#include <raylib.h>
#include <player.h>
#include <map.h>

void InitPlayer(Player *player) {
    player->x = 0;
    player->y = 0;
    player->pos.col = 0;
    player->pos.row = 0;
}

void UpdatePlayer(Player *player, Map map) {
    float deltaTime = GetFrameTime();
    float speed = PLAYER_SPEED * deltaTime;

    // 1. Captura de comandos do teclado
    bool moveLeft  = IsKeyDown(KEY_LEFT)  || IsKeyDown(KEY_A);
    bool moveRight = IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D);
    bool moveUp    = IsKeyDown(KEY_UP)    || IsKeyDown(KEY_W);
    bool moveDown  = IsKeyDown(KEY_DOWN)  || IsKeyDown(KEY_S);

    // 2. Determinar a localização atual na grade (Grid)
    int centerCol = (int)((player->x + TILE_SIZE / 2) / TILE_SIZE);
    int centerRow = (int)((player->y + TILE_SIZE / 2) / TILE_SIZE);

    // Identifica qual o caractere lógico que o jogador está ocupando no momento
    char current_tile = ' ';
    if (centerRow >= 0 && centerRow < MAP_ROWS && centerCol >= 0 && centerCol < MAP_COLS) {
        current_tile = map.tiles[centerRow][centerCol];
    }

    // Verifica se o jogador está verticalmente desalinhado (entre duas linhas/andares)
    bool is_between_rows = ((int)player->y % TILE_SIZE != 0);

    // REGRAS DE ESCADA ('H'): Se estiver em H ou se movendo entre linhas, está preso na escada
    bool is_on_ladder_only = (current_tile == 'H') || is_between_rows;

    if (is_on_ladder_only) {
        // Bloqueia completamente comandos horizontais até que ele saia da escada
        moveLeft = false;
        moveRight = false;
    }

    // REGRAS DE ENTRADA/SAÍDA ('S' e 'D'): Ao pressionar comandos laterais, força o alinhamento com a plataforma
    if ((current_tile == 'S' || current_tile == 'D') && !is_between_rows) {
        if (moveLeft || moveRight) {
            player->y = centerRow * TILE_SIZE; // Garante alinhamento perfeito com o chão (elimina flutuação)
        }
    }

    // 3. PROCESSAMENTO DO MOVIMENTO VERTICAL (ESCADA)
    if (moveUp) {
        // Só permite subir se estiver em 'S' (base), 'H' (corpo da escada) ou já subindo entre linhas
        if (current_tile == 'S' || current_tile == 'H' || is_between_rows) {
            player->y -= speed;
            player->x = centerCol * TILE_SIZE; // Snapping: mantém centralizado no eixo da escada

            // Trava de segurança: Se subir e atingir o topo da escada ('D'), estaciona perfeitamente nele
            int newCenterRow = (int)((player->y + TILE_SIZE / 2) / TILE_SIZE);
            if (newCenterRow >= 0 && newCenterRow < MAP_ROWS) {
                char new_tile = map.tiles[newCenterRow][centerCol];
                if (new_tile == 'D' && player->y <= newCenterRow * TILE_SIZE) {
                    player->y = newCenterRow * TILE_SIZE; 
                }
            }
        }
    }
    else if (moveDown) {
        // Só permite descer se estiver em 'D' (topo), 'H' (corpo da escada) ou já descendo entre linhas
        if (current_tile == 'D' || current_tile == 'H' || is_between_rows) {
            player->y += speed;
            player->x = centerCol * TILE_SIZE; // Snapping: mantém centralizado no eixo da escada

            // Trava de segurança: Se descer e atingir a base da escada ('S'), estaciona perfeitamente nele
            int newCenterRow = (int)((player->y + TILE_SIZE / 2) / TILE_SIZE);
            if (newCenterRow >= 0 && newCenterRow < MAP_ROWS) {
                char new_tile = map.tiles[newCenterRow][centerCol];
                if (new_tile == 'S' && player->y >= newCenterRow * TILE_SIZE) {
                    player->y = newCenterRow * TILE_SIZE; 
                }
            }
        }
    }

    // Atualiza variáveis de estado após o movimento vertical para uso do movimento horizontal
    is_between_rows = ((int)player->y % TILE_SIZE != 0);
    centerCol = (int)((player->x + TILE_SIZE / 2) / TILE_SIZE);
    centerRow = (int)((player->y + TILE_SIZE / 2) / TILE_SIZE);
    if (centerRow >= 0 && centerRow < MAP_ROWS && centerCol >= 0 && centerCol < MAP_COLS) {
        current_tile = map.tiles[centerRow][centerCol];
    }

    // 4. PROCESSAMENTO DO MOVIMENTO HORIZONTAL
    if (!is_between_rows && (moveLeft || moveRight)) {
        float nextX = player->x;
        if (moveLeft)  nextX -= speed;
        if (moveRight) nextX += speed;

        int nextMinCol = (int)(nextX / TILE_SIZE);
        int nextMaxCol = (int)((nextX + TILE_SIZE - 1) / TILE_SIZE);

        // Colisão Lateral com blocos sólidos 'Z' no mesmo nível de linha atual
        bool horizontalCollision = false;
        if (centerRow >= 0 && centerRow < MAP_ROWS) {
            for (int c = nextMinCol; c <= nextMaxCol; c++) {
                if (c < 0 || c >= MAP_COLS || map.tiles[centerRow][c] == 'Z') {
                    horizontalCollision = true;
                }
            }
        } else {
            horizontalCollision = true;
        }

        // Restrição de Chão: Não caminhar fora de plataformas (evitar queda livre) [cite: 92]
        int floorRow = centerRow + 1;
        bool hasFloorBelow = false;

        if (floorRow >= 0 && floorRow < MAP_ROWS) {
            char leftFloor  = map.tiles[floorRow][nextMinCol];
            char rightFloor = map.tiles[floorRow][nextMaxCol];

            // O chão é válido se for uma plataforma 'Z' ou qualquer elemento de escada ('H', 'S', 'D') 
            // que compõe o buraco de passagem do piso
            bool leftValid  = (leftFloor == 'Z' || leftFloor == 'H' || leftFloor == 'S' || leftFloor == 'D');
            bool rightValid = (rightFloor == 'Z' || rightFloor == 'H' || rightFloor == 'S' || rightFloor == 'D');

            if (leftValid && rightValid) {
                hasFloorBelow = true;
            }
        }

        // Se o personagem estiver pisando em 'S' ou 'D', ele está na saída/entrada da plataforma
        if (current_tile == 'S' || current_tile == 'D') {
            hasFloorBelow = true;
        }

        if (!hasFloorBelow) {
            horizontalCollision = true;
        }

        // Aplica o movimento se não houver colisões impeditivas
        if (!horizontalCollision) {
            player->x = nextX;
        }
    }

    // 5. Sincronização final da posição lógica da struct
    player->pos.col = (int)((player->x + TILE_SIZE / 2) / TILE_SIZE);
    player->pos.row = (int)((player->y + TILE_SIZE / 2) / TILE_SIZE);
}
    void DrawPlayer(Player player) {
    // Usando o ponto (.) pois o parâmetro aqui recebe a cópia da struct direta
    DrawRectangle((int)player.x, (int)player.y, TILE_SIZE, TILE_SIZE, BLUE);
    DrawRectangleLines((int)player.x, (int)player.y, TILE_SIZE, TILE_SIZE, SKYBLUE);
}