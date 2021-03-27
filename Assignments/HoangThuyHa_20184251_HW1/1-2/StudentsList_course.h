/* 
 * @copyright (c) 2020, ICT, Hanoi University of Technology
 * @author HoangThuy Ha
 * @version 1.0
 */
// https://www.geeksforgeeks.org/avl-tree-set-2-deletion/

#ifndef __StudentsList_ACourse_h__
#define __StudentsList_ACourse_h__

#include <stdlib.h>
#include <stddef.h>
#include "Student_course.h"
#define datatype student_course
// #define AVL_compare(a, b) scoreCmp(student_course a, student_course b)

typedef struct _AVL_node {
    datatype data;
    struct _AVL_node *left;
    struct _AVL_node *right;
    int height;
} AVL_node;
typedef AVL_node* studentList_course;

int AVL_height(AVL_node* a) {
    if(a == NULL) return 0;
    return a->height;
}
int AVL_balanceFactor(AVL_node* a) {
    if(a == NULL) return 0;
    return AVL_height(a->left) - AVL_height(a->right);
}
int AVL_maxInt(int a, int b) {
    return (a > b) ? a : b;
}

AVL_node* AVL_newNode(datatype data) {
    AVL_node* node = (AVL_node*)malloc(sizeof(AVL_node));
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    node->height = 0;
    return node;
}

AVL_node* AVL_rightRotation(AVL_node* y) {
    if(y == NULL) return NULL;
    
    AVL_node* x = y->left;
    AVL_node* T = x->right;

    x->right = y;
    y->left = T;

    y->height = AVL_maxInt(AVL_height(y->left), AVL_height(y->right)) + 1;
    x->height = AVL_maxInt(AVL_height(x->left), AVL_height(x->right)) + 1;
    
    return x;
}
AVL_node* AVL_leftRotation(AVL_node* y) {
    if(y == NULL) return NULL;

    AVL_node* x = y->right;
    AVL_node* T = x->left;

    x->left = y;
    y->right = T;

    y->height = AVL_maxInt(AVL_height(y->left), AVL_height(y->right)) + 1;
    x->height = AVL_maxInt(AVL_height(x->left), AVL_height(x->right)) + 1;
    
    return x;
}


AVL_node* AVL_insert(AVL_node* node, datatype data) {
    if(node == NULL) 
        return AVL_newNode(data);
    
    if(scoreCmp(data, node->data) == -1)
        node->left =  AVL_insert(node->left, data);
    if(scoreCmp(data, node->data) == 1)
        node->right =  AVL_insert(node->right, data);
    if(scoreCmp(data, node->data) == 0)
        return node;

    node->height = AVL_maxInt(AVL_height(node->left), AVL_height(node->right)) + 1;
    int balance = AVL_balanceFactor(node);

    // Left left case
    if(balance == 2 && scoreCmp(data, node->data) == -1) 
        return AVL_rightRotation(node);
    // Right right case
    if(balance == -2 && scoreCmp(data, node->data) == 1)
        return AVL_leftRotation(node);
    // Left right case
    if(balance == 2 && scoreCmp(data, node->data) == 1) {
        node->left =  AVL_leftRotation(node->left);
        return AVL_rightRotation(node);
    }
    // Right left case
    if(balance == -2 && scoreCmp(data, node->data) == -1) {
        node->right = AVL_rightRotation(node->right);
        return AVL_leftRotation(node);
    }

    return node;
}

AVL_node* AVL_minNode(AVL_node* node) {
    if(node == NULL) return node;
    AVL_node* current = node;
    while(current->left != NULL)
        current = current->left; 
    return current;
}
AVL_node* AVL_maxNode(AVL_node* node) {
    if(node == NULL) return node;
    AVL_node* current = node;
    while(current->right != NULL)
        current = current->right; 
    return current;
}

AVL_node* AVL_deleteNode(AVL_node* root, datatype data) {
    if(root == NULL)
        return NULL;
    if(scoreCmp(data, root->data) == -1)
        root->left = AVL_deleteNode(root->left, data);
    else if(scoreCmp(data, root->data) == 1)
        root->right = AVL_deleteNode(root->right, data);
    else {
        if(root->left == NULL && root->right == NULL) {
            free(root);
            root = NULL;
        }   
        else if(root->left == NULL && root->right != NULL) {
            AVL_node* tmp = root->right;
            free(root);
            root = tmp;
        }
        else if(root->left != NULL && root->right == NULL) {
            AVL_node* tmp = root->left;
            free(root);
            root = tmp;
        }
        else {
            AVL_node* tmp = AVL_minNode(root->right);
            root->data = tmp->data;
            root->right = AVL_deleteNode(root->right, tmp->data);   
        }
    }

    if(root == NULL)
        return root;
    root->height = AVL_maxInt(AVL_height(root->left),AVL_height(root->right)) + 1;

    int balance = AVL_balanceFactor(root);
    // Left left case
    if(balance == 2 && scoreCmp(data, root->data) == -1) 
        return AVL_rightRotation(root);
    // Right right case
    if(balance == -2 && scoreCmp(data, root->data) == 1)
        return AVL_leftRotation(root);
    // Left right case
    if(balance == 2 && scoreCmp(data, root->data) == 1) {
        root->left =  AVL_leftRotation(root->left);
        return AVL_rightRotation(root);
    }
    // Right left case
    if(balance == -2 && scoreCmp(data, root->data) == -1) {
        root->right = AVL_rightRotation(root->right);
        return AVL_leftRotation(root);
    }

    return root;
}

#endif