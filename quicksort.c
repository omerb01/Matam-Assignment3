//
// Created by Omer on 03/12/2017.
//

#include <stdio.h>
#include <stdlib.h>

typedef void *Element;

typedef int (*cmpFunction)(Element, Element);

void swap(Element *p1, Element *p2) {
    Element temp = *p1;
    *p1 = *p2;
    *p2 = temp;
}

int partition(Element *array, int n, cmpFunction compare) {
    int left = 0, right = n - 1;
    Element pivot = array[0];

    while (left <= right) {
        while (left <= right && compare(array[left], pivot) <= 0) left++;
        while (left <= right && compare(array[right], pivot) > 0) right--;

        if (left < right) {
            swap(array + left, array + right);
            left++;
            right--;
        }
    }
    return right;
}

void quick_sort(Element *array, int n, cmpFunction compare) {
    if (n < 2) return;

    int t = partition(array, n, compare);
    swap(&array[0], &array[t]);

    quick_sort(array, t, compare);
    quick_sort(array + t + 1, n - t - 1, compare);
}

int cmpInt(Element e1, Element e2) {
    int* n1 = (int*)(e1);
    int* n2 = (int*)(e2);

    if(*n1 == *n2) return 0;
    else if(*n1 > *n2) return 1;
    else return -1;
}

int main() {

    int array[10] = {5,2,7,3,1,7,2,9,3,0};
    void* parray[10];
    for (int i = 0; i < 10; ++i) {
        parray[i] = array+i;
    }

    quick_sort(parray, 10, cmpInt);

    printf("\n");
    for (int j = 0; j < 10; ++j) {
        printf("%d, ", *(int*)parray[j]);
    }
    printf("\n");

    return 0;
}
