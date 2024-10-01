#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define LIMITE_CADASTROS 10
#define MAX_LINE 256
#define MAX_TRANSACOES 100

int sair = 0;
char cpf_logado[20];

// Cotações das criptomoedas
float cotacao_bitcoin = 250000.0; // Cotação para Bitcoin
float cotacao_ethereum = 15000.0; // Cotação para Ethereum
float cotacao_ripple = 5.0;       // Cotação para Ripple

// Função para gerar uma variação aleatória de -5% a +5%
float gerarVariacao() { return (float)(rand() % 11 - 5) / 100; }

// Função para contar o número de registros no arquivo
int ContarQuantCadastros() {
  FILE *arquivo = fopen("usuarios.txt", "r");
  if (arquivo == NULL) {
    return 0; // Se o arquivo não existir, começa com zero
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
int VarVerificarCPFnumero(const char *cpf) {
  for (int i = 0; i < strlen(cpf); i++) {
    if (!isdigit(cpf[i])) {
      return 0;
    }
  }
  return 1;
}

// Função para verificar se o CPF e a senha estão corretos
int verificarLogin(const char *cpf, const char *senha) {
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

// Função para verificar se o CPF já está cadastrado
int verificarCPF(const char *cpf) {
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

// Função para depositar
void adicionar_saldo() {
  FILE *arquivo = fopen("usuarios.txt", "r");
  FILE *temp = fopen("temp.txt", "w");
  char linha[200];
  float valor, saldoAtual = 0.0;
  float saldoBitcoin = 0.000000, saldoEth = 0.000000, saldoRipple = 0.000000;
  int encontrado = 0;
  char cpf_logado[20];
  char senha_logada[50];

  if (arquivo == NULL || temp == NULL) {
    printf("Erro ao abrir os arquivos!\n");
    return;
  }

  printf("Digite o CPF do usuario logado: ");
  scanf("%s", cpf_logado);
  printf("Digite a senha do usuario logado: ");
  scanf("%s", senha_logada);

  // Consulta o saldo atual do usuário logado
  while (fgets(linha, sizeof(linha), arquivo) != NULL) {
    char cpfArquivo[20], senhaArquivo[50];

    sscanf(linha,
           "CPF: %s SENHA: %s REAL: %f BITCOIN: %f ETHEREUM: %f RIPPLE: %f",
           cpfArquivo, senhaArquivo, &saldoAtual, &saldoBitcoin, &saldoEth,
           &saldoRipple);

    cpfArquivo[strcspn(cpfArquivo, "\r\n")] = 0;

    // Verifica CPF e senha
    if (strcmp(cpf_logado, cpfArquivo) == 0 &&
        strcmp(senha_logada, senhaArquivo) == 0) {
      encontrado = 1;
      break;
    }
  }

  if (!encontrado) {
    printf("Usuario nao encontrado ou senha incorreta!\n");
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

    sscanf(linha,
           "CPF: %s SENHA: %s REAL: %f BITCOIN: %f ETHEREUM: %f RIPPLE: %f",
           cpfArquivo, senhaArquivo, &saldoArquivo, &bitcoinArquivo,
           &ethArquivo, &rippleArquivo);

    cpfArquivo[strcspn(cpfArquivo, "\r\n")] = 0;

    if (strcmp(cpf_logado, cpfArquivo) == 0) {
      saldoAtual += valor;
      fprintf(temp,
              "CPF: %s\tSENHA: %s\tREAL: %.2f\tBITCOIN: %.6f\tETHEREUM: "
              "%.6f\tRIPPLE: %.6f\n",
              cpfArquivo, senhaArquivo, saldoAtual, bitcoinArquivo, ethArquivo,
              rippleArquivo);
      FILE *extrato = fopen("extrato.txt", "a");
      if (extrato != NULL) {
        time_t agora;
        struct tm *tm_info;
        char dataHora[20];
        time(&agora);
        agora -= 3 * 3600;
        tm_info = localtime(&agora);
        strftime(dataHora, sizeof(dataHora), "%d/%m/%Y %H:%M:%S", tm_info);
        fprintf(extrato, "CPF: %s - DATA/HORA: %s - DEPOSITOU: R$%.2f\n",
                cpf_logado, dataHora, valor);
        fclose(extrato);
      } else {
        printf("Erro ao abrir o arquivo!\n");
      }
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

// Função para o extrato ter no máximo 100 linhas
void manterUltimasTransacoes() {
  FILE *extratoFile = fopen("extrato.txt", "r");
  if (extratoFile == NULL) {
    printf("Erro ao abrir o arquivo de extrato.\n");
    return;
  }

  // Contar o número de linhas no arquivo
  char line[MAX_LINE];
  int numLinhas = 0;

  // Contar o número de linhas no arquivo
  while (fgets(line, sizeof(line), extratoFile) != NULL) {
    numLinhas++;
  }
  fclose(extratoFile);

  // Se houver mais de 100 linhas, remover as linhas excedentes
  if (numLinhas > MAX_TRANSACOES) {
    extratoFile = fopen("extrato.txt", "r");
    FILE *tempFile = fopen("extrato_temp.txt", "w");

    if (tempFile == NULL) {
      printf("Erro ao abrir o arquivo temporario.\n");
      fclose(extratoFile);
      return;
    }

    // Ignorar as primeiras (numLinhas - 100) linhas
    int linhasIgnoradas = numLinhas - MAX_TRANSACOES;
    int linhaAtual = 0;

    // Copiar as últimas 100 linhas para o arquivo temporário
    while (fgets(line, sizeof(line), extratoFile) != NULL) {
      if (linhaAtual >= linhasIgnoradas) {
        fputs(line, tempFile);
      }
      linhaAtual++;
    }

    fclose(extratoFile);
    fclose(tempFile);

    // Substituir o arquivo original pelo temporário
    remove("extrato.txt");
    rename("extrato_temp.txt", "extrato.txt");
  }
}

// Função para consultar o extrato
void consultarExtrato() {
  char cpf[20]; // CPF de até 20 caracteres
  char line[MAX_LINE];
  int cpfEncontrado = 0;
  char cpfFormatado[20]; // Para armazenar "CPF: X"

  // Solicita o CPF do usuário
  printf("Digite o CPF do usuario: ");
  scanf("%19s", cpf); // Limita a entrada para até 19 caracteres
  printf("--------------------------------\n");

  // Formata o CPF para a busca, no formato "CPF: %s"
  snprintf(cpfFormatado, sizeof(cpfFormatado), "CPF: %s", cpf);

  // Verifica se o CPF existe no arquivo usuarios.txt
  FILE *usuariosFile = fopen("usuarios.txt", "r");
  if (usuariosFile == NULL) {
    printf("Erro ao abrir o arquivo de usuarios.\n");
    return;
  }

  while (fgets(line, sizeof(line), usuariosFile) != NULL) {
    if (strncmp(line, cpfFormatado, strlen(cpfFormatado)) == 0) {
      cpfEncontrado = 1;
      break;
    }
  }
  fclose(usuariosFile);

  if (!cpfEncontrado) {
    printf("CPF não encontrado no arquivo de usuarios.\n");
    return;
  }

  // Exibe o extrato para o CPF encontrado
  FILE *extratoFile = fopen("extrato.txt", "r");
  if (extratoFile == NULL) {
    printf("Erro ao abrir o arquivo de extrato.\n");
    return;
  }

  printf("Extrato para o CPF %s:\n", cpf);
  while (fgets(line, sizeof(line), extratoFile) != NULL) {

    // Verifica se a linha começa com o CPF completo no formato "CPF: X"
    if (strncmp(line, cpfFormatado, strlen(cpfFormatado)) == 0) {
      printf("%s", line); // Exibe a linha com a transação
    }
  }
  fclose(extratoFile);

  // Depois de consultar o extrato, garante que o arquivo tenha no máximo 100
  // transações
  manterUltimasTransacoes();
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
  float SaldoBitcoin = 0.000000;
  float SaldoEth = 0.000000;
  float SaldoRipple = 0.000000;
  int encontrado = 0;
  char cpfInput[20], senhaInput[50];

  printf("Digite seu CPF: ");
  scanf("%s", cpfInput);
  printf("Digite sua senha: ");
  scanf("%s", senhaInput);

  // Localiza o saldo do usuário
  while (fgets(linha, sizeof(linha), arquivo) != NULL) {
    char cpfArquivo[20], senhaArquivo[50];

    sscanf(linha,
           "CPF: %s SENHA: %s REAL: %f BITCOIN: %f ETHEREUM: %f RIPPLE: %f",
           cpfArquivo, senhaArquivo, &saldoAtual, &SaldoBitcoin, &SaldoEth,
           &SaldoRipple);

    // Verifica CPF e senha
    if (strcmp(cpfInput, cpfArquivo) == 0 &&
        strcmp(senhaInput, senhaArquivo) == 0) {
      encontrado = 1;
      break;
    }
  }

  fclose(arquivo);

  if (encontrado) {
    printf("CPF: %s\n", cpfInput);
    printf("Seu saldo atual:\n");
    printf("REAL: %.2f\n", saldoAtual);
    printf("BITCOIN: %.6f\n", SaldoBitcoin);
    printf("ETHEREUM: %.6f\n", SaldoEth);
    printf("RIPPLE: %.6f\n", SaldoRipple);
  } else {
    printf("CPF ou senha incorretos!\n");
  }
}

// Função para sacar
void sacar_saldo() {
  FILE *arquivo = fopen("usuarios.txt", "r");
  FILE *temp = fopen("temp.txt", "w");
  char linha[100];
  float valor, saldoAtual = 0;
  int encontrado = 0;
  float saldoBitcoin = 0.000000, saldoEth = 0.000000, saldoRipple = 0.000000;

  if (arquivo == NULL || temp == NULL) {
    printf("Erro ao abrir os arquivos!\n");
    return;
  }

  char cpfInput[20], senhaInput[50];

  printf("Digite seu CPF: ");
  scanf("%s", cpfInput);
  printf("Digite sua senha: ");
  scanf("%s", senhaInput);

  // Consulta o saldo atual do usuário com base no CPF e senha digitados
  while (fgets(linha, sizeof(linha), arquivo) != NULL) {
    char cpfArquivo[20], senhaArquivo[50];

    sscanf(linha, "CPF: %s SENHA: %s REAL: %f", cpfArquivo, senhaArquivo,
           &saldoAtual);

    // Verifica se o CPF e a senha estão corretos
    if (strcmp(cpfInput, cpfArquivo) == 0 &&
        strcmp(senhaInput, senhaArquivo) == 0) {
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

  saldoAtual -= valor;

  // Atualiza o arquivo temporário com o novo saldo SOMENTE se o saque for
  // autorizado
  rewind(arquivo);
  while (fgets(linha, sizeof(linha), arquivo) != NULL) {
    char cpfArquivo[20], senhaArquivo[50];
    float saldoArquivo;

    sscanf(linha, "CPF: %s SENHA: %s REAL: %f", cpfArquivo, senhaArquivo,
           &saldoArquivo);

    if (strcmp(cpfInput, cpfArquivo) == 0) {
      // Escreve o saldo atualizado para o usuário
      fprintf(temp,
              "CPF: %s\tSENHA: %s\tREAL: %.2f\tBITCOIN: %.6f\tETHEREUM: "
              "%.6f\tRIPPLE: %.6f\n",
              cpfArquivo, senhaArquivo, saldoAtual, saldoBitcoin, saldoEth,
              saldoRipple);
    } else {
      // Copia a linha original
      fprintf(temp, "%s", linha);
    }
  }

  // Registra o saque no extrato
  FILE *extrato = fopen("extrato.txt", "a");
  if (extrato != NULL) {
    time_t agora;
    struct tm *tm_info;
    char dataHora[20];
    time(&agora);
    agora -= 3 * 3600;
    tm_info = localtime(&agora);
    strftime(dataHora, sizeof(dataHora), "%d/%m/%Y %H:%M:%S", tm_info);
    fprintf(extrato, "CPF: %s - DATA/HORA: %s - SACOU: R$%.2f\n", cpfInput,
            dataHora, valor);
    fclose(extrato);
  } else {
    printf("Erro ao abrir o arquivo extrato.txt!\n");
  }

  printf("Saque realizado com sucesso! Novo saldo: %.2f\n", saldoAtual);

  fclose(arquivo);
  fclose(temp);
  remove("usuarios.txt");
  rename("temp.txt", "usuarios.txt");
}

// Função para cadastrar um novo CPF e senha
void CadastrarUsuario(const char *cpf, const char *senha) {
  FILE *arquivo = fopen("usuarios.txt", "a");

  if (arquivo == NULL) {
    printf("Erro ao abrir o arquivo!\n");
    return;
  }

  fprintf(arquivo,
          "CPF: %s\tSENHA: %s\tREAL: %.2f\tBITCOIN: %.6f\tETHEREUM: "
          "%.6f\tRIPPLE: %.6f\n",
          cpf, senha);
  fclose(arquivo);
}

// Função para atualizar cotações
void atualizarCotacoes() {
  cotacao_bitcoin += cotacao_bitcoin * gerarVariacao();
  cotacao_ethereum += cotacao_ethereum * gerarVariacao();
  cotacao_ripple += cotacao_ripple * gerarVariacao();

  printf("Cotacoes atualizadas:\n");
  printf("Bitcoin: R$%.2f\n", cotacao_bitcoin);
  printf("Ethereum: R$%.2f\n", cotacao_ethereum);
  printf("Ripple: R$%.2f\n", cotacao_ripple);

  return;
}

// Função para comprar criptomoedas
void comprarCripto() {
  FILE *arquivo = fopen("usuarios.txt", "r");
  FILE *temp = fopen("temp.txt", "w");
  char linha[200];
  float valor;
  float taxa;
  float taxa_compra_bitcoin = 0.02;
  float taxa_compra_ethereum = 0.01;
  float taxa_compra_ripple = 0.01;
  int opcao;
  char cripto[20];
  int encontrado = 0;
  float saldoAtual, saldoBitcoin, saldoEth, saldoRipple;
  char cpf_logado[20];
  char senha_logada[20];
  int compra_realizada = 0; // Para controlar se a compra foi feita ou não

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

    sscanf(linha,
           "CPF: %s SENHA: %s REAL: %f BITCOIN: %f ETHEREUM: %f RIPPLE: %f",
           cpfArquivo, senhaArquivo, &saldoAtual, &saldoBitcoin, &saldoEth,
           &saldoRipple);
    senhaArquivo[strcspn(senhaArquivo, "\r\n")] =
        0; // Remover quebra de linha da senha

    // Verificar CPF e senha
    if (strcmp(cpf_logado, cpfArquivo) == 0 &&
        strcmp(senha_logada, senhaArquivo) == 0) {
      encontrado = 1;

      float cotacao_selecionada;
      if (opcao == 1)
        cotacao_selecionada = cotacao_bitcoin;
      else if (opcao == 2)
        cotacao_selecionada = cotacao_ethereum;
      else
        cotacao_selecionada = cotacao_ripple;

      // Valor total em reais que o usuário deseja comprar
      float valor_com_taxa = valor + (valor * taxa); // Valor total com taxa
      float valor_em_cripto = valor / cotacao_selecionada;

      // Verificar se o saldo é suficiente para a compra
      if (saldoAtual >= valor_com_taxa) {
        // Exibir resumo da compra
        printf("Resumo da compra:\n");
        printf("Criptomoeda: %s\n", cripto);
        printf("Valor em reais: %.2f\n", valor);
        printf("Taxa: %.2f\n", valor * taxa);
        printf("Valor final com taxa: %.2f\n", valor_com_taxa);
        printf("Quantidade comprada (baseada na cotacao atual): %.6f %s\n",
               valor_em_cripto, cripto);

        // Confirmação da compra
        char confirmacao;
        printf("Deseja confirmar a compra? (S ou N): ");
        scanf(" %c", &confirmacao);

        if (confirmacao == 'S' || confirmacao == 's') {

          // Atualizar saldos
          saldoAtual -= valor_com_taxa;
          if (opcao == 1)
            saldoBitcoin += valor_em_cripto;
          else if (opcao == 2)
            saldoEth += valor_em_cripto;
          else if (opcao == 3)
            saldoRipple += valor_em_cripto;
          // Registro da venda no extrato
          FILE *extrato = fopen("extrato.txt", "a");
          if (extrato != NULL) {
            time_t agora;
            struct tm *tm_info;
            char dataHora[20];
            time(&agora);
            agora -= 3 * 3600;
            tm_info = localtime(&agora);
            strftime(dataHora, sizeof(dataHora), "%d/%m/%Y %H:%M:%S", tm_info);
            fprintf(extrato, "CPF: %s - DATA/HORA: %s - COMPROU: R$%.2f %s\n",
                    cpf_logado, dataHora, saldoAtual, cripto);
            fclose(extrato);
          } else {
            printf("Erro ao abrir o arquivo!\n");
          }

          printf("Compra realizada com sucesso!\n");
          compra_realizada = 1; // Marca que a compra foi feita
        } else {
          printf("Compra cancelada.\n");
        }
      } else {
        printf("Saldo insuficiente para realizar a compra!\n");
      }

      // Mesmo que a compra não seja realizada, copiar os dados para o arquivo
      // temporário
      fprintf(temp,
              "CPF: %s\t SENHA: %s\t REAL: %.2f\t BITCOIN: %.6f\t ETHEREUM: "
              "%.6f\t RIPPLE: %.6f\n",
              cpfArquivo, senhaArquivo, saldoAtual, saldoBitcoin, saldoEth,
              saldoRipple);
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

// Função para vender criptomoedas
void venderCripto() {
  FILE *arquivo = fopen("usuarios.txt", "r");
  FILE *temp = fopen("temp.txt", "w");

  char linha[200];
  float quantidade_venda;
  float taxa;
  float taxa_venda_bitcoin = 0.03;
  float taxa_venda_ethereum = 0.02;
  float taxa_venda_ripple = 0.01;
  int opcao;
  char cripto[20];
  int encontrado = 0;
  float saldoAtual, saldoBitcoin, saldoEth, saldoRipple;
  char cpf_logado[20];
  char senha_logada[20];
  int venda_realizada = 0; // Para controlar se a venda foi feita ou não

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

  printf("Digite a quantidade de %s que deseja vender: \n", cripto);
  scanf("%f", &quantidade_venda);

  printf("Digite seu CPF: ");
  scanf("%s", cpf_logado);
  printf("Digite sua senha para confirmar a venda: ");
  scanf("%s", senha_logada);

  // Busca pelo usuário no arquivo
  while (fgets(linha, sizeof(linha), arquivo) != NULL) {
    char cpfArquivo[20], senhaArquivo[50];

    sscanf(linha,
           "CPF: %s SENHA: %s REAL: %f BITCOIN: %f ETHEREUM: %f RIPPLE: %f",
           cpfArquivo, senhaArquivo, &saldoAtual, &saldoBitcoin, &saldoEth,
           &saldoRipple);
    senhaArquivo[strcspn(senhaArquivo, "\r\n")] =
        0; // Remover quebra de linha da senha

    // Verificar CPF e senha
    if (strcmp(cpf_logado, cpfArquivo) == 0 &&
        strcmp(senha_logada, senhaArquivo) == 0) {
      encontrado = 1;

      float cotacao_selecionada;
      if (opcao == 1)
        cotacao_selecionada = cotacao_bitcoin;
      else if (opcao == 2)
        cotacao_selecionada = cotacao_ethereum;
      else
        cotacao_selecionada = cotacao_ripple;

      // Verificar se o saldo de criptomoedas é suficiente para a venda
      if ((opcao == 1 && saldoBitcoin >= quantidade_venda) ||
          (opcao == 2 && saldoEth >= quantidade_venda) ||
          (opcao == 3 && saldoRipple >= quantidade_venda)) {

        // Valor em reais que o usuário receberá pela venda, descontando a taxa
        float valor_venda = quantidade_venda * cotacao_selecionada;
        float valor_com_taxa =
            valor_venda - (valor_venda * taxa); // Valor final após taxa

        // Exibir resumo da venda
        printf("Resumo da venda:\n");
        printf("Criptomoeda: %s\n", cripto);
        printf("Quantidade vendida: %.6f %s\n", quantidade_venda, cripto);
        printf("Valor da venda (sem taxa): R$%.2f\n", valor_venda);
        printf("Taxa: R$%.2f\n", valor_venda * taxa);
        printf("Valor final (apos taxa): R$%.2f\n", valor_com_taxa);

        // Confirmação da venda
        char confirmacao;
        printf("Deseja confirmar a venda? (S ou N): ");
        scanf(" %c", &confirmacao);

        if (confirmacao == 'S' || confirmacao == 's') {
          // Atualizar saldos
          saldoAtual += valor_com_taxa;
          if (opcao == 1)
            saldoBitcoin -= quantidade_venda;
          else if (opcao == 2)
            saldoEth -= quantidade_venda;
          else if (opcao == 3)
            saldoRipple -= quantidade_venda;

          printf("Venda realizada com sucesso!\n");
          venda_realizada = 1; // Marca que a venda foi feita

          // Registro da venda no extrato
          FILE *extrato = fopen("extrato.txt", "a");
          if (extrato != NULL) {
            time_t agora;
            struct tm *tm_info;
            char dataHora[20];
            time(&agora);
            agora -= 3 * 3600;
            tm_info = localtime(&agora);
            strftime(dataHora, sizeof(dataHora), "%d/%m/%Y %H:%M:%S", tm_info);
            fprintf(extrato, "CPF: %s - DATA/HORA: %s - VENDEU: R$%.2f %s\n",
                    cpf_logado, dataHora, valor_venda, cripto);
            fclose(extrato);
          } else {
            printf("Erro ao abrir o arquivo!\n");
          }
        } else {
          printf("Venda cancelada.\n");
        }
      } else {
        printf("Saldo insuficiente de %s para realizar a venda!\n", cripto);
      }

      // Mesmo que a venda não seja realizada, copiar os dados para o arquivo
      // temporário
      fprintf(temp,
              "CPF: %s\t SENHA: %s\t REAL: %.2f\t BITCOIN: %.6f\t ETHEREUM: "
              "%.6f\t RIPPLE: %.6f\n",
              cpfArquivo, senhaArquivo, saldoAtual, saldoBitcoin, saldoEth,
              saldoRipple);
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
    printf("7. Atualizar cotacoes\n");
    printf("8. Sair da Conta\n");
    printf("Digite a opcao desejada: ");
    scanf("%d", &opcao);

    switch (opcao) {
    case 1:
      consultar_saldo(); // Lógica para consultar saldo
      break;
    case 2:
      consultarExtrato(); // Lógica para consultar extrato
      break;
    case 3:
      adicionar_saldo(); // Lógica para depositar
      break;
    case 4:
      sacar_saldo(); // Lógica para sacar
      break;
    case 5:
      comprarCripto(); // Lógica para comprar criptomoedas
      break;
    case 6:
      venderCripto(); // Lógica para vender criptomoedas
      break;
    case 7:
      atualizarCotacoes(); // Lógica para atualizar cotações das criptomoedas
      break;
    case 8:
      printf("Sistema Finalizado\n");
      sair = 1; // Define sair como 1 para encerrar o sistema
      break;
    default:
      printf("Opcao invalida!\n");
      break;
    }
  } while (opcao != 8);
}

// Função principal
int main() {
  char cpf[20];
  char senha[50];
  int opcao, cadastrosExistentes;

  while (!sair) {
    cadastrosExistentes = ContarQuantCadastros();
    printf("Bem-vindo ao FEINANCE!\n");
    printf("Digite 1 para logar ou 2 para cadastrar um novo usuario (ou 0 para "
           "sair): ");
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
        printf(
            "Digite o CPF para login (somente numeros) (ou 0 para cancelar): ");
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
        printf("O arquivo ja contem o limite de %d cadastros.\n",
               LIMITE_CADASTROS);
        continue;
      }

      while (1) {
        printf("Digite o CPF (somente numeros) do usuário que deseja cadastrar "
               "(%d/%d): ",
               cadastrosExistentes + 1, LIMITE_CADASTROS);
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

  return 0;
}