#ifndef __CONCAT_STRING_TREE_H__
#define __CONCAT_STRING_TREE_H__

#include "main.h"

static int max_id=0;
enum STATUS { NIL, NON_EMPTY, DELETED }; //status type
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
    ConcatStringTree(ConcatStringTree&& obj) noexcept {
        Root = move(obj.Root);
        obj.Root = NULL;
    }
    ConcatStringTree(const char* s);
    int length() const;
    char get(int index) const;
    int indexOf(char c) const;
    virtual string toStringPreOrder() const;
    virtual string toString() const;
    virtual ConcatStringTree concat(const ConcatStringTree& otherS) const;
    ConcatStringTree subString(int from, int to) const;
    ConcatStringTree reverse() const;

    virtual int getParTreeSize(const string& query) const;
    virtual string getParTreeStringPreOrder(const string& query) const;
    virtual ~ConcatStringTree();

    //Helper Function
    //get and indexOf
    char search_index(Node* cur, int index) const;
    int search_dfs(Node* cur, int index, char c) const;
    //preorder
    string pre_order(Node* cur) const;
    string toStringNode(Node* cur) const;
    //toString
    string toString_helper(Node* cur) const;
    //subString
    Node* combine(Node* L, Node* R) const;
    Node* build_bottom(Node* root, int start, int from, int to) const;
    //reverse
    Node* deepRe(Node* cur) const;
    //add parent recursion
    void Parents_add(Node* cur, int key) const;
    void Parents_delete(Node* cur, int key) const;
    //delete parent recursion
    
    void Concat_delete(Node* &cur);
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
    //Helper preorder
    string Format_ParNode(ConcatStringTree::ParNode* cur) const;
    string PreOrder(ConcatStringTree::ParNode* cur) const;
};

class ReducedConcatStringTree; // forward declaration
class HashConfig {
private:
    int p;
    double c1, c2;
    double lambda;
    double alpha;
    int initSize;
    friend class LitStringHash;
public:
    HashConfig();
    HashConfig(const HashConfig& other);
    HashConfig(int p, double c1, double c2, double lambda,
               double alpha, int initSize);
};

class LitStringHash {
public:
    struct LitString {

        int num_refs;
        string nod;
        LitString(int nums = 1, string node = "")
        {
            num_refs = nums;
            nod = node;
        }
    };
public:

    HashConfig hashConfig;
    int m;
    int all_nodes;
    int last_index;
    LitString* bucket;
    STATUS* status;
public:
    LitStringHash();
    LitStringHash(const HashConfig& hashConfig);
    int getLastInsertedIndex() const;
    string toString() const;

    int h(string s);
    int hp(string s, int i);

    void Insert(string s, bool rehashing = false, LitString ele = LitString());
    void insert(string s);
    void Rehash();
    void remove(string s);
    int search(string s);
    ~LitStringHash();
};

class ReducedConcatStringTree:public ConcatStringTree  {

public:
    LitStringHash* litStringHash;

public:
    ReducedConcatStringTree();
    ReducedConcatStringTree(ReducedConcatStringTree&& obj) noexcept {
        Root = move(obj.Root);
        litStringHash = move(obj.litStringHash);
        obj.Root = NULL;
        obj.litStringHash = NULL;
    } 
    ReducedConcatStringTree(const char* s, LitStringHash* litStringHash);
    ReducedConcatStringTree concat(const ReducedConcatStringTree& otherS) const;
    
    //Destructor
    void ReducedConcat_delete(Node* &cur);
    ~ReducedConcatStringTree();
};

#endif // __CONCAT_STRING_TREE_H__