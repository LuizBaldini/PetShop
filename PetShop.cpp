#include <Windows.h>
#include <iostream>
#include <locale>
#include <string.h>

struct Pet {
  char nome[50];
  char tipo[10];
  int numeroDono;
  char raca[50];
  char status[20];
};

void cadastrar(FILE *arq);
void geraArquivo(FILE *arq);
void entregar(FILE *arq);
void consultar(FILE *arq);
int tamanho(FILE *arq);

int main() {
  FILE *arquivo;
  int opcao;

  if ((arquivo = fopen("animais.dat", "rb+")) == NULL) {
    if ((arquivo = fopen("animais.dat", "wb+")) == NULL) {
      printf("Falha ao abrir o arquivo!\n");
      system("pause");
    }
  }

  do {
    printf("\n======= PET SHOP =======\n");
    printf("1. Cadastrar animal\n");
    printf("2. Gerar arquivo\n");
    printf("3. Entregar animal\n");
    printf("4. Consultar animal\n");
    printf("5. Sair\n");

    printf("Opcao: ");
    scanf("%d", &opcao);

    switch (opcao) {
    case 1:
      cadastrar(arquivo);
      break;
    case 2:
      geraArquivo(arquivo);
      break;
    case 3:
      entregar(arquivo);
      break;
    case 4:
      consultar(arquivo);
      break;
    case 5:
      fclose(arquivo);
      break;
    default:
      printf("Opcao invalida. Tente novamente.\n");
    }
  } while (opcao != 5);

  return 0;
}

void cadastrar(FILE *arq) {
  struct Pet pet;
  char confirma;
  strcpy(pet.status, "Nao Entregue");
  fflush(stdin);

  printf("Cadastrando novo animal:\n");
  printf("\nNumero do codigo:%d\n", tamanho(arq) + 1);
  printf("Nome do animal: ");
  scanf("%s", pet.nome);

  printf("Tipo do animal (cao, gato, etc...): ");
  scanf("%s", pet.tipo);

  printf("Numero do dono: ");
  scanf("%d", &pet.numeroDono);

  printf("Raca do animal: ");
  scanf("%s", pet.raca);

  printf("\nConfirma o cadastro? (s/n): ");
  fflush(stdin);
  scanf(" %c", &confirma);

  if (confirma == 's' || confirma == 'S') {
    fseek(arq, 0, SEEK_END);
    fwrite(&pet, sizeof(struct Pet), 1, arq);
    printf("Cadastro realizado com sucesso!\n");

  } else {
    printf("Cadastro cancelado.\n");
  }
}

void geraArquivo(FILE *arq) {
  char nomeArquivo[50];
  FILE *arqTxt;

  printf("Nome do arquivo texto: ");
  scanf("%s", nomeArquivo);

  if ((arqTxt = fopen(nomeArquivo, "w")) == NULL) {
    printf("Falha ao abrir o arquivo de texto!\n");
    return;
  }

  fprintf(
      arqTxt,
      "Nome do animal    Tipo    Numero do dono    Raca           Status \n");
  fprintf(arqTxt, "============================================================"
                  "==========\n");

  fseek(arq, 0, SEEK_SET);
  struct Pet pet;

  while (fread(&pet, sizeof(struct Pet), 1, arq) == 1) {
    fprintf(arqTxt, "%-18s%-8s%-18d%-15s%-s\n", pet.nome, pet.tipo,
            pet.numeroDono, pet.raca, pet.status);
  }

  fprintf(arqTxt, "------------------------------------------------------------"
                  "----------\n");
  fclose(arqTxt);
  printf("Arquivo gerado com sucesso!\n");
}
void entregar(FILE *arq) {
  struct Pet pet;
  char confirma;
  int nr;

  printf("\nInforme o codigo do registro para ser entregue\n");
  scanf("%d", &nr);

  if ((nr <= tamanho(arq)) && (nr > 0)) {
    fseek(arq, (nr - 1) * sizeof(struct Pet), SEEK_SET);
    fread(&pet, sizeof(struct Pet), 1, arq);

    if (strcmp(pet.status, "Entregue") != 0) {
      printf("\nNome..........:%s", pet.nome);
      printf("\nTipo..........:%s", pet.tipo);
      printf("\nNumero do dono:%d", pet.numeroDono);
      printf("\nRaca..........:%s\n", pet.raca);
      printf("\nConfirmar entrega do pedido: <s/n>\n");
      fflush(stdin);
      scanf(" %c", &confirma);

      if (toupper(confirma) == 'S' || toupper(confirma) == 's') {
        printf("\nEntregue!\n\n");
        fseek(arq, (nr - 1) * sizeof(struct Pet), SEEK_SET);
        strcpy(pet.status, "Entregue");
        fwrite(&pet, sizeof(struct Pet), 1, arq);
      }

    } else
      printf("Registro inexistente! \n");
  } else {
    printf("\nNumero de registro invalido!\n");
  }
  system("pause");
}

void consultar(FILE *arq) {
  struct Pet pet;
  int nr;

  printf("\nConsulta pelo codigo\n");
  printf("\nInforme o numero do registro...:");
  scanf("%d", &nr);

  if ((nr <= tamanho(arq)) && (nr > 0)) {
    fseek(arq, (nr - 1) * sizeof(struct Pet), SEEK_SET);
    fread(&pet, sizeof(struct Pet), 1, arq);

    if (strcmp(pet.status, "Entregue") != 0) {
      printf("\nNome......:%s", pet.nome);
      printf("\nTipo......:%s", pet.tipo);
      printf("\nNumero do dono:%d", pet.numeroDono);
      printf("\nRaca......:%s\n\n", pet.raca);
    } else {
      printf("\nAnimal entregue! \n");
    }
  } else {
    printf("\nNumero de registro invalido!\n");
  }

  system("pause");
}

int tamanho(FILE *arq) {
  fseek(arq, 0, SEEK_END);
  return ftell(arq) / sizeof(struct Pet);
}
