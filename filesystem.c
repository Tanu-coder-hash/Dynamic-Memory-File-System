#include "filesystem.h"

Node* createRoot() {
    Node *root = (Node*)malloc(sizeof(Node));
    if (!root) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    strcpy(root->name, "/");
    root->isFile = 0;
    root->size = 0;
    root->ctime = time(NULL);
    root->parent = NULL;
    root->child = NULL;
    root->next = NULL;
    return root;
}

Node* findChildByName(Node *current, const char *name) {
    if (!current)
        return NULL;
    Node *temp = current->child;
    while (temp) {
        if (strcmp(temp->name, name) == 0)
            return temp;
        temp = temp->next;
    }
    return NULL;
}

static Node* createNodeGeneric(Node *parent, const char *name, int isFile, int size) {
    Node *newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    strncpy(newNode->name, name, NAME_MAX_LEN - 1);
    newNode->name[NAME_MAX_LEN - 1] = '\0';
    newNode->isFile = isFile;
    newNode->size = size;
    newNode->ctime = time(NULL);
    newNode->parent = parent;
    newNode->child = NULL;
    newNode->next = parent ? parent->child : NULL;
    if (parent)
        parent->child = newNode;
    return newNode;
}

void createDirectory(Node *current, const char *name) {
    if (!current)
        return;
    if (findChildByName(current, name)) {
        printf("Error: '%s' already exists.\n", name);
        return;
    }
    createNodeGeneric(current, name, 0, 0);
    printf("Directory '%s' created successfully.\n", name);
}

void createFile(Node *current, const char *name, int size) {
    if (!current)
        return;
    if (findChildByName(current, name)) {
        printf("Error: '%s' already exists.\n", name);
        return;
    }
    createNodeGeneric(current, name, 1, size);
    printf("File '%s' created successfully (size=%d).\n", name, size);
}

void listDirectory(Node *current) {
    if (!current)
        return;
    Node *temp = current->child;
    if (!temp) {
        printf("Directory is empty.\n");
        return;
    }
    while (temp) {
        char timestr[64];
        struct tm* tm_info = localtime(&temp->ctime);
        strftime(timestr, sizeof(timestr), "%Y-%m-%d %H:%M:%S", tm_info);
        if (temp->isFile)
            printf("- %s (file, size=%d, created=%s)\n", temp->name, temp->size, timestr);
        else
            printf("- %s/ (dir, created=%s)\n", temp->name, timestr);
        temp = temp->next;
    }
}

void changeDirectory(Node **current, const char *name) {
    if (!current || !(*current))
        return;

    if (strcmp(name, "..") == 0) {
        if ((*current)->parent)
            *current = (*current)->parent;
        else
            printf("Already at root directory.\n");
        return;
    }

    Node *temp = (*current)->child;
    while (temp) {
        if (!temp->isFile && strcmp(temp->name, name) == 0) {
            *current = temp;
            return;
        }
        temp = temp->next;
    }

    printf("Directory '%s' not found.\n", name);
}

void getFullPath(Node *node, char *outPath) {
    if (!node) {
        strcpy(outPath, "");
        return;
    }
    if (!node->parent) {
        strcpy(outPath, "/");
        return;
    }
    char stack[1024][NAME_MAX_LEN];
    int top = 0;
    Node *cur = node;
    while (cur && cur->parent) {
        strncpy(stack[top++], cur->name, NAME_MAX_LEN);
        cur = cur->parent;
    }
    char buf[PATH_MAX_LEN] = "";
    strcat(buf, "/");
    for (int i = top - 1; i >= 0; --i) {
        strcat(buf, stack[i]);
        if (i != 0)
            strcat(buf, "/");
    }
    strcpy(outPath, buf);
}

void printPath(Node *current) {
    char path[PATH_MAX_LEN];
    getFullPath(current, path);
    printf("%s", path);
}

void freeTree(Node *node) {
    if (!node)
        return;
    freeTree(node->child);
    freeTree(node->next);
    free(node);
}

void deleteNode(Node *current, const char *name) {
    if (!current)
        return;
    Node *temp = current->child;
    Node *prev = NULL;
    while (temp) {
        if (strcmp(temp->name, name) == 0) {
            if (prev)
                prev->next = temp->next;
            else
                current->child = temp->next;
            freeTree(temp);
            printf("Deleted '%s' successfully.\n", name);
            return;
        }
        prev = temp;
        temp = temp->next;
    }
    printf("No such file or directory '%s'.\n", name);
}

static void findFilesRec(Node *node, const char *name) {
    if (!node)
        return;
    Node *temp = node->child;
    while (temp) {
        if (strcmp(temp->name, name) == 0) {
            char path[PATH_MAX_LEN];
            getFullPath(temp, path);
            if (temp->isFile)
                printf("Found file: %s\n", path);
            else
                printf("Found directory: %s/\n", path);
        }
        findFilesRec(temp, name);
        temp = temp->next;
    }
}

void findFiles(Node *root, const char *name) {
    if (!root)
        return;
    findFilesRec(root, name);
}

void renameNode(Node *current, const char *oldname, const char *newname) {
    if (!current)
        return;
    Node *target = findChildByName(current, oldname);
    if (!target) {
        printf("'%s' not found.\n", oldname);
        return;
    }
    if (findChildByName(current, newname)) {
        printf("Name '%s' already exists.\n", newname);
        return;
    }
    strncpy(target->name, newname, NAME_MAX_LEN - 1);
    target->name[NAME_MAX_LEN - 1] = '\0';
    printf("Renamed '%s' to '%s'.\n", oldname, newname);
}
