#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h> 
#define LIMITE_CADASTROS 10

int sair = 0; 
char cpf_logado[20];

// Função para contar o número de registros no arquivo
int ContarQuantCadastros() {
    FILE *arquivo = fopen("usuarios.txt", "r");
    if (arquivo == NULL) {
        return 0;  // Se o arquivo não existir, começa com zero 
    }
    int contador = 0;
    char linha[100];

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        if (strlen(linha) > 1) {
            contador++;
        }
    }

    fclose(arquivo);
    return contador;
}

// Função para verificar se o CPF contém apenas números
int VarVerificarCPFnumero(const char* cpf) {
    for (int i = 0; i < strlen(cpf); i++) {
        if (!isdigit(cpf[i])) {
            return 0;
        }
    }
    return 1;
}

// Função para verificar se o CPF já está cadastrado
int verificarCPF(const char* cpf) {
    FILE *arquivo = fopen("usuarios.txt", "r");
    if (arquivo == NULL) {
        return 0; 
    }

    char linha[100], cpfArquivo[20];

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        sscanf(linha, "CPF: %s", cpfArquivo);
        if (strcmp(cpf, cpfArquivo) == 0) {
            fclose(arquivo);
            return 1;
        }
    }

    fclose(arquivo);
    return 0;
}

// Função para verificar se o CPF e a senha estão corretos
int verificarLogin(const char* cpf, const char* senha) {
    FILE *arquivo = fopen("usuarios.txt", "r");
    if (arquivo == NULL) {
        return 0;
    }

    char linha[100], cpfArquivo[20], senhaArquivo[50];

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        sscanf(linha, "CPF: %s SENHA: %s ", cpfArquivo, senhaArquivo);
        if (strcmp(cpf, cpfArquivo) == 0 && strcmp(senha, senhaArquivo) == 0) {
            fclose(arquivo);
            return 1;
        }
    }

    fclose(arquivo);
    return 0;
}

// Função para adicionar saldo
void adicionar_saldo() {
    FILE *arquivo = fopen("usuarios.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    char linha[200];
    float valor, saldoAtual = 0.0;
    float saldoBitcoin = 0.0, saldoEth = 0.0, saldoRipple = 0.0;
    int encontrado = 0;
    char cpf_logado[20];  // Supondo que o CPF do usuário logado já esteja disponível nessa variável.
    char senha_logada[20];

    if (arquivo == NULL || temp == NULL) {
        printf("Erro ao abrir os arquivos!\n");
        return;
    }

    printf("Digite o CPF do usuário logado: ");
    scanf("%s", cpf_logado);  // Lê o CPF do usuário logado (substitua pela sua lógica)
    printf("Digite a senha do usuário logado: ");
    scanf("%s", cpf_logado);  // Lê a senha do usuário logado (substitua pela sua lógica)

    // Consulta o saldo atual do usuário logado
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        char cpfArquivo[20], senhaArquivo[50];

        // Ajuste: também captura os valores das criptomoedas
        sscanf(linha, "CPF: %s SENHA: %s REAL: %f BITCOIN: %f ETHEREUM: %f RIPPLE: %f", 
            cpfArquivo, senhaArquivo, &saldoAtual, &saldoBitcoin, &saldoEth, &saldoRipple);

        // Remove possíveis quebras de linha e espaços extras do CPF do arquivo
        cpfArquivo[strcspn(cpfArquivo, "\r\n")] = 0;  // Remove quebra de linha do final da string, se houver

        if (strcmp(cpf_logado, cpfArquivo) == 0) {
            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        printf("Usuário não encontrado ou senha incorreta!\n");
        fclose(arquivo);
        fclose(temp);
        return;
    }

    printf("Digite o valor que deseja adicionar ao saldo: ");
    scanf("%f", &valor);

    // Adiciona o saldo ao usuário logado
    rewind(arquivo);  // Volta para o início do arquivo para reescrever todas as linhas
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        char cpfArquivo[20], senhaArquivo[50];
        float saldoArquivo, bitcoinArquivo, ethArquivo, rippleArquivo;

        // Ajuste: captura os saldos das criptomoedas corretamente
        sscanf(linha, "CPF: %s SENHA: %s REAL: %f BITCOIN: %f ETHEREUM: %f RIPPLE: %f", 
            cpfArquivo, senhaArquivo, &saldoArquivo, &bitcoinArquivo, &ethArquivo, &rippleArquivo);

        // Remove possíveis quebras de linha e espaços extras do CPF do arquivo
        cpfArquivo[strcspn(cpfArquivo, "\r\n")] = 0;

        // Verifica se é o usuário logado
        if (strcmp(cpf_logado, cpfArquivo) == 0) {
            saldoAtual += valor;  // Atualiza o saldo com o valor inserido
            // Escreve a linha atualizada para o usuário logado com o saldo novo
            fprintf(temp, "CPF: %s\tSENHA: %s\tREAL: %.2f\tBITCOIN: %.2f\tETHEREUM: %.2f\tRIPPLE: %.2f\n",
                cpfArquivo, senhaArquivo, saldoAtual, bitcoinArquivo, ethArquivo, rippleArquivo);
        } else {
            // Copia as demais linhas sem modificações
            fprintf(temp, "%s", linha);
        }
    }

    printf("Saldo adicionado com sucesso! Novo saldo: %.2f\n", saldoAtual);

    fclose(arquivo);
    fclose(temp);

    // Substitui o arquivo original pelo arquivo temporário
    remove("usuarios.txt");
    rename("temp.txt", "usuarios.txt");
}

// Função para consultar saldo
void consultar_saldo() {
    FILE *arquivo = fopen("usuarios.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    char linha[100];
    float saldoAtual = 0;
    float SaldoBitcoin = 0;
    float SaldoEth = 0;
    float SaldoRipple = 0;
    int encontrado = 0;

    // Localiza o saldo do usuário logado
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        char cpfArquivo[20], senhaArquivo[50];

        sscanf(linha, "CPF: %s SENHA: %s REAL: %f BITCOIN: %f ETHEREUM: %f RIPPLE: %f", cpfArquivo, senhaArquivo, &saldoAtual, &SaldoBitcoin, &SaldoEth, &SaldoRipple);

        if (strcmp(cpf_logado, cpfArquivo) == 0) {
            encontrado = 1;
            break;
        }
    }

    fclose(arquivo);

    if (encontrado) {
        printf("CPF: %s\n", cpf_logado);
        printf("Seu saldo atual:\n");
        printf("REAL: %.2f\n", saldoAtual);
        printf("BITCOIN: %.2f\n", SaldoBitcoin);
        printf("ETHEREUM: %.2f\n", SaldoEth);
        printf("RIPPLE: %.2f\n", SaldoRipple);
    } else {
        printf("Usuario nao encontrado!\n");
    }
}

// Função para sacar saldo
void sacar_saldo() {
    FILE *arquivo = fopen("usuarios.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    char linha[100];
    float valor, saldoAtual = 0;
    int encontrado = 0;
    float saldoBitcoin = 0.0, saldoEth = 0.0, saldoRipple = 0.0;

    if (arquivo == NULL || temp == NULL) {
        printf("Erro ao abrir os arquivos!\n");
        return;
    }

    // Consulta o saldo atual do usuário logado
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        char cpfArquivo[20], senhaArquivo[50];

        sscanf(linha, "CPF: %s SENHA: %s REAL: %f", cpfArquivo, senhaArquivo, &saldoAtual);

        if (strcmp(cpf_logado, cpfArquivo) == 0) {
            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        printf("Usuario nao encontrado!\n");
        fclose(arquivo);
        fclose(temp);
        return;
    }

    // Solicita a senha para confirmação
    char senha_digitada[50];
    printf("Digite sua senha para confirmar o saque: ");
    scanf("%s", senha_digitada);

    // Compara a senha digitada com a senha do arquivo
    rewind(arquivo); // Voltar ao início do arquivo para comparar a senha
    int senha_correta = 0;
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        char cpfArquivo[20], senhaArquivo[50];
        sscanf(linha, "CPF: %s SENHA: %s REAL: %f", cpfArquivo, senhaArquivo, &saldoAtual);

        if (strcmp(cpf_logado, cpfArquivo) == 0) {
            if (strcmp(senha_digitada, senhaArquivo) == 0) {
                senha_correta = 1;
                printf("Senha correta.\n");
                break; // Encontrou o usuário e a senha, sair do loop
            } else {
                printf("Senha incorreta. Saque cancelado.\n");
                fclose(arquivo);
                fclose(temp);
                return;
            }
        }
    }

    if (!senha_correta) {
        printf("Senha incorreta. Saque cancelado.\n");
        fclose(arquivo);
        fclose(temp);
        return;
    }

    // Loop para solicitar um valor válido para saque
    do {
        printf("Digite o valor que deseja sacar: ");
        scanf("%f", &valor);

        if (valor > saldoAtual) {
            printf("Valor insuficiente, insira um valor que voce possui.\n");
        }
    } while (valor > saldoAtual);

    saldoAtual -= valor; // Deduz o valor do saldo

    // Atualiza o arquivo temporário com o novo saldo SOMENTE se o saque for autorizado
    rewind(arquivo);
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        char cpfArquivo[20], senhaArquivo[50];
        float saldoArquivo;

        sscanf(linha, "CPF: %s SENHA: %s REAL: %f", cpfArquivo, senhaArquivo, &saldoArquivo);

        if (strcmp(cpf_logado, cpfArquivo) == 0) {
            fprintf(temp, "CPF: %s\tSENHA: %s\tREAL: %.2f\tBITCOIN: %.2f\tETHEREUM: %.2f\tRIPPLE: %.2f\n", cpfArquivo, senhaArquivo, saldoAtual, saldoBitcoin, saldoEth, saldoRipple);
        } else {
            fprintf(temp, "%s", linha); // Copia a linha original
        }
    }

    printf("Saque realizado com sucesso! Novo saldo: %.2f\n", saldoAtual);

    fclose(arquivo);
    fclose(temp);
    remove("usuarios.txt");
    rename("temp.txt", "usuarios.txt");
}

// Função para abrir o menu principal
void menuPrincipal() {
    int opcao;
    do {
        printf("\nMenu Principal:\n");
        printf("1. Consultar saldo\n");
        printf("2. Consultar extrato\n");
        printf("3. Depositar\n");
        printf("4. Sacar\n");
        printf("5. Comprar criptomoedas\n");
        printf("6. Vender criptomoedas\n");
        printf("7. Sair da Conta\n");
        printf("Digite a opcao desejada: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                // Lógica para consultar saldo 
                consultar_saldo();
                break;
            case 2:
                // Lógica para consultar extrato
                break;
            case 3:
                adicionar_saldo(); // Chama a função de adicionar saldo
                break;
            case 4:
                sacar_saldo();
                break;
            case 5:
                comprarCripto(); // Chama a função para comprar criptomoedas
                break;
            case 6:
                // Lógica para vender criptomoedas
                break;
            case 7:
                printf("Sistema Finalizado\n");
                sair = 1; // Define sair como 1 para encerrar o sistema
                break;
            default:
                printf("Opcao invalida!\n");
                break;
        }
    } while (opcao != 7);
}

// Função para cadastrar um novo CPF e senha
void CadastrarUsuario(const char* cpf, const char* senha) {
    FILE *arquivo = fopen("usuarios.txt", "a");

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    fprintf(arquivo, "CPF: %s\tSENHA: %s\tREAL: %.2f\tBITCOIN: %.2f\tETHEREUM: %.2f\tRIPPLE: %.2f\n",cpf, senha);
    fclose(arquivo);
}

//Função para comprar criptomoedas
void comprarCripto() {
    FILE *arquivo = fopen("usuarios.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    char linha[200];
    float valor;
    float taxa;
    float taxa_compra_bitcoin = 0.02;   // 2% de taxa para Bitcoin
    float taxa_compra_ethereum = 0.01;  // 1% de taxa para Ethereum
    float taxa_compra_ripple = 0.01;    // 1% de taxa para Ripple
    int opcao;
    char cripto[20];
    int encontrado = 0;
    float saldoAtual, saldoBitcoin, saldoEth, saldoRipple;
    char cpf_logado[20];  
    char senha_logada[20];
    float valor_com_taxa = valor + (valor * taxa);  // Subtrair taxa da compra
    float valor_saldo = valor - (valor * taxa);  // Subtrair taxa da compra

    if (arquivo == NULL || temp == NULL) {
        printf("Erro ao abrir os arquivos!\n");
        return;
    }

    printf("Digite o numero da criptomoeda que deseja comprar: \n");
    printf("1 - Bitcoin\t 2 - Ethereum\t 3 - Ripple\n");
    scanf("%d", &opcao);

    if (opcao == 1) {
        strcpy(cripto, "Bitcoin");
        taxa = taxa_compra_bitcoin;
    } else if (opcao == 2) {
        strcpy(cripto, "Ethereum");
        taxa = taxa_compra_ethereum;
    } else if (opcao == 3) {
        strcpy(cripto, "Ripple");
        taxa = taxa_compra_ripple;
    } else {
        printf("Opcao invalida!\n");
        fclose(arquivo);
        fclose(temp);
        return;
    }

    printf("Digite o valor que deseja comprar: \n");
    scanf("%f", &valor);

    // Pedir senha para validação
    printf("Digite sua senha para confirmar a compra: ");
    scanf("%s", senha_logada);

    // Busca pelo usuário no arquivo
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        char cpfArquivo[20], senhaArquivo[50];

        sscanf(linha, "CPF: %s SENHA: %s REAL: %f BITCOIN: %f ETHEREUM: %f RIPPLE: %f", 
               cpfArquivo, senhaArquivo, &saldoAtual, &saldoBitcoin, &saldoEth, &saldoRipple);
        senhaArquivo[strcspn(senhaArquivo, "\r\n")] = 0; // Remover quebra de linha da senha

        if (strcmp(senha_logada, senhaArquivo) == 0) {
            encontrado = 1;

            // Verificar se o saldo é suficiente para a compra
            if (saldoAtual >= valor + (valor * taxa)) {
                float valor_com_taxa = valor + (valor * taxa);  // Subtrair taxa da compra
                float valor_saldo = valor - (valor * taxa);  // Subtrair taxa da compra

                // Exibir resumo da compra
                printf("Resumo da compra:\n");
                printf("Criptomoeda: %s\n", cripto);
                printf("Valor: %.2f\n", valor);
                printf("Taxa: %.2f\n", valor * taxa);
                printf("Valor final com taxa: %.2f\n", valor_com_taxa);

                // Confirmação da compra
                char confirmacao;
                printf("Deseja confirmar a compra? (S ou N): ");
                scanf(" %c", &confirmacao);

                if (confirmacao == 'S' || confirmacao == 's') {
                    // Atualizar saldos
                    saldoAtual -= valor_com_taxa;
                    if (opcao == 1) saldoBitcoin += valor - (valor * taxa_compra_bitcoin);  // Compra de Bitcoin
                    else if (opcao == 2) saldoEth += valor - (valor * taxa_compra_ethereum); // Compra de Ethereum
                    else if (opcao == 3) saldoRipple += valor - (valor * taxa_compra_ripple); // Compra de Ripple

                    printf("Compra realizada com sucesso!\n");

                } else {
                    printf("Compra cancelada.\n");
                    fclose(arquivo);
                    fclose(temp);
                    return;
                }
            } else {
                printf("Saldo insuficiente para realizar a compra!\n");
                fclose(arquivo);
                fclose(temp);
                return;
            }

            // Atualizar arquivo temporário com os novos saldos
            rewind(arquivo);  // Voltar ao início do arquivo
            while (fgets(linha, sizeof(linha), arquivo) != NULL) {
                char cpfArquivoTemp[20], senhaArquivoTemp[50];
                float saldoAtualTemp, saldoBitcoinTemp, saldoEthTemp, saldoRippleTemp;

                sscanf(linha, "CPF: %s SENHA: %s REAL: %f BITCOIN: %f ETHEREUM: %f RIPPLE: %f", 
                       cpfArquivoTemp, senhaArquivoTemp, &saldoAtualTemp, &saldoBitcoinTemp, &saldoEthTemp, &saldoRippleTemp);

                if (strcmp(cpfArquivo, cpfArquivoTemp) == 0) {
                    // Atualizar os valores do usuário
                    fprintf(temp, "CPF: %s\t SENHA: %s\t REAL: %.2f\t BITCOIN: %.2f\t ETHEREUM: %.2f\t RIPPLE: %.2f\n", 
                            cpfArquivo, senhaArquivo, saldoAtual, saldoBitcoin, saldoEth, saldoRipple);
                } else {
                    // Copiar linha sem alterações
                    fputs(linha, temp);
                }
            }

            break;
        }
    }

    if (!encontrado) {
        printf("Senha incorreta.\n");
    }

    fclose(arquivo);
    fclose(temp);

    // Substituir o arquivo original pelo temporário
    remove("usuarios.txt");
    rename("temp.txt", "usuarios.txt");
}

//Função principal
int main() {
    char cpf[20]; 
    char senha[50];
    int opcao, cadastrosExistentes;

    while (!sair) {
        cadastrosExistentes = ContarQuantCadastros();
        printf("Bem-vindo ao FEINANCE!\n");
        printf("Digite 1 para logar ou 2 para cadastrar um novo usuario (ou 0 para sair): ");
        scanf("%d", &opcao);
        getchar(); 

        if (opcao == 0) {
            printf("Saindo do sistema.\n");
            sair = 1;
            break;
        }

        if (opcao == 1) {
            // Login
            while (1) { 
                printf("Digite o CPF para login (somente numeros) (ou 0 para cancelar): ");
                fgets(cpf, sizeof(cpf), stdin);
                cpf[strcspn(cpf, "\n")] = '\0'; 

                if (strcmp(cpf, "0") == 0) {
                    printf("Login cancelado.\n");
                    break;
                }

                if (!VarVerificarCPFnumero(cpf)) {
                    printf("CPF invalido! Digite apenas numeros.\n");
                    continue; 
                }

                printf("Digite a senha: ");
                fgets(senha, sizeof(senha), stdin);
                senha[strcspn(senha, "\n")] = '\0';  

                if (verificarLogin(cpf, senha)) {
                    printf("Login bem-sucedido! Bem-vindo!\n");
                    strcpy(cpf_logado, cpf); // Armazena o CPF logado
                    menuPrincipal();
                    if (sair) {
                        break;
                    }
                } else {
                    printf("CPF ou senha incorretos. Tente novamente.\n");
                }
            }

        } else if (opcao == 2) {
            // Cadastro
            if (cadastrosExistentes >= LIMITE_CADASTROS) {
                printf("O arquivo ja contem o limite de %d cadastros.\n", LIMITE_CADASTROS);
                continue; 
            }

            while (1) { 
                printf("Digite o CPF (somente numeros) do usuário que deseja cadastrar (%d/%d): ", cadastrosExistentes + 1, LIMITE_CADASTROS);
                fgets(cpf, sizeof(cpf), stdin);
                cpf[strcspn(cpf, "\n")] = '\0';  

                if (!VarVerificarCPFnumero(cpf)) {
                    printf("CPF invalido! Digite apenas numeros.\n");
                    continue; 
                }

                if (verificarCPF(cpf)) {
                    printf("CPF ja cadastrado! Tente outro CPF ou faça login.\n");
                } else {
                    printf("Digite a senha: ");
                    fgets(senha, sizeof(senha), stdin);
                    senha[strcspn(senha, "\n")] = '\0'; 

                    CadastrarUsuario(cpf, senha);
                    printf("Cadastrado com sucesso!\n");
                    printf("Bem-vindo ao FEINANCE!\n");
                    menuPrincipal();
                    if (sair) {
                        break; // Finaliza tudo se o sair for igual a 1
                    }
                    break;
                }
            }

        } else {
            printf("Opcao invalida! Tente novamente.\n");
        }
    }

    return 0; //FAZER COMPRA, VENDA E TAXAS DAS CRIPTOMOEDAS
}
