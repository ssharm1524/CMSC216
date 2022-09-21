/* Name: Shiva Sharma
 * UID: 117912678
 * Directory ID: ssharm08 */
#include "document.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

static int is_blank_line(const char *line);
static void modify_text(Document *doc, const char *target,
                        const char *replacement);
static void replace_in_line(char *src, const char *replacement,
                            const int target_len, const char *start);

/* OVERALL NOTE: Anywhere you see  something like "paragraph-1" or "line-1" is
 * due to the fact that lines and paragraphs use zero-based indexing, but are
 * passed with one-based indexing when these functions are called */

int init_document(Document *doc, const char *name) {
    if (doc != NULL && name != NULL && strlen(name) < MAX_STR_SIZE) {
        strcpy(doc->name, name);
        doc->number_of_paragraphs = 0;
        return SUCCESS;
    }
    return FAILURE;
}

int reset_document(Document *doc) {
    /* call init_document which does all the work for us, including NULL
     * checks.*/
    if (init_document(doc, doc->name) == SUCCESS) {
        return SUCCESS;
    }
    return FAILURE;
}

int print_document(Document *doc) {
    int par_index, line_index, num_pars, num_lines = 0,
                                         *num_lines_ptr = &num_lines;
    Paragraph curr_par;

    if (doc != NULL) {
        num_pars = doc->number_of_paragraphs;
        /* print doc name and number of paragraphs */
        printf("Document name: \"%s\"\n", doc->name);
        printf("Number of Paragraphs: %d\n", num_pars);
        /* for each paragraph in our document: */
        for (par_index = 0; par_index < num_pars; par_index++) {
            /* get the current paragraph */
            curr_par = (doc->paragraphs)[par_index];
            get_number_lines_paragraph(doc, par_index + 1, num_lines_ptr);
            /* for each line in that paragraph: */
            for (line_index = 0; line_index < num_lines; line_index++) {
                /* print that line */
                printf("%s\n", curr_par.lines[line_index]);
            }
            if (par_index == num_pars - 1 || num_lines != 0) {
                printf("\n"); /* print blank line to separate paragraphs */
            }
        }
        return SUCCESS;
    }
    return FAILURE;
}

int add_paragraph_after(Document *doc, int paragraph_number) {
    int curr_par;
    Paragraph new_par = {0}; /* create new empty paragraph to be added */

    if (doc != NULL && doc->number_of_paragraphs < MAX_PARAGRAPHS &&
        paragraph_number <= (doc->number_of_paragraphs)) {
        /* go through the doc and shift the paragraphs after the given
         * number an index forward */
        for (curr_par = (doc->number_of_paragraphs - 1);
             curr_par > paragraph_number - 1; curr_par--) {
            doc->paragraphs[curr_par + 1] = doc->paragraphs[curr_par];
        }
        /* set the paragraph after the given one to the new empty paragraph */
        doc->paragraphs[paragraph_number] = new_par;
        /* increment the number of paragraphs to refelct the addition */
        (doc->number_of_paragraphs)++;
        return SUCCESS;
    }
    return FAILURE;
}

int add_line_after(Document *doc, int paragraph_number, int line_number,
                   const char *new_line) {
    int curr_line, num_lines = 0, *num_lines_ptr = &num_lines;
    Paragraph *curr_par = NULL;

    /* checks if doc!=NULL && paragraph_number<=doc->number_of_paragraphs) for
     * us */
    if (get_number_lines_paragraph(doc, paragraph_number, num_lines_ptr) ==
        SUCCESS) {
        /* checks rest of the conditions */
        if (num_lines < MAX_PARAGRAPH_LINES && line_number <= num_lines &&
            new_line != NULL) {
            /* set a pointer to the current paragraph */
            curr_par = &(doc->paragraphs[paragraph_number - 1]);
            /* go through the paragraph and shift the lines after the given
             * number an index forward */
            for (curr_line = num_lines - 1; curr_line > line_number - 1;
                 curr_line--) {
                strcpy(curr_par->lines[curr_line + 1],
                       curr_par->lines[curr_line]);
            }
            /* set the line after the passed target line to the passed
             * replacement line */
            strcpy(curr_par->lines[line_number], new_line);
            /* increment the number of lines to refelct the addition */
            (curr_par->number_of_lines)++;
            return SUCCESS;
        }
    }
    return FAILURE;
}

int get_number_lines_paragraph(Document *doc, int paragraph_number,
                               int *number_of_lines) {
    if (doc != NULL && paragraph_number <= doc->number_of_paragraphs) {
        *number_of_lines =
            doc->paragraphs[paragraph_number - 1].number_of_lines;
        return SUCCESS;
    }
    return FAILURE;
}

int append_line(Document *doc, int paragraph_number, const char *new_line) {
    if (add_line_after(doc, paragraph_number,
                       doc->paragraphs[paragraph_number - 1].number_of_lines,
                       new_line) == SUCCESS) {
        return SUCCESS;
    };
    return FAILURE;
}

int remove_line(Document *doc, int paragraph_number, int line_number) {
    int curr_line, num_lines = 0, *num_lines_ptr = &num_lines;
    Paragraph *curr_par = NULL;

    /* checks if doc!=NULL && paragraph_number<=doc->number_of_paragraphs) for
     * us */
    if (get_number_lines_paragraph(doc, paragraph_number, num_lines_ptr) ==
        SUCCESS) {
        /* checks rest of the conditions */
        if (line_number <= num_lines) {
            /* set a pointer to the current paragraph */
            curr_par = &(doc->paragraphs[paragraph_number - 1]);
            /* go through the paragraph and shift the lines after the given
             * number an index backward */
            for (curr_line = line_number - 1; curr_line < num_lines - 1;
                 curr_line++) {
                strcpy(curr_par->lines[curr_line],
                       curr_par->lines[curr_line + 1]);
            }
            /* decrement the number of lines to refelct the subtraction/ */
            (curr_par->number_of_lines)--;
            return SUCCESS;
        }
    }
    return FAILURE;
}

int load_document(Document *doc, char data[][MAX_STR_SIZE + 1],
                  int data_lines) {
    int curr_data_line, par_num = 1;

    if (doc != NULL && data != NULL && data_lines != 0) {
        /* create first paragraph */
        add_paragraph_after(doc, 0);
        /* loop thru lines in data */
        for (curr_data_line = 0; curr_data_line < data_lines;
             curr_data_line++) {
            /* create a new paragraph if line is the empty string OR append line
             * to end of the current paragraph */
            if (strcmp(data[curr_data_line], "") == 0) {
                add_paragraph_after(doc, par_num);
                par_num++;
            } else {
                append_line(doc, par_num, data[curr_data_line]);
            }
        }
        return SUCCESS;
    }
    return FAILURE;
}

int replace_text(Document *doc, const char *target, const char *replacement) {
    if (doc != NULL && target != NULL && replacement != NULL) {
        modify_text(doc, target, replacement);
        return SUCCESS;
    }
    return FAILURE;
}

int highlight_text(Document *doc, const char *target) {
    char replacement[MAX_STR_SIZE + 1];

    if (doc != NULL && target != NULL) {
        strcpy(replacement, HIGHLIGHT_START_STR);
        strcat(replacement, target);
        strcat(replacement, HIGHLIGHT_END_STR);
        modify_text(doc, target, replacement);
        return SUCCESS;
    }
    return FAILURE;
}

int remove_text(Document *doc, const char *target) {
    if (doc != NULL && target != NULL) {
        modify_text(doc, target, "");
        return SUCCESS;
    }
    return FAILURE;
}

int load_file(Document *doc, const char *filename) {
    int par_num = 1;
    char curr_data_line[MAX_STR_SIZE + 1];
    FILE *input;

    if (doc != NULL && filename != NULL) {
        input = fopen(filename, "r");
        if (input == NULL) {
            return FAILURE;
        }
        /* create first paragraph */
        add_paragraph_after(doc, 0);
        /* loop thru lines in data */
        while (fgets(curr_data_line, MAX_STR_SIZE + 1, input) != NULL) {
            /* create a new paragraph if line is the empty string OR append line
             * to end of the current paragraph */
            if (is_blank_line(curr_data_line)) {
                add_paragraph_after(doc, par_num);
                par_num++;
            } else {
                append_line(doc, par_num, curr_data_line);
            }
        }
        fclose(input);
        return SUCCESS;
    }
    return FAILURE;
}

/**
 * @brief goes through a line and returns false if it finds a non-whitespace
 * character, otherwise returns true to represent that the passed line is blank.
 *
 * @param line the line to search
 * @return int whether the line is blank or not. 1 if true, 0 if false.
 */
static int is_blank_line(const char *line) {
    int i;

    if (strcmp(line, "\n") == 0) {
        return 1;
    }
    for (i = 0; i < strlen(line); i++) {
        if (!isspace(line[i])) {
            return 0;
        }
    }
    return 1;
}

int save_document(Document *doc, const char *filename) {
    FILE *output;
    int par_index, line_index, num_pars, num_lines = 0,
                                         *num_lines_ptr = &num_lines;
    Paragraph curr_par;

    if (doc != NULL && filename != NULL) {
        output = fopen(filename, "w");
        if (output == NULL) {
            return FAILURE;
        }
        num_pars = doc->number_of_paragraphs;
        /* for each paragraph in our document: */
        for (par_index = 0; par_index < num_pars; par_index++) {
            /* get the current paragraph */
            curr_par = (doc->paragraphs)[par_index];
            get_number_lines_paragraph(doc, par_index + 1, num_lines_ptr);
            /* for each line in that paragraph: */
            for (line_index = 0; line_index < num_lines; line_index++) {
                /* print that line */
                fprintf(output, "%s\n", curr_par.lines[line_index]);
            }
            if (par_index == num_pars - 1 || num_lines != 0) {
                fprintf(output,
                        "\n"); /* print blank line to separate paragraphs */
            }
        }
        fclose(output);
        return SUCCESS;
    }
    return FAILURE;
}

/* This function takes in a document pointer variable, a target string, and a
 * replacement string. It then goes through each line in each paragraph of the
 * document and replaces the target string with the replacement string. This
 * function performs no checks for validity of the passed arguments, as it is
 * meant to be called inside other functions where data validity is already
 * checked for*/
static void modify_text(Document *doc, const char *target,
                        const char *replacement) {
    int par_index, line_index, target_len = strlen(target);
    char *ptr_to_start_index;
    Paragraph *curr_par = NULL;

    /* for each paragraph in the doc */
    for (par_index = 0; par_index < doc->number_of_paragraphs; par_index++) {
        /* set a pointer to the current paragraph being modified in this
         * iteration */
        curr_par = &(doc->paragraphs[par_index]);
        /* for each line in the paragraph being modified */
        for (line_index = 0; line_index < curr_par->number_of_lines;
             line_index++) {
            /* use strstr() to set a pointer to the first byte of the first
             * character of the first instance of the target string in the line
             */
            ptr_to_start_index = strstr(curr_par->lines[line_index], target);
            /* while that pointer is not NULL (meaning there are instances of
             * the target string left in the line) and still pointing to a valid
             * character... */
            while (ptr_to_start_index != NULL &&
                   ptr_to_start_index - curr_par->lines[line_index] <
                       MAX_STR_SIZE &&
                   ptr_to_start_index - curr_par->lines[line_index] > -1) {
                /* call the replace in line funciton on the current pointer to
                 * replace that instance of the target string in our line */
                replace_in_line(curr_par->lines[line_index], replacement,
                                target_len, ptr_to_start_index);
                /* now find the first byte of the first character of the next
                 * instance of the target string in the remaining line.
                 * "ptr_to_start_index + strlen(replacement)" is done to begin
                 * looking at string started by the character contained by the
                 * next  logical unit of memory AFTER the end of the replacement
                 */
                ptr_to_start_index =
                    strstr(ptr_to_start_index + strlen(replacement), target);
            }
            /* if the loop never runs it means the curent line contains no
             * instances of the target string. If it runs then ends, it means
             * all instances have been replaced. */
        }
    }
}

/* This function takes in a souce string, replacement string, length of the
 * target string being replaced in the source, and a pointer to the first byte
 * of the first character of the target string being replaced. It then creates a
 * new string by concatonating the src string up til the index given by the
 * passed pointer, the replacement string, and finally the rest of the src
 * string. Finally, it copies this new string to the src string and therby
 * modifies it to reflect the replacement. */
static void replace_in_line(char *src, const char *replacement,
                            const int target_len, const char *start) {
    char temp[MAX_STR_SIZE + 1] = {""}, modded[MAX_STR_SIZE + 1] = {""};
    /* find index where the target string starts */
    int index_of_start = start - src;
    /* create the modded string */
    strncpy(modded, src, index_of_start);
    strcat(modded, replacement);
    strncpy(temp, src + index_of_start + target_len, strlen(src));
    strcat(modded, temp);
    /* copy the modded string to src */
    strcpy(src, modded);
}
