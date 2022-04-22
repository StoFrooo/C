#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#define LIST_SIZE 20

/* List implementation*/
/* Lista wiazana, element wskazuje na nastepny np. lista[4] zwroci pierwszy element pod nia, a nastepny mozna dostac pod pointerem next */
typedef struct
{
	struct ListNode* next;
	int val;
}ListNode;

/* Tworze liste pobliskich wieszcholkow, pod numerem wieszkolka istnieje lista wiazana wieszcholkow do ktorych jest polaczona */
ListNode adj[LIST_SIZE];

/* Zwraca ostatni dodany wieszcholek pod liste wiazana */
ListNode* getLast(int i, ListNode* list)
{
	ListNode* ptr = &list[i];
	/* Iteruje az znajdzie null na ostatnim czyli ze ostatni */
	while (ptr->next != NULL)
		ptr = ptr->next;
	return ptr;
}

/* Dodaje nowa wartosc do listy wiazanej na koncu (uzywa dlatego getLast) */
void addVal(int i, ListNode* list, int val)
{
	ListNode* last = getLast(i, adj);
	last->next = (ListNode*)malloc(sizeof(ListNode));
	last = last->next;
	last->next = NULL;
	last->val = val;
}

typedef struct
{
	int first;
	int last;
}Pair;
typedef struct
{
	int value;
	int priority;
}Data;
typedef struct
{
	int max_size;
	int cur_size;
	Data* queue_array;

}PriorityQueue;
int Left(int index)
{
	return index * 2 + 1;
}

int Right(int index)
{
	return index * 2 + 2;
}

int Parent(int index)
{
	return (index - 1) / 2;
}

void swap_data(Data* d1, Data* d2)
{
	Data tmp_data = *d1;
	*d1 = *d2;
	*d2 = tmp_data;
}


void addEdge(int** matrix, int N1, int N2, int dis);
void newNode(int** matrix);
void dijkstra_matrix(int** G, int v, int s);
void heapify(PriorityQueue* q, int index);
void insert_element(PriorityQueue* q, int value, int priority);
Data heap_extract_min(PriorityQueue* q);
bool czyCykle(int** matrix, int n);
void bfs(int u, int** G);


int N; //liczba wierzcholkow
int L; //liczba polaczen miedzy wierzcholkami

int main()
{
	printf("=== PROGRAM DO OPERACJI NA GRAFACH NIESKIEROWANYCH=== \n");
	printf("Podaj kolejno liczbe wierzcholkow i krawedzi: ");
	if (scanf_s(" %d %d", &N, &L) < 2 || ferror(stdin))
	{
		printf("Wystapil blad. Wprowadzone znaki musza byc liczbami calkowitymi wiekszymi od 1 i 0.\n");
		return 0;
	}
	if (N < 2 || L < 1 || L<N - 1 || L>N * (N - 1) / 2)  //min wierzcholkow to n-1 a max to n*(n-1)/2
	{
		printf("Z podanych liczb nie da sie stworzyc grafu nieskierowanego.");
		return 0;
	}
	int** Graph = (int**)malloc(N * sizeof(int*)); //Tworzenie macierzy z grafem
	for (int i = 0; i < N; i++)
	{
		Graph[i] = (int*)malloc(N * sizeof(int));
	}
	for (int i = 0; i < N; ++i)  //uzupe³nienie macierzy zerami
	{
		for (int j = 0; j < N; ++j)
		{
			Graph[i][j] = 0;
		}
	}
	printf("Wierzcholki musza byc z przedzialu 0-%d.", N - 1);
	int N1, N2, distance;
	for (int i = 0; i < L; ++i)   //dodawanie krawedzi do grafu
	{
		printf("\n%d. krawedz. Podaj kolejno numery wierzcholkow i odleglosc miedzy nimi: ", i + 1);
		if (scanf_s("%d %d %d", &N1, &N2, &distance) < 3 || ferror(stdin))
		{
			printf("Podano niepoprawne dane.\n");
			return 0;
		}
		else if (N1 >= N || N2 >= N || N1 == N2)  //podane wierzcholki nie moga byc rowne sobie
		{
			printf("Podano niepoprawne dane. Ponow. \n");
			i--;
		}
		else if (Graph[N1][N2] != 0)  //istniejace polaczenie
		{
			printf("To polaczenie juz istnieje. Ponow.\n");
			i--;
		}
		else if (distance <= 0)  //za maly dystans
		{
			printf("Odleglosc musi byc wieksze od 0. Ponow.\n");
			i--;
		}
		else
			addEdge(Graph, N1, N2, distance);  //dodanie krawedzi grafu
	}
	int choice;
	while (1) //Petla glowna programu
	{
		for (int i = 0; i < N; ++i)  //Wyswietlenie macierzy. Mozna to usunac jesli to bez sensu
		{
			for (int j = 0; j < N; ++j)
			{
				printf("%d ", Graph[i][j]);
			}
			printf("\n");
		}
		printf("\nCo chcesz teraz zrobic z grafem?\n\n");
		printf("1. Dodaj nowy wierzcholek.\n");
		printf("2. Usun wierzcholek.\n");
		printf("3. Wyszukaj najkrotsza sciezke.\n");
		printf("4. Wyszukaj najdluzsza sciezke.\n");
		printf("5. Wykryj cykle.\n");
		printf("Wpisz numer polecenia: ");
		if (scanf_s("%d", &choice) < 1 || ferror(stdin))
		{
			printf("Niepoprawne dane.");
			exit(0);
		}
		switch (choice)
		{
		case 1:  //nowy wierzcholek
		{
			N++;
			int** temp = (int**)realloc(Graph, sizeof(*Graph) * N);  //powiekszenie macierzy o 1
			Graph = temp;
			Graph[N - 1] = (int*)malloc(sizeof(int) * N);
			newNode(Graph);
			break;
		}
		case 2:
		{
			printf("\nKtory usunac? ");
			int nr;
			scanf_s("%d", &nr);
			if (nr < 0 || nr >= N)
			{
				getc(stdin);
				printf("\nBledny numer \n");
				break;
			}
			N--;
			int** temp = (int**)malloc(N * sizeof(int*)); //Tworzenie macierzy z grafem
			for (int i = 0; i < N; i++)
			{
				temp[i] = (int*)malloc(N * sizeof(int));
			}
			for (int i = 0; i < N; ++i)
			{
				for (int j = 0; j < N; ++j)
				{
					int i1 = i;
					int j1 = j;
					if (i >= nr)i1++;
					if (j >= nr)j1++;

					temp[i][j] = Graph[i1][j1];
				}
			}
			Graph = temp;
		}
		break;

		case 3:
		{
			printf("\nZ ktorego wierzcholka? \n");
			int nr;
			scanf_s("%d", &nr);
			getc(stdin);
			if (nr < 0 || nr >= N)
			{
				printf("\nBledny numer \n\n");
			}
			else
			{
				dijkstra_matrix(Graph, N, nr);
			}
			break;
		}
		case 4:
		{
			if (!czyCykle(Graph, N))
			{
				printf("\nZ ktorego wierzcholka? \n");
				int nr;
				scanf_s("%d", &nr);
				getc(stdin);

				if (nr < 0 || nr >= N)
				{
					printf("\nBledny numer \n\n");
					break;
				}
				else
					bfs(nr, Graph);
			}
			else
			{
				printf("\nOpcja niedostepna dla grafow z cyklem.\n\n");
			}
			break;
		}
		case 5:
		{
			if (czyCykle(Graph, N))
				printf("\nGraf ma cykle\n\n");
			else printf("\nGraf nie ma cykli\n\n");
			break;
		}
		default:
		{
			printf("Wrong mode\n");
		}

		}
	}
	return 0;
}

void addEdge(int** matrix, int N1, int N2, int dis)  //dodanie nowej krawedzi grafu
{
	/* Dodaje wierzcholkom N1 i N2 polaczenia miedzy soba (w liscie wiazanej) */
	addVal(N1, adj, N2);
	addVal(N2, adj, N1);

	matrix[N1][N2] = dis;
	matrix[N2][N1] = dis;
}

void newNode(int** matrix)
{
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if (i == N - 1 || j == N - 1)
			{
				matrix[i][j] = 0;
			}
		}
	}
	printf("Podaj ile krawedzi ma wychodzic z tego wierzcholka: "); //dodawanie krawedzi do nowego wierzcholka
	int newL = 0;
	while (newL <= 0 || newL > N - 1)
	{
		if (scanf_s("%d", &newL) < 1 || ferror(stdin))
		{
			printf("Wyrazenie musi byc liczba.\n");
			exit(0);
		}
		if (newL <= 0 || newL > N - 1)
		{
			printf("Liczba musi byc z zakresu od 1 do %d. ", N - 1);
		}
	}
	printf("Podawaj kolejno dwie liczby. Pierwsza to wierzcholek z ktora tworzysz krawedz a druga to waga polaczenia.");
	int newN, newDis;
	for (int i = 0; i < newL; ++i)
	{
		printf("\n%d. krawedz. ", i + 1);
		if (scanf_s("%d %d", &newN, &newDis) < 2 || ferror(stdin))
		{
			printf("Podano niepoprawne dane.\n");
			exit(0);
		}
		else if (newN >= N || newN == N - 1)  //podane wierzcholki nie moga byc rowne sobie
		{
			printf("Podano niepoprawne dane. Ponow. \n");
			i--;
		}
		else if (matrix[N - 1][newN] != 0)  //istniejace polaczenie
		{
			printf("To polaczenie juz istnieje. Ponow.\n");
			i--;
		}
		else if (newDis <= 0)  //za maly dystans
		{
			printf("Odleglosc musi byc wieksze od 0. Ponow.\n");
			i--;
		}
		else
			addEdge(matrix, N - 1, newN, newDis);  //dodanie krawedzi grafu
	}
}

void dijkstra_matrix(int** G, int v, int s)
{

	int* d = (int*)malloc(v * sizeof(int)), * parent = (int*)malloc(v * sizeof(int));
	for (int i = 0; i < v; i++)
	{
		d[i] = INT_MAX;
		parent[i] = -1;
	}

	PriorityQueue* Q = (PriorityQueue*)calloc(1, sizeof(PriorityQueue));
	Q->cur_size = 0;
	Q->max_size = v;
	Q->queue_array = (Data*)calloc(1, sizeof(Data));

	insert_element(Q, s, 0);
	d[s] = 0;

	Data vertex;
	while (Q->cur_size != 0)
	{
		vertex = heap_extract_min(Q);
		for (int i = 0; i < v; i++)
		{
			if (G[vertex.value][i] != 0 && (d[vertex.value] + G[vertex.value][i]) < d[i])
			{
				parent[i] = vertex.value;
				d[i] = d[vertex.value] + G[vertex.value][i];
				insert_element(Q, i, d[i]);
			}
		}
	}

	for (int i = 0; i < v; i++)
	{
		if (i != s) {
			printf("sciezka do: %d\nwaga: %d\nkolejnosc:", i, d[i]);

			int j = i;
			while (j != s)
			{
				printf("%d -> ", j);
				j = parent[j];
			}

			printf("%d\n\n", s);
		}
	}

	free(Q);
	free(d);
	free(parent);
}

void insert_element(PriorityQueue* q, int value, int priority)
{
	if (q->cur_size == q->max_size) return;
	q->queue_array[q->cur_size].value = value;
	q->queue_array[q->cur_size].priority = priority;

	int index = q->cur_size;
	while (index > 0 && q->queue_array[Parent(index)].priority > q->queue_array[index].priority)
	{
		swap_data(&(q->queue_array[Parent(index)]), &(q->queue_array[index]));
		index = Parent(index);
	}

	q->cur_size++;
}

void heapify(PriorityQueue* q, int index)
{
	int left = Left(index);
	int right = Right(index);
	int min = index;

	if (left < q->cur_size && q->queue_array[left].priority < q->queue_array[index].priority) min = left;
	if (right < q->cur_size && q->queue_array[right].priority < q->queue_array[min].priority) min = right;

	if (min != index)
	{
		swap_data(&(q->queue_array[index]), &(q->queue_array[min]));
		heapify(q, min);
	}
}

Data heap_extract_min(PriorityQueue* q)
{
	if (q->cur_size == 0)
	{
		perror("empty queue");
		exit(-1);
	}

	Data min = q->queue_array[0];
	q->cur_size--;
	q->queue_array[0] = q->queue_array[q->cur_size];
	heapify(q, 0);

	return min;
}

bool dfs(int** matrix, int n, int curr, int parent, bool* visited)
{
	visited[curr] = true;
	for (int i = 0; i < n; i++)
	{
		if (matrix[curr][i] != 0)
		{

			if (!visited[i])
			{
				if (dfs(matrix, n, i, curr, visited))return true;
			}
			else if (i != parent)return true;
		}
	}
	return false;
}

bool czyCykle(int** matrix, int n)
{
	bool* visited = (bool*)malloc(n * sizeof(bool));
	for (int i = 0; i < n; i++)
	{
		visited[i] = false;
	}
	for (int i = 0; i < n; i++)
	{
		if (!visited[i])
		{
			if (dfs(matrix, n, i, -1, visited))return true;
		}
	}
	return false;
}

void bfs(int u, int** G)
{
	/* Przygotowanie zmiennej dystansu */
	int* dis = (int*)malloc(N * sizeof(int));
	int i;
	for (i = 0; i < N; i++)
		dis[i] = -1;

	/* Inicjalizacja kolejki zawierajaca wazone wierzcholki grafu */
	PriorityQueue* Q = (PriorityQueue*)calloc(1, sizeof(PriorityQueue));
	Q->cur_size = 0;
	Q->max_size = N;
	Q->queue_array = (Data*)calloc(1, sizeof(Data));

	/* Dodaje do kolejki wierzcholek poczatkowy i ustawia jego wage na 0 */
	insert_element(Q, u, 0);

	/* Dystans do niej 0*/
	dis[u] = 0;

	/* Petla az do zakonczenia elementow w kolejce */
	while (Q->cur_size != 0)
	{
		/* Dostaje najblizszy? wierzcholek z kolejki */
		Data t = heap_extract_min(Q);

		/* Petla przechodzaca przez wszystkie pobliskie wierzcholki od otrzymanego (t) */
		ListNode* ptr = adj[t.value].next;
		while (ptr != NULL)
		{
			/* Numer wierzcholka */
			int v = ptr->val;

			/* Jesli dystans nie jest -1, zeby unikac ponownego przeliczania (i wczesniej zapetlen ale juz chyba nie macie) */
			if (dis[v] == -1)
			{
				/* Wrzuca kolejny wierzcholek do kolejki razem z numerem i waga i ustawia odleglosc do tego miejsca */
				dis[v] = 0;
				dis[v] = dis[t.value] + G[t.value][v];
				insert_element(Q, v, dis[v]);
			}
			/* Przez to idzie dalej petla (w elementach) */
			ptr = ptr->next;
		}
	}

	int maxDis = 0;
	int nodeIdx = 0;

	/* Petla sprawdza ktora droga do ktorego wierzcholka najdluzsza i zapisuje */
	int j;
	for (j = 0; j < N; j++)
	{
		if (dis[j] > maxDis)
		{
			maxDis = dis[j];
			nodeIdx = j;
		}
	}

	/* A tutaj wypisuje, wyrzuccie ten pair bo niepotrzebny i uzyjcie maxdis i nodeidx */
	Pair p;
	p.first = nodeIdx;
	p.last = maxDis;
	printf("Najdalszy wierzcholek: %d, odleglosc: %d\n\n", p.first, p.last);
	free(Q);
}