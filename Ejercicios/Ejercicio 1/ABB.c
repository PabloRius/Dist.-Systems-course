#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ABB.h"
#include "utils.h"

int tree_error(char *desc)
{
    printf(ANSI_COLOR_RED);
    printf("::Binary Tree Error:: %s\n", desc);
    printf(ANSI_COLOR_RESET);
    return -1;
}

int init_tree(struct Tree **tree)
{
    // Alojamos espacio de forma dinámica en memoria para el estruct del arbol binario
    *tree = malloc(sizeof(struct Tree));
    if (NULL == *tree)
    {
        return tree_error("No se ha podido reservar memoria para el árbol nuevo");
    }
    // Inicializamos el árbol con raiz nula
    (*tree)->root = NULL;
    (*tree)->N = 0;

    return 0;
}

int get_node(struct Tree *tree, int key, char cadena[MAX_LENGTH], int *N, double *vector)
{
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
            strcpy(cadena, currentNode->cadena);
            *N = currentNode->N;
            if (vector == NULL)
            {
                return tree_error("El vector introducido no está inicializado");
            }
            memcpy(vector, currentNode->vector, (*N) * (sizeof(double)));
            return 0;
        }
    }
    return tree_error("La clave introducida no existe");
}

int post_node(struct Tree *tree, int key, char cadena[MAX_LENGTH], int N, double *vector)
{
    struct TreeNode *newNode = malloc(sizeof(struct TreeNode));
    if (newNode == NULL)
    {
        return tree_error("No se ha podido reservar memoria para el nodo nuevo");
    }

    newNode->key = key;
    strcpy(newNode->cadena, cadena);
    newNode->N = N;
    newNode->vector = (double *)malloc(N * sizeof(double));
    if (newNode->vector == NULL)
    {
        free(newNode);
        return tree_error("No se ha podido reservar memoria para el vector nuevo");
    }
    memcpy(newNode->vector, vector, N * sizeof(double));

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
                free(newNode->vector);
                free(newNode);
                tree_error("La clave introducida ya existe");
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

int patch_node(struct Tree *tree, int key, char cadena[MAX_LENGTH], int N, double *vector)
{
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
            strcpy(currentNode->cadena, cadena);
            if (currentNode->N != N && currentNode->vector != NULL)
            {
                // En caso de que el nuevo vector de doubles
                // sea de diferente tamaño que el que ya había
                // se liberará la memoria del antiguo para
                // evitar un memory leak y se reasignará memoria
                // para el nuevo
                if (vector == NULL)
                {
                    return tree_error("El vector introducido no está inicializado");
                }
                free(currentNode->vector);
                currentNode->vector = malloc(N * sizeof(double));
                memcpy(currentNode->vector, vector, N * (sizeof(double)));
            }
            else
            {
                memcpy(currentNode->vector, vector, N * (sizeof(double)));
            }
            currentNode->N = N;

            return 0;
        }
    }
    return tree_error("La clave introducida no existe");
}

int delete_node(struct Tree *tree, int key)
{
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
            tree_error("La clave introducida no existe");
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
        free(currentNode->vector);
        free(currentNode);
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
        free(currentNode->vector);
        free(currentNode);
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
        free(currentNode->vector);
        free(currentNode);
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
        strcpy(currentNode->cadena, successor->cadena);
        currentNode->N = successor->N;
        free(currentNode->vector);
        currentNode->vector = malloc(currentNode->N * sizeof(double));
        memcpy(currentNode->vector, successor->vector, currentNode->N * sizeof(double));

        // Eliminar el sucesor
        if (successorParent->left == successor)
        {
            successorParent->left = successor->right;
        }
        else
        {
            successorParent->right = successor->right;
        }

        free(successor->vector);
        free(successor);
        return 0;
    }
    return tree_error("La clave introducida no existe");
}

int head_node(struct Tree *tree, int key)
{
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
            return 1;
        }
    }
    return 0;
}

void options()
{
    printf("The possible actions for a Binary Tree are:\n\t- GET (Returns the values from a given node key)\n\t- POST (Inserts a node with the given values)\n\t- PATCH (Updates the node with the given key to the given values)\n\t- HEAD (Returns 0 if there's a node with the given key, -1 if not)\n\t- DELETE (Deletes the node with the given key)\n");
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
        printf("|--%d\n", node->key);
    }
    else
    {
        printf("|--%d,\tCadena: %s, Vector: ", node->key, node->cadena);
        print_double_array(node->vector, node->N);
    }

    printf(ANSI_COLOR_RESET);

    // Imprimir los hijos recursivamente con un nivel de indentación adicional
    print_node(node->left, level + 1, expanded, "l");
    print_node(node->right, level + 1, expanded, "r");
}