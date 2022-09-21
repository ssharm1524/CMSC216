/*
 * Shiva Sharma
 * UID: 117912678
 * UMD Directory ID: ssharm08
 */
#include "calendar.h"
#include "event.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int init_calendar(const char *name, int days,
                  int (*comp_func)(const void *ptr1, const void *ptr2),
                  void (*free_info_func)(void *ptr), Calendar **calendar) {

    if (calendar != NULL && name != NULL && days >= 1 &&
        ((*calendar) = malloc(sizeof(Calendar))) &&
        ((*calendar)->name = malloc(strlen(name) + 1)) &&
        ((*calendar)->events = calloc(days, sizeof(Event)))) {

        strcpy((*calendar)->name, name);
        (*calendar)->days = days;
        (*calendar)->total_events = 0;
        (*calendar)->comp_func = comp_func;
        (*calendar)->free_info_func = free_info_func;
        return SUCCESS;
    }
    return FAILURE;
}

int print_calendar(Calendar *calendar, FILE *output_stream, int print_all) {
    if (calendar != NULL && output_stream != NULL) {
        int days_index;

        if (print_all) {
            fprintf(output_stream,
                    "Calendar's Name: \"%s\"\nDays: %d\nTotal Events: %d\n",
                    calendar->name, calendar->days, calendar->total_events);
        }
        fprintf(output_stream, "\n**** Events ****\n");
        if (calendar->total_events) {
            /*loop thru each day*/
            for (days_index = 0; days_index < calendar->days; days_index++) {
                Event *curr = calendar->events[days_index];
                fprintf(output_stream, "Day %d\n", days_index + 1);
                /*loop thru each event in the day*/
                while (curr) {
                    fprintf(
                        output_stream,
                        "Event's Name: \"%s\", Start_time: %d, Duration: %d\n",
                        curr->name, curr->start_time, curr->duration_minutes);
                    curr = curr->next;
                }
            }
        }
        return SUCCESS;
    }
    return FAILURE;
}

int add_event(Calendar *calendar, const char *name, int start_time,
              int duration_minutes, void *info, int day) {
    Event *new;

    if (calendar != NULL && name != NULL && start_time >= 0 &&
        start_time <= 2400 && duration_minutes > 0 && day >= 1 &&
        day <= calendar->days && find_event(calendar, name, NULL) == FAILURE &&
        (calendar->comp_func) && (new = malloc(sizeof(Event))) &&
        (new->name = malloc(strlen(name) + 1))) {
        Event *curr = calendar->events[day - 1], *prev = NULL;

        /*initialize the newly allocated event*/
        strcpy(new->name, name);
        new->info = info;
        new->duration_minutes = duration_minutes;
        new->start_time = start_time;
        new->next = NULL;

        /*loop thru the linked list til we find where to add*/
        while (curr && calendar->comp_func(curr, new) < 0) {
            prev = curr;
            curr = curr->next;
        }
        /*add the new event depending on the case (head/nonhead)*/
        if (prev == NULL) {
            calendar->events[day - 1] = new;
            new->next = curr;
        } else {
            prev->next = new;
            new->next = curr;
        }
        (calendar->total_events)++;
        return SUCCESS;
    }
    return FAILURE;
}

int find_event(Calendar *calendar, const char *name, Event **event) {
    if (calendar != NULL && name != NULL) {
        int days_index;
        for (days_index = 0; days_index < calendar->days; days_index++) {
            if (find_event_in_day(calendar, name, days_index + 1, event) ==
                SUCCESS) {
                return SUCCESS;
            }
        }
    }
    return FAILURE;
}

int find_event_in_day(Calendar *calendar, const char *name, int day,
                      Event **event) {
    if (calendar != NULL && name != NULL && day >= 1 && day <= calendar->days) {
        Event *curr = calendar->events[day - 1];
        /* go thru each node in the linked list and search for one with a
         * matching name*/
        while (curr) {
            if (!strcmp(curr->name, name)) {
                if (event && curr) {
                    *event = curr;
                }
                return SUCCESS;
            }
            curr = curr->next;
        }
    }
    return FAILURE;
}

int remove_event(Calendar *calendar, const char *name) {
    if (calendar != NULL && name != NULL) {
        int days_index;

        /*loop thru each day*/
        for (days_index = 0; days_index < calendar->days; days_index++) {
            Event *curr = calendar->events[days_index], *prev = NULL;
            /*loop thru each node til we find a match*/
            while (curr && strcmp(curr->name, name)) {
                prev = curr;
                curr = curr->next;
            }
            /*if match was found, remove that node. method of removal depends on
             * case (head/nonhead)*/
            if (curr != NULL) {
                if (prev == NULL) {
                    calendar->events[days_index] = curr->next;
                } else {
                    prev->next = curr->next;
                }

                if (curr->name) {
                    free(curr->name);
                    curr->name = NULL;
                }
                if (curr->info && calendar->free_info_func) {
                    calendar->free_info_func(curr->info);
                }
                free(curr);
                curr = NULL;

                /*decrement total_events to treflect the change*/
                (calendar->total_events)--;

                return SUCCESS;
            }
        }
    }
    return FAILURE;
}

void *get_event_info(Calendar *calendar, const char *name) {
    Event *target;
    if (find_event(calendar, name, &target) == SUCCESS) {
        if (target->info)
            return target->info;
    }
    return NULL;
}

int clear_calendar(Calendar *calendar) {
    if (calendar != NULL) {
        int days_index;
        for (days_index = 0; days_index < calendar->days; days_index++) {
            clear_day(calendar, days_index + 1);
        }
        return SUCCESS;
    }
    return FAILURE;
}

int clear_day(Calendar *calendar, int day) {
    if (calendar != NULL && day >= 1 && day <= calendar->days) {
        Event *curr = calendar->events[day - 1], *prev = NULL;
        /*loop thru each node*/
        while (curr) {
            /*set curr to the next node in the LL and store the current one in
             * prev so that we hold a referrence to both the node being removed
             * and the rest of the list*/
            prev = curr;
            curr = curr->next;

            /*free the node being removed*/
            if (prev->name) {
                free(prev->name);
                prev->name = NULL;
            }
            if (prev->info && calendar->free_info_func) {
                calendar->free_info_func(prev->info);
            }
            free(prev);
            prev = NULL;

            /*adjust total_events to reflect the removal*/
            (calendar->total_events)--;
        }
        /* set the index associated with this LL in the events array to NULL to
         * revoke user access to this LL */
        calendar->events[day - 1] = NULL;
        return SUCCESS;
    }
    return FAILURE;
}

int destroy_calendar(Calendar *calendar) {
    if (calendar != NULL) {
        if (calendar->name) {
            free(calendar->name);
        }
        clear_calendar(calendar);
        if (calendar->events) {
            free(calendar->events);
        }
        free(calendar);
        return SUCCESS;
    }
    return FAILURE;
}