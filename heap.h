typedef struct
{
    int x, y;
    float custo;
} Node;

void swap(Node *a, Node *b);
void bubble_down(int index);
void pq_push(int x, int y, float custo);
Node pq_pop();
int pq_empty();