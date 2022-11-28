#include "../AVLTree.h"
#include "../Player.h"
#include "../Team.h"
#include "../wet1util.h"
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>


bool searchTest(int& searchSucceededTests);
bool pushTest(int& pushSucceededTests);
bool removeTest(int& removeSucceededTests);




int main(){
    int searchSucceededTests, pushSucceededTests, removeSucceededTests;

    // bool searchFunctionTest = searchTest(searchSucceededTests);
    // std::cout << "search passed " << searchSucceededTests << "out of 2 tests" << std::endl;

    //bool pushFunctionTest = pushTest(pushSucceededTests);
    //std::cout << "push passed " << pushSucceededTests << "out of 7 tests" << std::endl;

    bool removeFunctionTest = removeTest(removeSucceededTests);
    std::cout << "remove passed " << removeSucceededTests << "out of 2 tests" << std::endl;


    return 0;
}


bool searchTest(int& searchSucceededTests){
    searchSucceededTests = 0;
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
    try{
        avlTree1.search(13);
        std::cout << "failed search test - key should not be exist " << std::endl;
    } catch (KeyDoesntExists& err) {
        searchSucceededTests++;
    }
    try{
        int* val1 = avlTree1.search(x);
        val1 = avlTree1.search(y);
        val1 = avlTree1.search(z);
        val1 = avlTree1.search(w);
        val1 = avlTree1.search(a);
        val1 = avlTree1.search(b);
        val1 = avlTree1.search(c);
    } catch (KeyDoesntExists){
        std::cout << "failed search test - key should be exist " << std::endl;
    }
    searchSucceededTests++;

    return true;

}


bool pushTest(int& pushSucceededTests)
{
    pushSucceededTests = 0;
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

    pushSucceededTests++;

    try{
        avlTree1.push(&x, x);
        std::cout << "failed push test - key should be already exist " << std::endl;
    } catch (KeyAlreadyExists){
        pushSucceededTests++;
        
    }
    try{
        for (int i = 20; i < 1013; i++)
        {
            avlTree1.push(&x, i);
        }
    } catch (KeyAlreadyExists){
        std::cout << "failed push test - should insert the node and throw nothing" << std::endl;
    }
    pushSucceededTests++;


    std::cout << "the height of the tree is " << avlTree1.get_height() << std::endl;
    assert(avlTree1.get_height() < 17);
    pushSucceededTests++;
    assert(avlTree1.get_size() == 1000);
    pushSucceededTests++;

    AVLTree<int, int> avlTree2;

    int numOfInserst = 1000000;
    int seed = 9187231;
    int treeSize = 0;
    
    for (int i = 0; i < numOfInserst; ++i)
        {
            try{
                srand(seed + i);
                int newVal = rand();
                avlTree2.push(&c, newVal);
                treeSize++;
            } catch (KeyAlreadyExists) {
                
            }
        }
    std::cout << "the height of the tree2 is " << avlTree2.get_height() << std::endl;
    std::cout << "the size of the tree2 is " << avlTree2.get_size() << "and should be " << treeSize << std::endl;
    AVLTree<int, int>::fix_tree_height(avlTree2.getRoot());
    bool treeBalanceFactor = avlTree2.check_balance_factor(avlTree2.getRoot());
    assert (treeBalanceFactor);
    pushSucceededTests++;

    AVLTree<int, int>::Node** array = new AVLTree<int, int>::Node*[avlTree2.get_size()];
    avlTree2.in_order(array);
    for (int i = 0; i < avlTree2.get_size() - 1; i++)
    {
        assert((array[i]->get_key()) < (array[i + 1]->get_key()));
    }
    pushSucceededTests++;
    delete[] array;
    return true;
}



bool removeTest(int& removeSucceededTests)
{
    int c = 0;
    AVLTree<int, int> tree1;
    int numOfInserst = 100000;
    int seed = 9187231;
    int treeSize = 0;
    
    for (int i = 0; i < numOfInserst; ++i)
        {
            try{
                srand(seed + i);
                int newVal = rand();
                tree1.push(&c, newVal);
                treeSize++;
            } catch (KeyAlreadyExists& err) {
                
            }
        }
    for (int i = 0; i < numOfInserst; i+=3)
        {
            try{
                srand(seed + i);
                int newVal = rand();
                tree1.remove(newVal);
                treeSize--;
            } catch (KeyDoesntExists& err) {
                std::cout << "in remove test, key should exist" << std::endl;
            }
        }
    removeSucceededTests++;
    assert(treeSize == tree1.get_size());
    removeSucceededTests++;
    return true;
}