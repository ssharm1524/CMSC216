/**
 * Shiva Sharma
 * UID: 117912678
 * UMD Directory ID: ssharm08
 */
#include "calendar.h"
#include "event.h"
#include "my_memory_checker_216.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int comp_minutes(const void *ptr1, const void *ptr2) {
    return ((Event *)ptr1)->duration_minutes -
           ((Event *)ptr2)->duration_minutes;
}

static void fail(const char *const str, int *retVal) {
    printf("FAILED AT %s\n", str);
    *retVal = FAILURE;
}

typedef struct task_info {
    double cost;
    char *prog_language;
} Task_info;

static Task_info *create_task_info(double cost, const char *prog_language) {
    Task_info *task_info = malloc(sizeof(Task_info));

    if (task_info) {
        task_info->prog_language = malloc(strlen(prog_language) + 1);
        if (task_info->prog_language) {
            task_info->cost = cost;
            strcpy(task_info->prog_language, prog_language);
            return task_info;
        }
    }

    return NULL;
}

static void free_task_info(void *ptr) {
    Task_info *task_info = (Task_info *)ptr;

    free(task_info->prog_language);
    free(task_info);
}

static int compare_tasks(Task_info *one, Task_info *two) {
    if ((one->cost == two->cost) &&
        (!strcmp(one->prog_language, two->prog_language))) {
        return 1;
    }
    return 0;
}

/*****************************************************/
/* Tests init_calender regualar and edge cases */
static int test1() {
    int retVal = 8, *p = &retVal;
    Calendar *calendar;

    printf("*********************************************************\n");
    printf("INITIALIZE CALENDER TESTS: \n");

    printf("Regular Case: \n");
    if (init_calendar("Reg", 7, comp_minutes, NULL, &calendar) == SUCCESS) {
        if (print_calendar(calendar, stdout, 1) == SUCCESS) {
            if (destroy_calendar(calendar) == SUCCESS) {
                printf("PASSED\n");
                retVal = SUCCESS;
            } else {
                fail("destroy", p);
            }
        } else {
            fail("print_calendar", p);
        }
    } else {
        fail("init_calendar", p);
    }
    if (retVal == FAILURE) {
        return FAILURE;
    }

    printf("Calendar Null Case: ");
    if (init_calendar("Reg", 7, comp_minutes, NULL, NULL) == FAILURE) {
        printf("PASSED\n");
        retVal = SUCCESS;
    } else {
        fail("init_calendar", p);
        return FAILURE;
    }

    printf("Name Null Case: ");
    if (init_calendar(NULL, 7, comp_minutes, NULL, &calendar) == FAILURE) {
        printf("PASSED\n");
        retVal = SUCCESS;
    } else {
        fail("init_calendar", p);
        return FAILURE;
    }

    printf("Days<1 Case: ");
    if (init_calendar("Test", 0, comp_minutes, NULL, &calendar) == FAILURE) {
        printf("PASSED\n");
        retVal = SUCCESS;
    } else {
        fail("init_calendar", p);
        return FAILURE;
    }

    if (retVal == 8) {
        printf("No tests ran :(\n");
        return FAILURE;
    }

    return retVal;
}
/*****************************************************/
/* Tests regular and edge cases for print_calendar*/
static int test2() {
    int retVal = 8, *p = &retVal;
    Calendar *calendar;

    printf("*********************************************************\n");
    printf("PRINT CALENDER TESTS: \n");
    printf("Regular Case: \n");
    if (init_calendar("Reg", 3, comp_minutes, NULL, &calendar) == SUCCESS) {

        if ((add_event(calendar, "Event 1", 800, 80, NULL, 1) == FAILURE) ||
            (add_event(calendar, "Event 2", 500, 60, NULL, 1) == FAILURE)) {
            fail("add_event", p);
            return FAILURE;
        }

        if (print_calendar(calendar, stdout, 1) == SUCCESS) {
            if (destroy_calendar(calendar) == SUCCESS) {
                printf("PASSED\n");
                retVal = SUCCESS;
            } else {
                fail("destroy", p);
                return FAILURE;
            }
        } else {
            destroy_calendar(calendar);
            fail("print_calendar", p);
            return FAILURE;
        }
    } else {
        fail("init_calendar", p);
        return FAILURE;
    }

    printf("Calendar Null Case: ");
    if (init_calendar("Reg", 7, comp_minutes, NULL, &calendar) == SUCCESS) {

        if (print_calendar(NULL, stdout, 1) == FAILURE) {
            printf("PASSED\n");
            retVal = SUCCESS;
        } else {
            fail("print_calendar", p);
            return FAILURE;
        }

        printf("Output Stream Null Case: ");
        if (print_calendar(calendar, NULL, 1) == FAILURE) {
            printf("PASSED\n");
            retVal = SUCCESS;
        } else {
            fail("print_calendar", p);
            return FAILURE;
        }

        destroy_calendar(calendar);
    } else {
        fail("init_calendar", p);
    }

    if (retVal == 8) {
        printf("No tests ran :(\n");
        retVal = FAILURE;
    }

    return retVal;
}
/*****************************************************/
/* Tests regular and edge cases for add event*/
static int test3() {
    int retVal = 8, *p = &retVal;
    Calendar *calendar;

    printf("*********************************************************\n");
    printf("ADD EVENT TESTS: \n");
    printf("Add at head of first index: \n");
    if (init_calendar("Reg", 3, comp_minutes, NULL, &calendar) == SUCCESS) {
        /*******FIRST INDEX******/
        if ((add_event(calendar, "Day 1 Event 1", 800, 80, NULL, 1) ==
             FAILURE)) {
            destroy_calendar(calendar);
            fail("add_event", p);
            return FAILURE;
        }
        if (print_calendar(calendar, stdout, 0) == FAILURE) {
            destroy_calendar(calendar);
            fail("print_calendar", p);
            return FAILURE;
        }
        printf("PASSED\n");

        printf("Add after head at first index: \n");
        if ((add_event(calendar, "Day 1 Event 2", 800, 90, NULL, 1) ==
             FAILURE)) {
            destroy_calendar(calendar);
            fail("add_event", p);
            return FAILURE;
        }
        if (print_calendar(calendar, stdout, 0) == FAILURE) {
            destroy_calendar(calendar);
            fail("print_calendar", p);
            return FAILURE;
        }
        printf("PASSED\n");

        printf("Replace head at first index: \n");
        if ((add_event(calendar, "Day 1 Event 3 (should appear first)", 800, 70,
                       NULL, 1) == FAILURE)) {
            destroy_calendar(calendar);
            fail("add_event", p);
            return FAILURE;
        }
        if (print_calendar(calendar, stdout, 0) == FAILURE) {
            destroy_calendar(calendar);
            fail("print_calendar", p);
            return FAILURE;
        }
        printf("PASSED\n");

        /***********MIDDLE INDEX************/
        printf("Add at head of middle index: \n");
        if ((add_event(calendar, "Day 2 Event 1", 800, 80, NULL, 2) ==
             FAILURE)) {
            destroy_calendar(calendar);
            fail("add_event", p);
            return FAILURE;
        }
        if (print_calendar(calendar, stdout, 0) == FAILURE) {
            destroy_calendar(calendar);
            fail("print_calendar", p);
            return FAILURE;
        }
        printf("PASSED\n");

        printf("Add after head at middle index: \n");
        if ((add_event(calendar, "Day 2 Event 2", 800, 90, NULL, 2) ==
             FAILURE)) {
            destroy_calendar(calendar);
            fail("add_event", p);
            return FAILURE;
        }
        if (print_calendar(calendar, stdout, 0) == FAILURE) {
            destroy_calendar(calendar);
            fail("print_calendar", p);
            return FAILURE;
        }
        printf("PASSED\n");

        printf("Replace head at middle index: \n");
        if ((add_event(calendar, "Day 2 Event 3 (should appear first)", 800, 70,
                       NULL, 2) == FAILURE)) {
            destroy_calendar(calendar);
            fail("add_event", p);
            return FAILURE;
        }
        if (print_calendar(calendar, stdout, 0) == FAILURE) {
            destroy_calendar(calendar);
            fail("print_calendar", p);
            return FAILURE;
        }
        printf("PASSED\n");
        /****************LAST INDEX*****************/
        printf("Add at head of last index: \n");
        if ((add_event(calendar, "Day 3 Event 1", 800, 80, NULL, 3) ==
             FAILURE)) {
            destroy_calendar(calendar);
            fail("add_event", p);
            return FAILURE;
        }
        if (print_calendar(calendar, stdout, 0) == FAILURE) {
            destroy_calendar(calendar);
            fail("print_calendar", p);
            return FAILURE;
        }
        printf("PASSED\n");

        printf("Add after head at last index: \n");
        if ((add_event(calendar, "Day 3 Event 2", 800, 90, NULL, 3) ==
             FAILURE)) {
            destroy_calendar(calendar);
            fail("add_event", p);
            return FAILURE;
        }
        if (print_calendar(calendar, stdout, 0) == FAILURE) {
            destroy_calendar(calendar);
            fail("print_calendar", p);
            return FAILURE;
        }
        printf("PASSED\n");

        printf("Replace head at last index: \n");
        if ((add_event(calendar, "Day 3 Event 3 (should appear first)", 800, 70,
                       NULL, 3) == FAILURE)) {
            destroy_calendar(calendar);
            fail("add_event", p);
            return FAILURE;
        }
        if (print_calendar(calendar, stdout, 0) == FAILURE) {
            destroy_calendar(calendar);
            fail("print_calendar", p);
            return FAILURE;
        }
        printf("PASSED\n");
        /**************EDGE CASES******************/
        printf("Calendar Null Case: ");
        if ((add_event(NULL, "ERROR", 800, 100, NULL, 3) == SUCCESS)) {
            destroy_calendar(calendar);
            fail("Calender Null Case", p);
            return FAILURE;
        }
        printf("PASSED\n");

        printf("Name Null Case: ");
        if ((add_event(calendar, NULL, 800, 100, NULL, 3) == SUCCESS)) {
            destroy_calendar(calendar);
            fail("Name Null Case", p);
            return FAILURE;
        }
        printf("PASSED\n");

        printf("Start Time < 0 Case: ");
        if ((add_event(calendar, "ERROR", -1, 100, NULL, 3) == SUCCESS)) {
            destroy_calendar(calendar);
            fail("Start Time < 0 Case", p);
            return FAILURE;
        }
        printf("PASSED\n");

        printf("Start Time > 2400 Case: ");
        if ((add_event(calendar, "ERROR", 2401, 100, NULL, 3) == SUCCESS)) {
            destroy_calendar(calendar);
            fail("Start Time > 2400 Case", p);
            return FAILURE;
        }
        printf("PASSED\n");

        printf("Duration Minutes = 0 Case: ");
        if ((add_event(calendar, "ERROR", 800, 0, NULL, 3) == SUCCESS)) {
            destroy_calendar(calendar);
            fail("Duration Minutes = 0 Case", p);
            return FAILURE;
        }
        printf("PASSED\n");

        printf("Duration Minutes < 0 Case: ");
        if ((add_event(calendar, "ERROR", 800, -1, NULL, 3) == SUCCESS)) {
            destroy_calendar(calendar);
            fail("Duration Minutes < 0 Case", p);
            return FAILURE;
        }
        printf("PASSED\n");

        printf("Day < 1 Case: ");
        if ((add_event(calendar, "ERROR", 800, 100, NULL, 0) == SUCCESS)) {
            destroy_calendar(calendar);
            fail("Day < 1 Case", p);
            return FAILURE;
        }
        printf("PASSED\n");

        printf("Day > num_days Case: ");
        if ((add_event(calendar, "ERROR", 800, 100, NULL, 4) == SUCCESS)) {
            destroy_calendar(calendar);
            fail("Day > num_days Case", p);
            return FAILURE;
        }
        printf("PASSED\n");

        printf("Event Alreaady Exists Case: ");
        if ((add_event(calendar, "Day 3 Event 2", 800, 90, NULL, 3) ==
             SUCCESS)) {
            destroy_calendar(calendar);
            fail("Event Already Exists Case", p);
            return FAILURE;
        }
        printf("PASSED\n");
        /*****************************************/

        destroy_calendar(calendar);
        retVal = SUCCESS;
    } else {
        fail("init_calendar", p);
        return FAILURE;
    }

    if (retVal == 8) {
        printf("No tests ran :(\n");
        retVal = FAILURE;
    }

    return retVal;
}
/*****************************************************/
/* Tests regular and edge cases for find event*/
static int test4() {
    int retVal = 8, *p = &retVal;
    Calendar *calendar;

    printf("*********************************************************\n");
    printf("FIND EVENT TESTS: \n");
    if (init_calendar("Reg", 3, comp_minutes, NULL, &calendar) == SUCCESS) {
        Event *event_ptr;
        char testing[14] = "Dummy Value";
        if ((add_event(calendar, "Day 1 Event 1", 800, 70, NULL, 1) ==
             FAILURE) ||
            (add_event(calendar, "Day 1 Event 2", 500, 80, NULL, 1) ==
             FAILURE) ||
            (add_event(calendar, "Day 1 Event 3", 500, 90, NULL, 1) ==
             FAILURE) ||
            (add_event(calendar, "Day 2 Event 1", 800, 70, NULL, 2) ==
             FAILURE) ||
            (add_event(calendar, "Day 2 Event 2", 500, 80, NULL, 2) ==
             FAILURE) ||
            (add_event(calendar, "Day 2 Event 3", 500, 90, NULL, 2) ==
             FAILURE) ||
            (add_event(calendar, "Day 3 Event 1", 800, 70, NULL, 3) ==
             FAILURE) ||
            (add_event(calendar, "Day 3 Event 2", 500, 80, NULL, 3) ==
             FAILURE) ||
            (add_event(calendar, "Day 3 Event 3", 500, 90, NULL, 3) ==
             FAILURE)) {
            destroy_calendar(calendar);
            fail("add_event", p);
            return FAILURE;
        }

        /**********FIRST INDEX***********/
        printf("Find head of first index: ");
        strcpy(testing, "Day 1 Event 1");
        if ((find_event(calendar, testing, &event_ptr) == FAILURE) ||
            strcmp(event_ptr->name, testing)) {
            destroy_calendar(calendar);
            fail(testing, p);
            return FAILURE;
        }
        printf("PASSED\n");

        printf("Find middle node of first index: ");
        strcpy(testing, "Day 1 Event 2");
        if ((find_event(calendar, testing, &event_ptr) == FAILURE) ||
            strcmp(event_ptr->name, testing)) {
            destroy_calendar(calendar);
            fail(testing, p);
            return FAILURE;
        }
        printf("PASSED\n");

        printf("Find tail node of first index: ");
        strcpy(testing, "Day 1 Event 3");
        if ((find_event(calendar, testing, &event_ptr) == FAILURE) ||
            strcmp(event_ptr->name, testing)) {
            destroy_calendar(calendar);
            fail(testing, p);
            return FAILURE;
        }
        printf("PASSED\n");
        /**********MIDDLE INDEX****************/
        printf("Find head of middle index: ");
        strcpy(testing, "Day 2 Event 1");
        if ((find_event(calendar, testing, &event_ptr) == FAILURE) ||
            strcmp(event_ptr->name, testing)) {
            destroy_calendar(calendar);
            fail(testing, p);
            return FAILURE;
        }
        printf("PASSED\n");

        printf("Find middle node of middle index: ");
        strcpy(testing, "Day 2 Event 2");
        if ((find_event(calendar, testing, &event_ptr) == FAILURE) ||
            strcmp(event_ptr->name, testing)) {
            destroy_calendar(calendar);
            fail(testing, p);
            return FAILURE;
        }
        printf("PASSED\n");

        printf("Find tail node of middle index: ");
        strcpy(testing, "Day 2 Event 3");
        if ((find_event(calendar, testing, &event_ptr) == FAILURE) ||
            strcmp(event_ptr->name, testing)) {
            destroy_calendar(calendar);
            fail(testing, p);
            return FAILURE;
        }
        printf("PASSED\n");
        /**********LAST INDEX****************/
        printf("Find head of last index: ");
        strcpy(testing, "Day 3 Event 1");
        if ((find_event(calendar, testing, &event_ptr) == FAILURE) ||
            strcmp(event_ptr->name, testing)) {
            destroy_calendar(calendar);
            fail(testing, p);
            return FAILURE;
        }
        printf("PASSED\n");

        printf("Find middle node of last index: ");
        strcpy(testing, "Day 3 Event 2");
        if ((find_event(calendar, testing, &event_ptr) == FAILURE) ||
            strcmp(event_ptr->name, testing)) {
            destroy_calendar(calendar);
            fail(testing, p);
            return FAILURE;
        }
        printf("PASSED\n");

        printf("Find tail node of last index: ");
        strcpy(testing, "Day 3 Event 3");
        if ((find_event(calendar, testing, &event_ptr) == FAILURE) ||
            strcmp(event_ptr->name, testing)) {
            destroy_calendar(calendar);
            fail(testing, p);
            return FAILURE;
        }
        printf("PASSED\n");
        /**********EDGE CASES****************/
        printf("Event arg is null case: ");
        event_ptr = NULL;
        strcpy(testing, "Day 2 Event 3");
        if ((find_event(calendar, testing, NULL) == FAILURE) || (event_ptr)) {
            destroy_calendar(calendar);
            fail("Event arg is null case", p);
            return FAILURE;
        }
        printf("PASSED\n");

        printf("Calendar Null Case: ");
        if ((find_event(NULL, testing, &event_ptr) == SUCCESS)) {
            destroy_calendar(calendar);
            fail("Calendar Null Case", p);
            return FAILURE;
        }
        printf("PASSED\n");

        printf("Name Null Case: ");
        if ((find_event(calendar, NULL, &event_ptr) == SUCCESS)) {
            destroy_calendar(calendar);
            fail("Calendar Null Case", p);
            return FAILURE;
        }
        printf("PASSED\n");

        printf("Event DNE Case: ");
        strcpy(testing, "Day 4 Event 1");
        if ((find_event(calendar, testing, &event_ptr) == SUCCESS)) {
            destroy_calendar(calendar);
            fail("Event DNE Case", p);
            return FAILURE;
        }
        printf("PASSED\n");
        /************************************/

        destroy_calendar(calendar);
        retVal = SUCCESS;
    } else {
        fail("init_calendar", p);
        return FAILURE;
    }

    if (retVal == 8) {
        printf("No tests ran :(\n");
        retVal = FAILURE;
    }

    return retVal;
}
/*****************************************************/
/* Tests regular and edge cases for find event in day*/
static int test5() {
    int retVal = 8, *p = &retVal;
    Calendar *calendar;

    printf("*********************************************************\n");
    printf("FIND EVENT IN DAY TESTS: \n");
    if (init_calendar("Reg", 3, comp_minutes, NULL, &calendar) == SUCCESS) {
        Event *event_ptr;
        char testing[14] = "Dummy Value";
        if ((add_event(calendar, "Day 1 Event 1", 800, 70, NULL, 1) ==
             FAILURE) ||
            (add_event(calendar, "Day 1 Event 2", 500, 80, NULL, 1) ==
             FAILURE) ||
            (add_event(calendar, "Day 1 Event 3", 500, 90, NULL, 1) ==
             FAILURE) ||
            (add_event(calendar, "Day 2 Event 1", 800, 70, NULL, 2) ==
             FAILURE) ||
            (add_event(calendar, "Day 2 Event 2", 500, 80, NULL, 2) ==
             FAILURE) ||
            (add_event(calendar, "Day 2 Event 3", 500, 90, NULL, 2) ==
             FAILURE) ||
            (add_event(calendar, "Day 3 Event 1", 800, 70, NULL, 3) ==
             FAILURE) ||
            (add_event(calendar, "Day 3 Event 2", 500, 80, NULL, 3) ==
             FAILURE) ||
            (add_event(calendar, "Day 3 Event 3", 500, 90, NULL, 3) ==
             FAILURE)) {
            destroy_calendar(calendar);
            fail("add_event", p);
            return FAILURE;
        }

        /**********FIRST INDEX***********/
        printf("Find head of first index: ");
        strcpy(testing, "Day 1 Event 1");
        if ((find_event_in_day(calendar, testing, 1, &event_ptr) == FAILURE) ||
            strcmp(event_ptr->name, testing)) {
            destroy_calendar(calendar);
            fail(testing, p);
            return FAILURE;
        }
        printf("PASSED\n");

        printf("Find middle node of first index: ");
        strcpy(testing, "Day 1 Event 2");
        if ((find_event_in_day(calendar, testing, 1, &event_ptr) == FAILURE) ||
            strcmp(event_ptr->name, testing)) {
            destroy_calendar(calendar);
            fail(testing, p);
            return FAILURE;
        }
        printf("PASSED\n");

        printf("Find tail node of first index: ");
        strcpy(testing, "Day 1 Event 3");
        if ((find_event_in_day(calendar, testing, 1, &event_ptr) == FAILURE) ||
            strcmp(event_ptr->name, testing)) {
            destroy_calendar(calendar);
            fail(testing, p);
            return FAILURE;
        }
        printf("PASSED\n");
        /**********MIDDLE INDEX****************/
        printf("Find head of middle index: ");
        strcpy(testing, "Day 2 Event 1");
        if ((find_event_in_day(calendar, testing, 2, &event_ptr) == FAILURE) ||
            strcmp(event_ptr->name, testing)) {
            destroy_calendar(calendar);
            fail(testing, p);
            return FAILURE;
        }
        printf("PASSED\n");

        printf("Find middle node of middle index: ");
        strcpy(testing, "Day 2 Event 2");
        if ((find_event_in_day(calendar, testing, 2, &event_ptr) == FAILURE) ||
            strcmp(event_ptr->name, testing)) {
            destroy_calendar(calendar);
            fail(testing, p);
            return FAILURE;
        }
        printf("PASSED\n");

        printf("Find tail node of middle index: ");
        strcpy(testing, "Day 2 Event 3");
        if ((find_event_in_day(calendar, testing, 2, &event_ptr) == FAILURE) ||
            strcmp(event_ptr->name, testing)) {
            destroy_calendar(calendar);
            fail(testing, p);
            return FAILURE;
        }
        printf("PASSED\n");
        /**********LAST INDEX****************/
        printf("Find head of last index: ");
        strcpy(testing, "Day 3 Event 1");
        if ((find_event_in_day(calendar, testing, 3, &event_ptr) == FAILURE) ||
            strcmp(event_ptr->name, testing)) {
            destroy_calendar(calendar);
            fail(testing, p);
            return FAILURE;
        }
        printf("PASSED\n");

        printf("Find middle node of last index: ");
        strcpy(testing, "Day 3 Event 2");
        if ((find_event_in_day(calendar, testing, 3, &event_ptr) == FAILURE) ||
            strcmp(event_ptr->name, testing)) {
            destroy_calendar(calendar);
            fail(testing, p);
            return FAILURE;
        }
        printf("PASSED\n");

        printf("Find tail node of last index: ");
        strcpy(testing, "Day 3 Event 3");
        if ((find_event_in_day(calendar, testing, 3, &event_ptr) == FAILURE) ||
            strcmp(event_ptr->name, testing)) {
            destroy_calendar(calendar);
            fail(testing, p);
            return FAILURE;
        }
        printf("PASSED\n");
        /**********EDGE CASES****************/
        printf("Event arg is null case: ");
        event_ptr = NULL;
        strcpy(testing, "Day 2 Event 3");
        if ((find_event_in_day(calendar, testing, 2, NULL) == FAILURE) ||
            (event_ptr)) {
            destroy_calendar(calendar);
            fail("Event arg is null case", p);
            return FAILURE;
        }
        printf("PASSED\n");

        printf("Calendar Null Case: ");
        if ((find_event_in_day(NULL, testing, 2, &event_ptr) == SUCCESS)) {
            destroy_calendar(calendar);
            fail("Calendar Null Case", p);
            return FAILURE;
        }
        printf("PASSED\n");

        printf("Name Null Case: ");
        if ((find_event_in_day(calendar, NULL, 2, &event_ptr) == SUCCESS)) {
            destroy_calendar(calendar);
            fail("Calendar Null Case", p);
            return FAILURE;
        }
        printf("PASSED\n");

        printf("Day < 1 Case: ");
        if ((find_event_in_day(calendar, NULL, 0, &event_ptr) == SUCCESS)) {
            destroy_calendar(calendar);
            fail("Calendar Null Case", p);
            return FAILURE;
        }
        printf("PASSED\n");

        printf("Day > num_days Case: ");
        if ((find_event_in_day(calendar, NULL, 4, &event_ptr) == SUCCESS)) {
            destroy_calendar(calendar);
            fail("Calendar Null Case", p);
            return FAILURE;
        }
        printf("PASSED\n");

        printf("Event DNE Case: ");
        strcpy(testing, "Day 4 Event 1");
        if ((find_event_in_day(calendar, testing, 3, &event_ptr) == SUCCESS)) {
            destroy_calendar(calendar);
            fail("Event DNE Case", p);
            return FAILURE;
        }
        printf("PASSED\n");
        /************************************/

        destroy_calendar(calendar);
        retVal = SUCCESS;
    } else {
        fail("init_calendar", p);
        return FAILURE;
    }

    if (retVal == 8) {
        printf("No tests ran :(\n");
        retVal = FAILURE;
    }

    return retVal;
}
/*****************************************************/
/* Tests regular and edge cases for remove event*/
static int test6() {
    int retVal = 8, *p = &retVal;
    Calendar *calendar;

    printf("*********************************************************\n");
    printf("REMOVE EVENT TESTS: \n");
    if (init_calendar("Reg", 3, comp_minutes, NULL, &calendar) == SUCCESS) {

        if ((add_event(calendar, "Day 1 Event 1", 800, 70, NULL, 1) ==
             FAILURE) ||
            (add_event(calendar, "Day 1 Event 2", 500, 80, NULL, 1) ==
             FAILURE) ||
            (add_event(calendar, "Day 1 Event 3", 500, 90, NULL, 1) ==
             FAILURE) ||
            (add_event(calendar, "Day 2 Event 1", 800, 70, NULL, 2) ==
             FAILURE) ||
            (add_event(calendar, "Day 2 Event 2", 500, 80, NULL, 2) ==
             FAILURE) ||
            (add_event(calendar, "Day 2 Event 3", 500, 90, NULL, 2) ==
             FAILURE) ||
            (add_event(calendar, "Day 3 Event 1", 800, 70, NULL, 3) ==
             FAILURE) ||
            (add_event(calendar, "Day 3 Event 2", 500, 80, NULL, 3) ==
             FAILURE) ||
            (add_event(calendar, "Day 3 Event 3", 500, 90, NULL, 3) ==
             FAILURE)) {
            destroy_calendar(calendar);
            fail("add_event", p);
            return FAILURE;
        }

        /*******FIRST INDEX, Remove order: head, middle, tail******/
        printf("Remove head at first index: \n");
        if ((remove_event(calendar, "Day 1 Event 1") == FAILURE)) {
            destroy_calendar(calendar);
            fail("remove_event", p);
            return FAILURE;
        }
        if (print_calendar(calendar, stdout, 0) == FAILURE) {
            destroy_calendar(calendar);
            fail("print_calendar", p);
            return FAILURE;
        }
        printf("PASSED\n");

        printf("Remove middle node at first index: \n");
        if ((remove_event(calendar, "Day 1 Event 2") == FAILURE)) {
            destroy_calendar(calendar);
            fail("remove_event", p);
            return FAILURE;
        }
        if (print_calendar(calendar, stdout, 0) == FAILURE) {
            destroy_calendar(calendar);
            fail("print_calendar", p);
            return FAILURE;
        }
        printf("PASSED\n");

        printf("Remove tail at first index: \n");
        if ((remove_event(calendar, "Day 1 Event 3") == FAILURE)) {
            destroy_calendar(calendar);
            fail("remove_event", p);
            return FAILURE;
        }
        if (print_calendar(calendar, stdout, 0) == FAILURE) {
            destroy_calendar(calendar);
            fail("print_calendar", p);
            return FAILURE;
        }
        printf("PASSED\n");

        /***********MIDDLE INDEX, Remove order: middle, tail, head************/
        printf("Remove middle node at middle index: \n");
        if ((remove_event(calendar, "Day 2 Event 2") == FAILURE)) {
            destroy_calendar(calendar);
            fail("remove_event", p);
            return FAILURE;
        }
        if (print_calendar(calendar, stdout, 0) == FAILURE) {
            destroy_calendar(calendar);
            fail("print_calendar", p);
            return FAILURE;
        }
        printf("PASSED\n");

        printf("Remove tail at middle index: \n");
        if ((remove_event(calendar, "Day 2 Event 3") == FAILURE)) {
            destroy_calendar(calendar);
            fail("remove_event", p);
            return FAILURE;
        }
        if (print_calendar(calendar, stdout, 0) == FAILURE) {
            destroy_calendar(calendar);
            fail("print_calendar", p);
            return FAILURE;
        }
        printf("PASSED\n");

        printf("Remove head of middle index: \n");
        if ((remove_event(calendar, "Day 2 Event 1") == FAILURE)) {
            destroy_calendar(calendar);
            fail("remove_event", p);
            return FAILURE;
        }
        if (print_calendar(calendar, stdout, 0) == FAILURE) {
            destroy_calendar(calendar);
            fail("print_calendar", p);
            return FAILURE;
        }
        printf("PASSED\n");
        /*******LAST INDEX, Remove order: tail, middle, head*****************/
        printf("Remove tail at last index: \n");
        if ((remove_event(calendar, "Day 3 Event 3") == FAILURE)) {
            destroy_calendar(calendar);
            fail("remove_event", p);
            return FAILURE;
        }
        if (print_calendar(calendar, stdout, 0) == FAILURE) {
            destroy_calendar(calendar);
            fail("print_calendar", p);
            return FAILURE;
        }
        printf("PASSED\n");

        printf("Remove head of last index: \n");
        if ((remove_event(calendar, "Day 3 Event 1") == FAILURE)) {
            destroy_calendar(calendar);
            fail("remove_event", p);
            return FAILURE;
        }
        if (print_calendar(calendar, stdout, 0) == FAILURE) {
            destroy_calendar(calendar);
            fail("print_calendar", p);
            return FAILURE;
        }
        printf("PASSED\n");

        printf("Remove middle node at last index: \n");
        if ((remove_event(calendar, "Day 3 Event 2") == FAILURE)) {
            destroy_calendar(calendar);
            fail("remove_event", p);
            return FAILURE;
        }
        if (print_calendar(calendar, stdout, 0) == FAILURE) {
            destroy_calendar(calendar);
            fail("print_calendar", p);
            return FAILURE;
        }
        printf("PASSED\n");

        /**************EDGE CASES******************/
        printf("Ensure num_events reflects changes: ");
        if (calendar->total_events) {
            destroy_calendar(calendar);
            fail("Ensure num_events reflects changes", p);
            return FAILURE;
        }
        printf("PASSED\n");

        printf("Calendar Null Case: ");
        if ((remove_event(NULL, "ERROR") == SUCCESS)) {
            destroy_calendar(calendar);
            fail("Calender Null Case", p);
            return FAILURE;
        }
        printf("PASSED\n");

        printf("Name Null Case: ");
        if ((remove_event(calendar, NULL) == SUCCESS)) {
            destroy_calendar(calendar);
            fail("Name Null Case", p);
            return FAILURE;
        }
        printf("PASSED\n");

        printf("Remove event twice case: ");
        if ((remove_event(calendar, "Day 3 Event 3") == SUCCESS)) {
            destroy_calendar(calendar);
            fail("Remove event twice case", p);
            return FAILURE;
        }
        printf("PASSED\n");

        printf("Remove non-existing event case: ");
        if ((remove_event(calendar, "Day 4 Event 1") == SUCCESS)) {
            destroy_calendar(calendar);
            fail("Remove non-existing event case", p);
            return FAILURE;
        }
        printf("PASSED\n");
        /****************************************/
        destroy_calendar(calendar);
        retVal = SUCCESS;
    } else {
        fail("init_calendar", p);
        return FAILURE;
    }
    if (retVal == 8) {
        printf("No tests ran :(\n");
        retVal = FAILURE;
    }
    return retVal;
}

/*****************************************************/
/* Tests regular and edge cases for get event info*/
static int test7() {
    int retVal = 8, *p = &retVal;
    Calendar *calendar;

    printf("*********************************************************\n");
    printf("GET EVENT INFO TESTS: \n");
    if (init_calendar("Reg", 3, comp_minutes, free_task_info, &calendar) ==
        SUCCESS) {
        Task_info *info1 = create_task_info(35000, "Python"),
                  *info2 = create_task_info(16000, "Assembly"), *info3;

        if ((add_event(calendar, "test1", 1600, 100, info1, 1) == FAILURE) ||
            (add_event(calendar, "test2", 1600, 110, info2, 2) == FAILURE)) {
            destroy_calendar(calendar);
            fail("add_event", p);
            return FAILURE;
        }

        /**********************REG CASES*************************/
        printf("Test1: ");
        info3 = get_event_info(calendar, "test1");
        if ((!info3) || (!compare_tasks(info1, info3))) {
            destroy_calendar(calendar);
            fail("test1", p);
            return FAILURE;
        }
        printf("PASSED\n");

        printf("Test2: ");
        info3 = get_event_info(calendar, "test2");
        if ((!info3) || (!compare_tasks(info2, info3))) {
            destroy_calendar(calendar);
            fail("test1", p);
            return FAILURE;
        }
        printf("PASSED\n");
        /**********************EDGE CASES*************************/
        printf("Event DNE Case: ");
        info3 = get_event_info(calendar, "test3");
        if (info3) {
            destroy_calendar(calendar);
            fail("Event DNE Case", p);
            return FAILURE;
        }
        printf("PASSED\n");
        /*********************************************************/
        destroy_calendar(calendar);
        retVal = SUCCESS;
    } else {
        fail("init_calendar", p);
        return FAILURE;
    }
    if (retVal == 8) {
        printf("No tests ran :(\n");
        retVal = FAILURE;
    }
    return retVal;
}
/*****************************************************/
/* Tests regular and edge cases for clear calendar*/
static int test8() {
    int retVal = 8, *p = &retVal;
    Calendar *calendar;

    printf("*********************************************************\n");
    printf("CLEAR CALENDAR TESTS: \n");
    if (init_calendar("Reg", 3, comp_minutes, NULL, &calendar) == SUCCESS) {

        if ((add_event(calendar, "Day 1 Event 1", 800, 70, NULL, 1) ==
             FAILURE) ||
            (add_event(calendar, "Day 1 Event 2", 500, 80, NULL, 1) ==
             FAILURE) ||
            (add_event(calendar, "Day 1 Event 3", 500, 90, NULL, 1) ==
             FAILURE) ||
            (add_event(calendar, "Day 2 Event 1", 800, 70, NULL, 2) ==
             FAILURE) ||
            (add_event(calendar, "Day 2 Event 2", 500, 80, NULL, 2) ==
             FAILURE) ||
            (add_event(calendar, "Day 2 Event 3", 500, 90, NULL, 2) ==
             FAILURE) ||
            (add_event(calendar, "Day 3 Event 1", 800, 70, NULL, 3) ==
             FAILURE) ||
            (add_event(calendar, "Day 3 Event 2", 500, 80, NULL, 3) ==
             FAILURE) ||
            (add_event(calendar, "Day 3 Event 3", 500, 90, NULL, 3) ==
             FAILURE)) {
            destroy_calendar(calendar);
            fail("add_event", p);
            return FAILURE;
        }
        if (print_calendar(calendar, stdout, 1) == FAILURE) {
            destroy_calendar(calendar);
            fail("print_calendar", p);
            return FAILURE;
        }
        /**********************REG CASE************************/
        printf("Clearing Calendar: ");
        if (clear_calendar(calendar) == FAILURE) {
            destroy_calendar(calendar);
            fail("clear_calender", p);
            return FAILURE;
        }

        /*printf("Total Events: %d\n", calendar->total_events);
        if (calendar->events[0]) {
            printf("Day 1 Name: %s\n", (calendar->events[0])->name);
            printf("Day 1 Duration: %d\n",
                   (calendar->events[0])->duration_minutes);
            printf("Day 1 Starttime: %d\n", (calendar->events[0])->start_time);
            printf("Day 1 Next: %p\n", (calendar->events[0])->next);
        } else {
            printf("Day 1 Null\n");
        }
        if (calendar->events[1]) {
            printf("Day 2 Name: %s\n", (calendar->events[1])->name);
            printf("Day 2 Duration: %d\n",
                   (calendar->events[1])->duration_minutes);
            printf("Day 2 Starttime: %d\n", (calendar->events[1])->start_time);
            printf("Day 2 Next: %p\n", (calendar->events[1])->next);
        } else {
            printf("Day 2 Null\n");
        }
        if (calendar->events[2]) {
            printf("Day 3 Name: %s\n", (calendar->events[2])->name);
            printf("Day 3 Duration: %d\n",
                   (calendar->events[2])->duration_minutes);
            printf("Day 3 Starttime: %d\n", (calendar->events[2])->start_time);
            printf("Day 3 Next: %p\n", (calendar->events[2])->next);
        } else {
            printf("Day 3 Null\n");
        }*/

        if ((calendar->total_events == 0) && (calendar->events[0] == NULL) &&
            (calendar->events[1] == NULL) && (calendar->events[2] == NULL)) {
            printf("PASSED\n");
        } else {
            destroy_calendar(calendar);
            fail("clear_calender", p);
            return FAILURE;
        }
        if (print_calendar(calendar, stdout, 1) == FAILURE) {
            destroy_calendar(calendar);
            fail("print_calendar", p);
            return FAILURE;
        }
        printf("Adding events back in... \n");
        if ((add_event(calendar, "Day 1 Event 1", 800, 70, NULL, 1) ==
             FAILURE) ||
            (add_event(calendar, "Day 1 Event 2", 500, 80, NULL, 1) ==
             FAILURE) ||
            (add_event(calendar, "Day 1 Event 3", 500, 90, NULL, 1) ==
             FAILURE) ||
            (add_event(calendar, "Day 2 Event 1", 800, 70, NULL, 2) ==
             FAILURE) ||
            (add_event(calendar, "Day 2 Event 2", 500, 80, NULL, 2) ==
             FAILURE) ||
            (add_event(calendar, "Day 2 Event 3", 500, 90, NULL, 2) ==
             FAILURE) ||
            (add_event(calendar, "Day 3 Event 1", 800, 70, NULL, 3) ==
             FAILURE) ||
            (add_event(calendar, "Day 3 Event 2", 500, 80, NULL, 3) ==
             FAILURE) ||
            (add_event(calendar, "Day 3 Event 3", 500, 90, NULL, 3) ==
             FAILURE)) {
            destroy_calendar(calendar);
            fail("add_event", p);
            return FAILURE;
        }
        if (print_calendar(calendar, stdout, 1) == FAILURE) {
            destroy_calendar(calendar);
            fail("print_calendar", p);
            return FAILURE;
        }
        /*********************EDGE CASE************************/
        printf("Calendar Null Case: ");
        if (clear_calendar(NULL) == SUCCESS) {
            destroy_calendar(calendar);
            fail("Calendar Null Case", p);
            return FAILURE;
        }
        printf("PASSED\n");
        /******************************************************/
        destroy_calendar(calendar);
        retVal = SUCCESS;
    } else {
        fail("init_calendar", p);
        return FAILURE;
    }
    if (retVal == 8) {
        printf("No tests ran :(\n");
        retVal = FAILURE;
    }
    return retVal;
}
/*****************************************************/
/* Tests regular and edge cases for clear day*/
static int test9() {
    int retVal = 8, *p = &retVal;
    Calendar *calendar;

    printf("*********************************************************\n");
    printf("CLEAR DAY TESTS: \n");
    if (init_calendar("Reg", 3, comp_minutes, NULL, &calendar) == SUCCESS) {

        if ((add_event(calendar, "Day 1 Event 1", 800, 70, NULL, 1) ==
             FAILURE) ||
            (add_event(calendar, "Day 1 Event 2", 500, 80, NULL, 1) ==
             FAILURE) ||
            (add_event(calendar, "Day 1 Event 3", 500, 90, NULL, 1) ==
             FAILURE) ||
            (add_event(calendar, "Day 2 Event 1", 800, 70, NULL, 2) ==
             FAILURE) ||
            (add_event(calendar, "Day 2 Event 2", 500, 80, NULL, 2) ==
             FAILURE) ||
            (add_event(calendar, "Day 2 Event 3", 500, 90, NULL, 2) ==
             FAILURE) ||
            (add_event(calendar, "Day 3 Event 1", 800, 70, NULL, 3) ==
             FAILURE) ||
            (add_event(calendar, "Day 3 Event 2", 500, 80, NULL, 3) ==
             FAILURE) ||
            (add_event(calendar, "Day 3 Event 3", 500, 90, NULL, 3) ==
             FAILURE)) {
            destroy_calendar(calendar);
            fail("add_event", p);
            return FAILURE;
        }
        if (print_calendar(calendar, stdout, 1) == FAILURE) {
            destroy_calendar(calendar);
            fail("print_calendar", p);
            return FAILURE;
        }
        /**********************REG CASE************************/
        printf("Clearing Day 1: ");
        if (clear_day(calendar, 1) == FAILURE) {
            destroy_calendar(calendar);
            fail("Clear Day 1", p);
            return FAILURE;
        }
        if ((calendar->total_events == 6) && (calendar->events[0] == NULL)) {
            printf("PASSED\n");
        } else {
            destroy_calendar(calendar);
            fail("Clear Day 1", p);
            return FAILURE;
        }
        if (print_calendar(calendar, stdout, 0) == FAILURE) {
            destroy_calendar(calendar);
            fail("print_calendar", p);
            return FAILURE;
        }

        printf("Clearing Day 2: ");
        if (clear_day(calendar, 2) == FAILURE) {
            destroy_calendar(calendar);
            fail("Clear Day 2", p);
            return FAILURE;
        }
        if ((calendar->total_events == 3) && (calendar->events[1] == NULL)) {
            printf("PASSED\n");
        } else {
            destroy_calendar(calendar);
            fail("Clear Day 2", p);
            return FAILURE;
        }
        if (print_calendar(calendar, stdout, 0) == FAILURE) {
            destroy_calendar(calendar);
            fail("print_calendar", p);
            return FAILURE;
        }

        printf("Clearing Day 3: ");
        if (clear_day(calendar, 3) == FAILURE) {
            destroy_calendar(calendar);
            fail("Clear Day 3", p);
            return FAILURE;
        }
        if ((calendar->total_events == 0) && (calendar->events[2] == NULL)) {
            printf("PASSED\n");
        } else {
            destroy_calendar(calendar);
            fail("Clear Day 3", p);
            return FAILURE;
        }
        if (print_calendar(calendar, stdout, 0) == FAILURE) {
            destroy_calendar(calendar);
            fail("print_calendar", p);
            return FAILURE;
        }

        printf("Adding events back in... \n");
        if ((add_event(calendar, "Day 1 Event 1", 800, 70, NULL, 1) ==
             FAILURE) ||
            (add_event(calendar, "Day 1 Event 2", 500, 80, NULL, 1) ==
             FAILURE) ||
            (add_event(calendar, "Day 1 Event 3", 500, 90, NULL, 1) ==
             FAILURE) ||
            (add_event(calendar, "Day 2 Event 1", 800, 70, NULL, 2) ==
             FAILURE) ||
            (add_event(calendar, "Day 2 Event 2", 500, 80, NULL, 2) ==
             FAILURE) ||
            (add_event(calendar, "Day 2 Event 3", 500, 90, NULL, 2) ==
             FAILURE) ||
            (add_event(calendar, "Day 3 Event 1", 800, 70, NULL, 3) ==
             FAILURE) ||
            (add_event(calendar, "Day 3 Event 2", 500, 80, NULL, 3) ==
             FAILURE) ||
            (add_event(calendar, "Day 3 Event 3", 500, 90, NULL, 3) ==
             FAILURE)) {
            destroy_calendar(calendar);
            fail("add_event", p);
            return FAILURE;
        }
        if (print_calendar(calendar, stdout, 1) == FAILURE) {
            destroy_calendar(calendar);
            fail("print_calendar", p);
            return FAILURE;
        }
        /*********************EDGE CASES***********************/
        printf("Calendar Null Case: ");
        if (clear_day(NULL, 1) == SUCCESS) {
            destroy_calendar(calendar);
            fail("Calendar Null Case", p);
            return FAILURE;
        }
        printf("PASSED\n");

        printf("Days<1 Case: ");
        if (clear_day(calendar, 0) == SUCCESS) {
            destroy_calendar(calendar);
            fail("Days<1 Case", p);
            return FAILURE;
        }
        printf("PASSED\n");

        printf("Days>num_days Case: ");
        if (clear_day(calendar, 0) == SUCCESS) {
            destroy_calendar(calendar);
            fail("Days>num_days Case", p);
            return FAILURE;
        }
        printf("PASSED\n");
        /******************************************************/
        destroy_calendar(calendar);
        retVal = SUCCESS;
    } else {
        fail("init_calendar", p);
        return FAILURE;
    }
    if (retVal == 8) {
        printf("No tests ran :(\n");
        retVal = FAILURE;
    }
    return retVal;
}
/*****************************************************/
/* Tests regular and edge cases for destroy calendar*/
static int test10() {
    int retVal = 8, *p = &retVal;
    Calendar *calendar;

    printf("*********************************************************\n");
    printf("DESTROY CALENDAR TESTS: \n");
    if (init_calendar("Reg", 3, comp_minutes, NULL, &calendar) == SUCCESS) {

        if ((add_event(calendar, "Day 1 Event 1", 800, 70, NULL, 1) ==
             FAILURE) ||
            (add_event(calendar, "Day 1 Event 2", 500, 80, NULL, 1) ==
             FAILURE) ||
            (add_event(calendar, "Day 1 Event 3", 500, 90, NULL, 1) ==
             FAILURE) ||
            (add_event(calendar, "Day 2 Event 1", 800, 70, NULL, 2) ==
             FAILURE) ||
            (add_event(calendar, "Day 2 Event 2", 500, 80, NULL, 2) ==
             FAILURE) ||
            (add_event(calendar, "Day 2 Event 3", 500, 90, NULL, 2) ==
             FAILURE) ||
            (add_event(calendar, "Day 3 Event 1", 800, 70, NULL, 3) ==
             FAILURE) ||
            (add_event(calendar, "Day 3 Event 2", 500, 80, NULL, 3) ==
             FAILURE) ||
            (add_event(calendar, "Day 3 Event 3", 500, 90, NULL, 3) ==
             FAILURE)) {
            destroy_calendar(calendar);
            fail("add_event", p);
            return FAILURE;
        }
        /**********************REG CASE************************/
        printf("Destorying Calendar: ");
        if ((destroy_calendar(calendar) == FAILURE)) {
            fail("destroy_calendar", p);
            return FAILURE;
        }
        printf("PASSED\n");
        /**********************EDGE CASE************************/
        printf("Calendar Null Case: ");
        if ((destroy_calendar(NULL) == SUCCESS)) {
            fail("destroy_calendar", p);
            return FAILURE;
        }
        printf("PASSED\n");

        retVal = SUCCESS;
    } else {
        fail("init_calendar", p);
        return FAILURE;
    }
    if (retVal == 8) {
        printf("No tests ran :(\n");
        retVal = FAILURE;
    }
    return retVal;
}
/*****************************************************/
int main() {
    int result = SUCCESS;

    /***** Starting memory checking *****/
    start_memory_check();
    /***** Starting memory checking *****/

    if (test1() == FAILURE)
        result = FAILURE;
    if (test2() == FAILURE)
        result = FAILURE;
    if (test3() == FAILURE)
        result = FAILURE;
    if (test4() == FAILURE)
        result = FAILURE;
    if (test5() == FAILURE)
        result = FAILURE;
    if (test6() == FAILURE)
        result = FAILURE;
    if (test7() == FAILURE)
        result = FAILURE;
    if (test8() == FAILURE)
        result = FAILURE;
    if (test9() == FAILURE)
        result = FAILURE;
    if (test10() == FAILURE)
        result = FAILURE;

    if (result != FAILURE) {
        printf("\n\nALL TESTS PASSED!!!!!!!\n");
    }

    /****** Gathering memory checking info *****/
    stop_memory_check();
    /****** Gathering memory checking info *****/

    if (result == FAILURE) {
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
