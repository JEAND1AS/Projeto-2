#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
typedef struct Pedido {
	int codigo_pedido;
	int quantidade;
	struct Pedido* proximo;
} Pedido;
typedef struct Mesa {
	int numero_mesa;
	int codigo_garcom;
	float total_pedido;
	Pedido* lista_pedidos;
	struct Mesa* proximo;
	struct Mesa* anterior;
} Mesa;
typedef struct {
	int codigo;
	char descricao[50];
	float preco;
} Item;
typedef struct {
	int codigo;
	char nome[50];
} Garcom;
Pedido* criarPedido(int codigo_pedido, int quantidade) {
	Pedido* novo = (Pedido*)malloc(sizeof(Pedido));
	if (!novo) {
		printf("Erro ao alocar memC3ria para o pedido.\n");
		exit(1);

	}
	novo->codigo_pedido = codigo_pedido;
	novo->quantidade = quantidade;
	novo->proximo = NULL;
	return novo;
}
void adicionarPedido(Mesa* mesa, int codigo_pedido, int quantidade) {
	Pedido* novo = criarPedido(codigo_pedido, quantidade);
	novo->proximo = mesa->lista_pedidos;
	mesa->lista_pedidos = novo;
}
Mesa* criarMesa(int numero_mesa, int codigo_garcom) {
	Mesa* nova = (Mesa*)malloc(sizeof(Mesa));
	if (!nova) {
		printf("Erro ao alocar memC3ria para a mesa.\n");
		exit(1);
	}
	nova->numero_mesa = numero_mesa;
	nova->codigo_garcom = codigo_garcom;
	nova->total_pedido = 0.0;
	nova->lista_pedidos = NULL;
	nova->proximo = NULL;
	nova->anterior = NULL;
	return nova;
}
void adicionarMesa(Mesa** lista_mesas, int numero_mesa, int codigo_garcom) {
	Mesa* nova = criarMesa(numero_mesa, codigo_garcom);
	nova->proximo = *lista_mesas;
	if (*lista_mesas != NULL) {
		(*lista_mesas)->anterior = nova;
	}
	*lista_mesas = nova;
}
void atualizarTotalMesa(Mesa* mesa, float valor_pedido) {
	mesa->total_pedido += valor_pedido;
}
char* encontrarNomeGarcom(Garcom* garcons, int num_garcons, int codigo_garcom) {
	for (int i = 0; i < num_garcons; i++) {
		if (garcons[i].codigo == codigo_garcom) {
			return garcons[i].nome;
		}
	}
	return "GarC'om nC#o encontrado";
}
float encontrarPrecoItem(Item* itens, int num_itens, int codigo_item) {
	for (int i = 0; i < num_itens; i++) {
		if (itens[i].codigo == codigo_item) {
			return itens[i].preco;
		}
	}
	return 0.0;
}
void fecharConta(Mesa** lista_mesas, int numero_mesa, Item* itens, int num_itens, Garcom* garcons, int num_garcons) {
	Mesa* mesa = *lista_mesas;
	while (mesa != NULL && mesa->numero_mesa != numero_mesa) {
		mesa = mesa->proximo;
	}
	if (mesa == NULL) {
		printf("Mesa nC#o encontrada.\n");
		return;

	}
	printf("------------------------------\n");
	printf("Nota Fiscal - Mesa %d\n", numero_mesa);
	printf("GarC'om: %s\n", encontrarNomeGarcom(garcons, num_garcons, mesa->codigo_garcom));
	printf("------------------------------\n");
	printf("================================================================================\n");
	printf("Pedidos:\n");
	printf("\n");
	Pedido* pedido = mesa->lista_pedidos;
	float total = 0.0;
	while (pedido != NULL) {
		float preco = encontrarPrecoItem(itens, num_itens, pedido->codigo_pedido);
		printf("CC3digo do Pedido: %d\n, Quantidade: %d\n, PreC'o UnitC!rio: R$ %.2f\n, Total: R$ %.2f\n", pedido->codigo_pedido, pedido->quantidade, preco, preco * pedido->quantidade);
		total += preco * pedido->quantidade;
		pedido = pedido->proximo;
	}
	printf("================================================================================\n");
	printf("------------------------------\n");
	printf("Total da Mesa: R$ %.2f\n", total);
	printf("------------------------------");
	if (mesa->anterior != NULL) {
		mesa->anterior->proximo = mesa->proximo;
	}
	if (mesa->proximo != NULL) {
		mesa->proximo->anterior = mesa->anterior;
	}
	if (mesa == *lista_mesas) {
		*lista_mesas = mesa->proximo;
	}
	pedido = mesa->lista_pedidos;
	while (pedido != NULL) {
		Pedido* temp = pedido;
		pedido = pedido->proximo;
		free(temp);
	}
	free(mesa);
}
int main() {
	srand(time(NULL));
	Mesa* lista_mesas = NULL;
	int num_garcons = rand() % 5 + 1;
	Garcom garcons[num_garcons];
	for (int i = 0; i < num_garcons; i++) {
		garcons[i].codigo = i + 1;
		sprintf(garcons[i].nome, "GarC'om %d", i + 1);
	}
	int num_itens = rand() % 10 + 1;
	Item itens[num_itens];
	for (int i = 0; i < num_itens; i++) {
		itens[i].codigo = i + 1;
		sprintf(itens[i].descricao, "Item %d", i + 1);
		itens[i].preco = (rand() % 2000 + 100) / 100.0;
	}
	int num_mesas = rand() % 5 + 1;
	for (int i = 0; i < num_mesas; i++) {
		int numero_mesa = i + 1;
		int codigo_garcom = rand() % num_garcons + 1;
		adicionarMesa(&lista_mesas, numero_mesa, codigo_garcom);

		int num_pedidos = rand() % 5 + 1;
		for (int j = 0; j < num_pedidos; j++) {
			int codigo_pedido = rand() % num_itens + 1;
			int quantidade = rand() % 10 + 1;
			adicionarPedido(lista_mesas, codigo_pedido, quantidade);
			float preco = encontrarPrecoItem(itens, num_itens, codigo_pedido);
			atualizarTotalMesa(lista_mesas, preco * quantidade);
		}
	}
	int fechar_mesa = rand() % num_mesas + 1;
	fecharConta(&lista_mesas, fechar_mesa, itens, num_itens, garcons, num_garcons);
	return 0;
}
