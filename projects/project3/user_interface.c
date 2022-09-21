/**
 * @file user_interface.c
 * @author Shiva Sharma
 * @brief UI for the doc manager in project 2
 * UID: 117912678
 * UMD Directory ID: ssharm08
 */
#include "document.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

#define LINE_LIMIT 1024

static int is_blank_line(const char *line);
static int command_picker(Document *doc, const char *line);
static int is_num(const char *str);
static int parse_target(const char *line, char *target);
static int parse_target_and_replacement(const char *line, char *target,
                                        char *replacement);

int main(int argc, char *argv[]) {
    FILE *input;
    Document main_document;
    int exit = 0;
    char line[LINE_LIMIT + 1];
    char initial_char_from_line;

    /* Deciding where input will come from */
    if (argc == 1) {
        /* Standard input */
        input = stdin;
        printf("> ");
        /* Create document to be edited */
        init_document(&main_document, "main_document");
        /* while there is input and exit is false */
        while (fgets(line, MAX_STR_SIZE + 1, input) != NULL && exit == 0) {
            /* if the inputted line is not blank, get the inital character to
             * ensure it is not a comment, edit the line to get rid of the '\n'
             * character at the end, and pass the line into our command picker
             * function */
            if (!is_blank_line(line)) {
                sscanf(line, " %c", &initial_char_from_line);
                line[strlen(line) - 1] = '\0';
                if (initial_char_from_line != '#') {
                    exit = command_picker(&main_document, line);
                }
            }
            /* extra if to ensure an extra carrot does not print if quit or exit
             * command has been entered */
            if (!exit) {
                printf("> ");
            }
        }
    } else if (argc == 2) {
        /* Using file provided by the user. Open file and check success */
        input = fopen(argv[1], "r");
        if (input == NULL) {
            fprintf(stderr, "%s cannot be opened", argv[1]);
            return EX_OSERR;
        }
        /* Create document to be edited */
        init_document(&main_document, "main_document");
        /* while there is input and exit is false */
        while (fgets(line, MAX_STR_SIZE + 1, input) != NULL && exit == 0) {
            /* if the inputted line is not blank, get the inital character to
             * ensure it is not a comment, edit the line to get rid of the '\n'
             * character at the end, and pass the line into our command picker
             * function */
            if (!is_blank_line(line)) {
                sscanf(line, " %c", &initial_char_from_line);
                line[strlen(line) - 1] = '\0';
                if (initial_char_from_line != '#') {
                    exit = command_picker(&main_document, line);
                }
            }
        }
        fclose(input);
    } else {
        fprintf(stderr, "Usage: user_interface\n");
        fprintf(stderr, "Usage: user_interface <filename>\n");
        return EX_USAGE;
    }

    return 0;
}

/**
 * @brief parses the passed line to read a command and its arguments. then
 * performs that command on the passed document.
 * @param doc the document to be edited
 * @param line the line being read
 */
static int command_picker(Document *doc, const char *line) {
    char initial_string_from_line[LINE_LIMIT + 1], overflow[LINE_LIMIT + 1];

    /* read and hold the first string */
    sscanf(line, " %s", initial_string_from_line);
    /* each if represents a valid command case for the initial string captured
     * from the line. if the first string captured is not a valid command case,
     * we print "Invalid Command" */
    if (strcmp(initial_string_from_line, "add_paragraph_after") == 0) {
        char par_num[LINE_LIMIT + 1];
        int par_num_int, num_arguments_read, status;

        /* parse the line for the expected input associated with the command */
        num_arguments_read = sscanf(line, " %s%s%s", initial_string_from_line,
                                    par_num, overflow);
        /* turn the necessary string into an int */
        par_num_int = atoi(par_num);
        /* if input is valid, run the command. else, print invalid message */
        if (num_arguments_read == 2 && is_num(par_num) && par_num_int >= 0) {
            status = add_paragraph_after(doc, par_num_int);
            /* print failure message if command failed */
            if (status == FAILURE) {
                fprintf(stdout, "%s failed\n", initial_string_from_line);
            }
        } else {
            fprintf(stdout, "Invalid Command\n");
        }
    } else if (strcmp(initial_string_from_line, "add_line_after") == 0) {
        char par_num[LINE_LIMIT + 1], line_num[LINE_LIMIT + 1],
            new_line[LINE_LIMIT + 1], line_to_add[LINE_LIMIT + 1];
        char *star_ptr;
        int par_num_int, line_num_int, num_arguments_read, status;

        /* parse the line for the expected input associated with the command */
        num_arguments_read = sscanf(line, " %s%s%s%s", initial_string_from_line,
                                    par_num, line_num, new_line);
        /* turn the necessary strings into ints */
        par_num_int = atoi(par_num);
        line_num_int = atoi(line_num);
        /* set a pointer to the third argument in the line. (should just be a *)
         */
        star_ptr = &(new_line[0]);
        /* if input is valid, run the command. else, print invalid message */
        if (num_arguments_read == 4 && is_num(par_num) && par_num_int > 0 &&
            is_num(line_num) && line_num_int >= 0 && *star_ptr == '*') {
            /* set a ptr to the star (one index behind where the text to
             * be added starts */
            star_ptr = strstr(line, "*");
            /* create line to pass into doc command using the pointer+1 */
            strcpy(line_to_add, ++star_ptr);
            status =
                add_line_after(doc, par_num_int, line_num_int, line_to_add);
            /* print failure message if command failed */
            if (status == FAILURE) {
                fprintf(stdout, "%s failed\n", initial_string_from_line);
            }
        } else {
            fprintf(stdout, "Invalid Command\n");
        }
    } else if (strcmp(initial_string_from_line, "print_document") == 0) {
        int num_arguments_read;

        /* parse the line for the expected input associated with the command */
        num_arguments_read =
            sscanf(line, " %s%s", initial_string_from_line, overflow);
        /* if input is valid, run the command. else, print invalid message */
        if (num_arguments_read == 1) {
            print_document(doc);
        } else {
            fprintf(stdout, "Invalid Command\n");
        }
    } else if (strcmp(initial_string_from_line, "append_line") == 0) {
        char par_num[LINE_LIMIT + 1], new_line[LINE_LIMIT + 1],
            line_to_add[LINE_LIMIT + 1];
        char *star_ptr;
        int par_num_int, num_arguments_read, status;

        /* parse the line for the expected input associated with the command */
        num_arguments_read = sscanf(line, " %s%s%s", initial_string_from_line,
                                    par_num, new_line);
        /* turn the necessary string into an int */
        par_num_int = atoi(par_num);
        /* set a pointer to the second argument in the line. (should just be a
         * *) */
        star_ptr = &(new_line[0]);
        /* if input is valid, run the command. else, print invalid message */
        if (num_arguments_read == 3 && is_num(par_num) && par_num_int > 0 &&
            *star_ptr == '*') {
            /* set a ptr to the star (one index behind where the text to
             * be added starts */
            star_ptr = strstr(line, "*");
            /* create line to pass into doc command using the pointer+1 */
            strcpy(line_to_add, ++star_ptr);
            status = append_line(doc, par_num_int, line_to_add);
            /* print failure message if command failed */
            if (status == FAILURE) {
                fprintf(stdout, "%s failed\n", initial_string_from_line);
            }
        } else {
            fprintf(stdout, "Invalid Command\n");
        }
    } else if (strcmp(initial_string_from_line, "remove_line") == 0) {
        char par_num[LINE_LIMIT + 1], line_num[LINE_LIMIT + 1];
        int par_num_int, line_num_int, num_arguments_read, status;

        /* parse the line for the expected input associated with the command */
        num_arguments_read = sscanf(line, " %s%s%s%s", initial_string_from_line,
                                    par_num, line_num, overflow);
        /* turn the necessary strings to ints */
        par_num_int = atoi(par_num);
        line_num_int = atoi(line_num);
        /* if input is valid, run the command. else, print invalid message */
        if (num_arguments_read == 3 && is_num(par_num) && par_num_int > 0 &&
            is_num(line_num) && line_num_int > 0) {
            status = remove_line(doc, par_num_int, line_num_int);
            /* print failure message if command failed */
            if (status == FAILURE) {
                fprintf(stdout, "%s failed\n", initial_string_from_line);
            }
        } else {
            fprintf(stdout, "Invalid Command\n");
        }
    } else if (strcmp(initial_string_from_line, "load_file") == 0) {
        char filename[LINE_LIMIT + 1];
        int num_arguments_read, status;

        /* parse the line for the expected input associated with the command */
        num_arguments_read = sscanf(line, " %s%s%s", initial_string_from_line,
                                    filename, overflow);
        /* if input is valid, run the command. else, print invalid message */
        if (num_arguments_read == 2) {
            status = load_file(doc, filename);
            /* print failure message if command failed */
            if (status == FAILURE) {
                fprintf(stdout, "%s failed\n", initial_string_from_line);
            }
        } else {
            fprintf(stdout, "Invalid Command\n");
        }
    } else if (strcmp(initial_string_from_line, "replace_text") == 0) {
        char target[LINE_LIMIT + 1] = {""}, replacement[LINE_LIMIT + 1] = {""};
        int status;

        /* parse the line for the expected input associated with the command.
         * if input is valid, run the command. else, print invalid message */
        if (parse_target_and_replacement(line, target, replacement) ==
            SUCCESS) {
            status = replace_text(doc, target, replacement);
            /* print failure message if command failed */
            if (status == FAILURE) {
                fprintf(stdout, "%s failed\n", initial_string_from_line);
            }
        } else {
            fprintf(stdout, "Invalid Command\n");
        }
    } else if (strcmp(initial_string_from_line, "highlight_text") == 0) {
        char target[LINE_LIMIT + 1] = {""};

        /* parse the line for the expected input associated with the command.
         * if input is valid, run the command. else, print invalid message */
        if (parse_target(line, target) == SUCCESS) {
            highlight_text(doc, target);
        } else {
            fprintf(stdout, "Invalid Command\n");
        }
    } else if (strcmp(initial_string_from_line, "remove_text") == 0) {
        char target[LINE_LIMIT + 1] = {""};

        /* parse the line for the expected input associated with the command.
         * if input is valid, run the command. else, print invalid message */
        if (parse_target(line, target) == SUCCESS) {
            remove_text(doc, target);
        } else {
            fprintf(stdout, "Invalid Command\n");
        }
    } else if (strcmp(initial_string_from_line, "save_document") == 0) {
        char filename[LINE_LIMIT + 1];
        int num_arguments_read, status;

        /* parse the line for the expected input associated with the command */
        num_arguments_read = sscanf(line, " %s%s%s", initial_string_from_line,
                                    filename, overflow);
        /* if input is valid, run the command. else, print invalid message */
        if (num_arguments_read == 2) {
            status = save_document(doc, filename);
            /* print failure message if command failed */
            if (status == FAILURE) {
                fprintf(stdout, "%s failed\n", initial_string_from_line);
            }
        } else {
            fprintf(stdout, "Invalid Command\n");
        }
    } else if (strcmp(initial_string_from_line, "reset_document") == 0) {
        int num_arguments_read;

        /* parse the line for the expected input associated with the command */
        num_arguments_read =
            sscanf(line, " %s%s", initial_string_from_line, overflow);
        /* if input is valid, run the command. else, print invalid message */
        if (num_arguments_read == 1) {
            reset_document(doc);
        } else {
            fprintf(stdout, "Invalid Command\n");
        }
    } else if (strcmp(initial_string_from_line, "quit") == 0) {
        int num_arguments_read;

        /* parse the line for the expected input associated with the command */
        num_arguments_read =
            sscanf(line, " %s%s", initial_string_from_line, overflow);
        /* if input is valid, run the command. else, print invalid message */
        if (num_arguments_read == 1) {
            return 1;
        } else {
            fprintf(stdout, "Invalid Command\n");
        }
    } else if (strcmp(initial_string_from_line, "exit") == 0) {
        int num_arguments_read;

        /* parse the line for the expected input associated with the command */
        num_arguments_read =
            sscanf(line, " %s%s", initial_string_from_line, overflow);
        /* if input is valid, run the command. else, print invalid message */
        if (num_arguments_read == 1) {
            return 1;
        } else {
            fprintf(stdout, "Invalid Command\n");
        }
    } else {
        fprintf(stdout, "Invalid Command\n");
    }
    return 0;
}

/**
 * @brief looks for the target string within a line which is marked by being
 * inside quotes. Example: This is the "target". Then, this function copies that
 * string into the passed target parameter. Returns an int representing whether
 * the target was found or not
 *
 * @param line the line to search
 * @param target the string to hold the text found by the search
 * @return int whether the search was successful or not. returns the macro
 * SUCCESS if search was successful and FAILURE if it was not.
 */
static int parse_target(const char *line, char *target) {
    int i, quotes_counter = 0, quotes_indexes[2];

    /* look for a set of qoutes and record the indexes where they occur */
    for (i = 0; i < (int)strlen(line); i++) {
        if (line[i] == '\"') {
            quotes_indexes[quotes_counter] = i;
            quotes_counter++;
        }
    }
    /* if a set is found, copy the text inside into target */
    if (quotes_counter == 2) {
        strncpy(target, line + quotes_indexes[0] + 1,
                quotes_indexes[1] - quotes_indexes[0] - 1);
        return SUCCESS;
    }
    return FAILURE;
}

/**
 * @brief looks for two strings within a line which are both marked by being
 * inside quotes. Example: This is the "target" and this is "replacement". Then,
 * this function copies both strings into the passed parameters. Returns an int
 * representing whether both were found or not.
 *
 * @param line the line to search
 * @param target the first string to hold the text found by the search
 * @param replacement the second string to hold the text found by the search
 * @return int whether the search was successful or not. returns the macro
 * SUCCESS if search was successful and FAILURE if it was not.
 */
static int parse_target_and_replacement(const char *line, char *target,
                                        char *replacement) {
    int i, quotes_counter = 0, quotes_indexes[4];

    /* look for two sets of qoutes and record the indexes where they occur */
    for (i = 0; i < (int)strlen(line); i++) {
        if (line[i] == '\"') {
            quotes_indexes[quotes_counter] = i;
            quotes_counter++;
        }
    }
    /* if two sets are found, copy the text inside into the passed arguments */
    if (quotes_counter == 4) {
        strncpy(target, line + quotes_indexes[0] + 1,
                quotes_indexes[1] - quotes_indexes[0] - 1);
        strncpy(replacement, line + quotes_indexes[2] + 1,
                quotes_indexes[3] - quotes_indexes[2] - 1);
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
    for (i = 0; i < (int)strlen(line); i++) {
        if (!isspace(line[i])) {
            return 0;
        }
    }
    return 1;
}

/**
 * @brief goes through a string and returns flase if it finds a non-digit
 * character, otherwise returns true to represent that the passed line is a
 * number.
 *
 * @param str the string to search
 * @return int whether the line is a number or not. 1 if true, 0 if false.
 */
static int is_num(const char *str) {
    int i;
    for (i = 0; i < (int)strlen(str); i++) {
        if (!isdigit(str[i])) {
            return 0;
        }
    }
    return 1;
}