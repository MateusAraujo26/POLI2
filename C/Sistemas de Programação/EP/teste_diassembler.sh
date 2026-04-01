#!/bin/bash

GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

echo "========================================"
echo "Iniciando Teste Round-Trip (MVN -> ASM -> MVN)"
echo "========================================"

# 1. Compila as duas ferramentas
echo "[1/3] Compilando mnem2op e op2mnem..."
gcc mnem2op.c -o mnem2op -Wall
gcc op2mnem.c -o op2mnem -Wall

if [ $? -ne 0 ]; then
    echo -e "${RED}[FALHA] Erro de compilação. Abortando.${NC}"
    exit 1
fi
echo -e "${GREEN}[OK] Compiladores prontos.${NC}"

# 2. Casos de Teste (Exemplos Oficiais + Nossos Casos Limite)
TEST_CASES=("ep-exemplo-1" "ep-exemplo-2" "ep-exemplo-3" "teste_subrotinas" "teste_sem_regiao" "teste_SO")

echo "[2/3] Executando ciclo de vida da engenharia reversa..."
PASSED=0
FAILED=0

# 3. Execução
for test in "${TEST_CASES[@]}"; do
    if [ ! -f "${test}.mvn" ]; then
        echo -e "${RED}[AVISO] Arquivo ${test}.mvn não encontrado. Pulando...${NC}"
        continue
    fi

    # Passo A: Desmontar (MVN -> ASM)
    ./op2mnem "${test}.mvn" > "${test}_temp.asm"
    
    # Passo B: Montar novamente (ASM -> MVN_RECONSTRUIDO)
    ./mnem2op "${test}_temp.asm" > "${test}_reconstruido.mvn"

    # Passo C: Comparar o MVN original com o MVN reconstruído
    DIFF_RESULT=$(diff -w -i "${test}.mvn" "${test}_reconstruido.mvn")

    if [ "$DIFF_RESULT" == "" ]; then
        echo -e "  -> ${test}: ${GREEN}PASSOU (Bit-Perfect)${NC}"
        PASSED=$((PASSED+1))
    else
        echo -e "  -> ${test}: ${RED}FALHOU${NC}"
        echo "     Diferenças encontradas no código de máquina:"
        echo "$DIFF_RESULT"
        FAILED=$((FAILED+1))
    fi
    
    # Limpeza dos arquivos temporários gerados durante o teste
    rm -f "${test}_temp.asm" "${test}_reconstruido.mvn"
done

# 4. Resumo Final
echo "========================================"
echo "[3/3] Resumo Round-Trip:"
echo -e "${GREEN}Sucessos: ${PASSED}${NC}"
if [ $FAILED -gt 0 ]; then
    echo -e "${RED}Falhas: ${FAILED}${NC}"
    echo "Dica: Verifique se o op2mnem está gerando as diretivas de origem (@) corretamente."
else
    echo -e "${GREEN}Mestria! Todos os códigos foram desmontados e remontados com exatidão térmica.${NC}"
fi
echo "========================================"