#include <stdio.h>

#define MAX_INSTR 4096
#define TYPE_ROT 1
#define TYPE_JUMP 2
#define TYPE_SUB 3

typedef struct {
    int addr;
    int val;
} Instruction;

// Mapeamento de opcodes
const char* mnemonics[] = {
    "JP", "JZ", "JN", "LV",
    "AD", "SB", "ML", "DV",
    "LD", "MM", "SC", "RS",
    "HM", "GD", "PD", "SO"
};

int main(int argc, char *argv[]) {
    (void)argc;
    FILE *file = fopen(argv[1], "r");
    Instruction insts[MAX_INSTR];
    int inst_count = 0;
    int addr, val;

    while (fscanf(file, "%x %x", &addr, &val) == 2) {
        insts[inst_count].addr = addr;
        insts[inst_count].val = val;
        inst_count++;
    }
    fclose(file);

    int label_types[MAX_INSTR] = {0};
    int label_ids[MAX_INSTR] = {0};

    // Descobrir os alvos de desvios e variáveis
    for (int i = 0; i < inst_count; i++) {
        if (insts[i].addr <= 0x02FF) { // Área de instruções
            int opcode = insts[i].val >> 12;
            int operand = insts[i].val & 0x0FFF;

            if (opcode == 0x0 || opcode == 0x1 || opcode == 0x2) {
                label_types[operand] = TYPE_JUMP; // salto
            } else if (opcode == 0xA) {
                label_types[operand] = TYPE_SUB;  // subrotina
            } else if (opcode >= 0x3 && opcode <= 0x9) {
                // Instruções que manipulam dados
                if (operand >= 0x0300 && operand <= 0x03FF) {
                    label_types[operand] = TYPE_ROT;
                }
            }
        } else if (insts[i].addr >= 0x0300 && insts[i].addr <= 0x03FF) {
            label_types[insts[i].addr] = TYPE_ROT;
        }
    }

    // Atribuição de IDs em ordem crescente
    int jump_cnt = 0, sub_cnt = 0, rot_cnt = 0;
    for (int i = 0; i < MAX_INSTR; i++) {
        if (label_types[i] == TYPE_JUMP) label_ids[i] = jump_cnt++;
        else if (label_types[i] == TYPE_SUB) label_ids[i] = sub_cnt++;
        else if (label_types[i] == TYPE_ROT) label_ids[i] = rot_cnt++;
    }

    // Geração do ASM
    int in_data_section = 0;

    for (int i = 0; i < inst_count; i++) {
        int a = insts[i].addr;
        int v = insts[i].val;

        // Impressão de  @
        if (i == 0 && a <= 0x02FF) {
            printf("@ /%04X\n", a);
        } else if (!in_data_section && a >= 0x0300) {
            printf("\n@ /%04X\n", a);
            in_data_section = 1;
        }

        // Prepara string do rótulo da linha atual
        char label_str[16] = "";
        if (label_types[a] == TYPE_JUMP) sprintf(label_str, "JUMP%02d", label_ids[a]);
        else if (label_types[a] == TYPE_SUB) sprintf(label_str, "SUB%02d", label_ids[a]);
        else if (label_types[a] == TYPE_ROT) sprintf(label_str, "ROT%02d", label_ids[a]);

        // Processa código e  os dados
        if (a <= 0x02FF) {
            int opcode = v >> 12;
            int operand = v & 0x0FFF;
            const char* mnem = mnemonics[opcode];
            char operand_str[16];

            // Decidiee o nome do operando pelo tipo de alvo
            if (opcode == 0x0 || opcode == 0x1 || opcode == 0x2) {
                sprintf(operand_str, "JUMP%02d", label_ids[operand]);
            } else if (opcode == 0xA) {
                sprintf(operand_str, "SUB%02d", label_ids[operand]);
            } else if (opcode >= 0x3 && opcode <= 0x9) {
                if (label_types[operand] == TYPE_ROT) sprintf(operand_str, "ROT%02d", label_ids[operand]);
                else sprintf(operand_str, "/%04X", operand);
            } else {
                // Operações HM, GD, PD, SO, RS com hexadecimal
                sprintf(operand_str, "/%04X", operand);
            }

            // Impressão com 8 espacos
            if (label_str[0] != '\0') {
                printf("%-8s%s %s\n", label_str, mnem, operand_str);
            } else {
                printf("        %s %s\n", mnem, operand_str);
            }
        } else { // Variáveis e constantes
            printf("%-8sK /%04X\n", label_str, v);
        }
    }

    return 0;
}