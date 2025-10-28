#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int vertex;

typedef struct node {
  vertex w;          // Vértice vecino
  struct node *next; // Siguiente nodo
} *link;

typedef struct {
  int V;     // Número de vértices (G->V)
  link *adj; // Arreglo de listas de adyacencia (G->adj)
} *Graph;

static int cnt;
int pre[1000];
static int indent;

link NEWnode(vertex w, link next) {
  link a = (link)malloc(sizeof(struct node));
  if (a == NULL) {
    printf("Error: Falta de memoria\n");
    exit(1);
  }
  a->w = w;
  a->next = next;
  return a;
}

Graph GRAPHinit(int V) {
  Graph G = (Graph)malloc(sizeof(*G));
  if (G == NULL) {
    printf("Error: Falta de memoria\n");
    exit(1);
  }
  G->V = V;
  G->adj = (link *)malloc(V * sizeof(link));
  if (G->adj == NULL) {
    printf("Error: Falta de memoria\n");
    exit(1);
  }
  for (vertex v = 0; v < V; ++v) {
    G->adj[v] = NULL;
  }
  return G;
}

void GRAPHinsertArc(Graph G, vertex v, vertex w) {
  link newNode = NEWnode(w, NULL);
  if (G->adj[v] == NULL) {
    G->adj[v] = newNode;
  } else {
    link a = G->adj[v];
    while (a->next != NULL) {
      a = a->next;
    }
    a->next = newNode;
  }
}

static void print_indent() {
  for (int i = 0; i < indent; ++i) {
    printf("  ");
  }
}

static void dfsR(Graph G, vertex v) {
  pre[v] = cnt++; // [cite: 42]

  for (link a = G->adj[v]; a != NULL; a = a->next) {
    vertex w = a->w;

    print_indent();
    printf("%d-%d", v, w);

    if (pre[w] == -1) {
      printf(" dfsR(G, %d)\n", w);

      indent++;
      dfsR(G, w);
      indent--;
      print_indent();
      printf("%d\n", w);
    } else {
      printf("\n");
    }
  }
}

void GRAPHdfs(Graph G) {
  cnt = 0;
  indent = 0;

  for (vertex v = 0; v < G->V; ++v) //
    pre[v] = -1;                    //

  for (vertex v = 0; v < G->V; ++v) {
    if (pre[v] == -1) {
      print_indent();
      printf("%d dfsR(G, %d)\n", v, v);
      indent++;
      dfsR(G, v);
      indent--;
      print_indent();
      printf("%d\n", v);
    }
  }
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Uso: %s <archivo_del_grafo>\n", argv[0]);
    printf("El formato del archivo se describe en el Ejercicio 5 [cite: "
           "266-283].\n");
    return 1;
  }

  FILE *file = fopen(argv[1], "r");
  if (file == NULL) {
    printf("Error: No se pudo abrir el archivo %s\n", argv[1]);
    return 1;
  }

  int V;
  // Leer el número de vértices
  if (fscanf(file, "%d", &V) != 1) {
    printf("Error: Formato de archivo incorrecto (no se pudo leer V).\n");
    fclose(file);
    return 1;
  }

  if (V > 1000) {
    printf("Error: El grafo es demasiado grande (max 1000 vértices).\n");
    fclose(file);
    return 1;
  }

  Graph G = GRAPHinit(V);

  char line_buffer[2048];

  fgets(line_buffer, sizeof(line_buffer), file);
  for (int i = 0; i < V; ++i) {
    if (fgets(line_buffer, sizeof(line_buffer), file) == NULL) {
      printf("Error: El archivo no tiene V líneas de adyacencia (línea %d).\n",
             i + 2);
      fclose(file);
      return 1;
    }

    char *token = strtok(line_buffer, " \t\n");

    if (token == NULL) {
      printf("Error: Formato de archivo incorrecto (línea %d está vacía).\n",
             i + 2);
      fclose(file);
      return 1;
    }

    vertex v_source = atoi(token);

    if (v_source != i) {
      printf("Advertencia: Se esperaba el vértice %d en la línea %d, pero se "
             "encontró %d.\n",
             i, i + 2, v_source);
    }

    token = strtok(NULL, " \t\n");
    while (token != NULL) {
      vertex w_neighbor = atoi(token);
      GRAPHinsertArc(G, v_source, w_neighbor);
      token = strtok(NULL, " \t\n");
    }
  }
  fclose(file);

  printf("--- Inicio del Rastreo de GRAPHdfs ---\n");
  GRAPHdfs(G);
  printf("Fin del Rastreo\n\n");
  printf("Vector pre[] final\n");
  printf("w:      ");
  for (int w = 0; w < G->V; ++w)
    printf("%3d ", w);
  printf("\npre[w]: ");
  for (int w = 0; w < G->V; ++w)
    printf("%3d ", pre[w]);
  printf("\n");

  // Liberar memoria
  for (vertex v = 0; v < G->V; ++v) {
    link a = G->adj[v];
    while (a != NULL) {
      link temp = a;
      a = a->next;
      free(temp);
    }
  }
  free(G->adj);
  free(G);

  return 0;
}
