#include <stdio.h>
#include <string.h>
#include <ctype.h>  // Biblioteca responsavel para inserir o isdigit(), server para verificar se o q foi digitado, é numero ou string.
#define LIMITE_CADASTROS 10

// Função para contar o número de registros no arquivo
int ContarQuantCadastros() {
    FILE *arquivo = fopen("usuarios.txt", "r");
    if (arquivo == NULL) {
        return 0;  // Se o arquivo não existir, começa com zero cadastros
    }
    int contador = 0;
    char linha[100];

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        if (strlen(linha) > 1) {  // Conta apenas linhas não vazias
            contador++;
        }
    }

    fclose(arquivo);
    return contador;
}

// Função para verificar se o CPF contém apenas números
int VarVerificarCPFnumero(const char* cpf) {
    for (int i = 0; i < strlen(cpf); i++) {
        if (!isdigit(cpf[i])) { // Verifica se o caractere não eh um digito string
            return 0;  // Se encontrar qualquer caractere não numérico, retorna o erro
        }
    }
    return 1;  // CPF válido (apenas números)
}

// Função para verificar se o CPF já está cadastrado
int verificarCPF(const char* cpf) {
    FILE *arquivo = fopen("usuarios.txt", "r");
    if (arquivo == NULL) {
        return 0;  // Se o arquivo não existir, considera que não há CPF cadastrado
    }

    char linha[100], cpfArquivo[20];

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        sscanf(linha, "CPF: %s", cpfArquivo);
        if (strcmp(cpf, cpfArquivo) == 0) {  // Verifica se o CPF já está cadastrado
            fclose(arquivo);
            return 1;  // CPF encontrado
        }
    }

    fclose(arquivo);
    return 0;  // CPF não encontrado
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
        printf("7. Fechar programa\n");
        printf("Digite a opção desejada: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                // Lógica para consultar saldo
                break;
            case 2:
                // Lógica para consultar extrato
                break;
            case 3:
                // Lógica para depositar
                break;
            case 4:
                // Lógica para sacar
                break;
            case 5:
                // Lógica para comprar criptomoedas
                break;
            case 6:
                // Lógica para vender criptomoedas
                break;
            case 7:
                printf("Encerrando o programa.\n");
                break;
            default:
                printf("Opção inválida!\n");
                break;
        }
    } while (opcao != 7);
}

// Função para verificar se o CPF e a senha estão corretos
int verificarLogin(const char* cpf, const char* senha) {
    FILE *arquivo = fopen("usuarios.txt", "r");
    if (arquivo == NULL) {
        return 0;  // Se o arquivo não existir, considera que não há dados cadastrados
    }

    char linha[100], cpfArquivo[20], senhaArquivo[50];

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        sscanf(linha, "CPF: %s SENHA: %s", cpfArquivo, senhaArquivo);
        if (strcmp(cpf, cpfArquivo) == 0 && strcmp(senha, senhaArquivo) == 0) {  // Verifica CPF e senha
            fclose(arquivo);
            menuPrincipal();
            return 1;  // Login bem-sucedido
        }
    }

    fclose(arquivo);
    return 0;  // Login falhou
}

// Função para cadastrar um novo CPF e senha
void CadastrarUsuario(const char* cpf, const char* senha) {
    FILE *arquivo = fopen("usuarios.txt", "a"); // Abre o arquivo para adicionar

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    // Insere o CPF e a senha no arquivo com formatação adequada
    fprintf(arquivo, "CPF: %s\tSENHA: %s\n", cpf, senha);
    fclose(arquivo); // Fecha o arquivo
}

int main() {
    char cpf[20]; 
    char senha[50];
    int opcao, cadastrosExistentes;

    while (1) {  // Loop principal que reinicia em caso de erro
        cadastrosExistentes = ContarQuantCadastros();
        printf("Bem-vindo ao FEINANCE!\n");
        printf("Digite 1 para logar ou 2 para cadastrar um novo usuário: ");
        scanf("%d", &opcao);
        getchar();  // Consumir '\n' residual do scanf

        if (opcao == 1) {
            // Login
            while (1) {  // Loop para tentativas de login
                printf("Digite o CPF para login (somente números) (ou 0 para cancelar): ");
                fgets(cpf, sizeof(cpf), stdin);
                cpf[strcspn(cpf, "\n")] = '\0'; 

                if (strcmp(cpf, "0") == 0) {
                    printf("Login cancelado.\n");
                    break;
                }

                if (!VarVerificarCPFnumero(cpf)) {
                    printf("CPF inválido! Digite apenas números.\n");
                    continue;  // Volta para o início do loop de login
                }

                printf("Digite a senha: ");
                fgets(senha, sizeof(senha), stdin);
                senha[strcspn(senha, "\n")] = '\0';  

                if (verificarLogin(cpf, senha)) {
                    printf("Login bem-sucedido! Bem-vindo!\n");
                    break; 
                } else {
                    printf("CPF ou senha incorretos. Tente novamente.\n");
                }
            }

        } else if (opcao == 2) {
            // Cadastro
            if (cadastrosExistentes >= LIMITE_CADASTROS) {
                printf("O arquivo já contém o limite de %d cadastros.\n", LIMITE_CADASTROS);
                continue;  // Volta para o início do loop principal
            }

            while (1) {  // Loop para tentativas de cadastro
                printf("Digite o CPF (somente números) do usuário que deseja cadastrar (%d/%d): ", cadastrosExistentes + 1, LIMITE_CADASTROS);
                fgets(cpf, sizeof(cpf), stdin);
                cpf[strcspn(cpf, "\n")] = '\0';  

                if (!VarVerificarCPFnumero(cpf)) {
                    printf("CPF inválido! Digite apenas números.\n");
                    continue;  // Volta para o início do loop de cadastro
                }

                // Verifica se o CPF já está cadastrado
                if (verificarCPF(cpf)) {
                    printf("CPF já cadastrado! Tente outro CPF ou faça login.\n");
                } else {
                    printf("Digite a senha: ");
                    fgets(senha, sizeof(senha), stdin);
                    senha[strcspn(senha, "\n")] = '\0'; 

                    CadastrarUsuario(cpf, senha);
                    printf("Cadastrado com sucesso!\n");
                    printf("Bem-vindo ao FEINANCE!\n");
                    menuPrincipal();
                    break;
                }
            }

        } else {
            printf("Opção inválida! Tente novamente.\n");
        }
    }

    return 0;
}
