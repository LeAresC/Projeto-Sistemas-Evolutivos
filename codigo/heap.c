#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_W 100  // Ajuste conforme seu grid
#define MAX_H 100
#define INF 1e9

// Estrutura para guardar no Heap
typedef struct {
    int x, y;
    float custo;
} Node;

// O Heap é apenas um vetor estático + contador de tamanho
// Tamanho máximo = total de celulas do grid
Node heap[MAX_W * MAX_H];
int heap_size = 0;

// --- FUNÇÕES DO HEAP (Priority Queue) ---

void swap(Node *a, Node *b) {
    Node temp = *a;
    *a = *b;
    *b = temp;
}

// Sobe o elemento (usado no Push)
void bubble_up(int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (heap[index].custo < heap[parent].custo) {
            swap(&heap[index], &heap[parent]);
            index = parent;
        } else {
            break;
        }
    }
}

// Desce o elemento (usado no Pop)
void bubble_down(int index) {
    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < heap_size && heap[left].custo < heap[smallest].custo)
        smallest = left;

    if (right < heap_size && heap[right].custo < heap[smallest].custo)
        smallest = right;

    if (smallest != index) {
        swap(&heap[index], &heap[smallest]);
        bubble_down(smallest);
    }
}

// Inserir na fila
void pq_push(int x, int y, float custo) {
    heap[heap_size].x = x;
    heap[heap_size].y = y;
    heap[heap_size].custo = custo;
    bubble_up(heap_size);
    heap_size++;
}

// Remover o menor (extrair min)
Node pq_pop() {
    Node min_node = heap[0];
    heap[0] = heap[heap_size - 1]; // Move o último para o topo
    heap_size--;
    bubble_down(0); // Reorganiza
    return min_node;
}

int pq_empty() {
    return heap_size == 0;
}