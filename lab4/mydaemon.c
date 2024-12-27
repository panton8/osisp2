#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>

#define MAX_SIGNALS 10
int signals_to_log[MAX_SIGNALS];
int num_signals = 0;
volatile sig_atomic_t reload_config = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void read_config_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    printf("Reading configuration file...\n");

    char line[10];
    while (fgets(line, sizeof(line), file)) {
        int signal_num = atoi(line);
        if (signal_num > 0) {
            pthread_mutex_lock(&mutex);
            signals_to_log[num_signals++] = signal_num;
            pthread_mutex_unlock(&mutex);
        }
    }

    fclose(file);

    printf("Configuration file read successfully.\n");
}

void sighup_handler(int sig) {
    const char* message = "Received SIGHUP signal. Reloading configuration file...\n";
    printf("%s", message);
    reload_config = 1;
}

void sigterm_handler(int sig) {
    const char* message = "Received SIGTERM signal. Exiting...\n";
    write(STDERR_FILENO, message, strlen(message));
    exit(EXIT_SUCCESS);
}

void log_signals() {
    int logfile = open("logfile.txt", O_CREAT | O_WRONLY | O_APPEND, 0644);
    if (logfile < 0) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    pthread_mutex_lock(&mutex);
    for (int i = 0; i < num_signals; i++) {
        char message[100];
        snprintf(message, sizeof(message), "Received signal %d\n", signals_to_log[i]);
        ssize_t bytes_written = write(logfile, message, strlen(message));
        if (bytes_written < 0) {
            perror("write");
            close(logfile);
            exit(EXIT_FAILURE);
        }
    }
    pthread_mutex_unlock(&mutex);

    close(logfile);
}

int main() {
    printf("Daemon started.\n");

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    umask(0);
    pid_t sid = setsid();
    if (sid < 0) {
        perror("setsid");
        exit(EXIT_FAILURE);
    }

    struct sigaction sa;
    sa.sa_handler = sighup_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGHUP, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
    sa.sa_handler = sigterm_handler;
    if (sigaction(SIGTERM, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    read_config_file("config.txt");

    printf("Daemon initialized.\n");

    while (1) {
        if (reload_config) {
            printf("Reloading configuration file...\n");
            read_config_file("config.txt");
            reload_config = 0;
        }

        log_signals();

        pause();
    }

    return 0;
}
