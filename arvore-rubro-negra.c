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

#define PRETO 0
#define VERMELHO 1

typedef struct Node {
    int valor;
    struct Node *dir, *esq, *pai; 
    int cor;  // 0: preto, 1: vermelho
} Node;

Node* raiz;
Node* N_NULO; // valor temporário para simplificar o código de apagar

Node* mknode(int valor, Node* pai) {
    // função para criar nós sem precisar repetir o código de inicialização
    Node* novo_no = malloc(sizeof(Node));
    novo_no->dir = N_NULO;
    novo_no->esq = N_NULO;
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
        if(raiz->dir != N_NULO) return inserir(raiz->dir, valor);
        
        // novo nó tem um valor maior que o pai -> novo nó vai pra direita
        Node* n = mknode(valor, raiz);
        raiz->dir = n;
        return n;
    } else if(valor < raiz->valor) {
        // verificar se a esquerda já está ocupada
        if(raiz->esq != N_NULO) return inserir(raiz->esq, valor);

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
    } else if(valor > raiz->valor && raiz->dir != N_NULO) {
        return buscar(raiz->dir, valor);
    } else if(valor < raiz->valor && raiz->esq != N_NULO) {
        return buscar(raiz->esq, valor);
    }
    return NULL;
}

void print_preordem(Node* raiz) {
    // ordem: raíz, esquerda, direita
    printf("(%d)%d ", raiz->cor, raiz->valor);
    if(raiz->esq != N_NULO) print_preordem(raiz->esq);
    if(raiz->dir != N_NULO) print_preordem(raiz->dir);
}

void print_ordem(Node* raiz) {
    // ordem: esquerda, raíz, direita
    if(raiz->esq != N_NULO) print_ordem(raiz->esq);
    printf("(%d)%d ", raiz->cor, raiz->valor);
    if(raiz->dir != N_NULO) print_ordem(raiz->dir);
}

void print_posordem(Node* raiz) {
    // ordem: esquerda, direita, raíz
    if(raiz->esq != N_NULO) print_posordem(raiz->esq);
    if(raiz->dir != N_NULO) print_posordem(raiz->dir);
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

void seg_rot_direita(Node* no) {
    // versão da rotação à direita que lida com a conexão ao pai
    if(no->pai != N_NULO) {
        if(no->pai->esq == no) no->pai->esq = rot_direita(no);
        else no->pai->dir = rot_direita(no);
    } else if(no == raiz) {
        raiz = rot_direita(no);
    } else {
        printf("ERRO(seg_rot_direita): Pai nulo mas não é raiz??");
        abort();
    }
}

void seg_rot_esquerda(Node* no) {
    // versão da rotação à esquerda que lida com a conexão ao pai
    if(no->pai != N_NULO && no->pai != NULL) {
        if(no->pai->esq == no) no->pai->esq = rot_esquerda(no);
        else no->pai->dir = rot_esquerda(no);
    } else if(no == raiz) {
        raiz = rot_esquerda(no);
    } else {
        printf("ERRO(seg_rot_esquerda): Pai nulo mas não é raiz??");
        abort();
    }
}

void substituir(Node* u, Node* v) {
    // Função que substitui u por v na árvore
    if(u->pai == N_NULO) {
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
        // Se o nó atual estiver à esquerda de seu pai, e...
        if(x == x->pai->esq) {
            Node* irmao = x->pai->dir;

            // ... o irmão do nó substituto é vermelho...
            if(irmao->cor == VERMELHO) {
                // ... pintamos o irmão de preto, o pai de vermelho e giramos
                // para a direita no pai
                irmao->cor = PRETO;
                x->pai->cor = VERMELHO;
                seg_rot_esquerda(x->pai);
                irmao = x->pai->dir;
            }

            // ... o irmão e seus filhos são pretos...
            if(irmao->esq->cor == PRETO && irmao->dir->cor == PRETO) {
                // ... pintamos o irmão de vermelho e mudamos x para o pai
                irmao->cor = VERMELHO;
                x = x->pai;
            }

            // ... o irmão e seu filho direito são pretos, mas o filho esquerdo é vermelho...
            if(irmao->esq->cor == VERMELHO && irmao->dir->cor == PRETO) {
                // ... pintamos o filho esquerdo de preto, o irmão de vermelho e
                // giramos para a direita no irmão
                irmao->esq->cor = PRETO;
                irmao->cor = VERMELHO;
                seg_rot_direita(irmao);
                //irmao = x->pai->dir;
                irmao = irmao->pai;
            }

            // ... o filho direito do irmão é vermelho...
            if(irmao->dir->cor == VERMELHO) {
                // ... pintamos o irmão da mesma cor do pai, o pai de preto,
                // o filho direito do irmão de preto, e giramos para a esquerda
                // no pai
                irmao->cor = irmao->pai->cor;
                irmao->pai->cor = PRETO;
                irmao->dir->cor = PRETO;
                seg_rot_esquerda(irmao->pai);
                x = raiz;
            }
        } else {  // Se o nó atual estiver à direita de seu pai, e...
            Node* irmao = x->pai->esq;

            // ... o irmão do nó substituto é vermelho...
            if(irmao->cor == VERMELHO) {
                // ... pintamos o irmão de preto, o pai de vermelho e giramos
                // para a direita no pai
                irmao->cor = PRETO;
                x->pai->cor = VERMELHO;
                seg_rot_direita(x->pai);
                irmao = x->pai->esq;
            }

            // ... o irmão e seus filhos são pretos...
            if(irmao->esq->cor == PRETO && irmao->dir->cor == PRETO) {
                // ... pintamos o irmão de vermelho e mudamos x para o pai
                irmao->cor = VERMELHO;
                x = x->pai;
            }

            // ... o irmão e seu filho esquerdo são pretos, mas o filho direito é vermelho...
            if(irmao->dir->cor == VERMELHO && irmao->esq->cor == PRETO) {
                // ... pintamos o filho direito de preto, o irmão de vermelho e
                // giramos para a esquerda no irmão
                irmao->dir->cor = PRETO;
                irmao->cor = VERMELHO;
                seg_rot_esquerda(irmao);
                //irmao = x->pai->dir;
                irmao = irmao->pai;
            }

            // ... o filho esquerdo do irmão é vermelho...
            if(irmao->esq->cor == VERMELHO) {
                // ... pintamos o irmão da mesma cor do pai, o pai de preto,
                // o filho esquerdo do irmão de preto, e giramos para a direita
                // no pai
                irmao->cor = irmao->pai->cor;
                irmao->pai->cor = PRETO;
                irmao->esq->cor = PRETO;
                seg_rot_direita(irmao->pai);
                x = raiz;
            }
        }
    }
    x->cor = PRETO;
    if(x == N_NULO) x->pai = NULL;
}

void apagar(Node* no) {
    Node* x;
    int cor_original;
    if(no->esq == N_NULO) {
        // Primeiro caso: esquerda nula
        x = no->dir;
        cor_original = no->cor;
        substituir(no, x);
        if(x != N_NULO) free(no);
    } else if(no->dir == N_NULO) {
        // Segundo caso: direita nula
        x = no->esq;
        cor_original = no->cor;
        substituir(no, x);
        if(x != N_NULO) free(no);
    } else {
        // Terceiro caso: nenhum lado nulo
        // Usamos o código de apagar da BST normal para encontrar o nó com o
        // menor valor da subárvore direita
        Node* minimo = N_NULO;
        Node* no_atual = no->dir;
        while(minimo == N_NULO) {
            if(no_atual->esq != N_NULO) {
                no_atual = no_atual->esq;
            } else {
                minimo = no_atual;
            }
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
        if(x != N_NULO) free(no);
    }

    if(cor_original == PRETO) {
        escanear_violacao_apagar(x);
    }
}

void escanear_violacao_inserir(Node* no) {
    // Essa função deverá ser chamada após inserir na árvore.
    Node* Z = no;

    // Se o novo nó causar uma violação do tipo dois vermelhos consecutivos...
    while(Z != raiz && Z->pai != N_NULO && Z->pai->cor == VERMELHO) {
        Node* pai = Z->pai;
        Node* avo = pai->pai;
        if(pai == avo->esq) {
            Node* tio = avo->dir;
            // ... e o tio for vermelho, trocamos as cores do pai, avô e tio
            if(tio->cor == VERMELHO) {
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
                seg_rot_direita(avo);
            }
        } else {
            // Mesmas etapas, com as direções invertidas
            // O pai está a direita do avô
            Node* tio = avo->esq;
            if(tio->cor == VERMELHO) {
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
                seg_rot_esquerda(avo);
            }
        }
    }
    raiz->cor = PRETO;
}

void liberar(Node* raiz) {
    // liberamos recursivamente os nós como se estivessemos printando em
    // pós-ordem
    if(raiz->esq != N_NULO) liberar(raiz->esq);
    if(raiz->dir != N_NULO) liberar(raiz->dir);
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
    N_NULO = mknode(-999, NULL);
    N_NULO->cor = PRETO;
    
    raiz = mknode(-1, N_NULO);
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