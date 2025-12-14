#include <stdio.h>
#include <stdlib.h>

typedef struct Node{
    int valor;
    struct Node *dir, *esq;
} Node;

Node* mknode(int valor) {
    // função para criar nós sem precisar repetir o código de inicialização
    Node* novo_no = malloc(sizeof(Node));
    novo_no->dir = NULL;
    novo_no->esq = NULL;
    novo_no->valor = valor;
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
        Node* n = mknode(valor);
        raiz->dir = n;
        return n;
    } else if(valor < raiz->valor) {
        // verificar se a esquerda já está ocupada
        if(raiz->esq != NULL) return inserir(raiz->esq, valor);

        // novo nó tem um valor menor que o pai -> novo nó vai pra esquerda
        Node* n = mknode(valor);
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

Node* buscar_pai(Node* raiz, Node* no) {
    // função que busca o pai de um nó, usado nas funções de remoção
    if(raiz->esq == no || raiz->dir == no) {
        return raiz;
    } else if(no->valor > raiz->valor && raiz->dir != NULL) {
        return buscar_pai(raiz->dir, no);
    } else if(no->valor < raiz->valor && raiz->esq != NULL) {
        return buscar_pai(raiz->esq, no);
    }
    return NULL;
}

void print_preordem(Node* raiz) {
    // ordem: raíz, esquerda, direita
    printf("%d ", raiz->valor);
    if(raiz->esq != NULL) print_preordem(raiz->esq);
    if(raiz->dir != NULL) print_preordem(raiz->dir);
}

void print_ordem(Node* raiz) {
    // ordem: esquerda, raíz, direita
    if(raiz->esq != NULL) print_ordem(raiz->esq);
    printf("%d ", raiz->valor);
    if(raiz->dir != NULL) print_ordem(raiz->dir);
}

void print_posordem(Node* raiz) {
    // ordem: esquerda, direita, raíz
    if(raiz->esq != NULL) print_posordem(raiz->esq);
    if(raiz->dir != NULL) print_posordem(raiz->dir);
    printf("%d ", raiz->valor);
}

void apagar(Node* raiz, Node* no);

void apagar_folha(Node* raiz, Node* no) {
    // podemos simplesmente chamar free() no nó, mas o pai iria apontar para
    // memória liberada (lixo), portanto precisamos percorrer a árvore inteira
    // e editar o pai para definir o ponteiro do filho para NULL.
    Node* no_pai = buscar_pai(raiz, no);
    if(no_pai != NULL) {
        if(no_pai->esq == no) {
            no_pai->esq = NULL;
        } else if(no_pai->dir == no) {
            no_pai->dir = NULL;
        }
    }

    free(no);
    return;
}

void apagar_um_filho(Node* raiz, Node* no) {
    // para apagar um nó com um filho, basta substituir o valor dele para o
    // valor do filho
    // aqui copiamos os valores do filho para o nó a ser apagado e apagamos
    // o filho. com dados mais complexos, o ideal seria alterar o ponteiro
    // do pai, mas esse é o método mais simples de entender.
    if(no->esq == NULL && no->dir != NULL) { // filho na direita
        Node* filho = no->dir;
        no->valor = filho->valor;
        no->esq = filho->esq;
        no->dir = filho->dir;
        free(filho);  // apagamos o filho para não termos valores repetidos
    } else if(no->dir == NULL && no->esq != NULL) { // filho na esquerda
        Node* filho = no->esq;
        no->valor = filho->valor;
        no->esq = filho->esq;
        no->dir = filho->dir;
        free(filho);
    }
}

void apagar_dois_filhos(Node* raiz, Node* no) {
    // para apagar um nó com dois filhos, olhamos para a direita do nó e
    // substituimos o nó a ser apagado com o nó que está mais a esquerda
    Node* no_esquerda = NULL;
    Node* no_atual = raiz->dir;
    while(no_esquerda == NULL) {
        if(no_atual->esq != NULL) {
            no_atual = no_atual->esq;
        } else {
            no_esquerda = no_atual;
        }
    }
    no->valor = no_esquerda->valor;
    apagar(raiz, no_esquerda);
}

void apagar(Node* raiz, Node* no) {
    // prints de depuração :)
    if(no->esq == NULL && no->dir == NULL) {
        //printf("Apagando nó folha\n");
        apagar_folha(raiz, no);
    } else if((no->esq != NULL) ^ (no->dir != NULL)) { // XOR: ou esquerda ou direita, mas não os dois
        //printf("Apagando nó com um filho\n");
        apagar_um_filho(raiz, no);
    } else if((no->esq != NULL) && (no->dir != NULL)) {
        //printf("Apagando nó com dois filhos\n");
        apagar_dois_filhos(raiz, no);
    }
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

void menu_rotacao_arvore(Node* raiz) {
     while(1) {
        int direcao, valor_no;
        printf("1 - Para a esquerda\n");
        printf("2 - Para a direita\n");

        printf("Sua escolha: ");
        scanf("%d", &direcao);

        switch(direcao) {
            case 1:
                printf("Valor do nó: ");
                scanf("%d", &valor_no);
                Node* no_busca = buscar(raiz, valor_no);
                if(no_busca == NULL) {
                    printf("O valor %d não está na árvore!\n", valor_no);
                    continue;
                }

                Node* pai = buscar_pai(raiz, no_busca);

                Node* nova_raiz = rot_esquerda(no_busca);
                if(pai != NULL) {
                    if(pai->dir == no_busca) pai->dir = nova_raiz;
                    else if(pai->esq == no_busca) pai->esq = nova_raiz;
                }
                return;
            case 2:
                printf("Valor do nó: ");
                scanf("%d", &valor_no);
                Node* no_busca2 = buscar(raiz, valor_no);
                if(no_busca2 == NULL) {
                    printf("O valor %d não está na árvore!\n", valor_no);
                    continue;
                }

                Node* pai2 = buscar_pai(raiz, no_busca2);

                Node* nova_raiz2 = rot_direita(no_busca2);
                if(pai2 != NULL) {
                    if(pai2->dir == no_busca2) pai2->dir = nova_raiz2;
                    else if(pai2->esq == no_busca2) pai2->esq = nova_raiz2;
                }
                return;
        }
    }
}

int main() {
    Node* raiz = mknode(-1);
    printf("Insira o valor da raíz: ");
    scanf("%d", &raiz->valor);

    while(1) {
        int temp;
        printf("--------------------------\n");
        printf("1 - Inserir valor\n");
        printf("2 - Buscar valor\n");
        printf("3 - Remover valor\n");
        printf("4 - Percorrer árvore\n");
        printf("56 - Girar nó\n");
        printf("0 - Sair\n");

        printf("Sua escolha: ");
        scanf("%d", &temp);
        printf("--------------------------\n");

        switch(temp) {
            case 1:
                printf("Digite o valor (valores repetidos não serão inseridos): ");
                scanf("%d", &temp);
                inserir(raiz, temp);
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
                    apagar(raiz, no_apagar);
                } else {
                    printf("Este valor não está na árvore!\n");
                }
                break;
            case 4:
                menu_percorrer_arvore(raiz);
                break;
            case 5:
                int valor_no, direcao;
                printf("Digite o valor para girar: ");
                scanf("%d", &valor_no);

                printf("1 - Para a esquerda\n");
                printf("2 - Para a direita\n");

                printf("Sua escolha: ");
                scanf("%d", &direcao);

                Node* no_girar = buscar(raiz, valor_no);
                if(no_girar == NULL) {
                    printf("O valor %d NÃO está na árvore!\n", valor_no);
                }

                Node* no_girar_pai = buscar_pai(raiz, no_girar);
                Node* nova_raiz;

                if(direcao == 1) {
                    nova_raiz = rot_esquerda(no_girar);
                } else if(direcao == 2) {
                    nova_raiz = rot_direita(no_girar);
                } else {
                    printf("Direção inválida\n");
                    break;
                }

                if(no_girar_pai == NULL) {
                    // Não há pai -> nó é a raíz
                    raiz = nova_raiz;
                } else {
                    if(no_girar_pai->dir == no_girar) no_girar_pai->dir = nova_raiz;
                    else if(no_girar_pai->esq == no_girar) no_girar_pai->esq = nova_raiz;
                }
                break;
            case 0:
                liberar(raiz);
                exit(0);
                break;
        }
    }
    return 0;
}