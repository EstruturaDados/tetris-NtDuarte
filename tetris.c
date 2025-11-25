/* TETRIS STACK - SOLUÇÃO COMPLETA (Novato, Aventureiro, Mestre)
   Desenvolvido por: Northon Duarte
*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* --- CONFIGURAÇÕES --- */
#define TAM_FILA 5
#define TAM_PILHA 3

/* --- ESTRUTURAS --- */

// Struct Peca (Simples)
typedef struct {
    char tipo; // I, O, T, L, J, Z, S
    int id;
} Peca;

// Estrutura Fila
typedef struct {
    Peca dados[TAM_FILA];
    int inicio;
    int fim;
    int qtd;
} Fila;

// Estrutura Pilha
typedef struct {
    Peca dados[TAM_PILHA];
    int topo;
} Pilha;

/* Variável Global */
int idGlobal = 1;

/* --- PROTÓTIPOS --- */
void inicializarFila(Fila *f);
void inicializarPilha(Pilha *p);
int filaCheia(Fila *f);
int filaVazia(Fila *f);
int pilhaCheia(Pilha *p);
int pilhaVazia(Pilha *p);
Peca gerarPeca();
void enqueue(Fila *f, Peca p);
Peca dequeue(Fila *f);
void push(Pilha *p, Peca item);
Peca pop(Pilha *p);
void mostrarEstado(Fila *f, Pilha *p);
void limparBuffer();
void pausar();

/* --- FUNÇÃO PRINCIPAL --- */
int main() {
    Fila fila;
    Pilha pilha;
    int opcao = 0;
    Peca tempPeca;

    // Inicializa gerador aleatório
    srand((unsigned int)time(NULL));

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Enche a fila inicialmente
    while (!filaCheia(&fila)) {
        enqueue(&fila, gerarPeca());
    }

    // Loop do Jogo
    do {
        mostrarEstado(&fila, &pilha);

        printf("\n=== MENU TETRIS STACK ===\n");
        printf("1 - Jogar peca (Fila)\n");
        printf("2 - Reservar peca (Pilha)\n");
        printf("3 - Usar peca reservada\n");
        printf("4 - [MESTRE] Trocar (1x1)\n");
        printf("5 - [MESTRE] Trocar (3x3)\n");
        printf("0 - Sair\n");
        printf("Escolha: ");

        // Leitura segura da opção
        if (scanf("%d", &opcao) != 1) {
            printf("\n[ERRO] Digite apenas numeros!\n");
            limparBuffer();
            pausar();
            continue;
        }
        limparBuffer(); // Limpa o ENTER que sobra

        switch (opcao) {
            case 1: // Jogar
                if (!filaVazia(&fila)) {
                    tempPeca = dequeue(&fila);
                    printf("\n>>> JOGOU: [%c #%d]\n", tempPeca.tipo, tempPeca.id);
                    enqueue(&fila, gerarPeca());
                } else {
                    printf("\n[AVISO] Fila vazia.\n");
                }
                break;

            case 2: // Reservar
                if (pilhaCheia(&pilha)) {
                    printf("\n[AVISO] Reserva cheia!\n");
                } else if (filaVazia(&fila)) {
                    printf("\n[AVISO] Nada na fila para reservar.\n");
                } else {
                    tempPeca = dequeue(&fila);
                    push(&pilha, tempPeca);
                    printf("\n>>> RESERVOU: [%c #%d]\n", tempPeca.tipo, tempPeca.id);
                    enqueue(&fila, gerarPeca());
                }
                break;

            case 3: // Usar Reserva
                if (pilhaVazia(&pilha)) {
                    printf("\n[AVISO] Reserva vazia!\n");
                } else {
                    tempPeca = pop(&pilha);
                    printf("\n>>> USOU RESERVA: [%c #%d]\n", tempPeca.tipo, tempPeca.id);
                }
                break;

            case 4: // Troca 1x1
                if (!filaVazia(&fila) && !pilhaVazia(&pilha)) {
                    Peca aux = fila.dados[fila.inicio];
                    fila.dados[fila.inicio] = pilha.dados[pilha.topo];
                    pilha.dados[pilha.topo] = aux;
                    printf("\n>>> TROCA 1x1 REALIZADA!\n");
                } else {
                    printf("\n[ERRO] Precisa de pecas na fila e na pilha.\n");
                }
                break;

            case 5: // Troca 3x3
                if (pilha.topo == 2 && fila.qtd >= 3) {
                    for (int i = 0; i < 3; i++) {
                        int idxF = (fila.inicio + i) % TAM_FILA;
                        int idxP = pilha.topo - i;
                        Peca aux = fila.dados[idxF];
                        fila.dados[idxF] = pilha.dados[idxP];
                        pilha.dados[idxP] = aux;
                    }
                    printf("\n>>> TROCA 3x3 REALIZADA!\n");
                } else {
                    printf("\n[ERRO] Requer Pilha cheia (3) e Fila com min 3.\n");
                }
                break;

            case 0:
                printf("\nSaindo...\n");
                break;

            default:
                printf("\nOpcao invalida!\n");
        }

        if (opcao != 0) pausar();

    } while (opcao != 0);

    return 0;
}

/* --- IMPLEMENTAÇÕES AUXILIARES --- */

Peca gerarPeca() {
    Peca p;
    char tipos[] = {'I', 'O', 'T', 'L', 'J', 'Z', 'S'};
    p.tipo = tipos[rand() % 7];
    p.id = idGlobal++;
    return p;
}

void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = -1;
    f->qtd = 0;
}

void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

int filaCheia(Fila *f) { return f->qtd == TAM_FILA; }
int filaVazia(Fila *f) { return f->qtd == 0; }
int pilhaCheia(Pilha *p) { return p->topo == TAM_PILHA - 1; }
int pilhaVazia(Pilha *p) { return p->topo == -1; }

void enqueue(Fila *f, Peca p) {
    if (filaCheia(f)) return;
    f->fim = (f->fim + 1) % TAM_FILA;
    f->dados[f->fim] = p;
    f->qtd++;
}

Peca dequeue(Fila *f) {
    Peca p = {0,0};
    if (filaVazia(f)) return p;
    p = f->dados[f->inicio];
    f->inicio = (f->inicio + 1) % TAM_FILA;
    f->qtd--;
    return p;
}

void push(Pilha *p, Peca item) {
    if (pilhaCheia(p)) return;
    p->topo++;
    p->dados[p->topo] = item;
}

Peca pop(Pilha *p) {
    Peca p = {0,0};
    if (pilhaVazia(p)) return p;
    p = p->dados[p->topo];
    p->topo--;
    return p;
}

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void pausar() {
    printf("\nPressione ENTER...");
    getchar();
}

void mostrarEstado(Fila *f, Pilha *p) {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif

    printf("\n--- ESTADO DO JOGO ---\n");
    printf("PILHA: ");
    if (pilhaVazia(p)) printf("[Vazia]");
    else {
        for (int i = p->topo; i >= 0; i--) 
            printf("[%c #%d] ", p->dados[i].tipo, p->dados[i].id);
    }
    
    printf("\nFILA:  ");
    if (filaVazia(f)) printf("[Vazia]");
    else {
        int idx = f->inicio;
        for (int i = 0; i < f->qtd; i++) {
            printf("[%c #%d] ", f->dados[idx].tipo, f->dados[idx].id);
            idx = (idx + 1) % TAM_FILA;
        }
    }
    printf("\n----------------------\n");
}
