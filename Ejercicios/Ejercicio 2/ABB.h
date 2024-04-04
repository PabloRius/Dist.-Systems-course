#ifndef ABB_H
#define ABB_H 1

#define MAX_LENGTH 255

#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_RESET "\x1b[0m"

typedef struct TreeNode
{
    int key;
    char cadena[MAX_LENGTH];
    int N;
    double *vector;

    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

typedef struct Tree
{
    struct TreeNode *root;
    int N;
} Tree;

int init_tree(struct Tree **tree);
int get_node(struct Tree *tree, int key, char cadena[MAX_LENGTH], int *N, double *vector);
int post_node(struct Tree *tree, int key, char cadena[MAX_LENGTH], int N, double *vector);
int patch_node(struct Tree *tree, int key, char cadena[MAX_LENGTH], int N, double *vector);
int delete_node(struct Tree *tree, int key);
int head_node(struct Tree *tree, int key);
void options();
void print_tree(struct Tree *tree, int expanded);
void print_node(struct TreeNode *node, int level, int expanded, char *side);

#endif /* ABB_H */