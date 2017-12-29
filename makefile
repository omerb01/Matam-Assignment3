CC=gcc
CFLAGS=-std=c99 -Wall -pedantic-errors -Werror -DNDEBUG
LIBRARIES=-L. -lmtm

GRAPH_OBJS=tests/Graph_test.o Graph.o
SHEET_OBJS=tests/GradesSheet_test.o GradesSheet.o UsefulFunctions.o
STUDENT_OBJS=tests/Student_test.o GradesSheet.o Student.o UsefulFunctions.o
MANAGER_OBJS=tests/CourseManager_test.o CourseManager.o GradesSheet.o Student.o Graph.o UsefulFunctions.o
LIST_OBJS=tests/list_mtm_test.o list_mtm/list_mtm.o
OBJS=main.o CourseManager.o GradesSheet.o Graph.o Student.o \
 UsefulFunctions.o
OBJS_FULL=$(GRAPH_OBJS) $(SHEET_OBJS) $(STUDENT_OBJS) $(MANAGER_OBJS) $(LIST_OBJS)

mtm_cm : $(OBJS)
	$(CC) $(DEBUG) $(OBJS) -o $@ $(LIBRARIES)

main.o: main.c list.h mtm_ex3.h CourseManager.h Graph.h set.h Student.h
Graph.o: Graph.c Graph.h set.h
CourseManager.o: CourseManager.c mtm_ex3.h Student.h \
 set.h CourseManager.h Graph.h Student.h UsefulFunctions.h
GradesSheet.o: GradesSheet.c list.h GradesSheet.h mtm_ex3.h UsefulFunctions.h set.h
Student.o: Student.c set.h list.h mtm_ex3.h GradesSheet.h Student.h set.h UsefulFunctions.h
UsefulFunctions.o: UsefulFunctions.c UsefulFunctions.h set.h
 
tests : $(OBJS_FULL)
	$(CC) $(DEBUG) $(MANAGER_OBJS) -o CourseManager_test $(LIBRARIES)
	$(CC) $(DEBUG) $(SHEET_OBJS) -o GradesSheet_test $(LIBRARIES)
	$(CC) $(DEBUG) $(GRAPH_OBJS) -o Graph_test $(LIBRARIES)
	$(CC) $(DEBUG) $(LIST_OBJS) -o list_mtm_test $(LIBRARIES)
	$(CC) $(DEBUG) $(STUDENT_OBJS) -o Student_test $(LIBRARIES)
 
list_mtm/list_mtm.o: list_mtm/list_mtm.c list_mtm/list_mtm.h

tests/CourseManager_test.o: tests/CourseManager_test.c tests/test_utilities.h \
 CourseManager.h Graph.h set.h Student.h
tests/GradesSheet_test.o: tests/GradesSheet_test.c tests/test_utilities.h GradesSheet.h
tests/Graph_test.o: tests/Graph_test.c tests/test_utilities.h Graph.h set.h
tests/list_mtm_test.o: tests/list_mtm_test.c list_mtm/list_mtm.h
tests/Student_test.o: tests/Student_test.c tests/test_utilities.h Student.h set.h \
 UsefulFunctions.h
 
clean:
	rm -f $(OBJS_FULL) main.o mtm_cm CourseManager_test GradesSheet_test Graph_test \
	 list_mtm_test Student_test