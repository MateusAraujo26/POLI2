#!/bin/bash

# Cores para o terminal
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

echo "========================================"
echo "Iniciando Bateria de Testes: mnem2op"
echo "========================================"

# 1. Compilação
echo "[1/3] Compilando mnem2op.c..."
gcc mnem2op.c -o mnem2op -Wall
if [ $? -ne 0 ]; then
    echo -e "${RED}[FALHA] Erro3 de compilação. Abortando.${NC}"
    exit 1
fi
echo -e "${GREEN}[OK] Compilação bem-sucedida.${NC}"

# 2. Definição dos casos de teste
TEST_CASES=("ep-exemplo-1" "ep-exemplo-2" "ep-exemplo-3" "teste_const_hex" "teste_@_jump" "teste_identacao")

echo "[2/3] Rodando testes..."
PASSED=0
FAILED=0

# 3. Execução e Comparação
for test in "${TEST_CASES[@]}"; do
    if [ ! -f "${test}.asm" ] || [ ! -f "${test}.mvn" ]; then
        echo -e "${RED}[AVISO] Arquivos para ${test} não encontrados. Pulando...${NC}"
        continue
    fi

    # Roda o nosso montador
    ./mnem2op "${test}.asm" > "${test}_out.mvn"

    # Compara a saída com o gabarito original (ignorando espaços em branco/quebras de linha extras com -w)
    DIFF_RESULT=$(diff -w "${test}_out.mvn" "${test}.mvn")

    if [ "$DIFF_RESULT" == "" ]; then
        echo -e "  -> ${test}: ${GREEN}PASSOU${NC}"
        PASSED=$((PASSED+1))
    else
        echo -e "  -> ${test}: ${RED}FALHOU${NC}"
        echo "     Diferenças encontradas:"
        echo "$DIFF_RESULT"
        FAILED=$((FAILED+1))
    fi
    
    # Limpa o arquivo temporário
    rm "${test}_out.mvn"
done

# 4. Resumo
echo "========================================"
echo "[3/3] Resumo dos Testes:"
echo -e "${GREEN}Passaram: ${PASSED}${NC}"
if [ $FAILED -gt 0 ]; then
    echo -e "${RED}Falharam: ${FAILED}${NC}"
else
    echo -e "${GREEN}Todos os testes passaram! Código blindado.${NC}"
fi
echo "========================================"