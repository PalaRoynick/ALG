/*
    Josephus problem. Knuth [I-1.3.2(2)-22].
    Use linked lists. ~O(NM) comlexity. Not the best one.
*/
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

typedef struct Node
{
    int data;
    struct Node *next;
} Node;

Node *allocate(int N)
{
    Node *nodes = calloc(N, sizeof(Node));
    return nodes;
}

void iota(Node *nodes, int size)
{
    int i = 0;
    for (; i < size - 1; ++i)
    {
        nodes[i].data = i + 1;
        nodes[i].next = &nodes[i + 1];
    }
    nodes[size - 1].data = i + 1;
}

void print(Node *head)
{
    Node *node = head;
    while (node)
    {
        printf("%d -> ", node->data);
        node = node->next;
    }
    printf("\n");
}

void deallocate(Node *head)
{
    free(head);
}

Node *find_last(Node *start)
{
    Node *node = start;
    while (node->next)
    {
        node = node->next;
    }
    return node;
}

void make_cycle(Node *start)
{
    Node *last = find_last(start);
    last->next = start;
}

void remove_node(Node *node)
{
    assert(node->next);
    Node *next_node = node->next;
    node->data = next_node->data;
    node->next = next_node->next;
    // free(next_node); // safe?
}

int is_cycle(Node *node)
{
    if (node == NULL) return 0;
    Node *slow = node;
    Node *fast = node;
    while (slow && fast && fast->next)
    {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) return 1;
    }
    return 0;
}

// we assume that the cycle invariant of list is supported
Node *make_step(Node *start, int m, int *order)
{
    static int count = 1;
    assert(is_cycle(start));
    Node *node = start;
    if (node->next == node)
    {
        printf("%d ~ ", start->data);
        order[node->data - 1] = count++;
        return NULL;
    }
    for (int i = 0; i < m - 1; ++i)
    {
        node = node->next;
    }
    printf("%d ~ ", node->data);
    order[node->data - 1] = count++;
    remove_node(node); // correctly "removes" the node from list
    start = node;
    return start;
}

int main()
{
    int N = 8;
    Node *nodes = allocate(N);

    iota(nodes, N);

    print(nodes);
    print(find_last(nodes));
    print(nodes);

    make_cycle(nodes);
    Node *start = nodes;

    int *order = calloc(N, sizeof(int));
    while (start) { start = make_step(start, 4, order); }

    printf("\n");
    for (int i = 0; i < N; ++i)
    {
        printf("%d ", order[i]);
    }

    free(order);
    free(nodes);
}