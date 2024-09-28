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
    char cpf_logado[20];  
    char senha_logada[50];  // Aumentado para acomodar senhas maiores

    if (arquivo == NULL || temp == NULL) {
        printf("Erro ao abrir os arquivos!\n");
        return;
    }

    printf("Digite o CPF do usuário logado: ");
    scanf("%s", cpf_logado);
    printf("Digite a senha do usuário logado: ");
    scanf("%s", senha_logada);  // Corrigido para ler a senha correta

    // Consulta o saldo atual do usuário logado
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        char cpfArquivo[20], senhaArquivo[50];

        sscanf(linha, "CPF: %s SENHA: %s REAL: %f BITCOIN: %f ETHEREUM: %f RIPPLE: %f", 
            cpfArquivo, senhaArquivo, &saldoAtual, &saldoBitcoin, &saldoEth, &saldoRipple);

        cpfArquivo[strcspn(cpfArquivo, "\r\n")] = 0;

        // Verifica CPF e senha
        if (strcmp(cpf_logado, cpfArquivo) == 0 && strcmp(senha_logada, senhaArquivo) == 0) {
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
    rewind(arquivo);  
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        char cpfArquivo[20], senhaArquivo[50];
        float saldoArquivo, bitcoinArquivo, ethArquivo, rippleArquivo;

        sscanf(linha, "CPF: %s SENHA: %s REAL: %f BITCOIN: %f ETHEREUM: %f RIPPLE: %f", 
            cpfArquivo, senhaArquivo, &saldoArquivo, &bitcoinArquivo, &ethArquivo, &rippleArquivo);

        cpfArquivo[strcspn(cpfArquivo, "\r\n")] = 0;

        if (strcmp(cpf_logado, cpfArquivo) == 0) {
            saldoAtual += valor;  
            fprintf(temp, "CPF: %s\tSENHA: %s\tREAL: %.2f\tBITCOIN: %.2f\tETHEREUM: %.2f\tRIPPLE: %.2f\n",
                cpfArquivo, senhaArquivo, saldoAtual, bitcoinArquivo, ethArquivo, rippleArquivo);
        } else {
            fprintf(temp, "%s", linha);
        }
    }

    printf("Saldo adicionado com sucesso! Novo saldo: %.2f\n", saldoAtual);

    fclose(arquivo);
    fclose(temp);
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
    char cpfInput[20], senhaInput[50];

    // Solicitar CPF e senha do usuário
    printf("Digite seu CPF: ");
    scanf("%s", cpfInput);
    printf("Digite sua senha: ");
    scanf("%s", senhaInput);

    // Localiza o saldo do usuário
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        char cpfArquivo[20], senhaArquivo[50];

        sscanf(linha, "CPF: %s SENHA: %s REAL: %f BITCOIN: %f ETHEREUM: %f RIPPLE: %f", cpfArquivo, senhaArquivo, &saldoAtual, &SaldoBitcoin, &SaldoEth, &SaldoRipple);

        // Verifica CPF e senha
        if (strcmp(cpfInput, cpfArquivo) == 0 && strcmp(senhaInput, senhaArquivo) == 0) {
            encontrado = 1;
            break;
        }
    }

    fclose(arquivo);

    if (encontrado) {
        printf("CPF: %s\n", cpfInput);
        printf("Seu saldo atual:\n");
        printf("REAL: %.2f\n", saldoAtual);
        printf("BITCOIN: %.2f\n", SaldoBitcoin);
        printf("ETHEREUM: %.2f\n", SaldoEth);
        printf("RIPPLE: %.2f\n", SaldoRipple);
    } else {
        printf("CPF ou senha incorretos!\n");
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

    char cpfInput[20], senhaInput[50];

    // Solicitar CPF e senha do usuário
    printf("Digite seu CPF: ");
    scanf("%s", cpfInput);
    printf("Digite sua senha: ");
    scanf("%s", senhaInput);

    // Consulta o saldo atual do usuário com base no CPF e senha digitados
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        char cpfArquivo[20], senhaArquivo[50];

        sscanf(linha, "CPF: %s SENHA: %s REAL: %f", cpfArquivo, senhaArquivo, &saldoAtual);

        // Verifica se o CPF e a senha estão corretos
        if (strcmp(cpfInput, cpfArquivo) == 0 && strcmp(senhaInput, senhaArquivo) == 0) {
            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        printf("CPF ou senha incorretos!\n");
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

    // Solicita a senha para confirmação do saque
    char senha_digitada[50];
    printf("Digite sua senha para confirmar o saque: ");
    scanf("%s", senha_digitada);

    // Compara a senha digitada com a senha do arquivo
    if (strcmp(senhaInput, senha_digitada) != 0) {
        printf("Senha incorreta. Saque cancelado.\n");
        fclose(arquivo);
        fclose(temp);
        return;
    }

    saldoAtual -= valor; // Deduz o valor do saldo

    // Atualiza o arquivo temporário com o novo saldo SOMENTE se o saque for autorizado
    rewind(arquivo);
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        char cpfArquivo[20], senhaArquivo[50];
        float saldoArquivo;

        sscanf(linha, "CPF: %s SENHA: %s REAL: %f", cpfArquivo, senhaArquivo, &saldoArquivo);

        if (strcmp(cpfInput, cpfArquivo) == 0) {
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
                venderCripto(); // Chama a função para vender criptomoedas
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
    int compra_realizada = 0;  // Para controlar se a compra foi feita ou não

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

    // Pedir CPF e senha para validação
    printf("Digite seu CPF: ");
    scanf("%s", cpf_logado);
    printf("Digite sua senha para confirmar a compra: ");
    scanf("%s", senha_logada);

    // Busca pelo usuário no arquivo
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        char cpfArquivo[20], senhaArquivo[50];

        sscanf(linha, "CPF: %s SENHA: %s REAL: %f BITCOIN: %f ETHEREUM: %f RIPPLE: %f", 
               cpfArquivo, senhaArquivo, &saldoAtual, &saldoBitcoin, &saldoEth, &saldoRipple);
        senhaArquivo[strcspn(senhaArquivo, "\r\n")] = 0; // Remover quebra de linha da senha

        // Verificar CPF e senha
        if (strcmp(cpf_logado, cpfArquivo) == 0 && strcmp(senha_logada, senhaArquivo) == 0) {
            encontrado = 1;

            // Verificar se o saldo é suficiente para a compra
            float valor_com_taxa = valor + (valor * taxa);  // Valor total com taxa
            if (saldoAtual >= valor_com_taxa) {
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
                    if (opcao == 1) saldoBitcoin += valor - (valor * taxa_compra_bitcoin);
                    else if (opcao == 2) saldoEth += valor - (valor * taxa_compra_ethereum);
                    else if (opcao == 3) saldoRipple += valor - (valor * taxa_compra_ripple);

                    printf("Compra realizada com sucesso!\n");
                    compra_realizada = 1;  // Marca que a compra foi feita
                } else {
                    printf("Compra cancelada.\n");
                }
            } else {
                printf("Saldo insuficiente para realizar a compra!\n");
            }

            // Mesmo que a compra não seja realizada, copiar os dados para o arquivo temporário
            fprintf(temp, "CPF: %s\t SENHA: %s\t REAL: %.2f\t BITCOIN: %.2f\t ETHEREUM: %.2f\t RIPPLE: %.2f\n", 
                    cpfArquivo, senhaArquivo, saldoAtual, saldoBitcoin, saldoEth, saldoRipple);
        } else {
            // Copiar os dados de outros usuários sem alteração
            fputs(linha, temp);
        }
    }

    if (!encontrado) {
        printf("CPF ou senha incorretos.\n");
    }

    fclose(arquivo);
    fclose(temp);

    // Se a compra foi confirmada e realizada com sucesso, atualizar o arquivo
    if (compra_realizada) {
        remove("usuarios.txt");
        rename("temp.txt", "usuarios.txt");
    } else {
        // Se a compra não foi realizada, remover o arquivo temporário
        remove("temp.txt");
    }
}

//Função para vender criptomoedas
void venderCripto() {
    FILE *arquivo = fopen("usuarios.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    char linha[200];
    float valor;
    float taxa;
    float taxa_venda_bitcoin = 0.03;   // 3% de taxa para Bitcoin
    float taxa_venda_ethereum = 0.02;  // 2% de taxa para Ethereum
    float taxa_venda_ripple = 0.01;    // 1% de taxa para Ripple
    int opcao;
    char cripto[20];
    int encontrado = 0;
    float saldoAtual, saldoBitcoin, saldoEth, saldoRipple;
    char cpf_logado[20];  
    char senha_logada[20];
    int venda_realizada = 0;  // Para controlar se a venda foi feita ou não

    if (arquivo == NULL || temp == NULL) {
        printf("Erro ao abrir os arquivos!\n");
        return;
    }

    printf("Digite o numero da criptomoeda que deseja vender: \n");
    printf("1 - Bitcoin\t 2 - Ethereum\t 3 - Ripple\n");
    scanf("%d", &opcao);

    if (opcao == 1) {
        strcpy(cripto, "Bitcoin");
        taxa = taxa_venda_bitcoin;
    } else if (opcao == 2) {
        strcpy(cripto, "Ethereum");
        taxa = taxa_venda_ethereum;
    } else if (opcao == 3) {
        strcpy(cripto, "Ripple");
        taxa = taxa_venda_ripple;
    } else {
        printf("Opcao invalida!\n");
        fclose(arquivo);
        fclose(temp);
        return;
    }

    printf("Digite o valor que deseja vender: \n");
    scanf("%f", &valor);

    // Pedir CPF e senha para validação
    printf("Digite seu CPF: ");
    scanf("%s", cpf_logado);
    printf("Digite sua senha para confirmar a venda: ");
    scanf("%s", senha_logada);

    // Busca pelo usuário no arquivo
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        char cpfArquivo[20], senhaArquivo[50];

        sscanf(linha, "CPF: %s SENHA: %s REAL: %f BITCOIN: %f ETHEREUM: %f RIPPLE: %f", 
               cpfArquivo, senhaArquivo, &saldoAtual, &saldoBitcoin, &saldoEth, &saldoRipple);
        senhaArquivo[strcspn(senhaArquivo, "\r\n")] = 0; // Remover quebra de linha da senha

        // Verificar CPF e senha
        if (strcmp(cpf_logado, cpfArquivo) == 0 && strcmp(senha_logada, senhaArquivo) == 0) {
            encontrado = 1;

            // Verificar se o saldo de criptomoeda é suficiente para a venda
            float valor_com_taxa = valor - (valor * taxa);  // Valor total menos a taxa
            int saldoSuficiente = 0;

            if (opcao == 1 && saldoBitcoin >= valor) {
                saldoSuficiente = 1;
            } else if (opcao == 2 && saldoEth >= valor) {
                saldoSuficiente = 1;
            } else if (opcao == 3 && saldoRipple >= valor) {
                saldoSuficiente = 1;
            }

            if (saldoSuficiente) {
                // Exibir resumo da venda
                printf("Resumo da venda:\n");
                printf("Criptomoeda: %s\n", cripto);
                printf("Valor: %.2f\n", valor);
                printf("Taxa: %.2f\n", valor * taxa);
                printf("Valor final com taxa: %.2f\n", valor_com_taxa);

                // Confirmação da venda
                char confirmacao;
                printf("Deseja confirmar a venda? (S ou N): ");
                scanf(" %c", &confirmacao);

                if (confirmacao == 'S' || confirmacao == 's') {
                    // Atualizar saldos
                    saldoAtual += valor_com_taxa;
                    if (opcao == 1) saldoBitcoin -= valor + (valor * taxa_venda_bitcoin);
                    else if (opcao == 2) saldoEth -= valor + (valor * taxa_venda_ethereum);
                    else if (opcao == 3) saldoRipple -= valor + (valor * taxa_venda_ripple);

                    printf("Venda realizada com sucesso!\n");
                    venda_realizada = 1;  // Marca que a venda foi feita
                } else {
                    printf("Venda cancelada.\n");
                }
            } else {
                printf("Saldo insuficiente de criptomoeda para realizar a venda!\n");
            }

            // Mesmo que a venda não seja realizada, copiar os dados para o arquivo temporário
            fprintf(temp, "CPF: %s\t SENHA: %s\t REAL: %.2f\t BITCOIN: %.2f\t ETHEREUM: %.2f\t RIPPLE: %.2f\n", 
                    cpfArquivo, senhaArquivo, saldoAtual, saldoBitcoin, saldoEth, saldoRipple);
        } else {
            // Copiar os dados de outros usuários sem alteração
            fputs(linha, temp);
        }
    }

    if (!encontrado) {
        printf("CPF ou senha incorretos.\n");
    }

    fclose(arquivo);
    fclose(temp);

    // Se a venda foi confirmada e realizada com sucesso, atualizar o arquivo
    if (venda_realizada) {
        remove("usuarios.txt");
        rename("temp.txt", "usuarios.txt");
    } else {
        // Se a venda não foi realizada, remover o arquivo temporário
        remove("temp.txt");
    }
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
