#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Definindo a estrutura dos meus nós
typedef struct no
{
    int valor[3]; // Usando um vetor para facilitar a ordenação dos valores, espaço extra pra facilitar o "overflow"
    int numero_valores; // Diz se tem 1 ou 2 valores (ou 3 temporariamente)
    struct no *filho[4]; // Ponteiros para os filhos (com espaço para um quarto filho temporário)
} No;

No *raiz;

// Se o primeiro ponteiro for nulo, todos são
// A Árvore 2-3 sempre vai ser perfeitamente balaceda em altura
bool EhFolha(No* atual_no) {
    if(atual_no->filho[0] == NULL){
        return true;
    }
    else{
        return false;
    }
}

// Estrutura basica pra criação de um nó
No* CriarNo(int valor){
    No* novo_no = calloc(1, sizeof(No)); // calloc() é usado para já inicializar o nó como NULL

    novo_no->valor[0] = valor;
    novo_no->numero_valores = 1;

    return novo_no;
}

No* DividirNo(No *atual_no, int* valor_promovido){
    // Aqui o nó da direita é criado e inicializado como NULL
    No* direita = calloc(1, sizeof(No));

    // Nesse ponto o nó do meio é salvo pra subir ele na estrutura (se tornar o novo pai)
    *valor_promovido = atual_no->valor[1];

    // O nó da direita vai ficar com o maior valor
    direita->valor[0] = atual_no->valor[2];
    direita->numero_valores = 1;

    // Se não for folha, metade dos filhos do atual_no passa para o novo nó
    if(!EhFolha(atual_no)){
        direita->filho[0] = atual_no->filho[2]; // Menor valor a esqueda
        direita->filho[1] = atual_no->filho[3]; // Maior valor a direira

        // O nó atual esquece que teve esses filhos (limpa os ponteiros)
        atual_no->filho[2] = NULL;
        atual_no->filho[3] = NULL;
    }

    atual_no->numero_valores = 1;

    // Esse atual_no é reciclado e usamos a sua estrutura para virar o nó da esquerda
    // Então retornamos o nó da direita
    return direita;
}

No* Inserir(No* atual_no, int valor, int* valor_promovido){
    // Verificando se chegamos em uma folha
    if(EhFolha(atual_no)){

        // Antes de inserir o nosso valor
        // Varremos o vetor buscando duplicidades 
        for (int j = 0; j < atual_no->numero_valores; j++){
            if (valor == atual_no->valor[j]){
                // Se for achado, cancelamos o procedimento
                return NULL;
            }
        }

        // Aqui começa a lógica para inserir o valor na posição correta (similar ao Insertion Sort)
        int j = atual_no->numero_valores - 1; // Começa do último valor existente

        // Enquanto não chegar no início E o valor atual for maior que o novo...
        while (j >= 0 && atual_no->valor[j] > valor) {
            // ... empurra o valor para a direita
            atual_no->valor[j + 1] = atual_no->valor[j];
            j--;
        }

        // Insere o valor na posição correta
        atual_no->valor[j + 1] = valor;
        
        // Incrementa o contador
        atual_no->numero_valores++;

        // Aqui verificamos se a folha tem 3 valores
        // Se não tiver, o processo é interrompido
        if(atual_no->numero_valores < 3){
            return NULL;
        }
        // Se tiver, seguimos com o procedimento de dividir o nó
        else{
            return DividirNo(atual_no, valor_promovido);
        }
    }


    // Com esse loop, vamos percorrer os valores do nó e buscar a folha correta pra inserir o valor
    int i = 0;
    while (i < atual_no->numero_valores && valor > atual_no->valor[i])
    {
        i++;
    }

    // Verificamos se o índice existe dentro dos valores e se o valor já existe. Caso exista, retorna NULL e evita duplicatas
    if (i < atual_no->numero_valores && valor == atual_no->valor[i]) {
        return NULL; 
    }

    // É aqui que guardamos o valor que vai subir
    int promovido_filho;

    // Aqui a função pausa e desce um nivel
    // Se for nulo, o filho resolve tudo sem precisar dividir
    No *novo_filho_direito = Inserir(atual_no->filho[i], valor, &promovido_filho);

    // Se for um ponteiro, o filho transbordou
    if(novo_filho_direito != NULL){  
        // Empurramos valores e ponteiros para a direita para abrir espaço
        // Começamos do fim (numero_valores) e vamos voltando até chegar em 'i'
        for(int k = atual_no->numero_valores; k > i; k--){
            // Deslocamos o valor
            atual_no->valor[k] = atual_no->valor[k - 1];
            
            // Deslocamos o filho correspondente
            atual_no->filho[k + 1] = atual_no->filho[k];
        }

        // Colocamos o valor promovido exatamente no índice 'i'
        atual_no->valor[i] = promovido_filho;

        // Colocamos o novo ponteiro logo à direita do valor inserido
        atual_no->filho[i + 1] = novo_filho_direito;

        // Incrementamos o tamanho
        atual_no->numero_valores++;

        // Se o pai tem menos de 3 valores, retorna NULL
        if(atual_no->numero_valores < 3){
            return NULL;    
        } else{
            // Se o pai tem 3 valores, dividimos o nó
            return DividirNo(atual_no, valor_promovido);
        }
    }

    return NULL;
}

void Adicionar(int valor){
    // Se a arvore não existe a gente cria ela
    if(raiz == NULL){
        raiz = CriarNo(valor);
    }

    //
    else{
        // Aqui prosseguimos se a raiz já existe
        int valor_promovido;
        No* novo_no = Inserir(raiz, valor, &valor_promovido);

        // Se a raiz atual transbordou, criamos uma nova raiz acima da antiga.
        if(novo_no != NULL){
            // Salvamos a raiz atual
            No* antiga_raiz = raiz;

            // Criamos uma nova raiz com o valor que subiu (o meio)
            raiz = CriarNo(valor_promovido);

            // Conectamos os filhos da nova raiz
            raiz->filho[0] = antiga_raiz;
            raiz->filho[1] = novo_no;
        }
    }
}

No *Buscar(No *no_atual, int valor){
    // Caso o nó atual seja nulo, a árvore foi percorrida mas o valor não foi encontrado
    if (no_atual == NULL) {
        return NULL; // Valor não existe na árvore, retorna NULL
    }

    // Varre os valores dentro do nó
    for(int i = 0; i < no_atual->numero_valores; i++){
        // Se achou o valor, retorna o nó atual
        if(valor == no_atual->valor[i]){
            return no_atual;
        }

        // Se o valor buscado é MENOR que o valor atual do nó,
        // temos que descer no filho que está logo à esquerda desse valor
        if(valor < no_atual->valor[i]){
            return Buscar(no_atual->filho[i], valor);
        }
    }

    // Se passou pelo loop e não entrou em nenhum 'if', 
    // significa que o valor é MAIOR que todos os valores deste nó
    // Então descemos no último filho disponível
    return Buscar(no_atual->filho[no_atual->numero_valores], valor);
}

void ImprimirBusca(No *no_atual, int valor){
    No *no_encontrado = Buscar(no_atual, valor);

    if(no_encontrado == NULL){
        printf("O número não existe na árvore!\n");
    } else{
        printf("O número existe na árvore!\n");
    }
}

// Remove um valor do vetor e desloca os outros para esquerda
// Ex: [10, 20] remove indice 0 -> [20]
void RemoverValorDoNo(No *no, int pos) {
    for (int i = pos; i < no->numero_valores - 1; i++) {
        no->valor[i] = no->valor[i + 1];
    }
    no->numero_valores--;
}

// Remove o ponteiro do filho e desloca os outros
void RemoverFilhoDoNo(No *no, int pos) {
    for (int i = pos; i < 3; i++) { // Até 3 pois max filhos é 4
        no->filho[i] = no->filho[i + 1];
    }
    no->filho[3] = NULL;
}

// Busca o maior valor da subárvore (o valor mais à direita possível)
// Usado para trocar quando removemos um nó interno
int ObterMaiorValor(No *no) {
    if (EhFolha(no)) {
        return no->valor[no->numero_valores - 1];
    }
    return ObterMaiorValor(no->filho[no->numero_valores]);
}

// Função que conserta o "buraco" deixado por um filho vazio (idx_filho)
void TratarUnderflow(No *pai, int idx) {
    No *filho_vazio = pai->filho[idx];
    No *irmao_esq;
    No *irmao_dir;

    if(idx > 0){
        irmao_esq = pai->filho[idx - 1];
    } else{
        irmao_esq = NULL;
    }

    if(idx < pai->numero_valores){
        irmao_dir = pai->filho[idx + 1];
    } else{
        irmao_dir = NULL;
    }

    // CASO 1: Tenta pedir emprestado do irmão da ESQUERDA
    if (irmao_esq && irmao_esq->numero_valores > 1) {
        // 1. O filho[0] se desloca para direita
        filho_vazio->filho[1] = filho_vazio->filho[0];
        
        // 2. Desce o valor do pai para o filho vazio
        filho_vazio->valor[0] = pai->valor[idx - 1];
        filho_vazio->numero_valores++;
        
        // 3. O filho adota o filho mais à direita do irmão (se não for folha)
        filho_vazio->filho[0] = irmao_esq->filho[irmao_esq->numero_valores];

        // 4. Sobe o maior valor do irmão para o pai
        pai->valor[idx - 1] = irmao_esq->valor[irmao_esq->numero_valores - 1];
        
        // 5. Remove o valor do irmão
        RemoverValorDoNo(irmao_esq, irmao_esq->numero_valores - 1);
        return;
    }

    // CASO 2: Tenta pedir emprestado do irmão da DIREITA
    if (irmao_dir && irmao_dir->numero_valores > 1) {
        // 1. Desce o valor do pai para o final do filho vazio
        filho_vazio->valor[filho_vazio->numero_valores] = pai->valor[idx];
        filho_vazio->numero_valores++;

        // 2. O filho adota o primeiro filho do irmão
        filho_vazio->filho[filho_vazio->numero_valores] = irmao_dir->filho[0];

        // 3. Sobe o menor valor do irmão para o pai
        pai->valor[idx] = irmao_dir->valor[0];

        // 4. Remove o valor do irmão e ajusta filhos
        RemoverValorDoNo(irmao_dir, 0);
        RemoverFilhoDoNo(irmao_dir, 0);
        return;
    }

    // CASO 3: MERGE (Fusão)
    // Se ninguém pode emprestar, temos que juntar (Filho Vazio + Pai + Irmão)
    // Vamos priorizar juntar com o irmão da esquerda
    if (irmao_esq) {
        // Desce valor do pai para o irmão da esquerda
        irmao_esq->valor[irmao_esq->numero_valores] = pai->valor[idx - 1];
        irmao_esq->numero_valores++;

        // Copia conteudo do filho vazio (se houver sobras) para o irmão
        // (Nota: como é underflow, geralmente está vazio, mas ponteiros importam)
        irmao_esq->filho[irmao_esq->numero_valores] = filho_vazio->filho[0];
        
        // Remove do pai o valor que desceu e o ponteiro para o filho vazio
        RemoverValorDoNo(pai, idx - 1);
        RemoverFilhoDoNo(pai, idx); // O ponteiro idx aponta pro filho vazio
        
        free(filho_vazio); // Libera memória
    } 
    else if (irmao_dir) {
        // Mesmo processo, mas fundindo com a direita
        // A lógica é espelhada: Joga tudo do irmão dir para o filho vazio (que agora cresce)
        
        filho_vazio->valor[filho_vazio->numero_valores] = pai->valor[idx];
        filho_vazio->numero_valores++;
        
        // Copia valores do irmão dir
        for(int i=0; i < irmao_dir->numero_valores; i++){
            filho_vazio->valor[filho_vazio->numero_valores] = irmao_dir->valor[i];
            filho_vazio->filho[filho_vazio->numero_valores] = irmao_dir->filho[i]; // copia filho esq do valor
            filho_vazio->numero_valores++;
        }
        filho_vazio->filho[filho_vazio->numero_valores] = irmao_dir->filho[irmao_dir->numero_valores]; // copia ultimo filho
        
        RemoverValorDoNo(pai, idx);
        RemoverFilhoDoNo(pai, idx + 1); // Remove ponteiro pro irmão dir
        
        free(irmao_dir);
    }
}

bool RemoverRecursivo(No *no, int valor){
    if(no == NULL){
        return false;
    }

    int idx = 0;
    // Procura o índice onde o valor está ou deveria estar
    while(idx < no->numero_valores && valor > no->valor[idx]){
        idx++;
    }

    // --- CENÁRIO A: Encontramos o valor no nó atual ---
    if(idx < no->numero_valores && valor == no->valor[idx]){
        if(EhFolha(no)) {
            // Se é folha, só remove
            RemoverValorDoNo(no, idx);
        } else{
            // Se não é folha, troca pelo ANTECESSOR (maior da esquerda)
            int antecessor = ObterMaiorValor(no->filho[idx]);
            no->valor[idx] = antecessor;
            // Recursão para ir lá embaixo remover o antecessor original
            bool underflow = RemoverRecursivo(no->filho[idx], antecessor);
            if (underflow){
                TratarUnderflow(no, idx);
            }
        }
    } 
    // --- CENÁRIO B: O valor não está aqui, desce no filho ---
    else{
        if(EhFolha(no)){
            return false; // Valor não existe na árvore
        }

        bool underflow = RemoverRecursivo(no->filho[idx], valor);
        if(underflow){
            TratarUnderflow(no, idx);
        }
    }

    // Retorna true se este nó ficou vazio (precisa de ajuda do pai)
    return (no->numero_valores == 0);
}

void Remover(int valor) {
    if (raiz == NULL){
        return;
    }

    bool raiz_zerada = RemoverRecursivo(raiz, valor);

    // Se a raiz ficou vazia após o processo (Underflow na raiz)
    if (raiz_zerada && raiz->numero_valores == 0) {
        No *temp = raiz;
        
        // Se a raiz ainda tem filhos (caso do Merge que puxou a raiz pra baixo),
        // o primeiro filho vira a nova raiz.
        if (!EhFolha(raiz)) {
            raiz = raiz->filho[0];
        } else {
            raiz = NULL; // Árvore ficou vazia
        }
        free(temp);
    }
}

void EmOrdem(No *no){
    if (no == NULL){
        return;
    }

    // Percorre os valores e os filhos intercalados
    for (int i = 0; i < no->numero_valores; i++){
        
        // Antes de imprimir o valor[i], visita o filho à esquerda dele (filho[i])
        EmOrdem(no->filho[i]);

        // Agora imprime o valor[i]
        printf("%d ", no->valor[i]);
    }

    // O laço não visita o filho mais a direita, então o visitamos manualmente fora do loop
    EmOrdem(no->filho[no->numero_valores]);
}

// Função auxiliar para descobrir a altura através da contagem de arestas
int ObterAltura(No *no) {
    // Se for nulo retorna -1 (altura será 0)
    if (no == NULL){
        return -1;
    }

    // Se for folha, retorna 0 (altura será 1)
    if (EhFolha(no)){
        return 0;
    }
    
    // A Árvore 2-3 é perfeitamente balanceada, então vemos apenas a altura do filho à esquerda
    return 1 + ObterAltura(no->filho[0]);
}

// Função que imprime apenas um andar específico
void ImprimirAndar(No *no, int nivel_atual, int nivel_alvo) {
    if (no == NULL){
        return;
    }

    // Se chegamos no andar que queremos imprimir:
    if (nivel_atual == nivel_alvo) {
        printf("[ ");
        for(int i = 0; i < no->numero_valores; i++){
            printf("%d ", no->valor[i]);
        }
        printf("] ");
    } // Se ainda não chegamos, continuamos descendo
    else if (nivel_atual < nivel_alvo) {
        for (int i = 0; i <= no->numero_valores; i++) {
            ImprimirAndar(no->filho[i], nivel_atual + 1, nivel_alvo);
        }
    }
}

// Wrapper para travessia por nível
void PorNivel(No *raiz) {
    // Garante que a travessia só é feita se a raiz existe
    if (raiz == NULL){
        return;
    }

    int altura = ObterAltura(raiz);

    // Para cada andar (do 0 até a altura máxima), mandamos imprimir
    for (int i = 0; i <= altura; i++) {
        printf("Nivel %d: ", i); // Mostra qual é o nível
        ImprimirAndar(raiz, 0, i);
        printf("\n");
    }
}

int main(){
    raiz = NULL; // A raiz começa nula
    int escolha; // Variável usada para a escolha no menu
    int valor; // Valor a ser inserido/removido/buscado na árvore

    while(true){
        printf("Escolha a operação que você quer fazer\n[1] Inserir\n[2] Remover\n[3] Buscar\n[4] Travessia\n[5] Sair\nResposta: ");
        scanf("%d", &escolha);

        // Caso a raiz seja nula, as únicas operações que o usuario pode realizar são Inserir ou Sair
        if(raiz == NULL && escolha != 1 && escolha != 5){
            printf("Você precisa inserir uma raiz primeiro!\n");
        } else{
            switch(escolha){
                case 1:
                    printf("Insira o valor que você quer inserir: ");
                    scanf("%d", &valor);
                    Adicionar(valor);
                    break;
                case 2:
                    printf("Insira o valor que você quer remover: ");
                    scanf("%d", &valor);
                    Remover(valor);
                    break;
                case 3:
                    printf("Insira o valor que você quer buscar: ");
                    scanf("%d", &valor);
                    ImprimirBusca(raiz, valor);
                    break;
                case 4:
                    printf("Insira que tipo de travessia você quer fazer\n[1] Ordem crescente\n[2] Por nível\nResposta: ");
                    scanf("%d", &escolha);
                    if(escolha == 1){
                        printf("Lista ordenada: ");
                        EmOrdem(raiz);
                        printf("\n");
                    } else if(escolha == 2){
                        printf("Travessia por nível: \n");
                        PorNivel(raiz);
                    } else{
                        printf("Escolha uma opção válida!\n");
                    }
                    break;
                case 5:
                    // Caso o usuário opte por encerrar o programa, ele retorna com código 0, indicando finalização normal
                    return 0;
                default:
                    // Caso o usuário não tenha inserido um número entre 1 e 5 a opção dele é inválida
                    printf("Escolha uma opção válida!\n");
                    break;
            }
        }
        printf("\n"); // Quebra de linha para manter a organização textual coerente, separada em blocos
    }

    return 0;
}