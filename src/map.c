#include <stdio.h>
#include <raylib.h>
#include <map.h>

// CARREGAMENTO DE MAPA
void LoadMap(Map *map, const char *filename, Player *player) {
    // INICIALIZAÇÃO
    // Limpa toda a matriz com espaços (tiles vazios/seguros)
    for (int r = 0; r < MAP_ROWS; r++) {
        for (int c = 0; c < MAP_COLS; c++) {
            map->tiles[r][c] = ' ';
        }
    }

    // LEITURA DO ARQUIVO
    // Tenta abrir o arquivo do mapa
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        /* Se arquivo não existe, cria um chão de emergência na última linha
         Isso evita o jogo travar*/
        for (int c = 0; c < MAP_COLS; c++) map->tiles[MAP_ROWS - 1][c] = 'Z';
        return;
    }

    // Buffer temporário para armazenar linhas lidas
    // Cada linha pode ter até 100 caracteres
    char tempLines[MAP_ROWS][100];
    int linesRead = 0;

    // Lê todas as linhas do arquivo para o buffer temporário
    while (linesRead < MAP_ROWS && fgets(tempLines[linesRead], sizeof(tempLines[linesRead]), file) != NULL) {
        linesRead++;
    }
    fclose(file); // Fecha arquivo assim que terminar de ler

    // CÁLCULO DE CENTRALIZAÇÃO VERTICAL
    // Se o arquivo tiver menos de 30 linhas, centraliza verticalmente na tela
    int rowOffset = (MAP_ROWS - linesRead) / 2;

    // COPIA PARA A MATRIZ FINAL
    // Copia do buffer temporário para a matriz final, aplicando a centralização vertical
    for (int i = 0; i < linesRead; i++) {
        int targetRow = rowOffset + i; // Linha real na matriz final

        // Processa cada caractere da linha
        for (int col = 0; col < MAP_COLS; col++) {
            char token = tempLines[i][col];

            // Interrompe se encontrar fim de linha
            if (token == '\0' || token == '\n' || token == '\r') {
                break;
            }

            // Se encontrou marcador de posição inicial do jogador
            if (token == 'P' && player != NULL) {
                // Posiciona o jogador neste local
                player->pos.col = col;
                player->pos.row = targetRow;
                player->x = col * TILE_SIZE;
                player->y = targetRow * TILE_SIZE;
                
                // Marca este tile como vazio (jogador não é um tile)
                map->tiles[targetRow][col] = ' '; 
            } else {
                // Copia o caractere para a matriz
                map->tiles[targetRow][col] = token;
            }
        }
    }
}

// RENDERIZAÇÃO DO MAPA

void DrawMap(Map map) {
    // Varre toda a matriz desenha cada tile conforme seu tipo
    for (int r = 0; r < MAP_ROWS; r++) {
        for (int c = 0; c < MAP_COLS; c++) {
            char tile = map.tiles[r][c];
            
            // Converte posição da grid para pixels na tela
            int pixelX = c * TILE_SIZE;
            int pixelY = r * TILE_SIZE;

            // Desenha conforme o tipo de tile
            switch (tile) {
                case 'Z': 
                    // Plataforma
                    
                    DrawRectangle(pixelX, pixelY, TILE_SIZE, TILE_SIZE, MAROON);
                    DrawRectangleLines(pixelX, pixelY, TILE_SIZE, TILE_SIZE, (Color){ 190, 60, 60, 255 });
                    break;

                case 'H': 
                    // Corpo da escada principal
                    // Desenhado como série de linhas horizontais representando degraus
                    DrawRectangle(pixelX + 4, pixelY, TILE_SIZE - 8, TILE_SIZE, LIGHTGRAY);
                    for (int i = 0; i < TILE_SIZE; i += 5) {
                        DrawLine(pixelX + 4, pixelY + i, pixelX + TILE_SIZE - 4, pixelY + i, GRAY);
                    }
                    break;

                case 'S': 
                    // Base da escada
                    // Desenhado como escada com degraus mais escuros
                    DrawRectangle(pixelX + 4, pixelY, TILE_SIZE - 8, TILE_SIZE, DARKGRAY);
                    for (int i = 0; i < TILE_SIZE; i += 5) {
                        DrawLine(pixelX + 4, pixelY + i, pixelX + TILE_SIZE - 4, pixelY + i, BLACK);
                    }
                    break;

                case 'F': 
                    // Porta de saída
                    // Desenhado como retângulo dourado
                    DrawRectangle(pixelX + 1, pixelY, TILE_SIZE - 2, TILE_SIZE, GOLD);
                    break;

                case 'D': 
                // Marcador lógico, não é desenhado
                case ' ':
                //Espaço em branco, não é desenhado
                case 'E':
                // Marcador lógico de inimigo, desenhado em enemy.c
                case 'P':
                // Marcador lógico de posição do jogador, desenhado em player.c
                default:
                    // Qualquer outro caractere (ignorado)
                    break;
            }
        }
    }
}