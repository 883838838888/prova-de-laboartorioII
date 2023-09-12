#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 100

typedef struct {
    char nome[100];
    char email[100];
    char telefone[20];
} Contato;

typedef struct {
    Contato* contatos[TABLE_SIZE];
} ListaContatos;

unsigned long hash(unsigned char* str) {
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash % TABLE_SIZE;
}

void criarContato(ListaContatos* lista) {
    Contato* novoContato = (Contato*)malloc(sizeof(Contato));

    printf("Digite o nome: ");
    scanf("%s", novoContato->nome);

    printf("Digite o email: ");
    scanf("%s", novoContato->email);

    printf("Digite o telefone: ");
    scanf("%s", novoContato->telefone);

    unsigned long index = hash(novoContato->email);
    lista->contatos[index] = novoContato;
}

void listarContatos(ListaContatos* lista) {
    printf("Lista de Contatos:\n");

    for (int i = 0; i < TABLE_SIZE; i++) {
        if (lista->contatos[i] != NULL) {
            printf("Nome: %s\n", lista->contatos[i]->nome);
            printf("Email: %s\n", lista->contatos[i]->email);
            printf("Telefone: %s\n", lista->contatos[i]->telefone);
            printf("--------------------\n");
        }
    }
}

Contato* buscarContato(ListaContatos* lista, char* email) {
    unsigned long index = hash(email);

    if (lista->contatos[index] != NULL && strcmp(lista->contatos[index]->email, email) == 0) {
        return lista->contatos[index];
    }

    return NULL;
}

void exportarContatos(ListaContatos* lista, char* arquivo) {
    FILE* file = fopen(arquivo, "w");

    if (file == NULL) {
        printf("Erro ao abrir o arquivo de exportacao.\n");
        return;
    }

    for (int i = 0; i < TABLE_SIZE; i++) {
        if (lista->contatos[i] != NULL) {
            fprintf(file, "Nome: %s\n", lista->contatos[i]->nome);
            fprintf(file, "Email: %s\n", lista->contatos[i]->email);
            fprintf(file, "Telefone: %s\n", lista->contatos[i]->telefone);
            fprintf(file, "--------------------\n");
        }
    }

    fclose(file);
}

int main() {
    ListaContatos lista;
    char opcao;
    char email[100];
    char arquivo[100];
    int i;

    for ( i = 0; i < TABLE_SIZE; i++) {
        lista.contatos[i] = NULL;
    }

    do {
        printf("--- Menu ---\n");
        printf("1. Criar contato\n");
        printf("2. Listar contatos\n");
        printf("3. Buscar contato\n");
        printf("4. Exportar contatos\n");
        printf("5. Sair\n");
        printf("Escolha uma opcao: \n");
        scanf(" %c", &opcao);

        switch (opcao) {
            case '1':
                criarContato(&lista);
                break;
            case '2':
                listarContatos(&lista);
                break;
            case '3':
                printf("Digite o email do contato que deseja buscar: ");
                scanf("%s", email);
                Contato* contato = buscarContato(&lista, email);
                if (contato != NULL) {
                    printf("Contato encontrado:\n");
                    printf("Nome: %s\n", contato->nome);
                    printf("Email: %s\n", contato->email);
                    printf("Telefone: %s\n", contato->telefone);
                } else {
                    printf("Contato nao encontrado.\n");
                }
                break;
            case '4':
                printf("Digite o nome do arquivo para exportacao: ");
                scanf("%s", arquivo);
                exportarContatos(&lista, arquivo);
                printf("Contatos exportados com sucesso.\n");
                break;
            case '5':
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida.\n");
                break;
        }
    } while (opcao != '5');

    // Liberar a memória alocada para os contatos
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (lista.contatos[i] != NULL) {
            free(lista.contatos[i]);
        }
    }

    return 0;
}
