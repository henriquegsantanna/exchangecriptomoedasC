#include <stdio.h>
#include <string.h>
#include <ctype.h>  // Biblioteca para isdigit()
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
        if (!isdigit(cpf[i])) {
            return 0;  // Se encontrar qualquer caractere não numérico, retorna erro
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
        if (strcmp(cpf, cpfArquivo) == 0) {
            fclose(arquivo);
            return 1;  // CPF encontrado
        }
    }

    fclose(arquivo);
    return 0;  // CPF não encontrado
}

// Função para verificar se o CPF e a senha estão corretos
int verificarLogin(const char* cpf, const char* senha) {
    FILE *arquivo = fopen("usuarios.txt", "r");
    if (arquivo == NULL) {
        return 0;  // Se o arquivo não existir, considera que não há dados cadastrados
    }

    char linha[100], cpfArquivo[20], senhaArquivo[50];

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        sscanf(linha, "CPF: %s SENHA: %s ", cpfArquivo, senhaArquivo);
        if (strcmp(cpf, cpfArquivo) == 0 && strcmp(senha, senhaArquivo) == 0) {
            fclose(arquivo);
            return 1; // Login bem-sucedido
        }
    }

    fclose(arquivo);
    return 0;  // Login falhou
}

   //função para adicionar saldo
void adicionar_saldo() {
    char senha[12];
    float valor;

    // Verifica se a senha está correta para o usuário logado
    printf("Adicionar saldo\n");
    printf("Digite sua senha para confirmar a operação: ");
    scanf(" %s", senha); // Add space before %s to ignore newline character

    if (verificarLogin(cpf_logado,senha)) {
        printf("Senha correta. Adicionando saldo.\n");
        printf("Digite o valor a ser depositado: ");
        scanf("%f", &valor);



        printf("Saldo atualizado com sucesso!\n");
    } else {
        printf("Senha incorreta. Operação cancelada.\n");
    }
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
        printf("Digite a opção desejada: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                adicionar_saldo();
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
                printf("Sistema Finalizado\n");
                sair = 1; // Define sair como 1 para encerrar o sistema
                break;
            default:
                printf("Opção inválida!\n");
                break;
        }
    } while (opcao != 7);
}


// Função para cadastrar um novo CPF e senha
void CadastrarUsuario(const char* cpf, const char* senha) {
    FILE *arquivo = fopen("usuarios.txt", "a"); // Abre o arquivo para adicionar

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    // Insere no arquivo o cpf e a senha
    fprintf(arquivo, "CPF: %s\tSENHA: %s\tREAL: 0\tBITCOIN: 0\n", cpf, senha);
    fclose(arquivo); // Fecha o arquivo
}

int main() {
    char cpf[20]; 
    char senha[50];
    int opcao, cadastrosExistentes;

    while (!sair) {  // Loop principal que reinicia em caso de erro
        cadastrosExistentes = ContarQuantCadastros();
        printf("Bem-vindo ao FEINANCE!\n");
        printf("Digite 1 para logar ou 2 para cadastrar um novo usuário (ou 0 para sair): ");
        scanf("%d", &opcao);
        getchar(); 

        if (opcao == 0) {
            printf("Saindo do sistema.\n");
            sair = 1;
            break;  // Encerra o programa
        }

        if (opcao == 1) {
            // Login
            while (1) { 
                printf("Digite o CPF para login (somente números) (ou 0 para cancelar): ");
                fgets(cpf, sizeof(cpf), stdin);
                cpf[strcspn(cpf, "\n")] = '\0'; 

                if (strcmp(cpf, "0") == 0) {
                    printf("Login cancelado.\n");
                    break;
                }

                if (!VarVerificarCPFnumero(cpf)) {
                    printf("CPF inválido! Digite apenas números.\n");
                    continue; 
                }

                printf("Digite a senha: ");
                fgets(senha, sizeof(senha), stdin);
                senha[strcspn(senha, "\n")] = '\0';  

                if (verificarLogin(cpf, senha)) {
                    printf("Login bem-sucedido! Bem-vindo!\n");

                    menuPrincipal(); // Chama o menu principal
                    if (sair) {
                        break; // Sai do loop principal se sair foi definido
                    }
                } else {
                    printf("CPF ou senha incorretos. Tente novamente.\n");
                }
            }

        } else if (opcao == 2) {
            // Cadastro
            if (cadastrosExistentes >= LIMITE_CADASTROS) {
                printf("O arquivo já contém o limite de %d cadastros.\n", LIMITE_CADASTROS);
                continue; 
            }

            while (1) { 
                printf("Digite o CPF (somente números) do usuário que deseja cadastrar (%d/%d): ", cadastrosExistentes + 1, LIMITE_CADASTROS);
                fgets(cpf, sizeof(cpf), stdin);
                cpf[strcspn(cpf, "\n")] = '\0';  

                if (!VarVerificarCPFnumero(cpf)) {
                    printf("CPF inválido! Digite apenas números.\n");
                    continue;  // Volta para o início do loop de cadastro
                }

                // Verifica se o CPF já foi cadastrado
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
                    if (sair) {
                        break; // Finaliza tudo se o sair for igual a 1
                    }
                    break;
                }
            }

        } else {
            printf("Opção inválida! Tente novamente.\n");
        }
    }

    return 0;
}
