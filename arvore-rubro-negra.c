/*
Implementação de árvore rubro-negra
Uma árvore rubro-negra possui 5 regras:
- Todos os nós são vermelhos ou pretos
- As raízes e as folhas (os NULLs) são pretas
- Se um nó for vermelho, seus filhos serão pretos
- Todos os caminhos de um nó para seus descendentes NULLs possuem o
  mesmo número de nós pretos
*/

#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int valor;
    struct Node *dir, *esq, *pai; 
    int cor;  // 0: preto, 1: vermelho
} Node;

Node* mknode(int valor, int cor) {
    // função para criar nós sem precisar repetir o código de inicialização
    Node* novo_no = malloc(sizeof(Node));
    novo_no->dir = NULL;
    novo_no->esq = NULL;
    novo_no->pai = NULL;
    novo_no->valor = valor;
    novo_no->cor = cor;
    return novo_no;
}

