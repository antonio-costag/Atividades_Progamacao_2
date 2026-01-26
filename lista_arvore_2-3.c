#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//definindo a estrutura dos meus nós
typedef struct no
{
    int valor[3]; // usando um vetor para facilitar a ordenação dos valore, espaço extra pra faculitar o "overflow"
    int numero_valores; // Diz se tem 1 ou 2 valores (ou 3 temporariamente)
    struct no *folha[4]; //ponteiros para os fihos
} No;

No *cabeca;
 
//aqui eu coloco lixo "-1" nos valores no meu nó pra preencher espaço
void IniciarValoresNo(No *atual_no){
    for(int i = 0; i < 3; i++){
        atual_no->valor[i] = -1;
    }
}

//iniciação basica dos ponteiros do meu no
void IniciarFolhas(No *atual_no){
    for(int i = 0; i < 4; i++){
        atual_no->folha[i] = NULL;
    }
}

//essa função garante que se meus valores estão como [30, 10], eles passem a ser [10, 30]
//isso é vital para facilitar a organização da minha arvore
void OrdenarNo(No *atual_no){
    // Usamos numero_valores para ordenar apenas o que existe de fato
    // Garante que não estamos comparando com o lixo "-1" das posições vazias
    //(o lixo sempre fica no fical e a ordenação não chega neles)
    for(int i = 0; i < atual_no->numero_valores - 1; i++){
        for(int j = i + 1; j < atual_no->numero_valores; j++){
            if(atual_no->valor[j] < atual_no->valor[i]){
                int valor_temp = atual_no->valor[i];

                atual_no->valor[i] = atual_no->valor[j];
                atual_no->valor[j] = valor_temp;
            }
        }
    }
}

//Se o primeiro ponteiro for nulo, todos são
//a Arvore 2-3 sempre vai ser perfeitamente balaceda em altura
bool EhFolha(No* atual_no) {
    if(atual_no->folha[0] == NULL){
        return true;
    }
    else{
        return false;
    }
}

//estrutura basica pra criação de um nó
No* CriarNo(int valor){
    No* novo_no = malloc(sizeof(No));

    IniciarValoresNo(novo_no);
    IniciarFolhas(novo_no);

    novo_no->valor[0] = valor;
    novo_no->numero_valores = 1;

    return novo_no;
}

/*
     [ 10 , 20 , 30 ]  <-- atual_no
     /    |    |    \
   (A)   (B)  (C)   (D)
    ^     ^    ^     ^
   f[0] f[1] f[2]  f[3]
*/
No* DividirNo(No *atual_no, int* valor_promovido){
    //aqui eu crio e inicio o nó da direita
    No* direita = malloc(sizeof(No));
    IniciarFolhas(direita);

    //nesse ponto eu salvo o nó do meio pra subir ele na estrutura
    *valor_promovido = atual_no->valor[1];

    //o nó da direita vai ficar com o maior valor
    //[0][1][2]
    direita->valor[0] = atual_no->valor[2];
    direita->numero_valores = 1;

    /*
          [ 10 ] ...sobe 20... [ 30 ] <-- direita
         /    |                 /    \
        (A)   (B)              (C)    (D)
    */

    //se não for folha, metade dos filhos do atual_no passa para o novo nó
    if(!EhFolha(atual_no)){
        direita->folha[0] = atual_no->folha[2]; //menor valor a esqueda
        direita->folha[1] = atual_no->folha[3]; //maior valor a direira

        //O nó atual esquece que teve esses filhos (limpa os ponteiros)
        atual_no->folha[2] = NULL;
        atual_no->folha[3] = NULL;
    }

    //so resta um valor aqui e o resto vira lixo
    atual_no->valor[1] = -1;
    atual_no->valor[2] = -1;

    atual_no->numero_valores = 1;

    //esse atual_no é reciclado e usamos a sua estrutura para virar o no da esquerda
    //então retornamos o no da direita
    return direita;
}

No* Inserir(No* atual_no, int valor, int* valor_promovido){
    //verificando se chegamos em uma folha
    if(EhFolha(atual_no)){

        //antes de inserir o nosso valor
        //varremos o vetor buscando duplicidades 
        for (int j = 0; j < atual_no->numero_valores; j++) 
        {
            if (valor == atual_no->valor[j]){
                //se for achado, cancelamos o procedimento
                return NULL;
            }
        }

        //aqui eu vou insarir o valor na minha primeira casa com lixo
        //numero_valores = 1;
        //  0   1   2
        //[10][-1][-1] ...
        //[10][valor][-1]
        atual_no->valor[atual_no->numero_valores] = valor;
        //valor incrementado no vetor
        atual_no->numero_valores++;
        //agoran organizamos esse nó
        OrdenarNo(atual_no);

        //aqui a gente verifica se a folha ja tem 3 valores
        //se não, o processo é interropido
        if(atual_no->numero_valores < 3){
            return NULL;
        }
        //se tiver a gente então segue com o processedimentom de dividir o no
        else{
            return DividirNo(atual_no, valor_promovido);
        }
    }


    //com esse loop vamos percorrer os valores do meu nó e buscar a folha correta pra enserir o valor
    //Ex:
    // i = 0, numero_valores = 2, valor = 25;
    // [10] [30]
    //|    |    |
    //0    1    2 continua...

    // i = 1, numero_valores = 2;
    // [10] [30]
    //|    |    |
    //0    1    2 parou. 25 é menor que 30

    // i = 2, numero_valores = 2;
    // [10] [30]
    //|    |    |
    //0   [25]    2 então ele fica entre o 10 e o 30
    int i = 0;
    while (i < atual_no->numero_valores && valor > atual_no->valor[i])
    {
        i++;
    }

    //verificaçãopra lidar com valores 
    if (i < atual_no->numero_valores && valor == atual_no->valor[i]) {
        return NULL; 
    }

    //é aqui que vamos guardar o valor que vai subir
    int promovido_filho;

    //nesse a função pausa e desce um nivel
    //se for nulo, o filho resolve tudo sem precisar dividir
    No*novo_filho_direito = Inserir(atual_no->folha[i], valor, &promovido_filho);

    //se for um ponteiro, o filho explodiu
    if(novo_filho_direito != NULL){
        //ele retorn o valor da direita e coloco o filho que subiu de nivel aqui no pai
        atual_no->valor[atual_no->numero_valores] = promovido_filho;
        atual_no->numero_valores++;

        //e eu ordeno esses valores para que não aja problemas
        OrdenarNo(atual_no);

        //aqui eu empurro os valores das folhas pro lado
        for(int j = 3; j > i + 1; j--){
            atual_no->folha[j] = atual_no->folha[j - 1];
        }

        //pra colocar meu filho no lugar correto pra ele
        atual_no->folha[i + 1] = novo_filho_direito;

        //se o pai tem menos de 2 valores, nada precisa ser feito e vida que segue
        if(atual_no->numero_valores < 3){
            return NULL;    
        }

        //se estourou o pai, a gente vai ter que resolver isso
        return DividirNo(atual_no, valor_promovido);
    }

    return NULL;
}

void Adicionar(int valor){
    // se a arvore não existe a gente ccria ela
    if(cabeca == NULL){
        cabeca = CriarNo(valor);
    }

    //
    else{
        //aqui prosseguimos se a raiz ja existe
        int valor_promovido;
        No* novo_no = Inserir(cabeca, valor, &valor_promovido);

        //a própria raiz atual transbordou
        //então criamos uma nova raiz acima da antiga.
        if(novo_no != NULL){
            //Salvamos a raiz atual
            No* antiga_raiz = cabeca;

            //Criamos uma nova raiz com o valor que subiu (o meio)
            cabeca = CriarNo(valor_promovido);

            //Conectamos os filhos da nova raiz
            cabeca->folha[0] = antiga_raiz;
            cabeca->folha[1] = novo_no;
        }
    }
}
void Imprimir(No* n) {
    if (n == NULL){
        return;
    }
    printf("[ ");
    for(int i=0; i < n->numero_valores; i++){
        printf("%d ", n->valor[i]);
    }
    printf("] ");
}
int main(){
    int valores[] = {10, 20, 30, 40, 50, 60, 70};
    for(int i=0; i<7; i++) Adicionar(valores[i]);

    printf("Estrutura da Raiz: ");
    Imprimir(cabeca);
    
    printf("\nFilhos da Raiz: ");
    Imprimir(cabeca->folha[0]);
    Imprimir(cabeca->folha[1]);
    
    printf("\nNetos do 60: ");
    Imprimir(cabeca->folha[1]->folha[0]);
    Imprimir(cabeca->folha[1]->folha[1]);
    
    return 0;
}