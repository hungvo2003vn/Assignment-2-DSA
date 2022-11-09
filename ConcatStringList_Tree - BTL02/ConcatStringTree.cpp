#include "ConcatStringTree.h"

ConcatStringTree::ConcatStringTree() {
	Root = NULL;
}
ConcatStringTree::ConcatStringTree(const char* s) {

	string tmp = string(s);
	Root = new Node(0, (int)tmp.length(), tmp, NULL, NULL);
	
	//Update Parent for Node
	Root->Par = new ParentsTree();
	Root->Par->Paroot = new ParNode(max_id, NULL, NULL, 0);
	Root->Par->nums_node++;
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

	string ans = "\"ConcatStringTree[" + pre_order(Root) + "]\"";
	return ans;
}
//Tostring 
string ConcatStringTree::toString_helper(Node* cur) const {
	if (!cur) return "";
	else return cur->data + toString_helper(cur->left) + toString_helper(cur->right);
}
string ConcatStringTree::toString() const {
	string ans = "\"ConcatStringTree[\""
		+ toString_helper(Root) + "\"]\"";
	return ans;
}
//Concat
ConcatStringTree ConcatStringTree::concat(const ConcatStringTree& otherS) const {

	ConcatStringTree* ans = new ConcatStringTree();
	ans->Root = new Node(Root->length, Root->length + otherS.Root->length, "", Root, otherS.Root);

	//Update Parent for Node
	ans->Root->Par = new ParentsTree();
	ParNode* new_ele = new ParNode(max_id, NULL, NULL, 0);
	Parents_add(ans->Root, new_ele);

	return *ans;
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
	ParNode* new_Parent = new ParNode(max_id, NULL, NULL, 0);
	Parents_add(root, new_Parent);

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
		tmp->Par->Paroot = new ParNode(max_id, NULL, NULL, 0);
		tmp->Par->nums_node++;

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

	ConcatStringTree* ans = new ConcatStringTree();
	ans->Root = build_bottom(Root, 0, from, to);
	return *ans;
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
		new_ele->Par->Paroot = new ParNode(max_id, NULL, NULL, 0);
		new_ele->Par->nums_node++;

		return new_ele;
	}

	return combine(deepRe(cur->right), deepRe(cur->left));
}
ConcatStringTree ConcatStringTree::reverse() const {
	ConcatStringTree* ans = new ConcatStringTree();
	ans->Root = deepRe(Root);
	return *ans;
}
//////////////////////PARENTSTREE IMPLEMENTATION/////////////////////////
ParentsTree::ParentsTree() {
	Paroot = NULL;
	nums_node = 0;
}
int ParentsTree::size() const {
	return this->nums_node;
}
int ParentsTree::height(ConcatStringTree::ParNode* cur) {
	if (!cur) return 0;
	return cur->height;
}
int ParentsTree::getBalance(ConcatStringTree::ParNode* cur) {
	if (!cur) return 0;
	return height(cur->left) - height(cur->right);
}
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
ConcatStringTree::ParNode* ParentsTree::MaxNode(ConcatStringTree::ParNode* cur) {
	ConcatStringTree::ParNode* start = cur;
	while (start && start->right) start = start->right;
	return start;
}
ConcatStringTree::ParNode* ParentsTree::insert(ConcatStringTree::ParNode* cur, ConcatStringTree::ParNode* ele) {
	if (!cur)
	{
		nums_node++;
		return ele;
	}

	if (ele->id < cur->id) cur->left = insert(cur->left, ele);
	else if (ele->id > cur->id) cur->right = insert(cur->right, ele);
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
ConcatStringTree::ParNode* ParentsTree::remove(ConcatStringTree::ParNode* cur, int key) {
	if (!cur) return cur;

	if (key < cur->id) cur->left = remove(cur->left, key);
	else if (key > cur->id) cur->right = remove(cur->right, key);
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

			delete(tmp); ////////////// WARNING!!!!!!!!!
			tmp = NULL;
		}
		else
		{
			ConcatStringTree::ParNode* tmp = MaxNode(cur->left);
			cur->id = tmp->id;
			cur->left = remove(cur->left, tmp->id);
		}
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
	string ans = "\"ParentsTree[" + PreOrder(Paroot) + "]\"";
	return ans;
}
//Traverse Sub tree and add Parents 
void ConcatStringTree::Parents_add(Node* cur, ParNode* ele) const {
	if (!ele || !cur) return;

	cur->Par->Paroot = cur->Par->insert(cur->Par->Paroot, ele);

	if(cur->left)
		Parents_add(cur->left, new ParNode(max_id, NULL, NULL, 0));
	if(cur->right)
		Parents_add(cur->right, new ParNode(max_id, NULL, NULL, 0));

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