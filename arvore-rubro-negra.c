/*
Implementação de árvore rubro-negra
Uma árvore rubro-negra possui algumas regras:
    1. Todos os nós são vermelhos ou pretos
    2. A raiz, as folhas e os NULLs são pretas
    3. Novos nós sempre começam vermelhos
    4. Se um nó for vermelho, seus filhos serão pretos (ou seja, não se pode ter
       nós vermelhos consecutivos)
    5. Todos os caminhos de um nó para seus descendentes possuem o mesmo número de 
       nós pretos
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

#define PRETO 0
#define VERMELHO 1

typedef struct Node {
    int valor;
    struct Node *dir, *esq, *pai; 
    int cor;  // 0: preto, 1: vermelho
} Node;

Node* raiz;
Node* NULO; // valor temporário para simplificar o código de apagar

Node* mknode(int valor, Node* pai) {
    // função para criar nós sem precisar repetir o código de inicialização
    Node* novo_no = malloc(sizeof(Node));
    novo_no->dir = NULL;
    novo_no->esq = NULL;
    novo_no->pai = pai;
    novo_no->valor = valor;
    novo_no->cor = VERMELHO;
    return novo_no;
}

Node* inserir(Node* raiz, int valor) {
    // inserindo novos nós na árvore de maneira recursiva
    // verificamos se o valor a ser inserido é maior ou menor que o nó pai e
    // executamos essa função no nó filho da direção escolhida caso ela já 
    // esteja ocpuada.
    //
    // de maneira mais formal, percorremos a árvore e as subárvores que a compõe
    // até encontrarmos um nó com a direção escolhida livre
    // OBS: essa implementação ignora valores repetidos.
    if(valor > raiz->valor) {
        // verificar se a direita já está ocupada
        if(raiz->dir != NULL) return inserir(raiz->dir, valor);
        
        // novo nó tem um valor maior que o pai -> novo nó vai pra direita
        Node* n = mknode(valor, raiz);
        raiz->dir = n;
        return n;
    } else if(valor < raiz->valor) {
        // verificar se a esquerda já está ocupada
        if(raiz->esq != NULL) return inserir(raiz->esq, valor);

        // novo nó tem um valor menor que o pai -> novo nó vai pra esquerda
        Node* n = mknode(valor, raiz);
        raiz->esq = n;
        return n;
    }

    return NULL;
}

Node* buscar(Node* raiz, int valor) {
    // buscar um nó em uma BST é basicamente o mesmo que inserir um novo nó,
    // exceto que nenhum nó é inserido e cada nó é comparado com o valor sendo
    // buscado
    if(valor == raiz->valor) {
        return raiz;
    } else if(valor > raiz->valor && raiz->dir != NULL) {
        return buscar(raiz->dir, valor);
    } else if(valor < raiz->valor && raiz->esq != NULL) {
        return buscar(raiz->esq, valor);
    }
    return NULL;
}

void print_preordem(Node* raiz) {
    // ordem: raíz, esquerda, direita
    printf("(%d)%d ", raiz->cor, raiz->valor);
    if(raiz->esq != NULL) print_preordem(raiz->esq);
    if(raiz->dir != NULL) print_preordem(raiz->dir);
}

void print_ordem(Node* raiz) {
    // ordem: esquerda, raíz, direita
    if(raiz->esq != NULL) print_ordem(raiz->esq);
    printf("(%d)%d ", raiz->cor, raiz->valor);
    if(raiz->dir != NULL) print_ordem(raiz->dir);
}

void print_posordem(Node* raiz) {
    // ordem: esquerda, direita, raíz
    if(raiz->esq != NULL) print_posordem(raiz->esq);
    if(raiz->dir != NULL) print_posordem(raiz->dir);
    printf("(%d)%d ", raiz->cor, raiz->valor);
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
    dN->pai = no->pai;
    no->pai = dN;
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
    eN->pai = no->pai;
    no->pai = eN;
    no->esq = edN;
    return eN;
}

void substituir(Node* u, Node* v) {
    // Função que substitui u por v na árvore
    if(u->pai == NULL) {
        raiz = v;
    } else if(u->pai->esq == u) {
        u->pai->esq = v;
    } else {
        u->pai->dir = v;
    }
    v->pai = u->pai;
}

void escanear_violacao_apagar(Node* no) {
    Node* x = no;
    // Atenção: Múltiplas correções podem ser aplicadas de uma vez

    while(x != raiz && x->cor == PRETO) {
        // Se o nó atual estiver à esquerda de seu pai...
        if(x == x->pai->esq) {
            // ... o irmão do nó atual estará a direita
            Node* w = x->pai->dir;

            // Se o irmão for vermelho...
            if(w != NULL && w->cor == VERMELHO) {
                // ... pintamos o irmão de preto, o pai de vermelho e giramos
                // à esquerda em torno do pai.
                w->cor = PRETO;
                x->pai->cor = PRETO;
                if(x->pai->pai != NULL) {
                    // editando os ponteiros do avô para manter a árvore
                    if(x->pai->pai->esq == x->pai) x->pai->pai->esq = rot_esquerda(x->pai);
                    else x->pai->pai->dir = rot_esquerda(x->pai);
                }
                else if(x->pai == raiz) raiz = rot_esquerda(x->pai);
                w = x->pai->dir;
            }

            // Se o irmão tiver filhos pretos...
            if(w != NULL && (w->esq == NULL || w->esq->cor == PRETO) 
                && (w->dir == NULL || w->dir->cor == PRETO)) {
                // ... pintamos o irmão de vermelho e movemos o nó atual para o
                // pai.
                w->cor = VERMELHO;
                if(x->valor == -999) free(x); // removendo o NULO da árvore
                x = x->pai;
            }

            // Se o irmão for preto e apenas o filho direito for preto...
            if(w != NULL && (w->dir == NULL || w->dir->cor == PRETO)) {
                // ... pintamos o filho esquerdo de preto, o irmão de vermelho
                // e giramos à direita em torno do irmão.
                w->esq->cor = PRETO;
                w->cor = VERMELHO;
                w->pai->dir = rot_direita(w);
                w = x->pai->dir;
            }

            // Em qualquer outro caso (filhos do irmão vermelhos)...
            if(w != NULL) {
                // ... o irmão copia a cor do pai, o pai e o filho direito do
                // irmão são pintados de preto, uma rotação à esquerda é feita
                // em torno do pai, e x passa a ser a raíz, encerrando o while.
                w->cor = x->pai->cor;
                x->pai->cor = PRETO;
                w->dir->cor = PRETO;
                if(x->pai->pai != NULL) {
                    if(x->pai->pai->esq == x->pai) x->pai->pai->esq = rot_esquerda(x->pai);
                    else x->pai->pai->dir = rot_esquerda(x->pai);
                } else if(x->pai == raiz) {
                    raiz = rot_esquerda(x->pai);
                }
                x = raiz;
            }
        }
    }
}

void apagar(Node* no) {
    Node* x;
    int cor_original;
    if(no->esq == NULL) {
        // Primeiro caso: esquerda nula
        x = no->dir;
        cor_original = no->cor;
        substituir(no, x);
        free(no);
    } else if(no->dir == NULL) {
        // Segundo caso: direita nula
        x = no->esq;
        cor_original = no->cor;
        substituir(no, x);
        free(no);
    } else if(no->esq != NULL && no->dir != NULL) {
        // Terceiro caso: nenhum lado nulo
        // Usamos o código de apagar da BST normal para encontrar o nó com o
        // menor valor da subárvore direita
        Node* minimo = NULL;
        Node* no_atual = no->dir;
        while(minimo == NULL) {
            if(no_atual->esq != NULL) {
                no_atual = no_atual->esq;
            } else {
                minimo = no_atual;
            }
        }

        // Se o nó encontrado tiver um filho nulo à direita, copiamos um NULO
        // para ser usado nas próximas funções
        if(minimo->dir == NULL) {
            Node* novo_nulo = malloc(sizeof(Node));
            memcpy(novo_nulo, NULO, sizeof(Node));
            minimo->dir = novo_nulo;
            novo_nulo->pai = minimo;
        }
        x = minimo->dir;
        cor_original = minimo->cor;

        // Substituimos o nó mínimo por seu filho direito...
        if(minimo->pai == no) {
            x->pai = minimo;
        } else {
            substituir(minimo, x);
            minimo->dir = no->dir;
            no->dir->pai = minimo;
        }

        // e substituimos o nó a ser apagado pelo nó mínimo.
        substituir(no, minimo);
        minimo->esq = no->esq;
        minimo->esq->pai = minimo;
        minimo->cor = no->cor;
        free(no);
    }

    if(cor_original == PRETO) {
        escanear_violacao_apagar(x);
    }
}

void escanear_violacao_inserir(Node* no) {
    // Essa função deverá ser chamada após inserir na árvore.
    Node* Z = no;

    // Se o novo nó causar uma violação do tipo dois vermelhos consecutivos...
    while(Z != raiz && Z->pai != NULL && Z->pai->cor == VERMELHO) {
        Node* pai = Z->pai;
        Node* avo = pai->pai;
        if(pai == avo->esq) {
            Node* tio = avo->dir;
            // ... e o tio for vermelho, trocamos as cores do pai, avô e tio
            if(tio != NULL && tio->cor == VERMELHO) {
                pai->cor = PRETO;
                avo->cor = VERMELHO;
                tio->cor = PRETO;
                Z = avo;
            } else {
                // ... e o tio for preto, efetuamos uma rotação no pai do novo
                // nó e trocamos as cores de modo que o pai seja preto e o avô
                // seja vermelho
                if(Z == pai->dir) {
                    // Se o novo nó estiver no lado direito da subárvore
                    // esquerda (ou seja, o nó está a direita e o pai está a
                    // esquerda), efetuamos uma rotação para a esquerda primeiro
                    Z = pai;
                    Z->pai->esq = rot_esquerda(Z);  // nova subárvore
                    pai = Z->pai;
                    avo = pai->pai;
                }
                pai->cor = PRETO;
                avo->cor = VERMELHO;
                if(avo->pai != NULL) {
                    if(avo->pai->dir == avo) avo->pai->dir = rot_direita(avo);
                    else if(avo->pai->esq == avo) avo->pai->esq = rot_direita(avo);
                    else raise(SIGINT);
                } else if(avo == raiz) {
                    raiz = rot_direita(avo);
                } else {
                    // Se o avô não tem pai e não é a raíz, tem algo de errado!
                    raise(SIGINT);
                }
            }
        } else {
            // Mesmas etapas, com as direções invertidas
            // O pai está a direita do avô
            Node* tio = avo->esq;
            if(tio != NULL && tio->cor == VERMELHO) {
                pai->cor = PRETO;
                avo->cor = VERMELHO;
                tio->cor = PRETO;
                Z = avo;
            } else {
                if(Z == pai->esq) {
                    Z = pai;
                    Z->pai->dir = rot_direita(Z);  // nova subárvore
                    pai = Z->pai;
                    avo = pai->pai;
                }
                pai->cor = PRETO;
                avo->cor = VERMELHO;
                if(avo->pai != NULL) {
                    if(avo->pai->esq == avo) avo->pai->esq = rot_esquerda(avo);
                    else if(avo->pai->dir == avo) avo->pai->dir = rot_esquerda(avo);
                    else raise(SIGABRT);
                } else if(avo == raiz) {
                    raiz = rot_esquerda(avo);
                } else {
                    // Se o avô não tem pai e não é a raíz, tem algo de errado!
                    raise(SIGABRT);
                }
            }
        }
    }
    raiz->cor = PRETO;
}

void liberar(Node* raiz) {
    // liberamos recursivamente os nós como se estivessemos printando em
    // pós-ordem
    if(raiz->esq != NULL) liberar(raiz->esq);
    if(raiz->dir != NULL) liberar(raiz->dir);
    free(raiz);
}

void menu_percorrer_arvore(Node* raiz) {
    while(1) {
        int temp;
        printf("1 - Pré-ordem\n");
        printf("2 - Em ordem\n");
        printf("3 - Pós-ordem\n");

        printf("Sua escolha: ");
        scanf("%d", &temp);

        switch(temp) {
            case 1:
                printf("Visualização em pré-ordem: ");
                print_preordem(raiz);
                printf("\n");
                return;
            case 2:
                printf("Visualização em ordem: ");
                print_ordem(raiz);
                printf("\n");
                return;
            case 3:
                printf("Visualização em pós-ordem: ");
                print_posordem(raiz);
                printf("\n");
                return;
        }
    }
}

int main() {
    NULO = mknode(-999, NULL);
    NULO->cor = PRETO;
    
    raiz = mknode(-1, NULL);
    raiz->cor = PRETO;
    printf("Insira o valor da raíz: ");
    scanf("%d", &raiz->valor);

    while(1) {
        int temp;
        printf("--------------------------\n");
        printf("1 - Inserir valor\n");
        printf("2 - Buscar valor\n");
        printf("3 - Remover valor\n");
        printf("4 - Percorrer árvore\n");
        printf("5 - Girar nó\n");
        printf("0 - Sair\n");

        printf("Sua escolha: ");
        scanf("%d", &temp);
        printf("--------------------------\n");

        switch(temp) {
            case 1:
                printf("Digite o valor (valores repetidos não serão inseridos): ");
                scanf("%d", &temp);
                Node* Z = inserir(raiz, temp);
                escanear_violacao_inserir(Z);
                break;
            case 2:
                printf("Digite o valor para buscar: ");
                scanf("%d", &temp);
                Node* no_busca = buscar(raiz, temp);
                if(no_busca != NULL) {
                    printf("O valor %d está na árvore!\n", temp);
                }
                else {
                    printf("O valor %d NÃO está na árvore!\n", temp);
                }
                break;
            case 3:
                printf("Digite o valor para remover: ");
                scanf("%d", &temp);
                Node* no_apagar = buscar(raiz, temp);
                if(no_apagar != NULL) {
                    apagar(no_apagar);
                } else {
                    printf("Este valor não está na árvore!\n");
                }
                break;
            case 4:
                menu_percorrer_arvore(raiz);
                break;
            case 0:
                liberar(raiz);
                exit(0);
                break;
        }
    }
    return 0;
}