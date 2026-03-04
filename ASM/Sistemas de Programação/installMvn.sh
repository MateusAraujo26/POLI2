#!/bin/bash

# Faz o download do utilitario de linha de comando da MVN
wget https://github.com/PCS3616/mvn-rs/releases/download/v0.1.1/mvn-cli

# Adiciona permissao de execucao
chmod +x mvn-cli 

# Move o arquivo executavel para o diretorio de binarios do usuario,
# i.e., "instala localmente" o programa
sudo mv mvn-cli /usr/local/bin/

echo 'Utilitario mvn-cli instalado.'

# Clona o repositorio do simulador da MVN no diretorio ~/.mvn
# Obs: "~" eh um atalho para sua home, exemplo, para o usuario pcs3616,
# o path "~/.mvn" eh equivalente a "/home/pcs3616/.mvn"
git clone https://github.com/PCS-Poli-USP/MVN.git ~/.mvn

# Cria o script de execucao do simulador
# O operador '>' redireciona a saida padrao, criando um novo arquivo
echo '#!/bin/bash' > ~/.mvnMonitor.sh
# O operador '>>' tambem redireciona a saida padrao, mas da append no arquivo
echo 'python3 ~/.mvn/MVN/mvnMonitor.py' >> ~/.mvnMonitor.sh 
# Adiciona permissao de execucao ao script
chmod +x ~/.mvnMonitor.sh 

# Cria um link simbolico (um 'atalho') no diretorio de binarios do usuario,
# i.e., "instala localmente" o programa
ln -s ~/.mvnMonitor.sh mvnMonitor
sudo mv mvnMonitor /usr/local/bin

echo 'Simulador da MVN instalado'

echo 'Para utilizar os programas, execute os comandos "mvn-cli" e "mvnMonitor".'
