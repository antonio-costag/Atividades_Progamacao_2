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

Node* rot_esquerda(Node* no) {
    // retorna a nova raíz da subárvore formada por no
    // uma rotação para a esqueda segue as seguintes etapas, com N sendo o nó
    // a ser girado:
    //  1. o filho à direita de N vira o pai de N, e N é movido para a esquerda
    //  2. o filho à esquerda do antigo filho à direita de N vira o filho à
    //     direita de N
    // os outros nós não são afetados

    Node* dN = no->dir;
    Node* deN = dN->esq;

    dN->esq = no;
    no->dir = deN;
    return dN;
}

Node* rot_direita(Node* no) {
    // uma rotação para a direita é o inverso da rotação p esquerda:
    //  1. o filho à esquerda de N vira o pai de N, e N é movido para a direita
    //  2. o filho à direita do antigo filho à esquerda de N vira o filho à
    //     esquerda de N

    Node* eN = no->esq;
    Node* edN = eN->dir;

    eN->dir = no;
    no->esq = edN;
    return eN;
}