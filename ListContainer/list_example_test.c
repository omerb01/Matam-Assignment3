#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "../headers/test_utilities.h"
#include "list_mtm.h"


/**
 * List of tests, one for each function is usually a good thumb rule.
 */
static bool testListCreate() {
	ASSERT_TEST(listCreate(NULL, NULL) == NULL);
	//ASSERT_TEST(listCreate(someCopyFunction, NULL) == NULL);
	//ASSERT_TEST(listCreate(NULL, someDestroyFunction) == NULL);

	return true;
}

static bool testListCopy() {
	return true;
}

static bool testListGetSize() {
	return true;
}

static bool testListGetFirst() {
	return true;
}

static bool testListGetNext() {
	return true;
}

static bool testListInsertFirst() {
	return true;
}

static bool testListInsertLast() {
	return true;
}

static bool testListInsertBeforeCurrent() {
	return true;
}

static bool testListInsertAfterCurrent() {
	return true;
}

static bool testListRemoveCurrent() {
	return true;
}

static bool testListSort() {
	return true;
}

static bool testListClear() {
	return true;
}

static bool testListDestroy() {
	return true;
}

int main() {
	RUN_TEST(testListCreate);
	RUN_TEST(testListCopy);
	RUN_TEST(testListGetSize);
	RUN_TEST(testListGetFirst);
	RUN_TEST(testListGetNext);
	RUN_TEST(testListInsertFirst);
	RUN_TEST(testListInsertLast);
	RUN_TEST(testListInsertBeforeCurrent);
	RUN_TEST(testListInsertAfterCurrent);
	RUN_TEST(testListRemoveCurrent);
	RUN_TEST(testListSort);
	RUN_TEST(testListClear);
	RUN_TEST(testListDestroy);
	return 0;
}
