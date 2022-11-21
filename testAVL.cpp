#include "AVLTree.h"
#include "Team.h"
#include "Player.h"
#include <iostream>
#include <stdio.h>

int main(){
    //std::cout << "fwfdew";
    AVLTree<int, int> avlTree1;
    int x = 5;
    int y = 6;
    int z = 7;
    int w = 8;
    int a = 4;
    int b = 3;


    avlTree1.push(&x, x);
    avlTree1.push(&y, y);
    avlTree1.push(&z, z);
    avlTree1.push(&w, w);
    avlTree1.push(&a, a);
    avlTree1.push(&b, b);

    std::cout<< *(avlTree1.getRoot()->m_data) << std::endl;
    std::cout<< *(avlTree1.getRoot()->m_left->m_data) << std::endl;
    std::cout<< *(avlTree1.getRoot()->m_right->m_data) << std::endl;

    std::cout<< *(avlTree1.getRoot()->m_left->m_left->m_data)<< std::endl;
    std::cout<< *(avlTree1.getRoot()->m_left->m_right->m_data) << std::endl;
    std::cout<< *(avlTree1.getRoot()->m_right->m_right->m_data) << std::endl;
    }


