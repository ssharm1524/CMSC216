/* Name: Shiva Sharma
 * UID: 117912678
 * Directory ID: ssharm08 */
#include "document.h"
#include <stdio.h>
#include <string.h>

void modify_text(Document *doc, const char *target, const char *replacement);
void replace_in_line(char *src, const char *replacement, const int target_len,
                     const char *start);

/* OVERALL NOTE: Anywhere you see  something like "paragraph-1" or "line-1" is
 * due to the fact that lines and paragraphs use zero-based indexing, but are
 * passed with one-based indexing when these functions are called */

/* Initializes the document to be empty by setting the passed documents' name
field to the passed name and number_of_paragraphs field to 0. The function will
return the macro FAILURE if doc is NULL, name is NULL, or if the length of the
name provided exceeds MAX_STR_SIZE; otherwise the function will return the macro
SUCCESS. */
int init_document(Document *doc, const char *name) {
    if (doc != NULL && name != NULL && strlen(name) < MAX_STR_SIZE) {
        strcpy(doc->name, name);
        doc->number_of_paragraphs = 0;
        return SUCCESS;
    }
    return FAILURE;
}

/* Sets the number of paragraphs to 0. The function returns FAILURE if doc is
NULL; otherwise the function will return SUCCESS. */
int reset_document(Document *doc) {
    /* call init_document which does all the work for us, including NULL
     * checks.*/
    if (init_document(doc, doc->name) == SUCCESS) {
        return SUCCESS;
    }
    return FAILURE;
}

/* Prints the document’s name, number of paragraphs, followed by the paragraphs.
Each paragraph is separated by a blank line. The function returns FAILURE if doc
is NULL; otherwise the function will return SUCCESS. */
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

/* Adds a paragraph after the specified paragraph number. Paragraph numbers
start at 1. If paragraph number is 0 the paragraph will be added at the
beginning of the document. The function will return FAILURE if doc is NULL, the
document has the maximum number of paragraphs allowed (MAX PARAGRAPHS) or if the
paragraph number is larger than the number of paragraphs available; otherwise,
the function will return SUCCESS. */
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

/* Adds a new line after the line with the specified line number. Line numbers
start at 1. If the line number is 0, the line will be added at the beginning of
the paragraph. The function will return FAILURE if doc is NULL, the paragraph
number exceeds the number of paragraphs available, the paragraph already has the
maximum number of lines allowed, the line number is larger than the available
number of lines or if new line is NULL; otherwise, the function will return
SUCCESS. */
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

/* Returns the number of lines in a paragraph using the number of lines out
parameter. The function will return FAILURE if doc or number of lines is NULL or
if the paragraph number is larger than the number of paragraphs available;
otherwise, the function will return SUCCESS. */
int get_number_lines_paragraph(Document *doc, int paragraph_number,
                               int *number_of_lines) {
    if (doc != NULL && paragraph_number <= doc->number_of_paragraphs) {
        *number_of_lines =
            doc->paragraphs[paragraph_number - 1].number_of_lines;
        return SUCCESS;
    }
    return FAILURE;
}

/* Appends a line to the end of the specified paragraph. The function will
return FAILURE if doc is NULL, the paragraph number exceeds the number of
paragraphs available, the paragraph already has the maximum number of lines
allowed, the line number is larger than the available number of lines or if new
line is NULL; otherwise, the function will return SUCCESS. */
int append_line(Document *doc, int paragraph_number, const char *new_line) {
    if (add_line_after(doc, paragraph_number,
                       doc->paragraphs[paragraph_number - 1].number_of_lines,
                       new_line) == SUCCESS) {
        return SUCCESS;
    };
    return FAILURE;
}

/* Removes the specified line from the paragraph. The function will return
FAILURE if doc is NULL, the paragraph number exceeds the number of paragraphs
available or line number is larger than the number of lines in the paragraph;
otherwise the function will return SUCCESS. */
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

/* The function will add the first data lines number of lines from the data
array to the document. An empty string in the array will create a new paragraph.
Notice that by default the function will create the first paragraph. You can
assume that if data lines is different than 0, the appropriate number of lines
will be present in the data array. The function will return FAILURE if doc is
NULL, data is NULL or data lines is 0; otherwise the function will return
SUCCESS. */
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
/* The function will replace the text target with the text replacement
everywhere it appears in the document. The replacement will not generate a line
that exceeds the maximum line length; also the target will not be the empty
string. The function will return FAILURE if doc, target or replacement are NULL;
otherwise the function will return SUCCESS. */
int replace_text(Document *doc, const char *target, const char *replacement) {
    if (doc != NULL && target != NULL && replacement != NULL) {
        modify_text(doc, target, replacement);
        return SUCCESS;
    }
    return FAILURE;
}

/* The function will highlight the text associated with target everywhere it
 * appears in the document by surrounding the text with the strings HIGHLIGHT
 * START STR and HIGHLIGHT END STR (see doc- ument.h). The highlighting will not
 * exceed the maximum line length; also the target will not be the empty string.
 * The function will return FAILURE if doc or target are NULL; otherwise the
 * function will return SUCCESS. */
int highlight_text(Document *doc, const char *target) {
    char replacement[MAX_STR_SIZE];

    if (doc != NULL && target != NULL) {
        strcpy(replacement, HIGHLIGHT_START_STR);
        strcat(replacement, target);
        strcat(replacement, HIGHLIGHT_END_STR);
        modify_text(doc, target, replacement);
        return SUCCESS;
    }
    return FAILURE;
}

/* The function will remove the text target everywhere it appears in the
 * document. The target will not be the empty string. The function will return
 * FAILURE if doc or target are NULL; otherwise the function will return
 * SUCCESS. */
int remove_text(Document *doc, const char *target) {
    if (doc != NULL && target != NULL) {
        modify_text(doc, target, "");
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
void modify_text(Document *doc, const char *target, const char *replacement) {
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
void replace_in_line(char *src, const char *replacement, const int target_len,
                     const char *start) {
    char temp[MAX_STR_SIZE] = {""}, modded[MAX_STR_SIZE] = {""};
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
