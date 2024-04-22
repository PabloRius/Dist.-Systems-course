#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ABB.h"
#include "utils.h"

int tree_error(char *desc, int errNo)
{
    printf(ANSI_COLOR_RED);
    printf("::Binary Tree Error:: %s\n", desc);
    printf(ANSI_COLOR_RESET);
    return errNo;
}

unsigned long parse_key(char *username){
    unsigned long hash = 5381;
    int c;

    while ((c = *username++) != '\0'){
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

int init_tree(struct Tree **tree)
{
    // Alojamos espacio de forma dinámica en memoria para el estruct del arbol binario
    *tree = malloc(sizeof(struct Tree));
    if (NULL == *tree)
    {
        return tree_error("No se ha podido reservar memoria para el árbol nuevo", -1);
    }
    // Inicializamos el árbol con raiz nula
    (*tree)->root = NULL;
    (*tree)->N = 0;

    return 0;
}

int register_user(struct Tree *tree, char username[MAX_LENGTH]) 
{
    unsigned long key = parse_key(username);

    struct TreeNode *newNode = malloc(sizeof(struct TreeNode));
    if (newNode == NULL)
    {
        return tree_error("No se ha podido reservar memoria para el nodo nuevo", 2);
    }

    newNode->key = key;
    strcpy(newNode->username, username);
    newNode->files = NULL;

    newNode->left = NULL;
    newNode->right = NULL;

    if (tree->root == NULL)
    {
        tree->root = newNode;
    }
    else
    {
        struct TreeNode *currentNode = tree->root;
        struct TreeNode *parent;
        while (currentNode != NULL)
        {
            parent = currentNode;
            if (key > currentNode->key)
            {
                currentNode = currentNode->right;
            }
            else if (key < currentNode->key)
            {
                currentNode = currentNode->left;
            }
            else
            {
                // La key ya existe
                free(newNode);
                return tree_error("La clave introducida ya existe", 1);
            }
        }

        if (key < parent->key)
        {
            parent->left = newNode;
        }
        else
        {
            parent->right = newNode;
        }
    }
    tree->N++;
    return 0;
}

int unregister_user(struct Tree *tree, char username[MAX_LENGTH])
{
    unsigned long key = parse_key(username);

    struct TreeNode *currentNode = tree->root;

    struct TreeNode *parent = NULL;
    while (currentNode != NULL && currentNode->key != key)
    {
        parent = currentNode;
        if (key > currentNode->key)
        {
            currentNode = currentNode->right;
        }
        else if (key < currentNode->key)
        {
            currentNode = currentNode->left;
        }

        if (currentNode == NULL)
        {
            tree_error("La clave introducida no existe", 1);
        }
    }
    // Posibles casos
    if (currentNode->left == NULL && currentNode->right == NULL)
    {
        // El nodo es una hoja
        if (parent == NULL)
        {
            // Es el nodo raiz
            tree->root = NULL;
        }
        else if (parent->right == currentNode)
        {
            // Es el hijo derecho
            parent->right = NULL;
        }
        else if (parent->left == currentNode)
        {
            // Es el hijo izquierdo
            parent->left = NULL;
        }
        free(currentNode->files);
        free(currentNode);
        tree->N--;
        return 0;
    }
    else if (currentNode->left != NULL && currentNode->right == NULL)
    {
        // Tiene sólo un hijo (izquierdo)
        if (parent == NULL)
        {
            // Es el nodo raiz
            tree->root = currentNode->left;
        }
        else if (parent->right == currentNode)
        {
            // Es el hijo derecho
            parent->right = currentNode->left;
        }
        else if (parent->left == currentNode)
        {
            // Es el hijo izquierdo
            parent->left = currentNode->left;
        }
        free(currentNode->files);
        free(currentNode);
        tree->N--;
        return 0;
    }
    else if (currentNode->right != NULL && currentNode->left == NULL)
    {
        // Tiene sólo un hijo (derecho)
        if (parent == NULL)
        {
            // Es el nodo raiz
            tree->root = currentNode->right;
        }
        else if (parent->right == currentNode)
        {
            // Es el hijo derecho
            parent->right = currentNode->right;
        }
        else if (parent->left == currentNode)
        {
            // Es el hijo izquierdo
            parent->left = currentNode->right;
        }
        free(currentNode->files);
        free(currentNode);
        tree->N--;
        return 0;
    }
    else if (currentNode->left != NULL && currentNode->right != NULL)
    {
        // Tiene hijo derecho e izquierdo
        struct TreeNode *successor = currentNode->right;
        struct TreeNode *successorParent = currentNode;

        // Encontrar el sucesor inmediato
        while (successor->left != NULL)
        {
            successorParent = successor;
            successor = successor->left;
        }

        // Copiar los valores del sucesor al nodo actual
        currentNode->key = successor->key;
        strcpy(currentNode->username, successor->username);
        currentNode->N_files = successor->N_files;
        free(currentNode->files);
        currentNode->files = (char **)malloc(currentNode->N_files * 256 * sizeof(char));
        memcpy(currentNode->files, successor->files, currentNode->N_files * 256 * sizeof(char));

        // Eliminar el sucesor
        if (successorParent->left == successor)
        {
            successorParent->left = successor->right;
        }
        else
        {
            successorParent->right = successor->right;
        }

        free(successor->files);
        free(successor);
        tree->N--;
        return 0;
    }
    return tree_error("La clave introducida no existe", 1);
}

int get_user(struct Tree *tree, char username[MAX_LENGTH], char **files)
{
    struct TreeNode *currentNode = tree->root;

    unsigned long key = parse_key(username);

    while (currentNode != NULL)
    {
        if (key > currentNode->key)
        {
            currentNode = currentNode->right;
        }
        else if (key < currentNode->key)
        {
            currentNode = currentNode->left;
        }
        else
        {
            // La key que buscamos
            int N_files = currentNode->N_files;
            *files = malloc(N_files * 256 * sizeof(char));
            memcpy(*files, currentNode->files, N_files * 256 * sizeof(char));

            return 0;
        }
    }
    return tree_error("La clave introducida no existe", 1);
}

int publish_file(struct Tree *tree, char username[MAX_LENGTH], char *file)
{
    unsigned long key = parse_key(username);

    struct TreeNode *currentNode = tree->root;

    while (currentNode != NULL)
    {
        if (key > currentNode->key)
        {
            currentNode = currentNode->right;
        }
        else if (key < currentNode->key)
        {
            currentNode = currentNode->left;
        }
        else
        {
            // La key que buscamos
            if (currentNode->files == NULL)
            {
                // Al introducir un fichero nuevo, primero se comprueba que no esté ya introducido
                // si no existe, se reasigna la memoria reservada para el vector                
                
                currentNode->files = (char **)malloc(256 * sizeof(char));
                currentNode->files[0] = (char *)malloc(strlen(file) + 1);
                strcpy(currentNode->files[0], file);
                currentNode->N_files = 1;
                return 0;
            }
            for (int i=0;i<currentNode->N_files; i++){
                if(strcmp(currentNode->files[i], file) == 0){
                    return tree_error("El fichero ya está publicado", 3); // El file ya existe
                }
            }
            currentNode->N_files ++;
            currentNode->files = (char **)realloc(currentNode->files, currentNode->N_files * 256 * sizeof(char));
            currentNode->files[currentNode->N_files - 1] = (char *)malloc(strlen(file) + 1);
            strcpy(currentNode->files[currentNode->N_files - 1], file);
            return 0;
        }
    }
    return tree_error("El usuario no existe", 1);
}

int delete_file(struct Tree *tree, char username[MAX_LENGTH], char *file)
{
    unsigned long key = parse_key(username);

    struct TreeNode *currentNode = tree->root;

    while (currentNode != NULL)
    {
        if (key > currentNode->key)
        {
            currentNode = currentNode->right;
        }
        else if (key < currentNode->key)
        {
            currentNode = currentNode->left;
        }
        else
        {
            // La key que buscamos
            if (currentNode->files == NULL)
            {
                return tree_error("El fichero no se ha publicado", 3);
            }
            else
            {
                for (int i=0;i<currentNode->N_files; i++){
                    if(strcmp(currentNode->files[i], file) == 0){
                        free(currentNode->files[i]);
                        for (int j=i;j<currentNode->N_files - 1;j++) {
                            currentNode->files[j] = currentNode->files[j+1];
                        }
                        currentNode->N_files--;
                        currentNode->files = (char **)realloc(currentNode->files, currentNode->N_files * 256 * sizeof(char));
                        return 0;
                    }
                }
                return tree_error("El fichero no se ha publicado", 3);

            }

            return 0;
        }
    }
    return tree_error("El usuario no existe", 1);
}

void print_tree(struct Tree *tree, int expanded)
{
    if (tree->root == NULL)
    {
        printf("The tree is empty\n");
    }
    else
    {
        print_node(tree->root, 0, expanded, "c");
    }
}

void print_node(struct TreeNode *node, int level, int expanded, char *side)
{
    if (node == NULL)
    {
        return;
    }
    if (strcmp(side, "l") == 0)
    {
        printf(ANSI_COLOR_BLUE);
    }
    else if (strcmp(side, "r") == 0)
    {
        printf(ANSI_COLOR_RED);
    }

    for (int i = 0; i < level; i++)
    {
        printf("  "); // Dos espacios por nivel de profundidad
    }
    if (expanded == 0)
    {
        printf("|--%s\n", node->username);
    }
    else
    {
        printf("|--%s\tFiles: ", node->username);
        print_char_array(node->files, node->N_files);
    }

    printf(ANSI_COLOR_RESET);

    // Imprimir los hijos recursivamente con un nivel de indentación adicional
    print_node(node->left, level + 1, expanded, "l");
    print_node(node->right, level + 1, expanded, "r");
}