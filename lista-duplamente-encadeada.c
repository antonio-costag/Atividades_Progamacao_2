#include <stdio.h>
#include <stdlib.h>

/*
typedef -> cria um apelido
struct -> cria uma estrutura
estrutura_no_rotulo -> o rótulo dessa estrutura enquanto o apelido estrutura_no ainda não foi criado
estrutura_no -> o apelido dessa estrutura
*/
typedef struct estrutura_no_rotulo{
    struct estrutura_no_rotulo *endereco_posterior;
    struct estrutura_no_rotulo *endereco_anterior;
    int valor;
} estrutura_no;

// a variável global que representa a cabeça (head) da lista
estrutura_no *cabeca;

int contar_nos() {
    int contador = 0;
    estrutura_no *ponteiro_atual = cabeca;

    while(ponteiro_atual != NULL){
        ponteiro_atual = ponteiro_atual->endereco_posterior;
        contador++;
    }

    return contador;
}

// cria o primeiro nó, que serve como cabeça (cabeça é o primeiro elemento da lista)
void criar_primeiro_no(int valor) {

    // modo de se alocar espaço para um novo nó, seguindo a fórmula: 
    //      tipo_objeto *nome_objeto = (tipo_objeto*) malloc(sizeof(tipo_objeto))
    // o asterisco (*) aparece na primeira vez para declarar que o objeto é um ponteiro 
    // e na segunda vez para fazer o casting de um tipo de ponteiro
    cabeca = (estrutura_no*) malloc(sizeof(estrutura_no));

    // o nó começa com os endereços anteriores e posteriores nulos, recebendo somente o valor
    cabeca->endereco_posterior = NULL;
    cabeca->endereco_anterior = NULL;
    cabeca->valor = valor;
}

// insere um novo nó no início da lista, que serve como a nova cabeça
void inserir_inicio(int valor) {

    // cria esse novo nó
    estrutura_no *novo_no = (estrutura_no*) malloc(sizeof(estrutura_no));

    // nem eu sei o que eu fiz aqui, mas tá funcionando (eu acho)
    novo_no->endereco_posterior = cabeca;
    cabeca->endereco_anterior = novo_no;
    novo_no->endereco_anterior = NULL;
    novo_no->valor = valor;
    cabeca = novo_no;
}

// insere um novo nó no meio de outros nós, não insere nem no início e nem no final
void inserir_meio(int valor, int posicao) {

    // ponteiro descartável que começa com o valor da cabeça. é usado para navegar pela
    // lista sem mudar o valor da cabeça (é usado em várias funções)
    estrutura_no *ponteiro_atual = cabeca;

    // posicao é a variável que determina em qual posicao da lista o novo nó irá ficar; 
    // considera-se a primeira posição como 1 ao invés de 0 (classroom que pediu)
    // se a posição for 1 (inicio do nó) ou menor (nem sequer existe na lista) a função 
    // retorna sem inserir nada
    if(posicao <= 1){
        return;
    }

    // a variável posicao vai ser decrementada aqui, o loop é executado enquanto ela for
    // maior que 1 (enquanto ainda não se chegou na posição de inserção)
    while(posicao > 1){
        // se o ponteiro atual não for a cauda da lista, o loop continua, caso contrário, 
        // a função retorna sem inserir nada (ela não insere no final da lista)
        if(ponteiro_atual->endereco_posterior != NULL){
            ponteiro_atual = ponteiro_atual->endereco_posterior;
            posicao--;
        } else {
            return;
        }
    }

    estrutura_no *novo_no = (estrutura_no*) malloc(sizeof(estrutura_no));

    // é a inserção mais difícil de entender por ser a com mais mudanças nos ponteiros
    // anteriores e posteriores, mas faz um esforço que tu entende (ou faz o L)
    ponteiro_atual->endereco_anterior->endereco_posterior = novo_no;
    novo_no->endereco_anterior = ponteiro_atual->endereco_anterior;
    novo_no->endereco_posterior = ponteiro_atual;
    ponteiro_atual->endereco_anterior = novo_no;
    novo_no->valor = valor;
}

// insere um novo nó no final da lista, que serve como cauda (cauda é o último elemento da lista)
void inserir_final(int valor) {

    estrutura_no *ponteiro_atual = cabeca;

    // o loop continua até que o ponteiro atual seja a cauda da lista
    while(ponteiro_atual->endereco_posterior != NULL){
        ponteiro_atual = ponteiro_atual->endereco_posterior;
    }

    estrutura_no *novo_no = (estrutura_no*) malloc(sizeof(estrutura_no));
    ponteiro_atual->endereco_posterior = novo_no;
    novo_no->endereco_anterior = ponteiro_atual;
    novo_no->endereco_posterior = NULL;
    novo_no->valor = valor;
}

// remove qualquer nó que não seja a cabeça
void remover_no(int posicao) {

    estrutura_no *ponteiro_atual = cabeca;

    if(posicao <= 1){
        return;
    }

    while(posicao > 1){
        if(ponteiro_atual->endereco_posterior != NULL){
            ponteiro_atual = ponteiro_atual->endereco_posterior;
            posicao--;
        } else {
            return;
        }
    }

    ponteiro_atual->endereco_anterior->endereco_posterior = ponteiro_atual->endereco_posterior;
    // esse if é necessário para quando se vai remover o último nó da lista
    if(ponteiro_atual->endereco_posterior != NULL){
        ponteiro_atual->endereco_posterior->endereco_anterior = ponteiro_atual->endereco_anterior;
    }

    // agora que o nó foi removido, free() é chamado para liberar esse espaço de memória
    // que antes estava sendo ocupado (essa função é chamada mais vezes)
    free(ponteiro_atual);
}

// remove apenas o nó cabeça
void remover_no_cabeca() {

    estrutura_no *ponteiro_lixo = cabeca;

    cabeca->endereco_posterior->endereco_anterior = NULL;
    cabeca = cabeca->endereco_posterior;

    free(ponteiro_lixo);
}

// busca e exibe qual o primeiro nó que possui o valor desejado pelo usuário 
// (ou se nenhum nó possui esse valor)
void buscar(int objetivo) {

    // contador que começa em 1 para caso o objetivo (valor que queremos encontrar) 
    // esteja logo no primeiro nó
    int contador = 1;
    estrutura_no *ponteiro_atual = cabeca;

    while(ponteiro_atual != NULL && ponteiro_atual->valor != objetivo){
        if(ponteiro_atual->endereco_posterior != NULL){
            ponteiro_atual = ponteiro_atual->endereco_posterior;
            contador++;
        } else {
            break;
        }
    }

    if(ponteiro_atual->valor == objetivo){
        printf("O valor %d pode ser encontrado no nó de número %d!\n", objetivo, contador);
    } else {
        printf("O valor %d não pôde ser encontrado dentre os nós :/\n", objetivo);
    }
}

// listamos os nós: número do nó, endereço do nó, endereço anterior ao nó, 
// endereço posterior ao nó e valor do nó
void listar() {

    int contador = 1;
    estrutura_no *ponteiro_atual = cabeca;
    
    while(ponteiro_atual != NULL){
        printf("-----Nó %d-----\nEndereço: %p\tEnd.Ant: %p\tEnd.Pos: %p\tValor: %d\n\n", 
            contador, ponteiro_atual, ponteiro_atual->endereco_anterior, ponteiro_atual->endereco_posterior, ponteiro_atual->valor);
        contador++;
        ponteiro_atual = ponteiro_atual->endereco_posterior;
    }
}

// Função wrapper usada para interagir com o usuário e chamar outras funções
// que efetivamente fazem coisas (modo simples do usuário interagir com o programa)
void insercao_no() {
    
    int tamanho_lista = contar_nos();
    int valor;
    int posicao;

    printf("Insira qual valor você deseja que o novo nó tenha: ");scanf("%d", &valor);
    do {
        printf("Insira em qual posição da lista você deseja inserir um nó (Insira uma posição que exista na lista ou a posição imediatamente posterior à última. A lista começa por 1): ");
        scanf("%d", &posicao);
    } while(posicao < 1 || posicao > tamanho_lista+1);

    if(posicao == 1){
        inserir_inicio(valor);
    } else if(posicao == tamanho_lista+1){
        inserir_final(valor);
    } else {
        inserir_meio(valor, posicao);
    }
}

// outra função wrapper, mas para remoção de nós ao invés de inserção
void remocao_no() {

    int tamanho_lista = contar_nos();
    int posicao;
    int entrada;

    do {
        printf("Insira a posição do nó que você deseja remover (Insira uma posição que exista na lista. A lista começa por 1): ");
        scanf("%d", &posicao);
    } while(posicao < 1 || posicao > tamanho_lista);

    if(posicao == 1 && tamanho_lista > 1){
        remover_no_cabeca();
    } else if(tamanho_lista == 1){
        printf("Você está sem nós! Crie um para continuar. Insira seu valor (CTRL+C PARA SAIR): ");
        scanf("%d", &entrada);
        free(cabeca);
        criar_primeiro_no(entrada);
    } else {
        remover_no(posicao);
    }
}

// onde toda a mágica acontece.
int main() {
    int entrada;

    printf("Bem-vindo! Crie seu primeiro nó dando um valor inteiro a ele (CTRL+C PARA SAIR): ");
    scanf("%d", &entrada);
    criar_primeiro_no(entrada);
    
    printf("Agora com seu primeiro nó, diga o que quer fazer!\n");

    // esse loop se repete até que o usuário decida deixar o programa
    while(1){
        printf("[1] Inserir nó\n");
        printf("[2] Remover nó\n");
        printf("[3] Buscar primeira aparição de um valor em um nó\n");
        printf("[4] Listar nós\n");
        printf("[5] Sair\n");
        printf("Resposta: ");
        scanf("%d", &entrada);
        
        switch(entrada){
            case 1: insercao_no(); break;
            case 2: remocao_no(); break;
            case 3: 
                printf("Insira o valor que você deseja buscar: ");
                scanf("%d", &entrada); 
                buscar(entrada); 
                break;
            case 4: listar(); break;
            case 5: exit(0); break;
            default: break;
        }
    }

    return 0;
}