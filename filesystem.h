#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NAME_MAX_LEN 256
#define PATH_MAX_LEN 2048

typedef struct Node {
    char name[NAME_MAX_LEN];
    int isFile;
    int size;
    time_t ctime;
    struct Node *parent;
    struct Node *child;
    struct Node *next;
} Node;

Node* createRoot();
void createDirectory(Node *current, const char *name);
void createFile(Node *current, const char *name, int size);
void listDirectory(Node *current);
void printPath(Node *current);
void changeDirectory(Node **current, const char *name);
void deleteNode(Node *current, const char *name);
void freeTree(Node *node);
void saveToFile(Node *root, const char *filename);
void findFiles(Node *root, const char *filename);
void renameNode(Node *current, const char *oldname, const char *newname);
void moveNode(Node *root, Node *current, const char *name, const char *targetDirName);
void getFullPath(Node *node, char *outPath);
Node* findChildByName(Node *current, const char *name);

#endif
