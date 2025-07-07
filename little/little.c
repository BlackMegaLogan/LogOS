#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>
#include <fcntl.h>
#include <sys/stat.h>
#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

#define MAX_INPUT 1024
#define MAX_TOKENS 64
#define DELIM " \t\r\n"

static char clipboard[PATH_MAX] = "";

void copy_file(const char *src, const char *dest) {
    int fd_in = open(src, O_RDONLY);
    if (fd_in < 0) {
        perror("open src");
        return;
    }
    int fd_out = open(dest, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_out < 0) {
        perror("open dest");
        close(fd_in);
        return;
    }
    char buf[4096];
    ssize_t n;
    while ((n = read(fd_in, buf, sizeof(buf))) > 0) {
        if (write(fd_out, buf, n) != n) {
            perror("write");
            break;
        }
    }
    if (n < 0) {
        perror("read");
    }
    close(fd_in);
    close(fd_out);
}

void print_help(void) {
    printf("Available commands:\n");
    printf("  l            - list files (ls)\n");
    printf("  d <dir>      - change directory\n");
    printf("  cf <file>    - create empty file\n");
    printf("  cd <dir>     - create directory\n");
    printf("  i <pkg>      - install package (placeholder)\n");
    printf("  ad           - admin mode (placeholder)\n");
    printf("  a c1 c2      - execute c1 && c2\n");
    printf("  w <file>     - write image to USB (placeholder)\n");
    printf("  p            - print working directory\n");
    printf("  fr <file>    - remove file\n");
    printf("  fc <file>    - copy file to clipboard\n");
    printf("  fp <dest>    - paste clipboard to destination\n");
    printf("  help         - show this help\n");
    printf("  exit/q       - exit the shell\n");
}

int main(void) {
    char line[MAX_INPUT];
    char *tokens[MAX_TOKENS];

    while (1) {
        printf("little$ ");
        fflush(stdout);
        if (!fgets(line, sizeof(line), stdin)) {
            break;
        }

        int token_count = 0;
        char *tok = strtok(line, DELIM);
        while (tok && token_count < MAX_TOKENS - 1) {
            tokens[token_count++] = tok;
            tok = strtok(NULL, DELIM);
        }
        tokens[token_count] = NULL;

        if (token_count == 0) {
            continue;
        }

        if (strcmp(tokens[0], "q") == 0 || strcmp(tokens[0], "exit") == 0) {
            break;
        } else if (strcmp(tokens[0], "l") == 0 || strcmp(tokens[0], "ls") == 0) {
            system("ls");
        } else if (strcmp(tokens[0], "d") == 0) {
            const char *path = token_count > 1 ? tokens[1] : getenv("HOME");
            if (chdir(path) != 0) {
                perror("cd");
            }
        } else if (strcmp(tokens[0], "cf") == 0) {
            if (token_count > 1) {
                int fd = open(tokens[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd < 0) {
                    perror("create file");
                } else {
                    close(fd);
                }
            } else {
                printf("Usage: cf <file>\n");
            }
        } else if (strcmp(tokens[0], "cd") == 0) {
            if (token_count > 1) {
                if (mkdir(tokens[1], 0755) != 0) {
                    perror("mkdir");
                }
            } else {
                printf("Usage: cd <dir>\n");
            }
        } else if (strcmp(tokens[0], "i") == 0) {
            if (token_count > 1) {
                printf("Installing %s (not implemented)\n", tokens[1]);
            } else {
                printf("Usage: i <package>\n");
            }
        } else if (strcmp(tokens[0], "ad") == 0) {
            printf("Admin mode not implemented\n");
        } else if (strcmp(tokens[0], "a") == 0) {
            if (token_count > 2) {
                char cmd[MAX_INPUT];
                snprintf(cmd, sizeof(cmd), "%s && %s", tokens[1], tokens[2]);
                system(cmd);
            } else {
                printf("Usage: a <cmd1> <cmd2>\n");
            }
        } else if (strcmp(tokens[0], "w") == 0) {
            printf("Write to USB not implemented\n");
        } else if (strcmp(tokens[0], "p") == 0) {
            char cwd[PATH_MAX];
            if (getcwd(cwd, sizeof(cwd))) {
                printf("%s\n", cwd);
            } else {
                perror("pwd");
            }
        } else if (strcmp(tokens[0], "fr") == 0) {
            if (token_count > 1) {
                if (remove(tokens[1]) != 0) {
                    perror("remove");
                }
            } else {
                printf("Usage: fr <file>\n");
            }
        } else if (strcmp(tokens[0], "fc") == 0) {
            if (token_count > 1) {
                strncpy(clipboard, tokens[1], sizeof(clipboard));
                clipboard[sizeof(clipboard) - 1] = '\0';
            } else {
                printf("Usage: fc <file>\n");
            }
        } else if (strcmp(tokens[0], "fp") == 0) {
            if (token_count > 1 && clipboard[0]) {
                copy_file(clipboard, tokens[1]);
            } else {
                printf("Usage: fp <destination>\n");
            }
        } else if (strcmp(tokens[0], "help") == 0) {
            print_help();
        } else {
            printf("Unknown command: %s\n", tokens[0]);
        }
    }

    return 0;
}

