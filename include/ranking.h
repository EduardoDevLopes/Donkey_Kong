#ifndef RANKING_H
#define RANKING_H

// Definição idêntica à especificação exigida no PDF do projeto
typedef struct tipo_placar {
    char nome[20];
    int time;
} TIPO_PLACAR;

// Interface de funções do módulo do placar
void CarregarPlacar(TIPO_PLACAR placar[]);
void VerificarESalvarPlacar(const char *nome, int tempoTotal);

#endif