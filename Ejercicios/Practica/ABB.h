#ifndef ABB_H
#define ABB_H 1

#include <stdbool.h>

#define MAX_LENGTH 256

#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_RESET "\x1b[0m"

typedef struct PublishedFile
{
    char name[MAX_LENGTH];
    char desc[MAX_LENGTH];
} PublishedFile;

typedef struct TreeNode
{
    unsigned long key;
    char username[MAX_LENGTH];
    int N_files;
    struct PublishedFile *files;
    bool connected;
    char hostname[MAX_LENGTH];
    int port;

    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

typedef struct Tree
{
    struct TreeNode *root;
    int N;
} Tree;

typedef struct User
{
    char username[MAX_LENGTH];
    char hostname[MAX_LENGTH];
    int port;
} User;

int init_tree(struct Tree **tree);
int register_user(struct Tree *tree, char username[MAX_LENGTH]);
int unregister_user(struct Tree *tree, char username[MAX_LENGTH]);
int connect_user(struct Tree *tree, char username[MAX_LENGTH], char hostname[MAX_LENGTH], int port);
int disconnect_user(struct Tree *tree, char username[MAX_LENGTH]);
int get_user(struct Tree *tree, char username[MAX_LENGTH], char **files);
int publish_file(struct Tree *tree, char username[MAX_LENGTH], char filename[MAX_LENGTH], char filedesc[MAX_LENGTH]);
int delete_file(struct Tree *tree, char username[MAX_LENGTH], char filename[MAX_LENGTH]);
int list_users(struct Tree *tree, char username[MAX_LENGTH], int *N_users_out, struct User **user_lst_out);
int list_content(struct Tree *tree, char username[MAX_LENGTH], char username_req[MAX_LENGTH], int *N_files_out, struct PublishedFile **file_lst_out);

void print_tree(struct Tree *tree, int expanded);
void print_node(struct TreeNode *node, int level, int expanded, char *side);

#endif /* ABB_H */