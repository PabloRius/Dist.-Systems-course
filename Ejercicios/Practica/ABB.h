#ifndef ABB_H
#define ABB_H 1

#define MAX_LENGTH 256

#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_RESET "\x1b[0m"

typedef struct TreeNode
{
    unsigned long key;
    char username[MAX_LENGTH];
    int N_files;
    char **files;

    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

typedef struct Tree
{
    struct TreeNode *root;
    int N;
} Tree;

int init_tree(struct Tree **tree);
int register_user(struct Tree *tree, char username[MAX_LENGTH]);
int unregister_user(struct Tree *tree, char username[MAX_LENGTH]);
int get_user(struct Tree *tree, char username[MAX_LENGTH], char **files);
int publish_file(struct Tree *tree, char username[MAX_LENGTH], char *file);
int delete_file(struct Tree *tree, char username[MAX_LENGTH], char *file);

void print_tree(struct Tree *tree, int expanded);
void print_node(struct TreeNode *node, int level, int expanded, char *side);

#endif /* ABB_H */