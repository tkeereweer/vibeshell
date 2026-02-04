// Test program to verify signal handling works
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

void handler(int sig) {
    (void)sig;
    printf("\n");
    rl_on_new_line();
    rl_redisplay();
}

int main() {
    signal(SIGINT, handler);
    signal(SIGQUIT, SIG_IGN);

    while (1) {
        char *line = readline("test> ");
        if (!line) break;
        if (line[0]) {
            printf("You entered: %s\n", line);
            add_history(line);
        }
        free(line);
    }
    return 0;
}
