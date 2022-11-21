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
    s1.~ConcatStringTree();
    _CrtDumpMemoryLeaks();
}
void tc2() {
    cout << "//////////////TEST CASE 2////////////////\n";
    cout << "\n";
    ConcatStringTree s1("Hello");
    ConcatStringTree s2(",_t");
    ConcatStringTree s3 = s1.concat(s2);
    cout << s3.toStringPreOrder() << "." << endl;
    cout << s3.toString() << "." << endl;
    cout << s3.subString(5, 6).toString()<< endl;
    cout << s3.reverse().toString() << "." <<endl;
    cout << endl;

    s1.~ConcatStringTree();
    s2.~ConcatStringTree();
    s3.~ConcatStringTree();

    _CrtDumpMemoryLeaks();
}
void tc3() {
    cout << "//////////////TEST CASE 3////////////////\n";
    cout << "\n";
    ConcatStringTree* s1 = DBG_NEW ConcatStringTree("a");
    ConcatStringTree* s2 = DBG_NEW ConcatStringTree("bc");
    ConcatStringTree* s3 = DBG_NEW ConcatStringTree(s1->concat(*s2));
    cout << s3->getParTreeSize("l") <<endl;
    cout << s3 -> getParTreeStringPreOrder("l") << endl;

    ConcatStringTree* s4 = new ConcatStringTree(s3->reverse());
    cout << s4->getParTreeSize("l") << endl;
    cout << s4->getParTreeStringPreOrder("l") << endl;


    delete s3;    
    delete s2;
    delete s1;
    delete s4;
    cout<<endl;

    _CrtDumpMemoryLeaks();
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
    cout << "char at index 1: " << s5.get(1) << endl;
    try {
        cout << "char at index 8: " << s5.get(8) << "." << endl;
    }
    catch (const out_of_range& ofr) {
        cout << "Exception out_of_range:" << ofr.what() << endl;
    }
    cout << "first appearance of a: " << s5.indexOf('a') << endl;
    //cout << s5.subString(1, 4).reverse().toString() << endl;

    cout << s5.getParTreeStringPreOrder("ll") << endl;
    cout << endl;

    s1.~ConcatStringTree();
    s2.~ConcatStringTree();
    s3.~ConcatStringTree();
    s4.~ConcatStringTree();
    s5.~ConcatStringTree();
    _CrtDumpMemoryLeaks();
}
void tc5() {
    cout << "//////////////TEST CASE 5////////////////\n";
    HashConfig hashConfig(
        2,
       0.5,
       0.5,
       0.75,
         2,
         4
    );
    LitStringHash * litStringHash = DBG_NEW LitStringHash(hashConfig);
    ReducedConcatStringTree * s1 = DBG_NEW ReducedConcatStringTree("a", litStringHash);
    ReducedConcatStringTree * s2 = DBG_NEW ReducedConcatStringTree("bb", litStringHash);
    ReducedConcatStringTree* s3 = DBG_NEW ReducedConcatStringTree("bb", litStringHash);

    cout << s2->getParTreeSize("") << endl;
    cout << s2->getParTreeStringPreOrder("") << endl;
    cout << s3->getParTreeSize("") << endl;
    cout << s3->getParTreeStringPreOrder("") << endl;

    cout << litStringHash->getLastInsertedIndex() << endl;
    cout << litStringHash->toString() << endl;
    
    delete litStringHash;
    delete s3;
    delete s1;
    delete s2;
    cout << endl;

    _CrtDumpMemoryLeaks();
}
void tc6() {
    cout << "//////////////TEST CASE 6////////////////\n";
    HashConfig hashConfig(
        2,
        0.5,
        0.5,
        0.75,
        2,
        4
    );
    LitStringHash* litStringHash = DBG_NEW LitStringHash(hashConfig);
    ReducedConcatStringTree* s1 = DBG_NEW ReducedConcatStringTree("a", litStringHash);
    ReducedConcatStringTree* s2 = DBG_NEW ReducedConcatStringTree("a", litStringHash);
    ReducedConcatStringTree* s3 = DBG_NEW ReducedConcatStringTree(s1->concat(*s2));

    cout << litStringHash->getLastInsertedIndex() << endl;
    cout << litStringHash->toString() << endl;

    cout << "Testing tostring\n";
    cout << s3->toString() << endl;
    cout << s3->toStringPreOrder() << endl;

    cout << "Testing ParentsTree\n";
    cout << s3->getParTreeSize("") << endl;
    cout << s3->getParTreeStringPreOrder("") << endl;

    cout << "Testing ParentsTree of node->left\n";
    cout << s3->getParTreeSize("l") << endl;
    cout << s3->getParTreeStringPreOrder("l") << endl;

    cout << "Testing ParentsTree of node->right\n";
    cout << s3->getParTreeSize("r") << endl;
    cout << s3->getParTreeStringPreOrder("r") << endl;

    cout << "Index Of char a: " << s3->indexOf('a') << "." << endl;
    cout << "Char at index 1 is: " << s3->get(1) << "." << endl;

    cout << "length of s3 is: " << s3->length() << endl;

    try {
        cout << "char at index 10: " << s3->get(10) << "." << endl;
    }
    catch (const out_of_range& ofr) {
        cout << "Exception out_of_range:" << ofr.what() << endl;
    }
    delete litStringHash;
    delete s1;
    delete s2;
    delete s3;
    cout << endl;

    _CrtDumpMemoryLeaks();
}
void tc7() {
    cout << "//////////////TEST CASE 7////////////////\n";
    cout << "\n";
    ConcatStringTree* s1= new ConcatStringTree("Hello");
    ConcatStringTree* s2 = new ConcatStringTree(",_t");
    ConcatStringTree* s3 = new ConcatStringTree("his_is");
    ConcatStringTree* s4 = new ConcatStringTree("_an");
    ConcatStringTree* s5 = new ConcatStringTree(s1->concat(*s2).concat(s3->concat(*s4))  );

    cout << s5->toStringPreOrder() << endl;
    cout << s5->toString() << endl;
    cout << "char at index 1: " << s5->get(1) << endl;
    try {
        cout << "char at index 8: " << s5->get(8) << "." << endl;
    }
    catch (const out_of_range& ofr) {
        cout << "Exception out_of_range:" << ofr.what() << endl;
    }
    cout << "first appearance of a: " << s5->indexOf('a') << endl;
   // cout << s5->subString(1, 4).toString() << endl;

    cout << s5->getParTreeStringPreOrder("") << endl;
    cout << endl;

    cout << s5->getParTreeStringPreOrder("ll") << endl;
    cout << s5->getParTreeStringPreOrder("lr") << endl;
    cout << s5->getParTreeStringPreOrder("rl") << endl;
    cout << s5->getParTreeStringPreOrder("rr") << endl;
    cout << s5->getParTreeStringPreOrder("l") << endl;
    cout << s5->getParTreeStringPreOrder("r") << endl;
    
    delete s5;
    delete s1;
    delete s2;
    delete s3;
    delete s4;
    
    _CrtDumpMemoryLeaks();
}
void tc8() {
    cout << "//////////////TEST CASE 8////////////////\n";
    HashConfig hashConfig(
        2,
        0.5,
        0.5,
        0.75,
        2,
        4
    );
    LitStringHash* litStringHash = new LitStringHash(hashConfig);

    try {
        litStringHash->insert("Hello");
        litStringHash->insert("this_is");
        litStringHash->insert("Assignment_2");
        litStringHash->insert("3");
    }
    catch (const runtime_error& re) {
        cout << "Exception runtime_error:" << re.what() << endl;
    }
    cout << litStringHash->getLastInsertedIndex() << endl;
    cout << litStringHash->toString() << endl;

    delete litStringHash;
    cout << endl;
}
int main() {
    
    //tc1();
    //tc2();
    //tc3();
    //tc4();
    //tc5();
    //tc6();
    //tc7();
    tc8();
    //_CrtDumpMemoryLeaks();
    system("pause");
    return 0;
}
