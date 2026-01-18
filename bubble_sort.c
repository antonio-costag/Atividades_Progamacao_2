#include <stdio.h>
#include <stdbool.h>

int main(){

    int tamanho, temp;
    bool trocou;
    int contador = 0;

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
        // Variável "trocou" é definida como false no início do loop
        trocou = false;
        
        // Loop interno vai até tamanho - 1 - i para não checar a parte direita do array
        for(int j = 0; j < tamanho - 1 - i; j++){
            // Se o número posterior for maior que o atual, os dois trocam de posição no array
            if(numeros[j] > numeros[j+1]){
                temp = numeros[j];
                numeros[j] = numeros[j+1];
                numeros[j+1] = temp;
                trocou = true;

            }
        }

        // Se "trocou" for falsa, o loop não fez nenhuma alteração na lista e portanto ela já está ordenada
        if(!trocou){
            break;
        }
        // Variável para verificar quantos loops foram necessários para ordenar a lista é incrementada
        contador++;
    }

    printf("\nContador de loops: %d\n\n", contador);

    for(int i = 0; i < tamanho;i++){
        printf("%d\n", numeros[i]);
    }
    
    return 0;
}