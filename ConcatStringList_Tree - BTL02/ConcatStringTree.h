#ifndef __CONCAT_STRING_TREE_H__
#define __CONCAT_STRING_TREE_H__

#include "main.h"

static int max_id=0;
class ParentsTree; //forward declaration
class ConcatStringTree {

public:
    struct ParNode {
        int id;
        ParNode* left;
        ParNode* right;
        int height;

        ParNode(int key = 0, ParNode* L = NULL, ParNode* R = NULL, int hi = 0)
        {
            id = key;
            left = L;
            right = R;
            height = hi;
        }
    };
    struct Node {
        int id;
        int leftLength;
        int length;
        string data;
        Node* left;
        Node* right;
        ParentsTree* Par;

        Node(int LLen = 0, int len = 0, string s = "", Node* L = NULL, Node* R = NULL)
        {

            leftLength = LLen;
            length = len;
            data = s;
            left = L;
            right = R;

            max_id++;
            id = max_id;

            if (max_id > 1e7)
                throw overflow_error("Id is overflow!");
        }
    };
    Node* Root;

public:
    ConcatStringTree();
    ConcatStringTree(const char* s);
    int length() const;
    char get(int index) const;
    int indexOf(char c) const;
    string toStringPreOrder() const;
    string toString() const;
    ConcatStringTree concat(const ConcatStringTree& otherS) const;
    ConcatStringTree subString(int from, int to) const;
    ConcatStringTree reverse() const;

    int getParTreeSize(const string& query) const;
    string getParTreeStringPreOrder(const string& query) const;
    ~ConcatStringTree();

    //Helper Function
    //
    char search_index(Node* cur, int index) const;
    int search_dfs(Node* cur, int index, char c) const;
    //
    string pre_order(Node* cur) const;
    string toStringNode(Node* cur) const;
    //
    string toString_helper(Node* cur) const;
    //
    Node* combine(Node* L, Node* R) const;
    Node* build_bottom(Node* root, int start, int from, int to) const;
    //
    Node* deepRe(Node* cur) const;
    //
    void Parents_add(Node* cur, int key) const;
    void Parents_delete(Node* cur, int key) const;
    //
    void Concat_delete(Node* cur);
};
class ParentsTree {
public:
    friend class ConcatSringTree;
    ConcatStringTree::ParNode* Paroot;
    int nums_node;
public:
    ParentsTree();
    int height(ConcatStringTree::ParNode* cur);
    int getBalance(ConcatStringTree::ParNode* cur);
    ConcatStringTree::ParNode* Rtate(ConcatStringTree::ParNode* cur);
    ConcatStringTree::ParNode* Ltate(ConcatStringTree::ParNode* cur);
    ConcatStringTree::ParNode* MaxNode(ConcatStringTree::ParNode* cur);
    ConcatStringTree::ParNode* insert(ConcatStringTree::ParNode* node, int key);
    ConcatStringTree::ParNode* remove(ConcatStringTree::ParNode* cur, int key, bool& deleted);
    void Insert(int key);
    void Remove(int key);

    int size() const;
    string toStringPreOrder() const;
    //Helper
    string Format_ParNode(ConcatStringTree::ParNode* cur) const;
    string PreOrder(ConcatStringTree::ParNode* cur) const;
};

//class ReducedConcatStringTree; // forward declaration
/*
class HashConfig {
private:
    int p;
    double c1, c2;
    double lambda;
    double alpha;
    int initSize;
    friend class ReducedConcatStringTree;
};
*/
//class ReducedConcatStringTree /* */ {
/*
public:
    class LitStringHash {
    public:
        LitStringHash(const HashConfig & hashConfig);
        int getLastInsertedIndex() const;
        string toString() const;
    };
public:
    static LitStringHash litStringHash;
};*/

#endif // __CONCAT_STRING_TREE_H__