#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

char *progname = "intsimple";

// Обработчик сигналов
void signal_handler(int signo) {
    switch (signo) {
        case SIGINT:
            printf("%s: Received SIGINT (Ctrl+C)\n", progname);
            break;
        case SIGTERM:
            printf("%s: Received SIGTERM\n", progname);
            // Вы можете добавить здесь код для корректного завершения программы
            exit(0);
            break;
        case SIGUSR1:
            printf("%s: Received SIGUSR1\n", progname);
            break;
        case SIGUSR2:
            printf("%s: Received SIGUSR2\n", progname);
            break;
        default:
            printf("%s: Received signal %d\n", progname, signo);
            break;
    }
}

int main(int argc, char **argv) {
    setvbuf(stdout, NULL, _IOLBF, 0);
    printf("%s: starting...\n", progname);

    // Настройка обработчиков сигналов
    struct sigaction action;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;

    action.sa_handler = signal_handler;
    if (sigaction(SIGINT, &action, NULL) < 0) {
        perror("sigaction SIGINT");
        exit(1);
    }

    action.sa_handler = signal_handler;
    if (sigaction(SIGTERM, &action, NULL) < 0) {
        perror("sigaction SIGTERM");
        exit(1);
    }

    action.sa_handler = signal_handler;
    if (sigaction(SIGUSR1, &action, NULL) < 0) {
        perror("sigaction SIGUSR1");
        exit(1);
    }

    action.sa_handler = signal_handler;
    if (sigaction(SIGUSR2, &action, NULL) < 0) {
        perror("sigaction SIGUSR2");
        exit(1);
    }

    while (1) {
        // Ожидаем событие или сигнал
        printf("%s: waiting for event or signal...\n", progname);
        sleep(1); // или любая другая операция ожидания
    }

    return 0;
}
