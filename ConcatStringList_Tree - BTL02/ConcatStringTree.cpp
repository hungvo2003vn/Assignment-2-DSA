#include "ConcatStringTree.h"

int max_id=0;
ConcatStringTree::ConcatStringTree() {
	Root = NULL;
}
ConcatStringTree::ConcatStringTree(const char* s) {

	string tmp = string(s);
	Root = new Node(0, (int)tmp.length(), tmp, NULL, NULL);
	
	//Update Parent for Node
	Root->Par = new ParentsTree();
	Root->Par->Insert(max_id);

}
//Get length
int ConcatStringTree::length() const {
	return Root->length;
}
//Get char at index
char ConcatStringTree::search_index(Node* cur, int index) const{
	if (!cur->left && !cur->right) return cur->data[index];
	if (index < cur->leftLength)
		return search_index(cur->left, index);
	else
	{
		index -= cur->leftLength;
		return search_index(cur->right, index);
	}
}
char ConcatStringTree::get(int index) const{
	if (!(0 <= index && index < Root->length))
		throw out_of_range("Index of string is invalid!");
	return search_index(Root, index);
}
//Get index of char
int ConcatStringTree::search_dfs(Node* cur, int index, char c) const{
	if (!cur) return -1;

	if (!cur->left && !cur->right)
	{
		int pos = (int)cur->data.find(c);
		if (pos != string::npos)
		{
			index += pos;
			return index;
		}
		else
		{
			index += (int)cur->data.length();
			return -1;
		}
	}
	int pos_left = search_dfs(cur->left, index, c);
	if (pos_left != -1) return pos_left;

	if (cur->left) index += cur->left->length;
	int pos_right = search_dfs(cur->right, index, c);
	if (pos_right != -1) return pos_right;

	return -1;
}
int ConcatStringTree::indexOf(char c) const{
	int index = 0;
	int ans= search_dfs(Root, index, c);
	return ans;
}
//PreOrder
string ConcatStringTree::toStringNode(Node* cur) const {

	if (!cur) return "";

	string ans = "(LL=" + to_string(cur->leftLength) + "," +
		"L=" + to_string(cur->length) + ",";

	if (cur->left || cur->right) ans += "<NULL>)";
	else ans += "\"" + cur->data + "\")";

	return ans;
}
string ConcatStringTree::pre_order(Node* cur) const {

	if (!cur) return "";

	string ans = toStringNode(cur);
	if (cur->left) ans += ";" + pre_order(cur->left);
	if (cur->right) ans += ";" + pre_order(cur->right);
	return ans;
}
string ConcatStringTree::toStringPreOrder() const {

	string ans = "ConcatStringTree[" + pre_order(Root) + "]";
	return ans;
}
//Tostring 
string ConcatStringTree::toString_helper(Node* cur) const {
	if (!cur) return "";
	else return cur->data + toString_helper(cur->left) + toString_helper(cur->right);
}
string ConcatStringTree::toString() const {
	string ans = "ConcatStringTree[\""
		+ toString_helper(Root) + "\"]";
	return ans;
}
//Concat
ConcatStringTree ConcatStringTree::concat(const ConcatStringTree& otherS) const {
	
	ConcatStringTree ans = ConcatStringTree();

	ans.Root = new Node(Root->length, Root->length + otherS.Root->length, "", Root, otherS.Root);
	//Update Parent for Node
	
	ans.Root->Par = new ParentsTree();
	Parents_add(ans.Root, max_id);
	
	return ans;
}
//subStr
ConcatStringTree::Node* ConcatStringTree::combine(Node* L, Node* R) const {

	if (!L && !R) return NULL;

	Node* root = new Node(0, 0, "", L, R);

	if (L) root->leftLength = L->length;
	root->length = root->leftLength;
	if (R) root->length += R->length;

	//Add Parent to all node in the subtree "root"
	root->Par = new ParentsTree();
	Parents_add(root, max_id);

	//Extra delete subtree
	if (root->left && root->left->Par) root->left->Par->Remove(root->left->id);
	if (root->right && root->right->Par) root->right->Par->Remove(root->right->id);

	return root;
}
ConcatStringTree::Node* ConcatStringTree::build_bottom(Node* cur, int start, int from, int to) const {

	if (!cur) return NULL;
	if (!cur->left && !cur->right)
	{
		if (start >= to || start + cur->length - 1 < from) return NULL;

		Node* tmp = NULL;

		if (start <= from && from < start + cur->length) //Current Node contain position from
			tmp = new Node(0, 0, cur->data.substr(from - start, min(cur->length + start - from, to - from)), NULL, NULL);
		
		else if (start < to && to <= start + cur->length) //Current Node contain position to-1
			tmp = new Node(0, 0, cur->data.substr(0, to - start), NULL, NULL);
		
		else //Current Node does not contain position from and to-1
			tmp = new Node(0, 0, cur->data, NULL, NULL);

		//Add parent for the node
		tmp->length = (int)tmp->data.length();

		tmp->Par = new ParentsTree();
		tmp->Par->Insert(max_id);

		return tmp;
	}
	else return combine(build_bottom(cur->left, start, from, to), 
		                build_bottom(cur->right, start + cur->leftLength, from, to));
}
ConcatStringTree ConcatStringTree::subString(int from, int to) const {
	
	if (from<0 || from>=Root->length || to>Root->length || to<0)
		throw out_of_range("Index of string is invalid!");
	if (from >= to)
		throw logic_error("Invalid range!");

	ConcatStringTree ans = ConcatStringTree();

	ans.Root = build_bottom(Root, 0, from, to);
	return ans;
}
//Reverse
ConcatStringTree::Node* ConcatStringTree::deepRe(Node* cur) const {
	if (!cur) return NULL;
	
	if (!cur->left && !cur->right) //Attemp Leaf Node
	{
		string s = string(cur->data.rbegin(), cur->data.rend());
		Node* new_ele = new Node(0, cur->length, s, NULL, NULL);

		//Update Parent for the node
		new_ele->Par = new ParentsTree();
		new_ele->Par->Insert(max_id);

		return new_ele;
	}

	return combine(deepRe(cur->right), deepRe(cur->left));
}
ConcatStringTree ConcatStringTree::reverse() const {
	
	ConcatStringTree ans = ConcatStringTree();

	ans.Root = deepRe(Root);
	return ans;
}
//////////////////////PARENTSTREE IMPLEMENTATION/////////////////////////
ParentsTree::ParentsTree() {
	Paroot = NULL;
	nums_node = 0;
}
//get size()
int ParentsTree::size() const {
	return this->nums_node;
}
//get height
int ParentsTree::height(ConcatStringTree::ParNode* cur) {
	if (!cur) return 0;
	return cur->height;
}
//get balance value
int ParentsTree::getBalance(ConcatStringTree::ParNode* cur) {
	if (!cur) return 0;
	return height(cur->left) - height(cur->right);
}
//Right rotate
ConcatStringTree::ParNode* ParentsTree::Rtate(ConcatStringTree::ParNode* cur) {
	ConcatStringTree::ParNode* L = cur->left;
	ConcatStringTree::ParNode* tmp = L->right;

	//Rotate
	L->right = cur;
	cur->left = tmp;
	//Update heights
	cur->height = max(height(cur->left), height(cur->right)) + 1;
	L->height = max(height(L->left), height(L->right)) + 1;

	return L;
}
//Left rotate
ConcatStringTree::ParNode* ParentsTree::Ltate(ConcatStringTree::ParNode* cur) {
	ConcatStringTree::ParNode* R = cur->right;
	ConcatStringTree::ParNode* tmp = R->left;

	//Rotate
	R->left = cur;
	cur->right = tmp;
	//Update heights
	cur->height = max(height(cur->left), height(cur->right)) + 1;
	R->height = max(height(R->left), height(R->right)) + 1;

	return R;
}
//Find max value and return node
ConcatStringTree::ParNode* ParentsTree::MaxNode(ConcatStringTree::ParNode* cur) {
	ConcatStringTree::ParNode* start = cur;
	while (start && start->right) start = start->right;
	return start;
}
//Insert with return node
ConcatStringTree::ParNode* ParentsTree::insert(ConcatStringTree::ParNode* cur, int key) {
	if (!cur)
	{
		nums_node++;
		return (new ConcatStringTree::ParNode(key, NULL, NULL, 1));
	}

	if (key < cur->id) cur->left = insert(cur->left, key);
	else if (key > cur->id) cur->right = insert(cur->right, key);
	else return cur;

	//Update height
	cur->height = max(height(cur->left), height(cur->right)) + 1;
	int balance = getBalance(cur);

	//LL rotate case
	if (balance > 1 && key < cur->left->id) return Rtate(cur);
	//RR rotate case
	if (balance < -1 && key > cur->right->id) return Ltate(cur);
	//LR rotate case
	if (balance > 1 && key > cur->left->id)
	{
		cur->left = Ltate(cur->left);
		return Rtate(cur);
	}
	//RL rotate case
	if (balance < -1 && key < cur->right->id)
	{
		cur->right = Rtate(cur->right);
		return Ltate(cur);
	}

	return cur;
}
//void insert
void ParentsTree::Insert(int key) {
	Paroot = insert(Paroot, key);
}
//remove with return node
ConcatStringTree::ParNode* ParentsTree::remove(ConcatStringTree::ParNode* cur, int key, bool& deleted) {
	if (!cur) return cur;

	if (key < cur->id) cur->left = remove(cur->left, key, deleted);
	else if (key > cur->id) cur->right = remove(cur->right, key, deleted);
	else
	{
		if (!cur->left || !cur->right)
		{
			ConcatStringTree::ParNode* tmp = cur->left ? cur->left : cur->right;
			if (!tmp)
			{
				tmp = cur;
				cur = NULL;
			}
			else *cur = *tmp;

			if(tmp) delete tmp; 
			tmp = NULL;
		}
		else
		{
			ConcatStringTree::ParNode* tmp = MaxNode(cur->left);
			cur->id = tmp->id;
			cur->left = remove(cur->left, tmp->id, deleted);
		}
		deleted = true;
	}
	if (!cur) return cur;
	//Update height
	cur->height = max(height(cur->left), height(cur->right)) + 1;

	//Balance step
	int balance = getBalance(cur);

	//LL rotate case
	if (balance > 1 && getBalance(cur->left) >= 0)
		return Rtate(cur);
	//LR rotate case
	if (balance > 1 && getBalance(cur->left) < 0)
	{
		cur->left = Ltate(cur->left);
		return Rtate(cur);
	}
	//RR rotate case
	if (balance < -1 && getBalance(cur->right) <= 0)
		return Ltate(cur);
	//RL rotate case
	if (balance < -1 && getBalance(cur->right) > 0)
	{
		cur->right = Rtate(cur->right);
		return Ltate(cur);
	}

	return cur;
}
//void remove
void ParentsTree::Remove(int key) {

	bool deleted = false;
	Paroot = remove(Paroot, key, deleted);
	if (deleted) --nums_node;
	 
	
	return;
}
///Format ParentsTree Functions
string ParentsTree::Format_ParNode(ConcatStringTree::ParNode* cur) const {
	if (!cur) return "";
	string ans = "(id=" + to_string(cur->id) + ")";
	return ans;
}
string ParentsTree::PreOrder(ConcatStringTree::ParNode* cur) const {
	if (!cur) return "";

	string ans = Format_ParNode(cur);
	if (cur->left) ans += ";" + PreOrder(cur->left);
	if (cur->right) ans += ";" + PreOrder(cur->right);

	return ans;
}
string ParentsTree::toStringPreOrder() const {
	string ans = "ParentsTree[" + PreOrder(Paroot) + "]";
	return ans;
}
//Traverse Sub tree and add Parents 
void ConcatStringTree::Parents_add(Node* cur, int key) const {
	if (!cur) return;

	cur->Par->Insert(key);
	
	if (cur->left) cur->left->Par->Insert(key);
	if (cur->right) cur->right->Par->Insert(key);

	return;
}
//Get Partree size
int ConcatStringTree::getParTreeSize(const string& query) const {
	Node* tmp = Root;
	for (char c : query) {
		if (!tmp)
			throw runtime_error("Invalid query: reaching NULL");

		if (c == 'l') tmp = tmp->left;
		else if (c == 'r') tmp = tmp->right;
		else
			throw runtime_error("Invalid character of query");
	}
	if (!tmp)
		throw runtime_error("Invalid query: reaching NULL");

	return tmp->Par->size();
}
//get Partree preorder
string ConcatStringTree::getParTreeStringPreOrder(const string& query) const {
	Node* tmp = Root;
	
	for (char c : query) {
		if (!tmp)
			throw runtime_error("Invalid query: reaching NULL");

		if (c == 'l') tmp = tmp->left;
		else if (c == 'r') tmp = tmp->right;
		else
			throw runtime_error("Invalid character of query");
	}
	if (!tmp)
		throw runtime_error("Invalid query: reaching NULL");

	return tmp->Par->toStringPreOrder();
}
///DESTRUCTOR OF CONCATSTRINGTREE///
//delete tree
void ConcatStringTree::Concat_delete(Node* &cur) {
	if (!cur) return;

	cur->Par->Remove(cur->id);
	if (cur->Par->size() == 0) {

		//Delete Left
		if (cur->left && cur->left->Par) cur->left->Par->Remove(cur->id);
		if (cur->left && cur->left->Par && cur->left->Par->size() == 0) Concat_delete(cur->left);

		//Delete Right
		if (cur->right && cur->right->Par) cur->right->Par->Remove(cur->id);
		if (cur->right && cur->right->Par && cur->right->Par->size() == 0) Concat_delete(cur->right);

		if (cur->Par) delete cur->Par;
		cur->Par = NULL;

		delete cur;
		cur = NULL;
	}
	
	return;
}
ConcatStringTree::~ConcatStringTree() {
	Concat_delete(Root);
	Root = NULL;
}
////////////CLASS LITSTRINGHASH AND REDUCEDCONCATSTRINGTREE//////////////////////
HashConfig::HashConfig() {
	this->p = 0;
	this->c1 = 0.0;
	this->c2 = 0.0;
	this->lambda = 0.0;
	this->alpha = 0.0;
	this->initSize = 0;
}
HashConfig::HashConfig(const HashConfig& other) {
	this->p = other.p;
	this->c1 = other.c1;
	this->c2 = other.c2;
	this->lambda = other.lambda;
	this->alpha = other.alpha;
	this->initSize = other.initSize;
}
HashConfig::HashConfig(int P, double C1, double C2, double Lambda,
					   double Alpha, int InitSize) {
	this->p = P;
	this->c1 = C1;
	this->c2 = C2;
	this->lambda = Lambda;
	this->alpha = Alpha;
	this->initSize = InitSize;
}
////////////CLASS LITSTRINGHASH///////////
LitStringHash::LitStringHash() {
	hashConfig = HashConfig();
	m = 0;
	all_nodes = 0;
	last_index = -1;
	bucket = NULL;
	status = NULL;
}
//deep copy constructor
LitStringHash::LitStringHash(const HashConfig& hashConfig) {

	this->hashConfig = HashConfig(hashConfig);

	m = this->hashConfig.initSize;
	all_nodes = 0;
	last_index = -1;

	bucket = new LitString[m];
	status = new STATUS[m];
	for (int i = 0; i < m; i++) status[i] = NIL;

}
//Hash fucntion
int MulMod(int a, int b, int M) {
	int res = 0;
	a = a % M;
	while (b > 0) {
		if (b % 2 == 1) res = (res + a) % M;
		a = (a * 2) % M;
		b = b / 2;
	}
	return res;
}
int PowMod(int a, int n, int M) {
	if (n == 0) return 1;
	if (a == 0) return 0;
	int tmp = PowMod(a, n / 2, M);
	if (n % 2 == 1) return MulMod(MulMod(tmp, tmp, M), a, M);
	else return MulMod(tmp, tmp, M);
}
int LitStringHash::h(string s) {
	int ans = 0;
	for (int i = 0; i < (int)s.length(); i++)
		ans += MulMod((int)s[i], PowMod(hashConfig.p, i, m), m) % m;

	return ans;
}
//Find function
int LitStringHash::hp(string s, int i) {
	return ((int)(h(s) + hashConfig.c1*i + hashConfig.c2*i*i))%m;
}
//Insert
void LitStringHash::Insert(string s, bool rehashing, LitString ele) {
	
	renew(); //Check if the hash table existed or not
	int i = 0;
	do 
	{
		int slot = hp(s,i);
		if (status[slot] == NIL || status[slot] == DELETED)
		{
			//Assign value
			if (rehashing) bucket[slot] = LitString(ele.num_refs, ele.nod);
			else bucket[slot] = LitString(1, s);

			status[slot] = NON_EMPTY;

			//Update nums
			if (!rehashing) all_nodes++;
			last_index = slot;

			return;
		}
		else if (status[slot] == NON_EMPTY && bucket[slot].nod==s) {
			if (!rehashing) bucket[slot].num_refs++;
			return;
		}
		else ++i;

	} while (i < m);
	//No slot
	throw runtime_error("No possible slot");
}
//Rehash
void LitStringHash::Rehash() {
	if ( (double)all_nodes/m - hashConfig.lambda > (1e-7) ) 
	{
		int new_size = (int)(hashConfig.alpha * m);
		int old_size = m;
		m = new_size;

		LitString* tmp = new LitString[old_size];
		STATUS* cop = new STATUS[old_size];

		//Copy old data
		for (int i = 0; i < old_size; i++)
		{
			cop[i] = status[i];
			if (cop[i] == NON_EMPTY) 
				tmp[i] = LitString(bucket[i].num_refs, bucket[i].nod);
		}

		//New status
		delete[] status;
		status = new STATUS[new_size];
		for (int i = 0; i < new_size; i++) status[i] = NIL;

		//New bucket
		delete[] bucket;
		bucket = new LitString[new_size];

		int before_hash = last_index;
		bool found = false;

		for (int i = 0; i < old_size; i++) 
		{
			if (cop[i] == NON_EMPTY) 
			{
				Insert(tmp[i].nod, true, tmp[i]);

				//Catch the new last_index that contain the value of the old last_index
				if (i == before_hash && !found) 
				{
					found = true;
					before_hash = last_index;
				}
			}	
		}
		last_index = before_hash;

		delete[] tmp;
		delete[] cop;
	}
}
void LitStringHash::insert(string s) {
	Insert(s);
	Rehash();
}
//Remove
void LitStringHash::remove(string s) {
	int i = 0;
	do 
	{
		int slot = hp(s, i);
		if (status[slot] == NIL) return;
		else if (status[slot]==NON_EMPTY && bucket[slot].nod == s)
		{
			bucket[slot].num_refs--;

			if (bucket[slot].num_refs == 0) 
			{
				--all_nodes;
				status[slot] = DELETED;
				retrieve();
			}
			return;
		}
		else ++i;
	} while (i < m);
	return;
}

//Required Function
int LitStringHash::getLastInsertedIndex() const {
	return this->last_index;
}
string LitStringHash::toString() const {
	string ans = "LitStringHash[";
	for (int i = 0; i < m; i++) 
	{
		ans += "(";
		if (status && status[i] == NON_EMPTY) ans += "litS=\"" + bucket[i].nod + "\"";
		ans += ");";
	}
	if (ans.back() == ';') ans.pop_back();
	return ans + "]";
}
//Check and Renew
void LitStringHash::renew() {
	if (m > 0) return;

	m = this->hashConfig.initSize;
	all_nodes = 0;
	last_index = -1;

	bucket = new LitString[m];
	status = new STATUS[m];
	for (int i = 0; i < m; i++) status[i] = NIL;
}
//Check and Retrieve
void LitStringHash::retrieve() {
	if (all_nodes > 0) return;

	m = 0;
	last_index = -1;
	delete[] bucket;
	delete[] status;
	bucket = NULL;
	status = NULL;
}
//Destructor for LitStringHash
LitStringHash::~LitStringHash() {
	
	m = 0;
	all_nodes = 0;
	last_index = -1;
	delete[] bucket;
	delete[] status;
	bucket = NULL;
	status = NULL;
}
////////////CLASS REDUCEDCONCATSTRINGTREE///////////
ReducedConcatStringTree::ReducedConcatStringTree(){
	this->litStringHash = NULL;
	this->Root = NULL;
}
//Constructor
ReducedConcatStringTree::ReducedConcatStringTree(const char* s, LitStringHash* litStringHash) {
	
	this->litStringHash = litStringHash;
	string tmp = string(s);

	//Find node in litstringhash
	this->litStringHash->insert(tmp);
	Root = new Node(0, (int)tmp.length(), tmp, NULL, NULL);

	//Update Parent for Node
	Root->Par = new ParentsTree();
	Root->Par->Insert(max_id);
}
//Concat
ReducedConcatStringTree ReducedConcatStringTree::concat(const ReducedConcatStringTree& otherS) const {
	
	ReducedConcatStringTree ans = ReducedConcatStringTree();

	ans.Root = new Node(Root->length, Root->length + otherS.Root->length, "", Root, otherS.Root);

	//Update Parent for Node
	ans.Root->Par = new ParentsTree();
	Parents_add(ans.Root, max_id);
	
	//Assgin listringHash
	ans.litStringHash = litStringHash;

	return ans;
}
//destructor 
void ReducedConcatStringTree::ReducedConcat_delete(Node* &cur) {
	if (!cur) return;
	
	cur->Par->Remove(cur->id);
	if (cur->Par->size() == 0) 
	{
		if (!cur->left && !cur->right) litStringHash->remove(cur->data);
		
		//Delete Left
		if (cur->left) cur->left->Par->Remove(cur->id);
		if (cur->left && cur->left->Par->size() == 0) ReducedConcat_delete(cur->left);

		//Delete Right
		if (cur->right) cur->right->Par->Remove(cur->id);
		if (cur->right && cur->right->Par->size() == 0) ReducedConcat_delete(cur->right);

		if (cur->Par) delete cur->Par;
		cur->Par = NULL;

		delete cur;
		cur = NULL;
	}
	return;
}
ReducedConcatStringTree::~ReducedConcatStringTree() {
	ReducedConcat_delete(Root);
	Root = NULL;
}

