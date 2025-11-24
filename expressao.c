#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "expressao.h"

#define MAX_STACK_SIZE 100
#define PI 3.14159265358979323846
#define MAX_STRING_LENGTH 256


float pilha_num[MAX_STACK_SIZE];
int topo_num = -1;

void empilha_num(float valor) {
    if (topo_num < MAX_STACK_SIZE - 1) {
        pilha_num[++topo_num] = valor;
    }
}
float desempilha_num() {
    if (topo_num >= 0) {
        return pilha_num[topo_num--];
    }
    return 0.0;
}

typedef struct {
    char str[MAX_STRING_LENGTH];
    int precedencia_maxima;
} InfixaNode;

InfixaNode pilha_node[MAX_STACK_SIZE];
int topo_str = -1;

void empilha_str_node(const char *str, int prec) {
    if (topo_str < MAX_STACK_SIZE - 1) {
        topo_str++;
        strncpy(pilha_node[topo_str].str, str, MAX_STRING_LENGTH - 1);
        pilha_node[topo_str].str[MAX_STRING_LENGTH - 1] = '\0';
        pilha_node[topo_str].precedencia_maxima = prec;
    }
}

InfixaNode desempilha_str_node() {
    if (topo_str >= 0) {
        return pilha_node[topo_str--];
    }
    InfixaNode empty = {"", 0};
    return empty;
}

int get_precedencia(const char *op) {
    if (strcmp(op, "^") == 0) return 3;
    if (strcmp(op, "*") == 0 || strcmp(op, "/") == 0 || strcmp(op, "%") == 0) return 2;
    if (strcmp(op, "+") == 0 || strcmp(op, "-") == 0) return 1;
    
    return 4; 
}

float graus_para_radianos(float graus) { return graus * (PI / 180.0); }

float executa_operacao(float op1, float op2, const char *op) {
    if (strcmp(op, "+") == 0) return op1 + op2;
    if (strcmp(op, "-") == 0) return op1 - op2;
    if (strcmp(op, "*") == 0) return op1 * op2;
    if (strcmp(op, "/") == 0) {
        if (op2 == 0.0) return 0.0;
        return op1 / op2;
    }
    if (strcmp(op, "^") == 0) return pow(op1, op2);
    if (strcmp(op, "%") == 0) return fmod(op1, op2);

    if (strcmp(op, "raiz") == 0) return sqrt(op1);
    
    float rad = graus_para_radianos(op1); 
    if (strcmp(op, "sen") == 0) return sin(rad);
    if (strcmp(op, "cos") == 0) return cos(rad);
    if (strcmp(op, "tg") == 0) return tan(rad);

    if (strcmp(op, "log") == 0) return log10(op1);
    
    return 0.0;
}

int is_operator(const char *token) {
    if (token[0] == '\0') return 0;
    if (strlen(token) == 1) {
        char c = token[0];
        if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == '%') return 2;
    }
    if (strcmp(token, "raiz") == 0 || strcmp(token, "sen") == 0 ||
        strcmp(token, "cos") == 0 || strcmp(token, "tg") == 0 ||
        strcmp(token, "log") == 0) return 1;
    
    return 0;
}

float getValorPosFixa (char *StrPosFixa) {
    char *copia_str = strdup(StrPosFixa);
    if (!copia_str) return 0.0;
    
    char *token = strtok(copia_str, " ");
    topo_num = -1;

    while (token != NULL) {
        int op_type = is_operator(token);
        float valor;
        
        if (op_type == 0) {
            if (sscanf(token, "%f", &valor) == 1) {
                empilha_num(valor);
            } else {
                free(copia_str);
                return 0.0;
            }
        } else if (op_type == 1) {
            float op1 = desempilha_num();
            float resultado = executa_operacao(op1, 0.0, token);
            empilha_num(resultado);
        } else if (op_type == 2) {
            float op2 = desempilha_num(); 
            float op1 = desempilha_num(); 
            float resultado = executa_operacao(op1, op2, token);
            empilha_num(resultado);
        }

        token = strtok(NULL, " ");
    }
    
    free(copia_str);

    if (topo_num == 0) {
        return desempilha_num();
    } else {
        return 0.0;
    }
}

char * getFormaInFixa (char *StrPosFixa) {
    char *copia_str = strdup(StrPosFixa);
    if (!copia_str) return NULL;
    
    char *token = strtok(copia_str, " ");
    topo_str = -1; 
    
    char *inFixa_result = (char*)malloc(MAX_STRING_LENGTH * sizeof(char));
    if (!inFixa_result) {
        free(copia_str);
        return NULL;
    }
    
    while (token != NULL) {
        int op_type = is_operator(token);

        if (op_type == 0) {
            empilha_str_node(token, get_precedencia(token));
        } else if (op_type == 1) {
            InfixaNode op_node = desempilha_str_node();
            
            char nova_str[MAX_STRING_LENGTH];
            snprintf(nova_str, MAX_STRING_LENGTH, "%s(%s)", token, op_node.str); 
            
            empilha_str_node(nova_str, get_precedencia(token));
        } else if (op_type == 2) {
            InfixaNode op2_node = desempilha_str_node();
            InfixaNode op1_node = desempilha_str_node();
            
            char *op_token = token;
            int op_prec = get_precedencia(op_token);
            
            char op1_final[MAX_STRING_LENGTH];
            char op2_final[MAX_STRING_LENGTH];
            
            if (op1_node.precedencia_maxima < op_prec || 
                (op1_node.precedencia_maxima == op_prec && strcmp(op_token, "^") == 0)) {
                snprintf(op1_final, MAX_STRING_LENGTH, "(%s)", op1_node.str);
            } else {
                strcpy(op1_final, op1_node.str);
            }

            if (op2_node.precedencia_maxima < op_prec || 
                (op2_node.precedencia_maxima == op_prec && strcmp(op_token, "^") != 0)) {
                snprintf(op2_final, MAX_STRING_LENGTH, "(%s)", op2_node.str);
            } else {
                strcpy(op2_final, op2_node.str);
            }

            char nova_str[MAX_STRING_LENGTH];
            snprintf(nova_str, MAX_STRING_LENGTH, "%s%s%s", op1_final, op_token, op2_final);
            
            empilha_str_node(nova_str, op_prec);
        }

        token = strtok(NULL, " ");
    }
    
    if (topo_str == 0) {
        strcpy(inFixa_result, desempilha_str_node().str);
    } else {
        free(inFixa_result);
        inFixa_result = NULL;
    }

    free(copia_str);
    return inFixa_result;

}
