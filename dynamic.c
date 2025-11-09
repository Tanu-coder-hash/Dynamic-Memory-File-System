#include "filesystem.h"

int main() {
    Node* root = createRoot();
    Node* current = root;
    char command[50], arg1[50], arg2[50];

    printf("=== Dynamic Memory File System (DMFS) ===\n");
    printf("Commands: mkdir, touch, ls, cd, rm, find, rename, exit\n\n");

    while (1) {
        printf("[");
        printPath(current);
        printf("]$ ");

        scanf("%s", command);

        if (strcmp(command, "mkdir") == 0) {
            scanf("%s", arg1);
            createDirectory(current, arg1);

        } else if (strcmp(command, "touch") == 0) {
            scanf("%s", arg1);
            int size;
            printf("Enter file size: ");
            scanf("%d", &size);
            createFile(current, arg1, size);

        } else if (strcmp(command, "ls") == 0) {
            listDirectory(current);

        } else if (strcmp(command, "cd") == 0) {
            scanf("%s", arg1);
            changeDirectory(&current, arg1);

        } else if (strcmp(command, "rm") == 0) {
            scanf("%s", arg1);
            deleteNode(current, arg1);

        } else if (strcmp(command, "find") == 0) {
            scanf("%s", arg1);
            findFiles(root, arg1);

        } else if (strcmp(command, "rename") == 0) {
            scanf("%s %s", arg1, arg2);
            renameNode(current, arg1, arg2);

        } else if (strcmp(command, "exit") == 0) {
            freeTree(root);
            printf("Exiting DMFS.\n");
            break;

        } else {
            printf("Unknown command: %s\n", command);
        }
    }

    return 0;
}
