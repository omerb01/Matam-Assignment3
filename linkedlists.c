//
// Created by Ilya on 12/3/2017.
//
#include <stdlib.h>


typedef struct node_t* Node;
struct node_t {
    int n;
    Node next;
};

void reverseList(Node list, Node* head){
    if(list == NULL){
        return;
    }
    reverseList(list->next,head);
    list->n = (*head)->n;
    (*head) = (*head)->next;
}

void destroyList(Node list){
}

Node listCopy(Node list){
    if(list == NULL)
        return NULL;
    
    Node head = malloc(sizeof(*head));
    if (head == NULL) {
        destroyList(head);
        return NULL;
    }
    head->n = list->n;
    head->next = list->next;
    list=head->next;

    while(list != NULL) {
        Node node_list_copy = malloc(sizeof(*node_list_copy));
        if (node_list_copy == NULL) {
            destroyList(head);
            return NULL;
        }
        node_list_copy->n = list->n;
        node_list_copy->next = list->next;
        list = list->next;
    }
    return head;
}


Node listCopyReversed(Node list){
    Node copied_list = listCopy(list);
    Node *head = &copied_list;
    reverseList(copied_list, head);
    return copied_list;
}

Node getLastNode(Node node){
    while(node->next != NULL){
        node = node->next;
    }
    return node;
}

Node listJoinAlternating(Node *node_lists_array, int n){
    Node node, head;
    if(n==0) return NULL;
    Node first = listCopy(node_lists_array[0]);
    head = first;
    for (int i = 1; i < n; ++i) {
        if(i%2 != 0){
            node = listCopyReversed(node_lists_array[i]);
        }
        else{
            node = listCopy(node_lists_array[i]);
        }
        node->next = getLastNode(first);
        first = node;
    }
    return head;
}
