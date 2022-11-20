
#ifndef AVLTREE_H
#define AVLTREE_H

#include "wet1util.h"
#include <memory>
#include <stdbool.h>

template <class T>
class AVLTree{
public:
    template<class K>
    AVLTree(K key);

    ~AVLTree() = default;

    AVLTree(const AVLTree<T>& other) = delete;
    
    AVLTree<T>& operator=(const AVLTree<T>& other) = delete;

    class Node;

    Node search(int id);

    StatusType push(T item);
    
    StatusType remove(int id);

private:
    std::unique_ptr<Node> root;
    K m_key;
    int calc_BF(std::unique_ptr<Node> node);
    void LL_fix(std::unique_ptr<Node> node);
    void RL_fix(std::unique_ptr<Node> node);
    void LR_fix(std::unique_ptr<Node> node);
    void RR_fix(std::unique_ptr<Node> node);
};

template <class T>
class AVLTree<T>::Node{
    Node(const Node&) = delete;
    Node& operator=(const Node& other) = delete;
private:
    T m_data;
    int height; 
    std::unique_ptr<Node> m_right;
    std::unique_ptr<Node> m_left; 
    std::unique_ptr<Node> m_parent;
    Node(const T data);
    friend class AVLTree<T>;
};

//-------------------- Node Implemantation --------------------
template <class T>
AVLTree<T>::Node::Node(const T data) : m_data(data), height(0), m_right(nullptr), m_left(nullptr), m_parent(nullptr){}

//-------------------- AVLTree Implemantation --------------------
template <class T>
template <class K>
AVLTree<T>::AVLTree(K key): root(nullptr), m_key(key){}

template <class T>
typename AVLTree<T>::Node AVLTree<T>::search(int id){
    bool found = false;
    AVLTree<T>::Node currentNode = *root;
    while (!found)
    {
        if (id > key((currentNode->m_data))){
            currentNode = *(currentNode->m_right);
        }else if(id < key((currentNode->m_data))){
            currentNode = *(currentNode->m_left);
        }else{
            found = true;
            return currentNode;
        }
    }
}

#endif