#include <raylib.h>
#include <player.h>
#include <map.h>

// INICIALIZAÇÃO DO JOGADOR

void InitPlayer(Player *player) {
    // Define posição inicial em pixels (canto superior esquerdo)
    player->x = 0;
    player->y = 0;
    
    // Define posição inicial na grid (canto superior esquerdo)
    player->pos.col = 0;
    player->pos.row = 0;
}

// ATUALIZAÇÃO DO JOGADOR

void UpdatePlayer(Player *player, Map map) {
    // Calcula quanto o jogador se mover este frame (baseado em tempo real, não frames)
    // Isso garante movimento suave independente da taxa de FPS
    float deltaTime = GetFrameTime();
    float speed = PLAYER_SPEED * deltaTime;

    // CAPTURA DE COMANDOS DO TECLADO
    // Detecta quais botões o jogador pressionou neste frame
    bool moveLeft  = IsKeyDown(KEY_LEFT)  || IsKeyDown(KEY_A);
    bool moveRight = IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D);
    bool moveUp    = IsKeyDown(KEY_UP)    || IsKeyDown(KEY_W);
    bool moveDown  = IsKeyDown(KEY_DOWN)  || IsKeyDown(KEY_S);

    // DETERMINAR LOCALIZAÇÃO ATUAL NA GRID
    // Encontra qual tile (quadrado 20x20) o jogador está ocupando no momento
    // Usa o centro do sprite do jogador para uma detecção mais precisa
    int centerCol = (int)((player->x + TILE_SIZE / 2) / TILE_SIZE);
    int centerRow = (int)((player->y + TILE_SIZE / 2) / TILE_SIZE);

    // Descobre qual caractere do mapa está na posição atual
    char current_tile = ' ';
    if (centerRow >= 0 && centerRow < MAP_ROWS && centerCol >= 0 && centerCol < MAP_COLS) {
        current_tile = map.tiles[centerRow][centerCol];
    }

    // Verifica se o jogador está verticalmente desalinhado (entre dois andares/linhas)
    // Exemplo: se y = 25 e TILE_SIZE = 20, então 25 % 20 = 5 (não é zero, logo está entre linhas)
    bool is_between_rows = ((int)player->y % TILE_SIZE != 0);

    // LÓGICA DE ESCADAS
    // Se estiver em uma escada ('H') ou já está subindo/descendo entre linhas,
    // o jogador fica "preso" na escada e não pode se mover horizontalmente
    bool is_on_ladder_only = (current_tile == 'H') || is_between_rows;

    if (is_on_ladder_only) {
        // Bloqueia completamente comandos horizontais até que ele saia da escada
        moveLeft = false;
        moveRight = false;
    }

    // SAÍDA/ENTRADA DE ESCADAS
    // Quando sai de uma entrada/saída de escada ('S' ou 'D') para caminhar na plataforma,
    // o jogador é alinhado perfeitamente (snap-to-grid) para evitar flutuação visual
    if ((current_tile == 'S' || current_tile == 'D') && !is_between_rows) {
        if (moveLeft || moveRight) {
            player->y = centerRow * TILE_SIZE; // Força alinhamento perfeito com o chão
        }
    }

    // PROCESSAMENTO DO MOVIMENTO VERTICAL (ESCADAS)
    if (moveUp) {
        // Só permite subir se estiver em ponto de entrada ('S'), corpo da escada ('H'), ou já subindo
        if (current_tile == 'S' || current_tile == 'H' || is_between_rows) {
            player->y -= speed; // Move para cima
            player->x = centerCol * TILE_SIZE; // Mantém centralizado no eixo X da escada

            // Verifica se atingiu o topo da escada (ponto de saída 'D')
            int newCenterRow = (int)((player->y + TILE_SIZE / 2) / TILE_SIZE);
            if (newCenterRow >= 0 && newCenterRow < MAP_ROWS) {
                char new_tile = map.tiles[newCenterRow][centerCol];
                if (new_tile == 'D' && player->y <= newCenterRow * TILE_SIZE) {
                    player->y = newCenterRow * TILE_SIZE; // Para perfeitamente no topo
                }
            }
        }
    }
    else if (moveDown) {
        // Só permite descer se estiver no topo da escada ('D'), corpo ('H'), ou já descendo
        if (current_tile == 'D' || current_tile == 'H' || is_between_rows) {
            player->y += speed; // Move para baixo
            player->x = centerCol * TILE_SIZE; // Mantém centralizado no eixo X da escada

            // Verifica se atingiu a base da escada (ponto de entrada 'S')
            int newCenterRow = (int)((player->y + TILE_SIZE / 2) / TILE_SIZE);
            if (newCenterRow >= 0 && newCenterRow < MAP_ROWS) {
                char new_tile = map.tiles[newCenterRow][centerCol];
                if (new_tile == 'S' && player->y >= newCenterRow * TILE_SIZE) {
                    player->y = newCenterRow * TILE_SIZE; // Para perfeitamente na base
                }
            }
        }
    }

    // ATUALIZAR ESTADO APÓS MOVIMENTO VERTICAL
    // Recalcula a posição na grid após ter se movido verticalmente
    // Isso é necessário para a próxima seção (movimento horizontal) ter dados atualizados
    is_between_rows = ((int)player->y % TILE_SIZE != 0);
    centerCol = (int)((player->x + TILE_SIZE / 2) / TILE_SIZE);
    centerRow = (int)((player->y + TILE_SIZE / 2) / TILE_SIZE);
    if (centerRow >= 0 && centerRow < MAP_ROWS && centerCol >= 0 && centerCol < MAP_COLS) {
        current_tile = map.tiles[centerRow][centerCol];
    }

    // PROCESSAMENTO DO MOVIMENTO HORIZONTAL
    // Só permite caminhar se não estiver entre linhas (ou seja, pisando em chão sólido)
    if (!is_between_rows && (moveLeft || moveRight)) {
        // Calcula a posição futura
        float nextX = player->x;
        if (moveLeft)  nextX -= speed;
        if (moveRight) nextX += speed;

        // Encontra qual coluna o jogador ocuparia após o movimento
        int nextMinCol = (int)(nextX / TILE_SIZE);
        int nextMaxCol = (int)((nextX + TILE_SIZE - 1) / TILE_SIZE);

        // COLISÃO COM BLOCOS HORIZONTAL
        // Verifica se há uma parede (bloco 'Z') no caminho
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

        // SENSOR DE CHÃO
        // Evita que o jogador caminhe para fora de uma plataforma e caia livremente
        // Verifica se há um tile sólido imediatamente abaixo
        int floorRow = centerRow + 1;
        bool hasFloorBelow = false;

        if (floorRow >= 0 && floorRow < MAP_ROWS) {
            // Verifica tanto o lado esquerdo quanto direito do jogador
            char leftFloor  = map.tiles[floorRow][nextMinCol];
            char rightFloor = map.tiles[floorRow][nextMaxCol];

            // O chão é válido se for plataforma ('Z') ou partes de escada ('H', 'S', 'D')
            bool leftValid  = (leftFloor == 'Z' || leftFloor == 'H' || leftFloor == 'S' || leftFloor == 'D');
            bool rightValid = (rightFloor == 'Z' || rightFloor == 'H' || rightFloor == 'S' || rightFloor == 'D');

            if (leftValid && rightValid) {
                hasFloorBelow = true;
            }
        }

        // Se o personagem estiver em uma entrada/saída de escada, ele está seguro
        if (current_tile == 'S' || current_tile == 'D') {
            hasFloorBelow = true;
        }

        // Se não há chão abaixo, impede o movimento (prevenção de queda livre)
        if (!hasFloorBelow) {
            horizontalCollision = true;
        }

        // Aplica o movimento apenas se nenhuma colisão foi detectada
        if (!horizontalCollision) {
            player->x = nextX;
        }
    }

    // SINCRONIZAÇÃO FINAL
    // Atualiza a posição na grid para corresponder com a posição em pixels
    // Garante que os dois sistemas de coordenadas estão sempre sincronizados
    player->pos.col = (int)((player->x + TILE_SIZE / 2) / TILE_SIZE);
    player->pos.row = (int)((player->y + TILE_SIZE / 2) / TILE_SIZE);
}

// RENDERIZAÇÃO DO JOGADOR

void DrawPlayer(Player player) {
    // Desenha um retângulo azul representando o jogador
    DrawRectangle((int)player.x, (int)player.y, TILE_SIZE, TILE_SIZE, BLUE);
    
    // Desenha um contorno em azul claro para melhor visibilidade
    DrawRectangleLines((int)player.x, (int)player.y, TILE_SIZE, TILE_SIZE, SKYBLUE);
}