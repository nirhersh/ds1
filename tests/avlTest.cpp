#include "../AVLTree.h"
#include "../Player.h"
#include "../Team.h"
#include "../wet1util.h"
#include <stdbool.h>
#include <assert.h>


bool searchTest(int& searchSucceededTests);
bool pushTest(int& pushSucceededTests);




int main(){
    int searchSucceededTests, pushSucceededTests;

    bool searchFunctionTest = searchTest(searchSucceededTests);
    std::cout << "search passed " << searchSucceededTests << "out of 2 tests" << std::endl;

    bool pushFunctionTest = pushTest(pushSucceededTests);
    std::cout << "search passed " << pushSucceededTests << "out of 5 tests" << std::endl;


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

    std::cout << "the size of the tree is " << avlTree1.get_size() << std::endl;
    assert(avlTree1.get_size() == 1000);
    pushSucceededTests++;

    return true;

}