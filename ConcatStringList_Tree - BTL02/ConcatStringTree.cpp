#include "ConcatStringTree.h"

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

	union S //Avoid auto-call destructor when get out of the scope
	{
		ConcatStringTree ans;
		S()
		{
			ans = ConcatStringTree();
		}
		~S()
		{

		}
	} pro;

	pro.ans.Root = new Node(Root->length, Root->length + otherS.Root->length, "", Root, otherS.Root);
	//Update Parent for Node
	pro.ans.Root->Par = new ParentsTree();
	Parents_add(pro.ans.Root, max_id);
	
	return pro.ans;
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
	if (from<0 || to>Root->length)
		throw out_of_range("Index of string is invalid!");
	if (from >= to)
		throw logic_error("Invalid range!");

	union S //Avoid auto-call destructor when get out of the scope
	{
		ConcatStringTree ans;
		S()
		{
			ans = ConcatStringTree();
		}
		~S()
		{

		}
	} pro;

	pro.ans.Root = build_bottom(Root, 0, from, to);
	return pro.ans;
}
//Reverse
ConcatStringTree::Node* ConcatStringTree::deepRe(Node* cur) const {
	if (!cur) return NULL;
	
	if (!cur->left && !cur->right) //Attemp Leaf Node
	{
		string s = string(cur->data.rbegin(), cur->data.rend());
		Node* new_ele = new Node(cur->length - cur->leftLength, cur->length, s, NULL, NULL);

		//Update Parent for the node
		new_ele->Par = new ParentsTree();
		new_ele->Par->Insert(max_id);

		return new_ele;
	}

	return combine(deepRe(cur->right), deepRe(cur->left));
}
ConcatStringTree ConcatStringTree::reverse() const {
	
	union S //Avoid auto-call destructor when get out of the scope
	{
		ConcatStringTree ans;
		S()
		{
			ans = ConcatStringTree();
		}
		~S()
		{

		}
	} pro;

	pro.ans.Root = deepRe(Root);
	return pro.ans;
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
		return (new ConcatStringTree::ParNode(key, NULL, NULL, 0));
	}

	if (key < cur->id) cur->left = insert(cur->left, key);
	else if (key > cur->id) cur->right = insert(cur->right, key);
	else return cur;

	//Update height
	cur->height = max(height(cur->left), height(cur->right)) + 1;
	int balance = getBalance(cur);

	//LL rotate case
	if (balance > 1 && cur->id < cur->left->id) return Rtate(cur);
	//RR rotate case
	if (balance < -1 && cur->id > cur->right->id) return Ltate(cur);
	//LR rotate case
	if (balance > 1 && cur->id > cur->left->id)
	{
		cur->left = Ltate(cur->left);
		return Rtate(cur);
	}
	//RL rotate case
	if (balance < -1 && cur->id < cur->right->id)
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
			else
			{
				cur->id = tmp->id;
				cur->left = tmp->left;
				cur->right = tmp->right;
				cur->height = tmp->height; 
			}

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
//Traverse Subtree and remove Parents
void ConcatStringTree::Parents_delete(Node* cur, int key) const {
	if (!cur) return;

	cur->Par->Remove(key);

	if (cur->left && cur->left->Par) cur->left->Par->Remove(key);
	if (cur->right && cur->right->Par) cur->right->Par->Remove(key);

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
	return tmp->Par->toStringPreOrder();
}
///DESTRUCTOR OF CONCATSTRINGTREE///
//delete tree
void ConcatStringTree::Concat_delete(Node* &cur) {
	if (!cur) return;

	if (cur->Par && cur->Par->size() != 0) Parents_delete(cur, cur->id);

	if (cur->Par && cur->Par->size() == 0) 
	{
		Node* L = cur->left;
		Node* R = cur->right;
		cur->left = NULL;
		cur->right = NULL;

		if (cur->Par) delete cur->Par;
		cur->Par = NULL;

		delete cur;
		cur = NULL;

		if(L && L->Par && L->Par->size()==0) Concat_delete(L);
		if(R && R->Par && R->Par->size()==0) Concat_delete(R);
	}
	return;
}
ConcatStringTree::~ConcatStringTree() {
	Concat_delete(Root);
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
//Get the private value
int HashConfig::getP() const {
	return this->p;
}
double HashConfig::getC1() const {
	return this->c1;
}
double HashConfig::getC2() const {
	return this->c2;
}
double HashConfig::getLambda() const {
	return this->lambda;
}
double HashConfig::getAlpha() const {
	return this->alpha;
}
int HashConfig::getInitSize() const {
	return this->initSize;
}
//Chang the private value
void HashConfig::changeP(int P) {
	p = P;
}
void HashConfig::changeC1(double C1) {
	c1 = C1;
}
void HashConfig::changeC2(double C2) {
	c2 = C2;
}
void HashConfig::changeLambda(double Lambda) {
	lambda = Lambda;
}
void HashConfig::changeAlpha(double Alpha) {
	alpha = Alpha;
}
void HashConfig::changeInitSize(int InitSize) {
	initSize = InitSize;
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

	m = this->hashConfig.getInitSize();
	all_nodes = 0;
	last_index = -1;

	bucket = new LitString[m];
	status = new STATUS[m];
	for (int i = 0; i < m; i++) status[i] = NIL;

}
//Hash fucntion
int LitStringHash::h(string s) {
	int ans = 0;
	int times = 1;
	for (int i = 0; i < (int)s.length(); i++)
	{
		ans = (ans + ( ((int)s[i])%m * times)%m )%m;
		times = (times * ((hashConfig.getP())%m)  )%m;
	}
	return ans;
}
//Find function
int LitStringHash::hp(string s, int i) {
	int ans = h(s);
	double c1_i = hashConfig.getC1();
	double c2_i2 = hashConfig.getC2();

	c1_i*=i;
	c2_i2 *= i * i;

	ans =  (int)(ans*1.0+(c1_i+c2_i2)) % m;
	return ans;
}
//Insert
void LitStringHash::insert(string s) {
	int i = 0;
	do 
	{
		int slot = hp(s,i);
		if (status[slot] == NIL || status[slot] == DELETED)
		{
			//Assign value
			bucket[slot] = LitString(1, new ConcatStringTree::Node(0, (int)s.length(), s, NULL, NULL));
			bucket[slot].nod->Par = new ParentsTree();
			bucket[slot].nod->Par->Insert(max_id);

			status[slot] = NON_EMPTY;

			//Update nums
			all_nodes++;
			last_index = slot;

			Rehash(); //Rehashing
			return;
		}
		else if (status[slot] == NON_EMPTY && bucket[slot].nod->data==s) {
			bucket[slot].num_refs++;
			bucket[slot].nod->num_refs++;

			//Update nums
			all_nodes++;
			last_index = slot;

			Rehash();//rehashing
			return;
		}
		else ++i;

	} while (i < m);
	//No slot
	throw runtime_error("No possible slot");
}
//Rehash
void LitStringHash::Rehash() {
	if (all_nodes / (1.0*m) - hashConfig.getLambda() >1e-7) 
	{
		int new_size = (int)(hashConfig.getAlpha() * m);

		LitString* tmp = new LitString[new_size];
		STATUS* tatus = new STATUS[new_size];
		for (int i = 0; i < new_size; i++) tatus[i] = NIL;

		for (int i = 0; i < m && i < new_size; i++) 
		{
			tatus[i] = status[i];
			if (status[i] == NON_EMPTY) 
				tmp[i] = LitString(bucket[i].num_refs, bucket[i].nod);
		}
		delete[] status;
		delete[] bucket;

		bucket = tmp;
		status = tatus;

		//Update size and hashConfig
		m = new_size;
		hashConfig.changeInitSize(m);
	}
}
//Remove
void LitStringHash::remove(string s) {
	int i = 0;
	do 
	{
		int slot = hp(s, i);
		if (status[slot] == NIL) return;
		else if (status[slot]==NON_EMPTY && bucket[slot].nod->data == s)
		{
			--all_nodes;
			bucket[slot].num_refs--;
			bucket[slot].nod->num_refs--;

			if(bucket[slot].num_refs==0) status[slot] = DELETED;

			return; //WARNING
		}
		else ++i;
	} while (i < m);
	return;
}
//Search
int LitStringHash::search(string s) {
	int i = 0;
	do 
	{
		int slot = hp(s, i);
		if (status[slot] == NON_EMPTY && bucket[slot].nod->data == s) return slot;
		else if (status[slot] == NIL ) return -1;
		else ++i;
	} while (i < m);
	return -1;
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
		if (status[i] == NON_EMPTY) ans += "litS=\"" + bucket[i].nod->data + "\"";
		ans += ");";
	}
	if (ans.back() == ';') ans.pop_back();
	return ans + "]";
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
	ReRoot = NULL;
	Root = NULL;
}
//Constructor
ReducedConcatStringTree::ReducedConcatStringTree(const char* s, LitStringHash* litStringHash) {
	
	this->litStringHash = litStringHash;
	string tmp = string(s);

	//Find node in litstringhash
	this->litStringHash->insert(tmp);
	int slot = this->litStringHash->search(tmp);
	ReRoot = this->litStringHash->bucket[slot].nod;

	//Initialize
	Root = NULL;
}
//Concat
ReducedConcatStringTree ReducedConcatStringTree::concat(const ReducedConcatStringTree& otherS) const {

	union S //Avoid auto-call destructor when get out of the scope
	{
		ReducedConcatStringTree ans;
		S()
		{
			ans = ReducedConcatStringTree();
		}
		~S()
		{

		}
	} pro;

	pro.ans.ReRoot = new Node(ReRoot->length, ReRoot->length + otherS.ReRoot->length, "", ReRoot, otherS.ReRoot);

	//Update Parent for Node
	pro.ans.ReRoot->Par = new ParentsTree();
	Parents_add(pro.ans.ReRoot, max_id);

	//Assign LitStringHash
	pro.ans.litStringHash = otherS.litStringHash;

	return pro.ans;
}
//tostring
string ReducedConcatStringTree::toStringPreOrder() const {

	string ans = "ConcatStringTree[" + pre_order(ReRoot) + "]";
	return ans;
}
string ReducedConcatStringTree::toString() const {
	string ans = "ConcatStringTree[\""
		+ toString_helper(ReRoot) + "\"]";
	return ans;
}
//Get Partree size
int ReducedConcatStringTree::getParTreeSize(const string& query) const {
	Node* tmp = ReRoot;
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
string ReducedConcatStringTree::getParTreeStringPreOrder(const string& query) const {
	Node* tmp = ReRoot;

	for (char c : query) {
		if (!tmp)
			throw runtime_error("Invalid query: reaching NULL");

		if (c == 'l') tmp = tmp->left;
		else if (c == 'r') tmp = tmp->right;
		else
			throw runtime_error("Invalid character of query");
	}
	return tmp->Par->toStringPreOrder();
}
//destructor 
void ReducedConcatStringTree::ReducedConcat_delete(Node* &cur) {
	if (!cur) return;
	
	if (cur->Par && cur->Par->size() != 0) 
	{
		//Leaf node maybe has some references
		if (!cur->left && !cur->right)
		{
			if (litStringHash && litStringHash->all_nodes>0) 
			{
				int slot = litStringHash->search(cur->data);
				litStringHash->remove(cur->data);
				if (litStringHash->status[slot] != DELETED) return;
			}
			else 
			{
				cur->num_refs--;
				if (cur->num_refs != 0) return;
			}
		}
		Parents_delete(cur, cur->id);
	}
	if (cur->Par && cur->Par->size() == 0)
	{
		bool same_node = (cur->left==cur->right);
		if (cur->left && cur->left->Par && cur->left->Par->size() == 0) ReducedConcat_delete(cur->left);

		if (!same_node) 
		{
			if(cur->right && cur->right->Par && cur->right->Par->size() == 0) ReducedConcat_delete(cur->right);
		}

		if (cur->Par) delete cur->Par;
		cur->Par = NULL;

		cur->left = NULL;
		cur->right = NULL;
		delete cur;
		cur = NULL;
	}
	return;
}
ReducedConcatStringTree::~ReducedConcatStringTree() {
	ReducedConcat_delete(ReRoot);
}

