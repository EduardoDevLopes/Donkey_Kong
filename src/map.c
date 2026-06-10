#include <stdio.h>
#include <raylib.h>
#include <map.h>

void LoadMap(Map *map, const char *filename, Player *player) {
    // 1. Inicializa a matriz inteira com espaços vazios por segurança
    for (int r = 0; r < MAP_ROWS; r++) {
        for (int c = 0; c < MAP_COLS; c++) {
            map->tiles[r][c] = ' ';
        }
    }

    // 2. Abre o arquivo texto do mapa
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        // Chão de emergência caso o arquivo suma
        for (int c = 0; c < MAP_COLS; c++) map->tiles[MAP_ROWS - 1][c] = 'Z';
        return;
    }

    // Buffer temporário para armazenar as linhas lidas antes de processar a centralização
    char tempLines[MAP_ROWS][100];
    int linesRead = 0;

    // Lê todas as linhas disponíveis no arquivo (até o limite de 30)
    while (linesRead < MAP_ROWS && fgets(tempLines[linesRead], sizeof(tempLines[linesRead]), file) != NULL) {
        linesRead++;
    }
    fclose(file); // Fecha o arquivo assim que terminar a leitura primária

    // 3. Cálculos de centralização vertical
    // Se o arquivo tiver 12 linhas, a sobra é 18. Dividido por 2 = começará na linha index 9.
    int rowOffset = (MAP_ROWS - linesRead) / 2;

    // 4. Transfere do buffer temporário para a matriz oficial aplicando o deslocamento
    for (int i = 0; i < linesRead; i++) {
        int targetRow = rowOffset + i; // Linha real onde o bloco vai residir na matriz

        for (int col = 0; col < MAP_COLS; col++) {
            char token = tempLines[i][col];

            // Interrompe se encontrar fim de linha
            if (token == '\0' || token == '\n' || token == '\r') {
                break;
            }

            if (token == 'P' && player != NULL) {
                // O jogador ganha a posição corrigida com o offset vertical automaticamente
                player->pos.col = col;
                player->pos.row = targetRow;
                player->x = col * TILE_SIZE;
                player->y = targetRow * TILE_SIZE;
                
                map->tiles[targetRow][col] = ' '; 
            } else {
                map->tiles[targetRow][col] = token;
            }
        }
    }
}

void DrawMap(Map map) {
    for (int r = 0; r < MAP_ROWS; r++) {
        for (int c = 0; c < MAP_COLS; c++) {
            char tile = map.tiles[r][c];
            int pixelX = c * TILE_SIZE;
            int pixelY = r * TILE_SIZE;

            switch (tile) {
                case 'Z': // Chão / Tijolo
                    DrawRectangle(pixelX, pixelY, TILE_SIZE, TILE_SIZE, MAROON);
                    DrawRectangleLines(pixelX, pixelY, TILE_SIZE, TILE_SIZE, (Color){ 190, 60, 60, 255 });
                    break;

                case 'H': // Escada Principal
                    DrawRectangle(pixelX + 4, pixelY, TILE_SIZE - 8, TILE_SIZE, LIGHTGRAY);
                    for (int i = 0; i < TILE_SIZE; i += 5) {
                        DrawLine(pixelX + 4, pixelY + i, pixelX + TILE_SIZE - 4, pixelY + i, GRAY);
                    }
                    break;

                case 'S': // Escada Superior
                    DrawRectangle(pixelX + 4, pixelY, TILE_SIZE - 8, TILE_SIZE, DARKGRAY);
                    for (int i = 0; i < TILE_SIZE; i += 5) {
                        DrawLine(pixelX + 4, pixelY + i, pixelX + TILE_SIZE - 4, pixelY + i, BLACK);
                    }
                    break;

                //Inimigo e Personagem não é desenhado de forma estática, por isso não está aqui

                case 'F': // Porta da fase (Gold)
                    DrawRectangle(pixelX + 1, pixelY, TILE_SIZE - 2, TILE_SIZE, GOLD);
                    break;

                case 'D': // Objeto invisível
                default:
                    break;
            }
        }
    }
}