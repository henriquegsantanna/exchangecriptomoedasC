# FEINANCE - Exchange de Criptomoedas

## 👨‍💻 **Programadores**
**Nome:**  Igor Botini da Silva **RA:** 22.124.023-7 <br>
**Nome:**  Yllan Leite da Graça Souza **RA:** 22.124.050-0 <br>
**Nome:**  Henrique Gaiardoni Sant Anna **RA:** 22.124.009-6 
## 📝 **Sobre o Projeto** 
Projeto se trata de um sistema de compra e venda de criptomoeda, gerenciado totalmente no terminal, utilizando a liguagem de programação: **C**
## 🪄 **Funcionalidade do Sistema** 

- **Cadastrar Usuario:** Possibilidade de cadastrar até 10 usuarios no sistema

    🔍 **Código do Comando:**  `RUN -> 2 para cadastrar um novo usuário `

- **Depositar Fundos:** Sistema suporta depositos na moeda: Real, sem limite de deposito.

    🔍 **Código do Comando:**  `3. Depositar`

- **Sacar Fundos:** Caso necessite sacar o valor em real, tambem é possivel, pois o sistema possui uma função de saque, basta possuir o valor na moeda Real (R$).

    🔍 **Código do Comando:**  `4. Sacar`

- **Comprar Criptomoedas:** Sistema possui algumas moedas para comprar, basta apenas ter o valor em Real (R$) na conta, escolher a moeda e efetuar a compra e com as devidas taxas aplicadas o saldo será atualizado

    🔍 **Código do Comando:**  `5. Comprar criptomoedas`
    

- **Vender Criptomoedas:** Sendo possivel tambem vender as moedas compradas, escolhendo a moeda e a quantidade que deseja vender apos confirmar, o valor de venda sera convertido pra real (R$), com as devidas taxas aplicadas e enviado ao saldo de Reais (R$)

    🔍 **Código do Comando:**  `6. Vender criptomoedas`

 - **Deslogar da Conta:** Sai da conta e finaliza o sistema, evitando que qualquer pessoa consiga analisar sua conta

    🔍 **Código do Comando:**  `7. Atualizar Cotações`
     - **Deslogar da Conta:** Atualizar Cotações 
## 🪙 **Moedas Disponiveis**

    💲 BITCOIN (BTC) 
    💲 ETHEREUM (ETH) 
    💲 RIPPLE (XRP) 

## ⌨️ **Como Usar o Sistema**

1. Abra o terminal e navegue até o diretório onde o arquivo `main.c` está localizado.

2. Compile o código usando o comando abaixo:
   ```bash
   gcc main.c 
   ```
3. Selecionar 2 para se cadastrar    

4. Desposite um valor, utilizando o comando:
   ```bash
   3 - Depositar Valor
   ```
5. Atualize as cotações, utilizando o comando:
    ```bash
   7 - Atualizar Cotações
   ```
6. Escolha a moeda que deseja comprar, digite o valor e confirme com (S), utilizando o comando abaixo:

    ```bash
   5 - Comprar Criptomoedas
   ```

7. Atualize a cotação para verificar se houve ganho ou perde

8. Seleciona o Comando abaixo para vender um valor de criptomoeda:
    ```bash
   6 - Vender Criptomoeda
   ```
9. Apos confirmar a venda, o valor sera inserido na sua carteira real, utilizando o comando abaixo, o usuario verifica o saldo total de sua carteira.
    ```bash
   1 - Consultar Saldo
   ```
10. Agora o usuario pode sacar o valor em real, removendo o valor de sua carteira do sistema, utilizando o comando:
    ```bash
     4 - Sacar
    ```
