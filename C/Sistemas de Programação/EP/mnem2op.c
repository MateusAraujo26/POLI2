#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Vamos usar uma tabela no C para achar os opcodes, por ser mais facil
typedef struct {
    char nome[50];
    int endereco;
} Symbol;

Symbol tabelaSimbolos[512];
int simboloContador = 0;


void adicionaSimbolo(const char* nome, int endereco) {
    strcpy(tabelaSimbolos[simboloContador].nome, nome);
    tabelaSimbolos[simboloContador].endereco = endereco;
    simboloContador++;
}

int buscaEnderecoSimbolo(const char* nome) {
    for (int i = 0; i < simboloContador; i++) {
        if (strcmp(tabelaSimbolos[i].nome, nome) == 0) {
            return tabelaSimbolos[i].endereco;
        }
    }
    return 0; 
}

// Mapeamento dos opcodes
int buscaOpcode(const char* mnem) {
    if (!strcmp(mnem, "JP")) return 0x0;
    if (!strcmp(mnem, "JZ")) return 0x1;
    if (!strcmp(mnem, "JN")) return 0x2;
    if (!strcmp(mnem, "LV")) return 0x3;
    if (!strcmp(mnem, "AD")) return 0x4;
    if (!strcmp(mnem, "SB")) return 0x5;
    if (!strcmp(mnem, "ML")) return 0x6;
    if (!strcmp(mnem, "DV")) return 0x7;
    if (!strcmp(mnem, "LD")) return 0x8;
    if (!strcmp(mnem, "MM")) return 0x9;
    if (!strcmp(mnem, "SC")) return 0xA;
    if (!strcmp(mnem, "RS")) return 0xB;
    if (!strcmp(mnem, "HM")) return 0xC;
    if (!strcmp(mnem, "GD")) return 0xD;
    if (!strcmp(mnem, "PD")) return 0xE;
    if (!strcmp(mnem, "SO")) return 0xF;
    return -1;
}

int converteDecimalHexa(const char* str) {
    if (str[0] == '/') return (int)strtol(str + 1, NULL, 16);
    if (str[0] == '=') return (int)strtol(str + 1, NULL, 10);
    return 0;
}



int main(int argc, char *argv[]) {
    FILE *file = fopen(argv[1], "r");

    char linha[256];
    int i = 0;

    // Mapeamento da tabela de simbolos
    while (fgets(linha, sizeof(linha), file)) {
        char *token = strtok(linha, " \t\r\n");
        if (!token) continue;

        if (strcmp(token, "@") == 0) {
            i = converteDecimalHexa(strtok(NULL, " \t\r\n"));
            continue;
        }

        // Verifica se primeiro token é um rótulo
        int rotulo = (buscaOpcode(token) == -1 && strcmp(token, "K") != 0);
        
        if (rotulo) {
            adicionaSimbolo(token, i);
            token = strtok(NULL, " \t\r\n");
        }

        if (token) {
            i += 2; // Cada palavra ocupa 2 bytes
        }
    }

    // Geração de código
    rewind(file);
    i = 0;

    while (fgets(linha, sizeof(linha), file)) {
        char *token = strtok(linha, " \t\r\n");
        if (!token) continue;

        if (strcmp(token, "@") == 0) {
            i = converteDecimalHexa(strtok(NULL, " \t\r\n"));
            continue;
        }

        // Ignora rótulo, já mapeado
        if (buscaOpcode(token) == -1 && strcmp(token, "K") != 0) {
            token = strtok(NULL, " \t\r\n");
        }
        if (!token) continue;

        if (strcmp(token, "K") == 0) {
            int val = converteDecimalHexa(strtok(NULL, " \t\r\n"));
            printf("%04X %04X\n", i, val & 0xFFFF);
        } else {
            // É uma instrução
            int opcode = buscaOpcode(token);
            char *operando_do_token = strtok(NULL, " \t\r\n");
            int operando;

            // Operando pode ser um valor direto ou rotulo
            if (operando_do_token && (operando_do_token[0] == '/' || operando_do_token[0] == '=')) {
                operando = converteDecimalHexa(operando_do_token);
            } else if (operando_do_token) {
                operando = buscaEnderecoSimbolo(operando_do_token);
            } else {
                operando = 0;
            }
            
            // Monta opcode (4 bits) + endereço (12 bits)
            int machine_code = (opcode << 12) | (operando & 0x0FFF);
            printf("%04X %04X\n", i, machine_code);
        }
        
        i += 2;
    }

    fclose(file);
    return 0;
}