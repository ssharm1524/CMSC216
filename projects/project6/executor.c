/* 117912678 */
#include "executor.h"
#include "command.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sysexits.h>
#include <unistd.h>

#define OPEN_FLAGS (O_WRONLY | O_TRUNC | O_CREAT)
#define DEF_MODE 0664

static void run_and_errcheck(int, char *);
static void run_errchk_exit(int op, char *opname);

int execute(struct tree *t) {
    if (t->conjunction == NONE) {
        if (!strcmp(t->argv[0], "exit")) { /*exit case*/
            exit(EXIT_SUCCESS);
        } else if (!strcmp(t->argv[0], "cd")) { /*cd case*/
            /* if a directory is specified use it otherwise use home*/
            if (t->argv[1]) {
                run_and_errcheck(chdir(t->argv[1]), t->argv[1]);
            } else {
                run_and_errcheck(chdir(getenv("HOME")), getenv("HOME"));
            }
            return 0;
        } else {
            pid_t pid;

            run_errchk_exit((pid = fork()), "fork");
            if (!pid) {
                /*if input/output is specified set stdin/out to point there*/
                if (t->input) {
                    int fd_num;
                    run_errchk_exit((fd_num = open(t->input, O_RDONLY)),
                                    "open");
                    run_errchk_exit(dup2(fd_num, STDIN_FILENO), "dup2");
                    run_errchk_exit(close(fd_num), "close");
                }
                if (t->output) {
                    int fd_num;
                    run_errchk_exit(
                        (fd_num = open(t->output, OPEN_FLAGS, DEF_MODE)),
                        "open");
                    run_errchk_exit(dup2(fd_num, STDOUT_FILENO), "dup2");
                    run_errchk_exit(close(fd_num), "close");
                }
                /*execute command in child process */
                if (execvp(t->argv[0], t->argv) == -1) {
                    fprintf(stderr, "Failed to execute %s\n", t->argv[0]);
                    fflush(stdout);
                    exit(EX_OSERR);
                }
            } else {
                /*reap child and return its exit status*/
                int status;
                wait(&status);
                return WEXITSTATUS(status);
            }
        }
    }

    if (t->conjunction == AND) {
        if (execute(t->left) == 0) {
            return execute(t->right);
        }
        return 1;
    }

    if (t->conjunction == PIPE) {
        pid_t pid;
        int pipefd[2];

        /* return 1 if command is an ambigous redirect*/
        if ((t->left)->output) {
            printf("Ambiguous output redirect.\n");
            fflush(stdout);
            return 1;
        }
        if ((t->right)->input) {
            printf("Ambiguous input redirect.\n");
            fflush(stdout);
            return 1;
        }
        /* pipe and hold the fd numbers of the read and write ends in pipefd */
        run_errchk_exit(pipe(pipefd), "pipe");
        run_errchk_exit((pid = fork()), "fork");
        if (!pid) { /* in first child: */
            /*reading*/
            run_errchk_exit(close(pipefd[1]), "close");
            run_errchk_exit(dup2(pipefd[0], STDIN_FILENO), "dup2");
            run_errchk_exit(close(pipefd[0]), "close");
            exit(execute(t->right));
        } else {
            int pid2;
            run_errchk_exit((pid2 = fork()), "fork");
            if (!pid2) { /* in second child */
                /*writing*/
                run_errchk_exit(close(pipefd[0]), "close");
                run_errchk_exit(dup2(pipefd[1], STDOUT_FILENO), "dup2");
                run_errchk_exit(close(pipefd[1]), "close");
                exit(execute(t->left));
            } else {
                /* close pipe ends and reap children */
                int status1, status2;
                run_errchk_exit(close(pipefd[0]), "close");
                run_errchk_exit(close(pipefd[1]), "close");
                wait(&status1);
                wait(&status2);
                /* return 0 if both children were successful and 1 otherise */
                if (WIFEXITED(status1) && WIFEXITED(status2) &&
                    WEXITSTATUS(status1) == EXIT_SUCCESS &&
                    WEXITSTATUS(status2) == EXIT_SUCCESS) {
                    return 0;
                } else {
                    return 1;
                }
            }
        }
    }

    if (t->conjunction == SUBSHELL) {
        pid_t pid;

        run_errchk_exit((pid = fork()), "fork");
        if (!pid) {
            /*execute command in a separate process*/
            exit(execute(t->left));
        } else {
            /*reap that separate process and return its exit status*/
            int status;
            wait(&status);
            return WEXITSTATUS(status);
        }
    }

    return 0;
}

/* this method will check if the op param is less than 0. if so, it will print
 * the passeed string "opname" to std err. it is intended to error check
 * functions that return a negative value on error. therefore, the funnction
 * call should take place in the "op" parameter.*/
static void run_and_errcheck(int op, char *opname) {
    if (op < 0) {
        perror(opname);
    }
}

/* this method will check if the op param is less than 0. if so, it will print
 * the passeed string "opname" to std err. then, it will exit the current
 * process and set EX_OSSERR as the exit status. it is intended to error
 * check functions that return a negative value on error. therefore, the
 * funnction call should take place in the "op" parameter.*/
static void run_errchk_exit(int op, char *opname) {
    if (op < 0) {
        perror(opname);
        exit(EX_OSERR);
    }
}
