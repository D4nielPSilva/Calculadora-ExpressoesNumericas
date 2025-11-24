// Arquivo main.c CORRIGIDO
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "expressao.h"

Expressao testes[] = {
    
    {"3 4 + 5 *", "(3+4)*5", 35.0f},
    {"7 2 * 4 +", "7*2+4", 18.0f},
    {"8 5 2 4 + * +", "8+5*(2+4)", 38.0f}, 
    {"6 2 / 3 + 4 *", "(6/2+3)*4", 24.0f},
    {"9 5 4 8 2 * + * +", "9+5*(4+8*2)", 109.0f}, 
    {"2 3 + log 5 /", "log(2+3)/5", 0.13979f},
    {"10 log 3 ^ 2 +", "log(10)^3+2", 3.0f},
    {"45 60 + 30 cos *", "(45+60)*cos(30)", 90.9326f},
    {"0.5 45 sen 2 ^ +", "0.5+sen(45)^2", 1.0f} 
};

#define NUM_TESTES (sizeof(testes) / sizeof(Expressao))
#define EPSILON 0.0001f

void liberar_string_retorno(char *s) {
    if (s != NULL) {
        free(s);
    }
}

int main() {
    int acertos = 0;

    printf("--- Avaliador de Expressoes Numericas ---\n");
    
    for (int i = 0; i < NUM_TESTES; i++) {
        char *resultado_infixa = NULL;
        float resultado_valor;
        int teste_aprovado = 1;
        
        printf("\nTeste %d: PosFixa = %s\n", i + 1, testes[i].posFixa);

        resultado_valor = getValorPosFixa(testes[i].posFixa);
        if (fabs(resultado_valor - testes[i].Valor) < EPSILON) {
            printf("  -> Avaliacao [OK]: Valor %.4f (Esperado: %.4f)\n", resultado_valor, testes[i].Valor);
        } else {
            printf("  -> Avaliacao [FAIL]: Valor %.4f (Esperado: %.4f)\n", resultado_valor, testes[i].Valor);
            teste_aprovado = 0;
        }

        resultado_infixa = getFormaInFixa(testes[i].posFixa);
        if (resultado_infixa != NULL && strcmp(resultado_infixa, testes[i].inFixa) == 0) {
            printf("  -> Conversao [OK]: Infixa %s\n", resultado_infixa);
        } else {
            printf("  -> Conversao [FAIL]: Infixa %s (Esperado: %s)\n", 
                   resultado_infixa ? resultado_infixa : "NULL", testes[i].inFixa);
            teste_aprovado = 0;
        }
        
        liberar_string_retorno(resultado_infixa);

        if (teste_aprovado) {
            acertos++;
        }
    }

    printf("\n--- RESULTADO FINAL ---\n");
    printf("Total de testes aprovados: %d de %d\n", acertos, (int)NUM_TESTES);

    return 0;
}
