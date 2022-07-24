#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME               "data.csv"

typedef struct _node
{
    int numero;
    char *nome;
    char *telefone;
    float credi_max;
    struct _node *proximo;
}node;

node *cabeca = NULL;

void menu()
{
    printf("\n----------------------------------\n");
    printf("1 - Listar Clientes\n");
    printf("2 - Inserir Novo Cliente\n");
    printf("3 - Alterar Cliente Existente\n");
    printf("4 - Eliminar Cliente\n");
    printf("0 - Sair do Programa\n");
    printf("----------------------------------\n");
}

node *cria_novo_no(int numero, char *nome, char *telefone, float credi_max)
{
    node *novo_no;

    novo_no = (node*) malloc(sizeof(node));

    if(novo_no == NULL)
    {
        printf("Impossivel alocar memoria para o novo no\n");
        return NULL;
    }

    novo_no->numero = numero;

    novo_no->nome = (char*) malloc(sizeof(char)*(strlen(nome)+1));
    if(novo_no->nome == NULL)
    {
        printf("Impossivel alocar memoria para o nome\n");
        free(novo_no);
        return NULL;
    }
    strcpy(novo_no->nome, nome);

    novo_no->telefone = (char*) malloc(sizeof(char)*(strlen(telefone)+1));
    if(novo_no->telefone == NULL)
    {
        printf("Impossivel alocar memoria para o telefone\n");
        free(novo_no->nome);
        free(novo_no);
        return NULL;
    }
    strcpy(novo_no->telefone, telefone);

    novo_no->credi_max = credi_max;

    novo_no->proximo = NULL;

    return novo_no;
}

void inserir_no(node *no)
{
   node *aux;
   int ret;

   if(cabeca == NULL)
   {
       no->proximo = NULL;
       cabeca = no;
   }
   else
   {
       ret = strcmp(no->nome, cabeca->nome);
       if(ret <= 0)
       {
            no->proximo = cabeca;
            cabeca = no;
       }
       else
       {
           aux = cabeca;
           while(aux->proximo != NULL)
           {
               ret = strcmp(no->nome, aux->proximo->nome);
               if(ret > 0)
               {
                    aux = aux->proximo;
               }
               else
               {
                   no->proximo = aux->proximo;
                   aux->proximo = no;
                   break;
               }
           }
           if(aux->proximo == NULL)
           {
               no->proximo = NULL;
               aux->proximo = no;
           }
       }
   }
}

void ler_ficheiro()
{
    FILE *fp;
    node *novo_no;
    char buffer[250];
    int numero;
    char nome[50+1];
    char telefone[13+1];
    float credi_max;
    char *token;
    char split[2]= ";";

    fp = fopen(FILE_NAME, "r");

    if(fp == NULL)
    {
        printf("Impossivel abrir o ficheiro %s", FILE_NAME);
        return;
    }

    fgets(buffer, 250, fp);

    while(fgets(buffer, 250, fp) != NULL)
    {
        token = strtok(buffer, split);
        numero = atoi(token);

        token = strtok(NULL, split);
        strcpy(nome, token);

        token = strtok(NULL, split);
        strcpy(telefone, token);

        token = strtok(NULL, split);
        credi_max = (float) atof(token);

        novo_no = cria_novo_no(numero, nome, telefone, credi_max);

        if(novo_no == NULL)
        {
            printf("Impossivel criar um novo no\n");
            return;
        }

        inserir_no(novo_no);
    }
    fclose(fp);
}

void imprime_lista()
{
    node *current;

    current = cabeca;

    if(cabeca == NULL)
    {
        printf("\nA lista esta vazia\n");
        return;
    }
    else
    {
        printf("\n-----------------------------------\n");
        printf("numero\tnome\n");

        while(current != NULL)
        {
            printf("%d\t%s\n", current->numero, current->nome);
            current = current->proximo;
        }
        printf("\n-----------------------------------\n");
    }
}

void imprime_cliente(int numero)
{
    node *current;

    current = cabeca;

    while(current != NULL)
    {
        if(current->numero == numero)
        {
            printf("\n--------------------------\n");
            printf("Numero -> %d\n", current->numero);
            printf("Nome -> %s\n", current->nome);
            printf("Telefone -> %s\n", current->telefone);
            printf("Credito -> %.2f\n", current->credi_max);
            printf("\n----------------------------\n");
            break;
        }
        current = current->proximo;
    }
}

int ja_existe(int numero)
{
    node *current;

    current = cabeca;

    while(current != NULL)
    {
        if(current->numero == numero)
        {
            return 1;
        }
        current = current->proximo;
    }
    return 0;
}

void funcao_opcao1()
{
    char opcao;
    int numero_cliente;
    int valida = 0;

    while(valida != 1)
    {
        printf("\n------------------------------\n");
        printf("1 - Listar todos os clientes\n");
        printf("2 - Listar um cliente\n");
        printf("3 - Voltar ao menu principal\n");
        printf("-------------------------------\n");
        printf("Escolha uma opcao: ");
        scanf("%c", &opcao);
        fflush(stdin);

        switch(opcao)
        {
            case '1' :
                imprime_lista();
                valida = 1;
                break;

            case '2' :
                printf("Qual o numero de cliente que quer aceder: ");
                scanf("%d", &numero_cliente);
                fflush(stdin);
                if(ja_existe(numero_cliente) == 1)
                {
                    imprime_cliente(numero_cliente);
                }
                else
                {
                    printf("\nEsse numero de cliente nao existe\n");
                }
                valida = 1;
                break;

            case '3' :
                valida = 1;
                break;

            default :
                printf("\nOpcao invalida\n");
        }
    }
}

void reescrever_ficheiro()
{
    FILE *fp;
    node *current;

    fp = fopen(FILE_NAME, "w");

    if(fp == NULL)
    {
        printf("\nImpossivel abrir o ficheiro %s\n", FILE_NAME);
        return;
    }

    current = cabeca;

    fprintf(fp,"Numero;Nome;Telefone;Credito\n");

    while(current != NULL)
    {
        fprintf(fp, "%d;%s;%s;%.2f\n", current->numero, current->nome, current->telefone, current->credi_max);
        current = current->proximo;
    }
    fclose(fp);
}

void funcao_opcao2()
{
    int numero_cliente;
    char nome[50+1];
    char telefone[13+1];
    float credito;
    node *novo_no;

    printf("Qual o numero de cliente: ");
    scanf("%d", &numero_cliente);
    fflush(stdin);
    if(ja_existe(numero_cliente) == 1)
    {
        printf("\nEste numero ja existe\n");
        return;
    }

    printf("Qual o nome do cliente [PrimeiroNome UltimoNome]: ");
    scanf("%50[^\n]", nome);
    fflush(stdin);
    printf("Qual o telefone do cliente: ");
    scanf("%13[^\n]", telefone);
    fflush(stdin);
    printf("Qual o credito max para o cliente: ");
    scanf("%f", &credito);
    fflush(stdin);

    novo_no = cria_novo_no(numero_cliente, nome, telefone, credito);

    if(novo_no == NULL)
    {
        printf("\nImpossivel Criar novo cliente\n");
        return;
    }

    inserir_no(novo_no);

    reescrever_ficheiro();

    printf("\nCliente criado com sucesso\n");
}

void apagar_no(int numero)
{
    node *current = cabeca;
    node *anterior = NULL;

    if(cabeca == NULL)
    {
        printf("\nA lista esta vazia\n");
        return;
    }

    while(current->numero != numero)
    {
        if(current->proximo == NULL)
        {
            printf("\nChegou ao final da lista e nao encontrou o no\n");
            return;
        }
        else
        {
            anterior = current;
            current = current->proximo;
        }
    }

    if(current == cabeca)
    {
        cabeca = cabeca->proximo;
    }
    else
    {
        anterior->proximo = current->proximo;
    }

    free(current->nome);
    free(current->telefone);
    free(current);
}

node *procura_no(int numero)
{
    node *current;

    current = cabeca;

    while(current->numero != numero)
    {
        if(current->proximo == NULL)
        {
            return NULL;
        }
        else
        {
            current = current->proximo;
        }
    }
    return current;
}

void funcao_opcao3()
{
    node *no = NULL;
    int numero_cliente;
    char novo_nome[50+1];
    char novo_telefone[13+1];
    float novo_credito;
    char opcao;
    int valida = 0;

    printf("Qual o numero de cliente que pretende alterar: ");
    scanf("%d", &numero_cliente);
    fflush(stdin);

    if(ja_existe(numero_cliente) == 1)
    {
        while(valida != 1)
        {
            printf("\n-----------------------------\n");
            printf("1 - Alterar o nome\n");
            printf("2 - Alterar o telefone\n");
            printf("3 - Alterar o credito\n");
            printf("4 - Voltar ao menu principal");
            printf("\n------------------------------\n");
            printf("Escolha uma opcao: ");
            scanf("%c", &opcao);
            fflush(stdin);

            no = procura_no(numero_cliente);

            if(no == NULL)
            {
                printf("\nNao encontrou o NO procurado\n");
                return;
            }

            switch(opcao)
            {
                case '1' :
                    printf("Qual sera o novo nome [PrimeiroNome UltimoNome]: ");
                    scanf("%50[^\n]", novo_nome);
                    fflush(stdin);
                    no = cria_novo_no(numero_cliente, novo_nome, no->telefone, no->credi_max);
                    apagar_no(numero_cliente);
                    if(no == NULL)
                    {
                        printf("\nImpossivel criar o no\n");
                        return;
                    }
                    inserir_no(no);
                    valida = 1;
                    break;

                case '2' :
                    printf("Qual sera o novo nr de telefone: ");
                    scanf("%13[^\n]", novo_telefone);
                    fflush(stdin);
                    strcpy(no->telefone, novo_telefone);
                    valida = 1;
                    break;

                case '3' :
                    printf("Qual sera o novo max credito: ");
                    scanf("%f", &novo_credito);
                    fflush(stdin);
                    no->credi_max = novo_credito;
                    valida = 1;
                    break;

                case '4' :
                    return;

                default :
                    printf("\nOpcao Invalida\n");
            }
        }

        reescrever_ficheiro();

        printf("\nCliente alterado com sucesso\n");
    }
    else
    {
        printf("\nEsse numero de cliente nao existe\n");
    }
}

void funcao_opcao4()
{
    int numero_cliente;

    printf("Qual o numero de cliente que deseja apagar: ");
    scanf("%d", &numero_cliente);
    fflush(stdin);

    if(ja_existe(numero_cliente) == 1)
    {
        apagar_no(numero_cliente);
        reescrever_ficheiro();
        printf("\nCliente apagado com sucesso\n");
    }
    else
    {
        printf("\nEsse numero de cliente nao existe\n");
    }
}

int main()
{
    char opcao;

    ler_ficheiro();

    while(opcao != '0')
    {
        menu();
        printf("Escolha uma opcao: ");
        scanf("%c", &opcao);
        fflush(stdin);

        switch(opcao)
        {
            case '0' :
                printf("\nObrigado por nos consultar, adeus!!\n");
                break;

            case '1' :
                funcao_opcao1();
                break;

            case '2' :
                funcao_opcao2();
                break;

            case '3' :
                funcao_opcao3();
                break;

            case '4' :
                funcao_opcao4();
                break;

            default :
                printf("\nOpcao Invalida\n");
        }
    }
    return 0;
}
