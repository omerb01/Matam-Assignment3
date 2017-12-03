//
// Created by Ilya on 12/3/2017.
//
#include <stdlib.h>
#include <stdio.h>

typedef struct node_t *Node;
struct node_t {
    int n;
    Node next;
};

void destroyList(Node list) {
}

Node listCopy(Node list) {
    if (list == NULL)
        return NULL;

    Node left_node = malloc(sizeof(*left_node));
    if (left_node == NULL) return NULL;
    left_node->n = list->n;

    Node result = left_node;

    list = list->next;
    while (list != NULL) {
        Node right_node = malloc(sizeof(*right_node));
        if (right_node == NULL) {
            destroyList(result);
            return NULL;
        }
        right_node->n = list->n;
        left_node->next = right_node;
        left_node = left_node->next;
        list = list->next;
    }

    left_node->next = NULL;
    return result;
}

void reverseList(Node list, Node *head) {
    if (list == NULL) {
        return;
    }
    reverseList(list->next, head);
    list->n = (*head)->n;
    *head = (*head)->next;
}

Node listCopyReversed(Node list) {
    Node copied_list = listCopy(list);
    Node temp_head = listCopy(list);

    reverseList(copied_list, &temp_head);
    destroyList(temp_head);
    return copied_list;
}

Node getLastNode(Node node) {
    while (node->next != NULL) {
        node = node->next;
    }
    return node;
}

Node listJoinAlternating(Node *node_lists_array, int n) {
    if (n == 0) return NULL;

    Node left_list = listCopy(node_lists_array[0]);
    Node right_list;
    Node head = left_list;

    for (int i = 1; i < n; ++i) {
        if (i % 2 != 0) {
            right_list = listCopyReversed(node_lists_array[i]);
        } else {
            right_list = listCopy(node_lists_array[i]);
        }

        if(right_list == NULL){
            destroyList(head);
            return NULL;
        }

        getLastNode(left_list)->next = right_list;
        left_list = right_list;
    }

    return head;
}

int main() {

    struct node_t n1;
    struct node_t n2;
    struct node_t n3;
    struct node_t n4;
    struct node_t n5;
    struct node_t n6;
    struct node_t n7;
    struct node_t n8;

    n1.n = 1;
    n2.n = 2;
    n3.n = 3;
    n4.n = 4;
    n5.n = 5;
    n6.n = 6;
    n7.n = 7;
    n8.n = 8;

    n1.next = &n2;
    n2.next = NULL;
    n3.next = &n4;
    n4.next = NULL;
    n5.next = &n6;
    n6.next = NULL;
    n7.next = &n8;
    n8.next = NULL;

    struct node_t* array[4] = {&n1, &n3, &n5, &n7};

    Node result = listJoinAlternating(array, 4);

    while(result != NULL){
        printf("%d => ", result->n);
        result = result->next;
    }
    printf("NULL\n");

    return 0;
}