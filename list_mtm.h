#ifndef LIST_H_
#define LIST_H_

#include <stdbool.h>
/**
 * Generic List Container
 *
 * Implements a list container type.
 * The list his an internal iterator for external use. At any given time, the
 * iterator must point either to a valid element in the list, or to NULL.
 *
 * The following functions are available:
 *
 *   listCreate               - Creates a new empty list
 *   listDestroy              - Deletes an existing list and frees all
 *                              resources
 *   listCopy                 - Copies an existing list
 *   listGetSize              - Returns the size of a given list
 *   listInsertFirst          - Inserts an element in the beginning of the list
 *   listInsertLast           - Inserts an element in the end of the list
 *   listInsertBeforeCurrent  - Inserts an element right before the place of
 *                              internal iterator
 *   listInsertAfterCurrent   - Inserts an element right after the place of the
 *                              internal iterator
 *   listRemoveCurrent        - Removes the element pointed by the internal
 *                              iterator
 *   listGetCurrent           - Return the current element (pointed by the
 *                              internal iterator)
 *   listGetFirst             - Sets the internal iterator (also called current
 *                              element) to be the first element in the list and
 *                              return it
 *   listGetNext              - Advances the list's iterator to the next element
 *                              and return it
 *   listSort                 - Sorts the list according to a given criteria
 *   listFilter               - Creates a copy of an existing list, filtered by
 *                              a boolean predicate
 *   listClear                - Clears all the data from the list
 */

/** Type for defining the list */
typedef struct List_t *List;

/** Type used for returning error codes from list functions */
typedef enum ListResult_t {
	LIST_SUCCESS,
	LIST_NULL_ARGUMENT,
	LIST_OUT_OF_MEMORY,
	LIST_INVALID_CURRENT,
} ListResult;

/** Element data type for list container */
typedef void* ListElement;

/**
 * Type of function for copying an element of the list.
 *
 * Such a function should be supplied to a list to allow it to copy its
 * elements. The function supplied should be able to dynamically copy the
 * object and return a pointer to a newly allocated object.
 * If this function fails for some reason, it should return NULL.
 *
 * For example, here is a proper function for copying a string that can be
 * used for storing strings in a list:
 * @code
 * ListElement copyString(ListElement str) {
 *   assert(str);
 *   char* copy = malloc(strlen(str) + 1);
 *   return copy ? strcpy(copy, str) : NULL;
 * }
 * @endcode
 */
typedef ListElement (*CopyListElement)(ListElement);

/** Type of function for deallocating an element of the list */
typedef void (*FreeListElement)(ListElement);

/**
 * Use this type to pass extra information needed by the sorting function when
 * calling listSort. (See the example for a CompareListElements function)
 */
typedef void* ListSortKey;

/**
 * Type of function that can be used by the list for sorting or finding.
 * This function should return a value greater than 0 if the first element is
 * greater, 0 if they're equal and a number smaller than 0 if the second
 * element is greater.
 * For example, the following function can use to sort a list of integers
 * according to which is closer to a given number (given as the key)
 * @code
 * int closerTo(ListElement num1, ListElement num2, ListSortKey value) {
 *   int distance1 = abs(*(int*)num1 - *(int*)value);
 *   int distance2 = abs(*(int*)num2 - *(int*)value);
 *   return distance1 - distance2;
 * }
 * @endcode
 */
typedef int (*CompareListElements)(ListElement, ListElement, ListSortKey);

/**
 * Use this type to pass extra information needed by the filtering function
 * when calling listFilter. (See the example for a FilterListElement function)
 */
typedef void* ListFilterKey;

/**
 * Function used for creating a filtered copy of a list.
 * A element is said to pass filtering if the function returns true.
 * For example, the following function can be used to filter a list of strings
 * from short strings, and leave only strnigs which are longer than the given
 * length:
 * @code
 * bool isLongerThan(ListElement string, ListFilterKey key) {
 *   return strlen(string) > *(int*)key;
 * }
 * @endcode
 */
typedef bool (*FilterListElement)(ListElement, ListFilterKey);

/**
 * Allocates a new List.
 *
 * Creates a new empty list. This function receives the functions which will be
 * used for copying elements into the list and freeing them when needed.
 * For example, in order to create a list of strings one need to define these
 * two functions:
 * @code
 * ListElement copyString(ListElement str);
 * void freeString(ListElement str);
 * @endcode
 * And then create a list as follows:
 * @code
 * List listOfStrings = listCreate(copyString, freeString);
 * @endcode
 *
 * The iterator of the new list should point to NULL.
 *
 * @param copyElement Function pointer to be used for copying elements into
 * the list or when copying the list.
 * @param freeElement Function pointer to be used for removing elements from
 * the list.
 * @return
 * NULL - if one of the parameters is NULL or allocations failed.
 * A new List in case of success.
 */
List listCreate(CopyListElement copyElement, FreeListElement freeElement);

/**
 * Creates a copy of target list.
 *
 * The new copy will contain all the elements from the source list in the same
 * order and will use the same functions as the original list for copying and
 * freeing elements.
 *
 * The iterator of the new list should point to the same element that the
 * original iterator is pointing to in the original list.
 *
 * @param list The target list to copy
 * @return
 * NULL if a NULL was sent or a memory allocation failed.
 * A List containing the same elements with same order as list otherwise.
 */
List listCopy(List list);

/**
 * Returns the number of elements in a list
 *
 * The iterator should not change.
 *
 * @param list The target list which size is requested.
 * @return
 * -1 if a NULL pointer was sent.
 * Otherwise the number of elements in the list.
 */
int listGetSize(List list);

/**
 * Sets the internal iterator to the first element and retrieves it.
 *
 * The list has an internal iterator (also called current element) to allow
 * iteration over the list's elements. This function sets the iterator to point
 * to the first element in the list and return it.
 * Use this to start iteraing over the list, searching from the beginning of
 * the list and/or get the first element in the list.
 * (To continue iteration use listGetNext)
 * @code
 * void f(List list) {
 *   ListElement first = listGetFirst(list);
 *   printf("The first element is at address %x\n", first);
 * }
 * @endcode
 *
 * Note: the element which is stored in the list is returned, not a copy.
 *
 * @param list The list for which to set the iterator and return the first
 * element.
 * @return
 * NULL is a NULL pointer was sent or the list is empty.
 * The first element of the list otherwise
 */
ListElement listGetFirst(List list);

/**
 * Advances the list's iterator to the next element and returns it. If the
 * iterator points to the last element, the iterator should be set to NULL and
 * returned.
 *
 * Note: the element which is stored in the list is returned, not a copy.
 *
 * @param list The list for which to advance the iterator
 * @return
 * NULL if reached the end of the list, the iterator points to NULL or a NULL
 * sent as argument
 * The next element on the list in case of success
 */
ListElement listGetNext(List list);

/**
 * Returns the current element (pointed by the iterator).
 *
 * Note: the element which is stored in the list is returned, not a copy.
 *
 * The iterator should not change.
 *
 * @param list The list for which to get the iterator
 * @return
 * NULL if the iterator points to NULL or a NULL sent as argument
 * The current element on the list in case of success
 */
ListElement listGetCurrent(List list);

/**
 * Adds a new element to the list, the new element will be the first element.
 *
 * The iterator should not change.
 *
 * @param list The list for which to add an element in its start
 * @param element The element to insert. A copy of the element will be
 * inserted as supplied by the copying function which is stored in the list
 * @return
 * LIST_NULL_ARGUMENT if a NULL was sent as list or element
 * LIST_OUT_OF_MEMORY if an allocation failed (Meaning the function for copying
 * an element failed)
 * LIST_SUCCESS the element has been inserted successfully
 */
ListResult listInsertFirst(List list, ListElement element);

/**
 * Adds a new element to the list, the new element will be the last element
 *
 * The iterator should not change.
 *
 * @param list The list for which to add an element in its end
 * @param element The element to insert. A copy of the element will be
 * inserted as supplied by the copying function which is stored in the list
 * @return
 * LIST_NULL_ARGUMENT if a NULL was sent as list or element
 * LIST_OUT_OF_MEMORY if an allocation failed (Meaning the function for copying
 * an element failed)
 * LIST_SUCCESS the element has been inserted successfully
 */
ListResult listInsertLast(List list, ListElement element);

/**
 * Adds a new element to the list, the new element will be place right before
 * the current element (as pointed by the inner iterator of the list)
 *
 * The iterator should not change.
 *
 * @param list The list for which to add an element before its current element
 * @param element The element to insert. A copy of the element will be
 * inserted as supplied by the copying function which is stored in the list
 * @return
 * LIST_NULL_ARGUMENT if a NULL was sent as list or element
 * LIST_INVALID_CURRENT if the list's iterator points to NULL
 * LIST_OUT_OF_MEMORY if an allocation failed (Meaning the function for copying
 * an element failed)
 * LIST_SUCCESS the element has been inserted successfully
 */
ListResult listInsertBeforeCurrent(List list, ListElement element);

/**
 * Adds a new element to the list, the new element will be place right after
 * the current element (as pointed by the inner iterator be of the list)
 *
 * The iterator should not change.
 *
 * @param list The list for which to add an element after its current element
 * @param element The element to insert. A copy of the element will be
 * inserted as supplied by the copying function which is stored in the list
 * @return
 * LIST_NULL_ARGUMENT if a NULL was sent as list or element
 * LIST_INVALID_CURRENT if the list's iterator points to NULL
 * LIST_OUT_OF_MEMORY if an allocation failed (Meaning the function for copying
 * an element failed)
 * LIST_SUCCESS the element has been inserted successfully
 */
ListResult listInsertAfterCurrent(List list, ListElement element);

/**
 * Removes the currently pointed element of the list using the stored freeing
 * function
 *
 * The iterator should be set to NULL.
 *
 * @param list The list for which the current element will be removed
 * @return
 * LIST_NULL_ARGUMENT if list is NULL
 * LIST_INVALID_CURRENT if the current pointer of the list points to NULL
 * LIST_SUCCESS the current element was removed successfully
 */
ListResult listRemoveCurrent(List list);

/**
 * Sorts the list according to the given function.
 *
 * For example, the following code will sort a list of integers according to
 * their distance from 0.
 * @code
 * int closerTo(ListElement num1, ListElement num2, ListSortKey value) {
 *   int distance1 = abs(*(int*)num1 - *(int*)value);
 *   int distance2 = abs(*(int*)num2 - *(int*)value);
 *   return distance1 - distance2;
 * }
 *
 * void sortInts(List listOfInts) {
 *   int key = 0;
 *   listSort(listOfInts, closerTo, &key);
 * }
 * @endcode
 *
 * Elements which are treated as equal by the comparison function don't have a
 * defined order.
 *
 * After sorting, the iterator points to the same node in order. That is, if the
 * list before sorting was (1, 5, 2, 6), and the iterator pointed to "5", after
 * the sorting, (1, 2, 5, 6), the iterator will still point to the second
 * element in the list, which is "2".
 *
 * @param list the target list to sort
 * @param compareElement A comparison function as defined in the type
 * CompareListElements. This function should return an integer indicating the
 * relation between two elements in the list
 *
 * @return
 * LIST_NULL_ARGUMENT if list or compareElement are NULL
 * LIST_OUT_OF_MEMORY if a memory allocation failed, the list will be intact
 * in this case.
 * LIST_SUCCESS if sorting completed successfully.
 */
ListResult listSort(List list, CompareListElements compareElement,
	ListSortKey key);

/**
 * Creates a new filtered copy of a list.
 *
 * This creates a new list with only the elements for which the filtering
 * function returned true.
 *
 * For example, the following code creates a new list, given a list of strings
 * containing only the strings which are longer than 10 characters.
 * @code
 *
 * bool isLongerThan(ListElement string, ListFilterKey key) {
 *   return strlen(string) > *(int*)key;
 * }
 *
 * List createFilteredList(List listOfStrings) {
 *   int key = 10;
 *   return listFilter(listOfStrings, isLongerThan, &key);
 * }
 * @endcode
 *
 * The iterator of the new list should point to NULL. The iterator of the
 * source list should not change.
 *
 * @param list The list for which a filtered copy will be made
 * @param filterElement The function used for determining whether a given
 * element should be in the resulting list or not.
 * @param key Any extra values that need to be sent to the filtering function
 * when called
 * @return
 * NULL if list or filterElement are NULL or a memory allocation failed.
 * A List containing only elements from list which filterElement returned true
 * for.
 */
List listFilter(List list, FilterListElement filterElement, ListFilterKey key);

/**
 * Removes all elements from target list.
 * The elements are deallocated using the stored freeing function.
 *
 * The iterator should be set to NULL.
 *
 * @param list Target list to remove all element from
 * @return
 * LIST_NULL_ARGUMENT - if a NULL pointer was sent.
 * LIST_SUCCESS - Otherwise.
 */
ListResult listClear(List list);

/**
 * Deallocates an existing list. Clears all elements by using the stored free
 * function.
 *
 * @param list Target list to be deallocated. If list is NULL nothing will be
 * done
 */
void listDestroy(List list);

/**
 * Macro for iterating over a list.
 *
 * Declares a new variable to hold each element of the list.
 * Use this macro for iterating through the list conveniently.
 * Note that this mcaro modifies the internal iterator.
 * For example, the following code will go through a list of strings and print
 * them to the standard output:
 * @code
 * void printList(List listOfStrings) {
 *   LIST_FOREACH(char*, str, listOfStrings) {
 *     printf("%s\\n", str);
 *   }
 * }
 * @endcode
 *
 * @param type The type of the elements in the list
 * @param iterator The name of the variable to hold the next list element
 * @param list the list to iterate over
 */
#define LIST_FOREACH(type,iterator,list) \
	for(type iterator = listGetFirst(list) ; \
		iterator ;\
		iterator = listGetNext(list))

#endif /* LIST_H_ */
