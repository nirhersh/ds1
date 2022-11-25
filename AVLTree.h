
#ifndef AVLTREE_H
#define AVLTREE_H

#include "wet1util.h"
#include "Exception.h"
#include <memory>
#include <assert.h>
#include <stdbool.h>
#include <iostream>

template<class T>
class R_Node{
public:
    R_Node* next;
    T* data;
};

template <class T, class K>
class AVLTree{
public:
    /*
    C'tor of the AVL Tree
    */
    AVLTree();

    ~AVLTree() = default;

    AVLTree(const AVLTree<T, K>& other) = delete;
    
    AVLTree<T, K>& operator=(const AVLTree<T, K>& other) = delete;

    /*
    Sub class of Nodes that hold the data in the tree
    */
    class Node;

    /*
    A function that searches for a value in the tree
    @param key: The key of the searched data
    @return: a pointer to the data
    */
    T* search(const K& key) const;

    /*
    A function that pushes a key-value pair into the tree
    @param key: The key of the pushed data
    @param item: the item to push into the tree
    @return: a pointer to the data
    */
    void push(T* item, K& key);
    
    /*
    A function that removes for a value from the tree
    @param key: The key of the removed data
    */
    void remove(const K& key);

    Node* getRoot(){
        return root;
    }

    int get_size()const;

    void in_order(T** array) const;

    void print_tree();

private:
    Node* root;
    int m_size;
    void fix_height(Node* node);
    void balance_tree(Node* newNode);
    Node* search_node(const K& key) const;
    Node* remove_node(Node* nodeToRemove);
    Node* get_following_node(Node* node)const;
    void switch_nodes(Node* node1,Node* node2);
    int in_order_recurtion(T** array, Node* current, int index = 0) const;
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
AVLTree<T, K>::AVLTree(): root(nullptr), m_size(0){}

template <class T, class K>
T* AVLTree<T, K>::search(const K& key) const{
    Node* nodeToReturn = search_node(key);
    return nodeToReturn->m_data;
}

template <class T, class K>
void AVLTree<T, K>::push(T* item, K& key){
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
    m_size++;
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
        if(balance == 2)
        {
            if(calc_BF(tempNode) > -1)
            {
                LL_fix(tempNode->m_parent);
            } else {
                LR_fix(tempNode->m_parent);
            }
            break;
        }
        else if(balance == -2){
            if(calc_BF(tempNode) < 1) {
                RR_fix(tempNode->m_parent);
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
    RR_fix(parent->m_left);
    LL_fix(parent);
}

template<class T, class K>
void AVLTree<T, K>::RL_fix(Node* parent){
    LL_fix(parent->m_right);
    RR_fix(parent);
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

template<class T, class K>
void AVLTree<T, K>::remove(const K& key)
{
    Node* nodeToRemove = search_node(key);
    Node* currentNode = remove_node(nodeToRemove); 
    Node* parent = currentNode->m_parent;
    do{
        int parentHeight = currentNode->m_parent? currentNode->m_parent->m_height: 0;
        fix_height(currentNode);
        balance_tree(currentNode);
        if(parent == nullptr || parentHeight == parent->m_height){
            break;
        }
        currentNode = currentNode->m_parent;
        parent = currentNode->m_parent;
    } while(currentNode != root);
    m_size--;


}

template<class T, class K>
typename AVLTree<T, K>::Node* AVLTree<T, K>::search_node(const K& key) const
{
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
            return currentNode;
        }else{
            currentNode = currentNode->m_left;
        }
    }
}

template<class T, class K>
typename AVLTree<T, K>::Node* AVLTree<T, K>::remove_node(Node* nodeToRemove){
    bool isRight = false;
    if(nodeToRemove->m_parent != nullptr && nodeToRemove->m_parent->m_right == nodeToRemove){
            isRight = true;;
    }
    Node* nodeToReturn;
    if (nodeToRemove->m_right == nullptr && nodeToRemove->m_left == nullptr)
    {
        if(!isRight){
           nodeToRemove->m_parent->m_left = nullptr; 
        }
        else{
            nodeToRemove->m_parent->m_right = nullptr;
        }
        nodeToReturn = nodeToRemove->m_parent;
        delete nodeToRemove; 
    }
    else if(nodeToRemove->m_right != nullptr && nodeToRemove->m_left == nullptr){
        if(isRight){
            nodeToRemove->m_parent->m_right = nodeToRemove->m_right;
        }
        else {
            nodeToRemove->m_parent->m_left = nodeToRemove->m_right;
        }
        nodeToRemove->m_right->m_parent = nodeToRemove->m_parent;
        nodeToReturn = nodeToRemove->m_right;
        delete nodeToRemove;
    }
    else if(nodeToRemove->m_right == nullptr && nodeToRemove->m_left != nullptr){
        if(isRight){
            nodeToRemove->m_parent->m_right = nodeToRemove->m_left;
        }
        else {
            nodeToRemove->m_parent->m_left = nodeToRemove->m_left;
        }
        nodeToRemove->m_left->m_parent = nodeToRemove->m_parent;
        nodeToReturn = nodeToRemove->m_left;
        delete nodeToRemove;
    }
    else{
        Node* followingNode = get_following_node(nodeToRemove);
        switch_nodes(nodeToRemove, followingNode);
        remove_node(followingNode);    
    }
    return nodeToReturn;
}

template<class T, class K>
void AVLTree<T, K>::switch_nodes(Node* node1, Node* node2){
    T* node1Data = node1->m_data;
    T* node2Data = node2->m_data;
    K key1 = node1->m_key;
    K key2 = node2->m_key;
    node1->m_data = node2Data;
    node1->m_key = key2;
    node2->m_data = node1Data;
    node2->m_key = key1;
}




template<class T, class K> 
typename AVLTree<T, K>::Node* AVLTree<T, K>::get_following_node(Node* node) const //if there is no right son return the parent
{
    assert(node != nullptr);
    assert(node->m_right != nullptr);
    node = node->m_right;
    while(node->m_left)
    {
        node = node->m_left;
    }
    return node;
}


template<class T, class K>
int AVLTree<T, K>::get_size()const{
    return m_size;
}

template<class T, class K>
void AVLTree<T ,K>::in_order(T** array) const{
    in_order_recurtion(array, root, 0);
    }



// template<class T, class K>
// void AVLTree<T, K>::in_order_recurtion(R_Node<T>* list, Node* current) const //should it be T**? do we want an array of pointers to the players?
// {
//     if(current == nullptr){
//         return;
//     }
//     //std::cout << "entered " << *(current->m_data) << std::endl;
//     in_order_recurtion(list, current->m_left);
//     assert(current != nullptr);
//     std::cout << "list " << *(current->m_data) << std::endl;
//     list->data = (current->m_data); 
//     list->next = new R_Node<T>;
//     list = list->next;
//     in_order_recurtion(list, current->m_right);
// }

template<class T, class K>
int AVLTree<T, K>::in_order_recurtion(T** array, Node* current, int index) const
{
    if(current == nullptr){
        return index;
    }
    //std::cout << "entered " << *(current->m_data) << std::endl;
    index = in_order_recurtion(array, current->m_left, index);
    if(index >= m_size){
        return index;
    }
    array[index++] = current->m_data;
    return in_order_recurtion(array, current->m_right, index);
}

#endif


