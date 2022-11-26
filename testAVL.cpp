#include "AVLTree.h"
#include "Team.h"
#include "Player.h"
#include <iostream>
#include <stdio.h>

int main(){
    AVLTree<int, int> avlTree1, avlTree2;
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

    int** array =  new int*[avlTree1.get_size()];
    avlTree1.in_order(array);
    for (size_t i = 0; i < avlTree1.get_size(); i++)
    {
        std::cout << *(array[i]) << ", ";
    }
    std::cout << std::endl;

    int x1 = 1;
    int y1 = 2;
    int z1 = 111;
    int w1 = 4;


    avlTree2.push(&x1, x1);
    avlTree2.push(&y1, y1);
    avlTree2.push(&z1, z1);
    avlTree2.push(&w1, w1);

    int** array1 =  new int*[avlTree2.get_size()];
    avlTree2.in_order(array1);
    for (size_t i = 0; i < avlTree2.get_size(); i++)
    {
        std::cout << *(array1[i]) << ", ";
    }
    std::cout << std::endl;  

    AVLTree<int, int> avlTree3;
    unite_trees(avlTree1, avlTree2, avlTree3);  

    delete[] array;
    }


