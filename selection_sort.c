#include <stdio.h>

int main(){

    int tamanho, temp;
    int indice_menor;

    printf("Insira qual tamanho sua lista terá: ");
    scanf("%d", &tamanho);

    // Cria um array de números inteiros do tamanho de "tamanho"
    int numeros[tamanho];

    // Enche o array com os valores inseridos pelo usuário
    for(int i = 0; i < tamanho; i++){
        printf("Insira um número no índice %d: ", i);
        scanf("%d", &numeros[i]);
    }

    // Loop externo vai até tamanho - 1 para evitar redundância
    for(int i = 0; i < tamanho - 1; i++){
        // Variável "indice_menor" começa com o valor de i no íncio do loop
        indice_menor = i;

        // j começa com o índice posterior a i (para evitar comparar i == i) e vai até o final do array
        for(int j = i+1; j < tamanho; j++){
            // Comparação para garantir que "indice_menor" contenha o indice do menor número igual ou à direita de i no array
            if(numeros[j] < numeros[indice_menor]){
                indice_menor = j;
            }
        }

        // Se o indice do menor número não for igual a i, os números dos índices trocam de lugar
        if(i != indice_menor){
            temp = numeros[i];
            numeros[i] = numeros[indice_menor];
            numeros[indice_menor] = temp;
        }
    }

    for(int i = 0; i < tamanho;i++){
        printf("%d\n", numeros[i]);
    }

    return 0;
}