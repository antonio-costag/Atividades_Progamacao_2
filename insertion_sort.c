#include <stdio.h>

int main(){

    int tamanho;
    int atual, j;

    printf("Insira qual tamanho sua lista terá: ");
    scanf("%d", &tamanho);

    // Cria um array de números inteiros do tamanho de "tamanho"
    int numeros[tamanho];

    // Enche o array com os valores inseridos pelo usuário
    for(int i = 0; i < tamanho; i++){
        printf("Insira um número no índice %d: ", i);
        scanf("%d", &numeros[i]);
    }

    // Loop externo começa em 1 pois o índice 0 já começa "ordenado", então a comparação começa com 1
    for(int i = 1; i < tamanho; i++){
        // Número que está sendo comparado atualmente é salvo na variável "atual"
        atual = numeros[i];
        
        // j começa com um índice a menos que i e vai sendo reduzida até o índice 0
        for(j = i - 1; j >= 0; j--){
            // Se o número atual for menor que um número anterior, o número anterior é gravado 1 índice para frente
            if(atual < numeros[j]){
                numeros[j+1] = numeros[j];
            } else{
                break;
            }
        }

        // A posição posterior ao último j recebe o valor do número atual
        numeros[j+1] = atual;
    }

    for(int i = 0; i < tamanho;i++){
        printf("%d\n", numeros[i]);
    }

    return 0;
}