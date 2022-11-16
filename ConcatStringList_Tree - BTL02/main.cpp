#include "ConcatStringTree.h"
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif
void tc1() {
    cout << "//////////////TEST CASE 1////////////////\n";
    cout << "\n";
    ConcatStringTree s1("acbbcab");
    cout << s1.length() << endl;
    cout << s1.get(1) <<"."<< endl;
    try {
        cout << "char at index 10: " << s1.get(10) << "." << endl;
    }
    catch (const out_of_range& ofr) {
        cout << "Exception out_of_range:" << ofr.what() << endl;
    }
    cout << s1.indexOf('b') << "." << endl;
    cout << endl;
}
void tc2() {
    cout << "//////////////TEST CASE 2////////////////\n";
    cout << "\n";
    ConcatStringTree s1("Hello");
    ConcatStringTree s2(",_t");
    ConcatStringTree s3 = s1.concat(s2);
    cout << s3.toStringPreOrder() << "." << endl;
    cout << s3.toString() << "." << endl;
    //cout << s3.subString(5, 6).toString()<< endl;
    //cout << s3.reverse().toString() << "." <<endl;
    cout << endl;
    
}
void tc3() {
    cout << "//////////////TEST CASE 3////////////////\n";
    cout << "\n";
    ConcatStringTree* s1 = DBG_NEW ConcatStringTree("a");
    ConcatStringTree* s2 = DBG_NEW ConcatStringTree("bc");
    ConcatStringTree* s3 = DBG_NEW ConcatStringTree(s1->concat(*s2));
    cout << s3->getParTreeSize("l") <<endl;
    cout << s3 -> getParTreeStringPreOrder("l") << endl;

    /*ConcatStringTree* s4 = new ConcatStringTree(s3->reverse());
    cout << s4->getParTreeSize("l") << endl;
    cout << s4->getParTreeStringPreOrder("l") << endl;*/


    delete s3;    
    delete s2;
    delete s1;
    //delete s4;
    cout<<endl;
}
void tc4() {
    cout << "//////////////TEST CASE 4////////////////\n";
    cout << "\n";
    ConcatStringTree s1("Hello");
    ConcatStringTree s2(",_t");
    ConcatStringTree s3("his_is");
    ConcatStringTree s4("_an");
    ConcatStringTree s5 = s1.concat(s2).concat(s3.concat(s4));

    cout << s5.toStringPreOrder() << endl;
    cout << s5.toString() << endl;
    cout << "char at index 8: " << s5.get(8) << endl;
    cout << "first appearance of a: " << s5.indexOf('a') << endl;
    //cout << s5.subString(1, 4).reverse().toString() << endl;

    cout << s5.getParTreeStringPreOrder("ll") << endl;
    cout << endl;
}/*
void tc5() {
    HashConfig hashConfig(
        2,
       0.5,
       0.5,
       0.75,
         2,
         4
    );
    LitStringHash * litStringHash = new LitStringHash(hashConfig);
    ReducedConcatStringTree * s1 = new ReducedConcatStringTree("a", litStringHash);
    ReducedConcatStringTree * s2 = new ReducedConcatStringTree("bb", litStringHash);

    cout << s1->toString() << endl;
    cout << s2->toString() << endl;
    ReducedConcatStringTree * s3 = new ReducedConcatStringTree("bb", litStringHash);

    cout << litStringHash->getLastInsertedIndex() << endl;
    cout << litStringHash->toString() << endl;

    delete litStringHash;
    delete s3;
    delete s1;
    delete s2;
     
}*/
void tc6() {
    HashConfig hashConfig(
        2,
        0.5,
        0.5,
        0.75,
        2,
        4
    );
    LitStringHash* litStringHash = new LitStringHash(hashConfig);
    string s1 = string("Hello");
    string s2 = string("there");
    string s3 = string("here");
    litStringHash->insert(s1);
    litStringHash->insert(s1);
    litStringHash->insert(s1);
    litStringHash->insert(s1);
    litStringHash->insert(s1);

    cout << litStringHash->getLastInsertedIndex() << endl;
    cout << litStringHash->toString() << endl;
}
int main() {
    
    //tc1();
    //tc2();
    //tc3();
    //tc4();
    //tc5();
    tc6();
    _CrtDumpMemoryLeaks();
    system("pause");
    return 0;
}