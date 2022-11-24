#include "AVLTree.h"
#include "Team.h"
#include "Player.h"
#include <iostream>
#include <stdio.h>

int main(){
    AVLTree<int, int> avlTree1;
    int x = 10;
    int y = 12;
    int z = 11;
    int w = 7;
    int a = 8;
    int b = 5;
    int c = 4;


    avlTree1.push(&x, x);
    avlTree1.push(&y, y);
    avlTree1.push(&z, z);
    avlTree1.push(&w, w);
    avlTree1.push(&a, a);
    avlTree1.push(&b, b);
    avlTree1.push(&c, c);
    avlTree1.remove(4);
    avlTree1.remove(11);
    avlTree1.remove(8);

    std::cout<< *(avlTree1.getRoot()->m_data) << std::endl;
    std::cout<< *(avlTree1.getRoot()->m_left->m_data) << std::endl;
    std::cout<< *(avlTree1.getRoot()->m_right->m_data) << std::endl;

    //std::cout<< *(avlTree1.getRoot()->m_left->m_left->m_data)<< std::endl;
    std::cout<< *(avlTree1.getRoot()->m_left->m_right->m_data) << std::endl;
    //std::cout<< *(avlTree1.getRoot()->m_right->m_left->m_data)<< std::endl;
    //std::cout<< *(avlTree1.getRoot()->m_right->m_left->m_data) << std::endl;
    // std::cout<< *(avlTree1.getRoot()->m_right->m_right->m_data) << std::endl;
    }


