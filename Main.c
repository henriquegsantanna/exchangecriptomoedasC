#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h> // Para usar atof
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
    char linha[100];
    float valor;

    if (arquivo == NULL || temp == NULL) {
        printf("Erro ao abrir os arquivos!\n");
        return;
    }

    printf("Digite o valor que deseja adicionar ao saldo: ");
    scanf("%f", &valor);

    // Adiciona o saldo ao usuário logado
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        char cpfArquivo[20], senhaArquivo[50];
        float saldoAtual;

        sscanf(linha, "CPF: %s SENHA: %s REAL: %f", cpfArquivo, senhaArquivo, &saldoAtual);

        // Verifica se é o usuário logado
        if (strcmp(cpf_logado, cpfArquivo) == 0) {
            saldoAtual += valor; // Atualiza o saldo
            fprintf(temp, "CPF: %s\tSENHA: %s\tREAL: %.2f\tBITCOIN: 0\n", cpfArquivo, senhaArquivo, saldoAtual);
        } else {
            fprintf(temp, "%s", linha); // Copia a linha original
        }
    }

    fclose(arquivo);
    fclose(temp);
    remove("usuarios.txt");
    rename("temp.txt", "usuarios.txt");

    printf("Saldo adicionado com sucesso! Novo saldo: %.2f\n", valor);
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
    int encontrado = 0;

    // Localiza o saldo do usuário logado
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        char cpfArquivo[20], senhaArquivo[50];

        sscanf(linha, "CPF: %s SENHA: %s REAL: %f", cpfArquivo, senhaArquivo, &saldoAtual);

        if (strcmp(cpf_logado, cpfArquivo) == 0) {
            encontrado = 1;
            break;
        }
    }

    fclose(arquivo);

    if (encontrado) {
        printf("Seu saldo atual:\n");
        printf("REAL: %.2f\n", saldoAtual);
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

    printf("Seu saldo atual: %.2f\n", saldoAtual);
    printf("Digite o valor que deseja sacar: ");
    scanf("%f", &valor);

    if (valor > saldoAtual) {
        printf("Saldo insuficiente para realizar o saque.\n");
    } else {
        saldoAtual -= valor; // Deduz o valor do saldo

        // Atualiza o arquivo temporário com o novo saldo
        rewind(arquivo);
        while (fgets(linha, sizeof(linha), arquivo) != NULL) {
            char cpfArquivo[20], senhaArquivo[50];
            float saldoArquivo;

            sscanf(linha, "CPF: %s SENHA: %s REAL: %f", cpfArquivo, senhaArquivo, &saldoArquivo);

            if (strcmp(cpf_logado, cpfArquivo) == 0) {
                fprintf(temp, "CPF: %s\tSENHA: %s\tREAL: %.2f\tBITCOIN: 0\n", cpfArquivo, senhaArquivo, saldoAtual);
            } else {
                fprintf(temp, "%s", linha); // Copia a linha original
            }
        }

        printf("Saque realizado com sucesso! Novo saldo: %.2f\n", saldoAtual);
    }

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
                // Lógica para comprar criptomoedas
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

    fprintf(arquivo, "CPF: %s\tSENHA: %s\tREAL: 0\tBITCOIN: 0\n", cpf, senha);
    fclose(arquivo);
}

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
