#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <fcntl.h>

char *subdoeval(char *);
void shc_sleep(double);
void doeval(char *);
char **get_input(char *, size_t *, bool);

void shc_sleep(double d) {
    usleep(d * 1000 * 1000);
}

char **get_input(char *input, size_t *pplace, bool issub) {
    size_t alloc = 8;
    size_t commandc = 0;
    char **command = malloc(alloc * sizeof(char *));
    size_t place = *pplace;
    while (input[place] == ' ' || input[place] == '\t' || input[place] == '\n' || input[place] == '\r' || input[place] == ';') {
        place ++;
    }
    while (input[place] != '\0' && input[place] != ';') {
        if (!issub && (input[place] == '\n' || input[place] == '\r' || input[place] == ';')) {
            break;
        }
        if (issub && (input[place] == ';')) {
            break;
        }
        if (commandc + 2 > alloc) {
            alloc *= 1.5;
            command = realloc(command, alloc * sizeof(char *));
        }
        if (input[place] != ' ' && input[place] != '\t' && input[place] != '\n' && input[place]
            && input[place] != '\0' && input[place] != ';'
            && input[place] != '{' && input[place] != '}'
            && input[place] != '[' && input[place] != ']') {
            size_t cmdalloc = 8;
            size_t cmdlen = 0;
            char *cmd = malloc(sizeof(char) * cmdalloc);
            while (input[place] != ' ' && input[place] != '\t' && input[place] != '\n' && input[place] != '\r'
                && input[place] != '\0' && input[place] != ';'
                && input[place] != '{' && input[place] != '}'
                && input[place] != '[' && input[place] != ']') {
                if (cmdlen + 2 > cmdalloc) {
                    cmdalloc *= 1.5;
                    cmd = realloc(cmd, sizeof(char) * cmdalloc);
                }
                cmd[cmdlen] = input[place];
                cmdlen ++;
                place ++;
            }
            cmd[cmdlen] = '\0';
            command[commandc] = cmd;
            commandc ++;
        }
        else if (input[place] == '{') {
            size_t cmdalloc = 8;
            size_t cmdlen = 0;
            char *cmd = malloc(sizeof(char) * cmdalloc);
            size_t depth = 1;
            place ++;
            while (depth != 0 && input[place] != '\0') {
                if (cmdlen + 2 > cmdalloc) {
                    cmdalloc *= 1.5;
                    cmd = realloc(cmd, sizeof(char) * cmdalloc);
                }
                cmd[cmdlen] = input[place];
                if (input[place] == '{') {
                    depth ++;
                }
                if (input[place] == '}') {
                    depth --;
                }
                cmdlen ++;
                place ++;
            };
            cmd[cmdlen-1] = '\0';
            command[commandc] = cmd;
            commandc ++;
        }
        else if (input[place] == '[') {
            size_t cmdalloc = 8;
            size_t cmdlen = 0;
            char *cmd = malloc(sizeof(char) * cmdalloc);
            size_t depth = 1;
            place ++;
            while (depth != 0 && input[place] != '\0') {
                if (cmdlen + 2 > cmdalloc) {
                    cmdalloc *= 1.5;
                    cmd = realloc(cmd, sizeof(char) * cmdalloc);
                }
                cmd[cmdlen] = input[place];
                if (input[place] == '[') {
                    depth ++;
                }
                if (input[place] == ']') {
                    depth --;
                }
                cmdlen ++;
                place ++;
            };
            cmd[cmdlen-1] = '\0';
            command[commandc] = subdoeval(cmd);
            free(cmd);
            commandc ++;
        }
        else if (input[place] == ' ' || input[place] == '\t'
            || (issub && (input[place] == '\n' || input[place] == '\r'))) {
            place ++;
        }
        else {
            fprintf(stderr, "unk char %d\n", input[place]);
            exit(1);
        }
    }
    command[commandc] = NULL;
    *pplace = place;
    return command;
}

char *subdoeval(char *code) {
    unsigned long alloc = 8;
    size_t retpl = 0;
    char *ret = malloc(sizeof(char) * alloc);
    size_t place = 0;
    while (code[place] != '\0') {
        char **inp = get_input(code, &place, true);
        int pipefd[2];
        pipe(pipefd);
        FILE *f = tmpfile();
        pid_t child = fork();
        if (child == 0) {
            dup2(pipefd[1], STDOUT_FILENO);
            close(pipefd[1]);
            if (inp[0] != NULL) {
                execv(inp[0], inp);
                fprintf(stderr, "no such command: %s\n", inp[0]);
            }
            exit(1);
        }
        else {
            fcntl(pipefd[0], F_SETFL, O_NONBLOCK);
            dup2(pipefd[0], fileno(f));
            close(pipefd[0]);
            int stat_loc;
            waitpid(child, &stat_loc, WUNTRACED);
            size_t pl = 0;
            while (inp[pl] != NULL) {
                free(inp[pl]);
                pl ++;
            }
            free(inp);
            rewind(f);
            while (fscanf(f, "%c", ret + retpl) == 1) {
                if (retpl + 2 > alloc) {
                    alloc *= 1.5;
                    ret = realloc(ret, sizeof(char) * alloc);
                }
                retpl ++;
            }
        }
    }
    ret[retpl] = '\0';
    return ret;
}

void doeval(char *code) {
    size_t place = 0;
    while (code[place] != '\0') {
        char **inp = get_input(code, &place, false);
        pid_t child = fork();
        if (child == 0) {
            if (inp[0] != NULL) {
                execv(inp[0], inp);
                fprintf(stderr, "no such command: %s\n", inp[0]);
            }
            exit(1);
        }
        else {
            int stat_loc;
            waitpid(child, &stat_loc, WUNTRACED);
            size_t pl = 0;
            while (inp[pl] != NULL) {
                free(inp[pl]);
                pl ++;
            }
            free(inp);
        }
    }
}

int main(int argc, char **argv) {
    if (argc == 1) {
        while (1) {
            char *input = readline("> ");
            add_history(input);
            doeval(input);
            free(input);
            printf("\n");
        }
    }
    else {
        for (uint32_t i = 1; i < argc; i++) {
            doeval(argv[i]);
        }
    }
}
