#include <stdio.h>
#include <stdlib.h>

#define MAX_V 1000
typedef struct Node {
    int w;
    struct Node *next;
} Node;

typedef struct {
    int V;
    Node **adj;
} Graph;

typedef struct {
    int *items;
    int top;
} Stack;

Stack* crearStack(int size) {
    Stack *s = malloc(sizeof(Stack));
    s->items = malloc(size * sizeof(int));
    s->top = 0;
    return s;
}

void push(Stack *s, int item) {
    s->items[s->top++] = item;
}

int pop(Stack *s) {
    return s->top > 0 ? s->items[--s->top] : -1;
}

int empty(Stack *s) {
    return s->top == 0;
}

Graph* createGraph(int V) {
    Graph *G = malloc(sizeof(Graph));
    G->V = V;
    G->adj = malloc(V * sizeof(Node*));
    for (int i = 0; i < V; i++) G->adj[i] = NULL;
    return G;
}

void addEdge(Graph *G, int v, int w) {
    Node *new = malloc(sizeof(Node));
    new->w = w;
    new->next = G->adj[v];
    G->adj[v] = new;
}

void dfsIterativo(Graph *G, int start, int *pre, int *cnt) {
    Stack *s = crearStack(G->V);
    push(s, start);
    pre[start] = (*cnt)++;
    
    printf("DFS desde vértice %d:\n", start);
    
    while (!empty(s)) {
        int v = pop(s);
        printf("Visitar: %d (pre[%d]=%d)\n", v, v, pre[v]);
        
        for (Node *a = G->adj[v]; a != NULL; a = a->next) {
            int w = a->w;
            if (pre[w] == -1) {
                pre[w] = (*cnt)++;
                push(s, w);
                printf("  Descubrir: %d->%d\n", v, w);
            }
        }
    }
    free(s->items);
    free(s);
}

void showGraph(Graph *G) {
    printf("Listas de adyacencia:\n");
    for (int v = 0; v < G->V; v++) {
        printf("%d: ", v);
        for (Node *a = G->adj[v]; a != NULL; a = a->next) {
            printf("%d ", a->w);
        }
        printf("\n");
    }
    printf("\n");
}

int main() {
    Graph *G = createGraph(7);
    addEdge(G, 0, 1);
    addEdge(G, 0, 6);
    
    addEdge(G, 1, 0);
    addEdge(G, 1, 6);
    addEdge(G, 1, 3);
    
    addEdge(G, 2, 4);
    
    addEdge(G, 3, 1);
    
    addEdge(G, 4, 2);
    
    addEdge(G, 6, 0);
    addEdge(G, 6, 1);
    
    printf("Grafo de entrada\n");
    showGraph(G);
    
    int pre[MAX_V], cnt = 0;
    for (int i = 0; i < G->V; i++) pre[i] = -1;
    
    printf("DFS Iterativo\n");
    for (int v = 0; v < G->V; v++) {
        if (pre[v] == -1) {
            dfsIterative(G, v, pre, &cnt);
        }
    }
    
    printf("Resultado final\n");
    printf("Vector pre[]:\n");
    for (int i = 0; i < G->V; i++) {
        printf("pre[%d] = %d\n", i, pre[i]);
    }
    
    printf("Orden de descubrimiento: \n");
    for (int i = 0; i < cnt; i++) {
        for (int v = 0; v < G->V; v++) {
            if (pre[v] == i) {
                printf("%d ", v);
                break;
            }
        }
    }
    printf("\n");
    
    return 0;
}