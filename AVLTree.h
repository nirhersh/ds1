
#ifndef AVLTREE_H
#define AVLTREE_H

#include "wet1util.h"
#include "Exception.h"
#include <memory>
#include <assert.h>
#include <stdbool.h>
#include <iostream>

template <class T, class K>
class AVLTree{
public:

    AVLTree();

    ~AVLTree() = default;

    AVLTree(const AVLTree<T, K>& other) = delete;
    
    AVLTree<T, K>& operator=(const AVLTree<T, K>& other) = delete;

    class Node;

    T* search(const K& key) const;

    void push(T* item, K& key);
    
    void remove(const K& key);

    Node* getRoot(){
        return root;
    }

private:
    Node* root;
    void fix_height(Node* node);
    void balance_tree(Node* newNode);
    int calc_BF(Node* node);
    void LL_fix(Node* node);
    void RL_fix(Node* node);
    void LR_fix(Node* node);
    void RR_fix(Node* node);
};

template <class T, class K>
class AVLTree<T, K>::Node{
public:
    Node(const Node&) = delete;
    Node& operator=(const Node& other) = delete;
//private:
    T* m_data;
    K m_key;
    int m_height; 
    Node* m_right;
    Node* m_left; 
    Node* m_parent;
    Node(T* data, const K& key);
    friend class AVLTree<T, K>;
};

//-------------------- Node Implemantation --------------------
template <class T, class K>
AVLTree<T, K>::Node::Node(T* data, const K& key) : m_data(data), m_key(key), m_height(0),
                         m_right(nullptr), m_left(nullptr), m_parent(nullptr){}

//-------------------- AVLTree Implemantation --------------------
template <class T, class K>
AVLTree<T, K>::AVLTree(): root(nullptr){}

template <class T, class K>
T* AVLTree<T, K>::search(const K& key) const{
    bool found = false;
    Node* currentNode = root;
    while (!found)
    {
        if(currentNode == nullptr)
        {
            throw KeyDoesntExists();
        }
        if (key > currentNode->m_key){
            currentNode = currentNode->m_right;
        }else if(key == currentNode->m_key){
            found = true;
            return currentNode->m_data;
        }else{
            currentNode = currentNode->m_left;
        }
    }
}

template <class T, class K>
void AVLTree<T, K>::push(T* item, K& key){
    std::cout << "entering: " << *item << std::endl;
    assert(item != nullptr);
    bool found = false;
    Node* currentNode = root;
    if(root == nullptr){
        Node* newNode(new AVLTree<T, K>::Node(item, key));
        root = newNode;
        found = true;
    }
    while (!found)
    {
        if(currentNode->m_key == key)
        {
            throw KeyAlreadyExists();
        }
        if (key > currentNode->m_key){
            if(currentNode->m_right == nullptr)
            {
                Node* newNode(new AVLTree<T, K>::Node(item, key));
                newNode->m_parent = currentNode;
                currentNode->m_right = newNode;
                balance_tree(newNode);
                found = true;
            } else {
                currentNode = currentNode->m_right;
            }
        }else{
            if((currentNode->m_left) == nullptr)
            {
                Node* newNode(new AVLTree<T, K>::Node(item, key));
                newNode->m_parent = currentNode;
                currentNode->m_left = newNode;
                balance_tree(newNode);
                found = true;
            } else {
                currentNode = ((currentNode)->m_left);
            }
        }
    }
}

template<class T, class K>
void AVLTree<T, K>::balance_tree(Node* newNode)
{
    assert(newNode != nullptr);
    Node* tempNode = newNode;
    while(tempNode != root)
    {
        if(tempNode->m_parent->m_height >= tempNode->m_height + 1)
        {
            return;
        }
        tempNode->m_parent->m_height = tempNode->m_height + 1;
        int balance =calc_BF(tempNode->m_parent);
        std::cout << "balance of "<< *(tempNode->m_parent->m_data) << "is:" << balance << std::endl; 
        if(balance == 2)
        {
            if(calc_BF(tempNode) > -1)
            {
                std::cout << "entered ll:" << std::endl;
                LL_fix(tempNode->m_parent);
            } else {
                LR_fix(tempNode->m_parent);
            }
            break;
        }
        else if(balance == -2){
            if(calc_BF(tempNode) < 1) {
                RR_fix(tempNode->m_parent);
                std::cout << "entered rr:" << std::endl;
            } else {
                RL_fix(tempNode->m_parent);
            }
            break;
        }
        tempNode = tempNode->m_parent;
    }

}

template<class T, class K>
void AVLTree<T, K>::LL_fix(Node* parent){
    Node* B = parent;
    Node* A = parent->m_left;
    Node* AR = A->m_right;
    assert(parent != nullptr || A != nullptr || B!= nullptr);
    A->m_parent = B->m_parent;
    A->m_right = B;
    B->m_left = AR;
    B->m_parent = A;
    if(A->m_parent)
    {
        if(A->m_parent->m_left == B){
            A->m_parent->m_left = A;
        }else{
            A->m_parent->m_right = A;
        }
    }
    if(AR != nullptr){
        AR->m_parent = B;
    }
    if(B == root){
        root = A;
    }
    fix_height(B);
}

template<class T, class K>
void AVLTree<T, K>::LR_fix(Node* parent)
{
    Node* CParent = parent->m_parent;
    Node* C = parent;
    Node* B = parent->m_left;
    Node* A = parent->m_left->m_right;
    Node* BL = A->m_left;
    Node* BR = A->m_right;
    assert(parent != nullptr || A != nullptr || B!= nullptr || C!= nullptr);
    C->m_left = BR;
    C->m_parent = B;
    B->m_right = C;
    B->m_left = A;
    B->m_parent = CParent;
    if(BL != nullptr){
        BL->m_parent = A;
    }
    if(BR != nullptr){
        BR->m_parent = C;
    }
    
    A->m_parent = B;
    A->m_right = BL;
    if(CParent)
    {
        if(CParent->m_left == C){
            CParent->m_left = B;
        }else{
            CParent->m_right = B;
        }
    }
    if(C == nullptr){
        root = B;
    }
    fix_height(C);
}

template<class T, class K>
void AVLTree<T, K>::RL_fix(Node* parent){
    Node* C = parent;
    Node* CParent = C->m_parent;
    Node* A = parent->m_right;
    Node* B = A->m_left;
    Node* BL = B->m_left;
    Node* BR = B->m_right;
    assert(parent != nullptr || A != nullptr || B!= nullptr || C!= nullptr);
    C->m_parent = B;
    C->m_right = BL;
    B->m_parent = CParent;
    B->m_left = C;
    if(BR != nullptr){
    BR->m_parent = A;
    }
    if(BL != nullptr){
    BL->m_parent = C;
    }
    B->m_right = A;
    A->m_left = BR;
    A->m_parent = B;
    if(C == root){
        root = B;
    }
    if(CParent)
    {
        if(CParent->m_left == C){
            CParent->m_left = B;
        }else{
            CParent->m_right = B;
        }
    }
    fix_height(C);
}

template<class T, class K>
void AVLTree<T, K>::RR_fix(Node* parent)
{
    Node* B = parent;
    Node* BParent = parent->m_parent;
    Node* A = B->m_right;
    Node* AL = A->m_left;
    assert(parent != nullptr || A != nullptr || B!= nullptr);
    A->m_parent = BParent;
    A->m_left = B;
    B->m_right = AL;
    B->m_parent = A;
    if(AL != nullptr){
        AL->m_parent = B;
    }
    if(B == root){
        root = A;
    }
    if(A->m_parent)
    {
        if(A->m_parent->m_left == B){
            A->m_parent->m_left = A;
        }else{
            A->m_parent->m_right = A;
        }
    }
    std::cout<< *(B->m_parent->m_data) <<"Dfs"<< std::endl;
    fix_height(B);
}

template<class T, class K>
int AVLTree<T, K>::calc_BF(Node* node)
{
    if(node == nullptr){
        return -2;
    }
    int leftTreeH = node->m_left == nullptr ? -1 : node->m_left->m_height;
    int rightTreeH = node->m_right == nullptr ? -1 : node->m_right->m_height;
    return leftTreeH - rightTreeH;
}

template<class T, class K>
void AVLTree<T, K>::fix_height(Node* node)
{
    assert(node != nullptr);
    int leftTreeH = node->m_left == nullptr ? -1 : node->m_left->m_height;
    int rightTreeH = node->m_right == nullptr ? -1 : node->m_right->m_height;
    node->m_height = leftTreeH > rightTreeH ? 1 + leftTreeH : 1 + rightTreeH;
}




#endif


