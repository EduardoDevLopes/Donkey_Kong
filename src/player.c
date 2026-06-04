#include <raylib.h>
#include <player.h>

void InitPlayer(Player *player) {
    player->pos.row = 25; 
    player->pos.col = 5;  
    
    // Inicializa a posição em pixels baseada na célula da matriz
    player->x = player->pos.col * TILE_SIZE;
    player->y = player->pos.row * TILE_SIZE;
    
    player->state = PLAYER_ALIVE;
    player->score = 0;
}

void UpdatePlayer(Player *player) {
    if (player->state == PLAYER_DEAD) return;

    // DeltaTime garante independência da taxa de quadros por segundo (FPS)
    float dt = GetFrameTime(); 
    float moveDistance = PLAYER_SPEED * dt;

    // Movimentação Horizontal com detecção contínua (IsKeyDown)
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
        player->x -= moveDistance;
        if (player->x < 0) player->x = 0; // Borda esquerda
    }
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
        player->x += moveDistance;
        if (player->x > SCREEN_WIDTH - TILE_SIZE) {
            player->x = SCREEN_WIDTH - TILE_SIZE; // Borda direita
        }
    }

    // Movimentação Vertical Temporária (Será substituída pela lógica de escadas no Passo 3)
    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) {
        player->y -= moveDistance;
        if (player->y < 0) player->y = 0;
    }
    if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) {
        player->y += moveDistance;
        if (player->y > SCREEN_HEIGHT - TILE_SIZE) {
            player->y = SCREEN_HEIGHT - TILE_SIZE;
        }
    }

    // Mapeamento Reverso: Calcula em qual linha/coluna da matriz o jogador está pisando atualmente.
    // Somamos TILE_SIZE / 2 para fazer o cálculo baseado no centro do quadrado do jogador.
    player->pos.col = (int)((player->x + (TILE_SIZE / 2)) / TILE_SIZE);
    player->pos.row = (int)((player->y + (TILE_SIZE / 2)) / TILE_SIZE);
}

void DrawPlayer(Player player) {
    if (player.state == PLAYER_DEAD) return;

   // Agora desenhamos diretamente nas coordenadas flutuantes reais (convertidas para int)
    DrawRectangle((int)player.x, (int)player.y, TILE_SIZE, TILE_SIZE, BLUE);
}