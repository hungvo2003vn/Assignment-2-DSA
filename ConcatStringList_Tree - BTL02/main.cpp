#include "ConcatStringTree.h"
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
    cout << s3.subString(5, 6).toString()<< endl;
    cout << s3.reverse().toString() << "." <<endl;
    cout << endl;
}
void tc3() {
    cout << "//////////////TEST CASE 3////////////////\n";
    cout << "\n";
    ConcatStringTree* s1 = new ConcatStringTree("a");
    ConcatStringTree* s2 = new ConcatStringTree("bc");
    ConcatStringTree* s3 = new ConcatStringTree(s1->concat(*s2));
    cout << s3->getParTreeSize("l") <<endl;
    cout << s3 -> getParTreeStringPreOrder("l") << endl;

    ConcatStringTree* s4 = new ConcatStringTree(s3->reverse());
    cout << s4->getParTreeSize("l") << endl;
    cout << s4->getParTreeStringPreOrder("l") << endl;

    //delete s1;
    //delete s2;
    //delete s3;
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
    cout << "first appearance of k: " << s5.indexOf('a') << endl;
    cout << s5.subString(1, 4).reverse().toString() << endl;

    cout << s5.getParTreeStringPreOrder("ll") << endl;
    cout << endl;
}
int main() {
    
    tc1();
    tc2();
    tc3();
    tc4();
    system("pause");
    return 0;
}