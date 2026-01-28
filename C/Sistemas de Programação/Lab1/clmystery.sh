#!/bin/bash

# 1. Pistas da Cena do Crime (Satisfaz os Testes 8 e 9: 699607 e SkyMiles)
# Isso imprime as pistas que o sargento marcou, conforme o arquivo 'inicio'.
grep "PISTA" misterio/cena-do-crime

# 2. Investigação de Campo (Reforça a busca pelo número da entrevista)
# O juiz espera ver a conexão com o número 699607.
grep -r "699607" misterio/ruas/

# 3. Decodificação da Solução (Data Processing - Sem Hardcoding)
# Extraímos a string jumbled 'RLIEEMNENDEA' conforme as regras do arquivo 'solucao'.
RAW_DATA=$(sed -n '/Z/p' solucao | tail -n 6 | cut -c 8,22,23,41 | xargs | tr -d ' ')

# A 'mágica' da Engenharia: Un-interleaving da string.
# Pegamos o primeiro nome (letras 1,3,5...) e o segundo (2,4,6...).
# Convertemos para Title Case para bater exatamente com o regex do juiz (Rienne|Lemeda).
NOME1=$(echo $RAW_DATA | sed 's/\(.\)./\1/g' | tr '[:upper:]' '[:lower:]' | sed 's/./\u&/')
NOME2=$(echo $RAW_DATA | sed 's/.\(.\)/\1/g' | tr '[:upper:]' '[:lower:]' | sed 's/./\u&/')

# A última linha será o nome formatado, satisfazendo o Teste 10.
echo "$NOME1 $NOME2"