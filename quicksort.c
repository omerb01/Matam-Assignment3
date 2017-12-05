//
// Created by Omer on 03/12/2017.
//

#include <stdio.h>
#include <stdlib.h>

typedef void *Element;

typedef int (*cmpFunction)(Element, Element);

void swap(Element *p1, Element *p2) {
    int temp = *(int*)p1;
    *(int*)p1 = *(int*)p2;
    *(int*)p2 = temp;
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

    int array[4] = {4,3,3,1};
    int* parray[4];
    for (int i = 0; i < 4; ++i) {
        parray[i] = array+i;
    }

    quick_sort((void**)parray, 4, cmpInt);

    for (int j = 0; j < 4; ++j) {
        printf("%d, ", *parray[j]);
    }

    return 0;
}
