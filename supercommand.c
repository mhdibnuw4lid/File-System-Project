#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>

// Function prototypes
void file_operations();
void directory_operations();
void print_menu();

int main() {
    int choose;

    while (1) {
        print_menu();
        printf("Enter your choice: ");
        if (scanf("%d", &choose) != 1) {
            printf("Invalid input! Please enter a number.\n");
            while(getchar() != '\n'); // Clear invalid input
            continue;
        }

        switch (choose) {
            case 1:
                file_operations();
                break;
            case 2:
                directory_operations();
                break;
            case 0:
                printf("Exiting program.\n");
                exit(0);
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }

    return 0;
}

void print_menu() {
    printf("\n--- SuperCommand Menu ---\n");
    printf("1. File Operations\n");
    printf("2. Directory Operations\n");
    printf("0. Exit\n");
    printf("-------------------------\n");
}

void file_operations() {
    int fchoice;
    char filename[256], content[1024];
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
    if (scanf("%d", &fchoice) != 1) {
        printf("Invalid input! Please enter a number.\n");
        while(getchar() != '\n'); // Clear invalid input
        return;
    }

    switch (fchoice) {
        case 1:
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

        case 2:
            printf("Enter file name: ");
            scanf("%s", filename);
            printf("Enter permission mode (e.g., 0777): ");
            if (scanf("%o", &mode) != 1) {
                printf("Invalid input! Please enter a valid mode.\n");
                return;
            }
            if (chmod(filename, mode) == 0) {
                printf("Permissions changed successfully for file '%s'.\n", filename);
            } else {
                perror("Error changing permissions");
            }
            break;

        case 3:
            printf("Enter file name: ");
            scanf("%s", filename);
            file = fopen(filename, "r");
            if (file) {
                printf("File content:\n");
                while (fgets(content, sizeof(content), file)) {
                    printf("%s", content);
                }
                fclose(file);
            } else {
                perror("Error reading file");
            }
            break;

        case 4:
            printf("Enter file name: ");
            scanf("%s", filename);
            printf("Enter content to write: ");
            getchar();
            fgets(content, sizeof(content), stdin);
            file = fopen(filename, "a");
            if (file) {
                fputs(content, file);
                printf("Content written to file '%s'.\n", filename);
                fclose(file);
            } else {
                perror("Error writing to file");
            }
            break;

        case 5:
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
    if (scanf("%d", &dchoice) != 1) {
        printf("Invalid input! Please enter a number.\n");
        while(getchar() != '\n'); // Clear invalid input
        return;
    }

    switch (dchoice) {
        case 1:
            printf("Enter directory name: ");
            scanf("%s", dirname);
            if (mkdir(dirname, 0777) == 0) {
                printf("Directory '%s' created successfully.\n", dirname);
            } else {
                perror("Error creating directory");
            }
            break;

        case 2:
            printf("Enter directory name: ");
            scanf("%s", dirname);
            if (rmdir(dirname) == 0) {
                printf("Directory '%s' removed successfully.\n", dirname);
            } else {
                perror("Error removing directory");
            }
            break;

        case 3:
            if (getcwd(cwd, sizeof(cwd)) != NULL) {
                printf("Current working directory: %s\n", cwd);
            } else {
                perror("Error getting current directory");
            }
            break;

        case 4:
            printf("Enter directory path (or '.' for current directory): ");
            scanf("%s", dirname);
            dir = opendir(dirname);
            if (dir) {
                printf("Directory contents:\n");
                while ((entry = readdir(dir)) != NULL) {
                    printf("%s\n", entry->d_name);
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

//Placeholder for keylogger
void keylogger_operations() {
    printf("Tunggu dulu haa\n");
}