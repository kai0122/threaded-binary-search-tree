#include<iostream>
using namespace std;

class node{
public:
	node();
	node(int value);
	~node();
	friend void gothrough(node *p);

private:
	node *left, *right; // the left child and the right child pointer
	int number; // record the node's number
	int is_threadl, is_threadr; //the flag to record whether the pointer is a thread or not

	friend class op_tree_totum;//you could access all valuables in class op_tree_totum
};

//ctor
node::node(){
	left = right = NULL;
	is_threadl = 1;
	is_threadr = 1;
}

//ctor
node::node(int value){
	number = value;
	left = right = NULL;
	is_threadl = 1;
	is_threadr = 1;
}

//dtor
node::~node(){

}

class op_tree_totum{

public:
	op_tree_totum();
	~op_tree_totum();
	void insertion(int s);
	void deletion(int s);
	void inorder_run();
	void reverseorder_run();
	int size();

private:
	node *root, *head, *tail;
	int num;//caculate how many nodes in the totum
};

//ctor
op_tree_totum::op_tree_totum(){
	head = new node();
	tail = new node();
	head->right = tail; //initialize the head node to connect to the tail node
	tail->left = head;
	head->left = tail->right = NULL;
	root = NULL;
	num = 0;
}

//dtor
op_tree_totum::~op_tree_totum(){
	node *p = root;
	if(p!=NULL)	gothrough(p);
	num = 0;
	if (head != NULL)delete head;
	if (tail != NULL)delete tail;
}

void gothrough(node *p){
	if (p->is_threadl==0 && p->left!= NULL) gothrough(p->left);
	if (p->is_threadr==0 && p->right!= NULL) gothrough(p->right);
	delete p;
}

void op_tree_totum::insertion(int s){
	//TODO: fill in the code to do the insertion of the node with number s
	num++;
	if(num==1){
		root = new node(s);
		root->left = NULL;
		root->is_threadl = 1;
		root->right = NULL;
		root->is_threadr = 1;
		head->right = root;
		tail->left = root;
		return;
	}
	else{
		node *now=root;
		while(1){
			if(s<now->number){ // go left
				if(now->is_threadl==0&&now->left!=NULL)
					now=now->left;
				else{
					node *new_node=new node(s);
					new_node->left=now->left;
					new_node->is_threadl=now->is_threadl;
					new_node->right=now;
					new_node->is_threadr=1;
					now->left=new_node;
					now->is_threadl=0;
					break;
				} 
			}
			else{ // go right
				if(now->is_threadr==0&&now->right!=NULL)
					now=now->right;
				else{
					node *new_node=new node(s);
					new_node->right=now->right;
					new_node->is_threadr=now->is_threadr;
					new_node->left=now;
					new_node->is_threadl=1;
					now->right=new_node;
					now->is_threadr=0;
					break;
				}
			}
		}
		
		// update head
		now=root;
		while(now->left!=NULL)
			now=now->left;
		head->right=now;
		
		// update tail
		now=root;
		while(now->right!=NULL)
			now=now->right;
		tail->left=now;
	}
}

void op_tree_totum::deletion(int s){
	node *now=root,*pre;
	while(now!=0&&now->number!=s){
		pre=now;
		if(s<now->number&&now->is_threadl==0)
			now=now->left;
		else if(s>now->number&&now->is_threadr==0)
			now=now->right;
		else
			break;
		
	}
	
	if(now->number==s){
		num--;
		if(now==root){
			if(root->is_threadl==1&&root->is_threadr==1){ // no child
				root=0;
			}
			else if(root->is_threadl==0&&root->is_threadr==1){ // left child
				node *temp=root->left;
				while(temp->is_threadr==0)
					temp=temp->right;
				deletion(temp->number);
				root->number=temp->number;
			}
			else if(root->is_threadl==1&&root->is_threadr==0){ // right child
				node *temp=root->right;
				while(temp->is_threadl==0)
					temp=temp->left;
				deletion(temp->number);
				root->number=temp->number;
			}
			else{ // two children
				if(root->left->is_threadr==1){
					root->left->right=root->right;
					root->left->is_threadr=root->is_threadr;
					node *temp=root->right;
					while(temp->is_threadl==0)
						temp=temp->left;
					temp->left=root->left;
					temp->is_threadl=1;
					root=root->left;
				}
				else if(root->left->is_threadr!=1){ // find the biggest element in root->small tree and put it to root's place
					node *temp=root->left,*pre_temp;
					while(temp->is_threadr==0){
						pre_temp=temp;
						temp=temp->right;
					}
					deletion(temp->number);
					now->number=temp->number;
				}
			}
		}	
		else{
			if(now->is_threadl==1&&now->is_threadr==1){ // no child
				if(s<pre->number){
					pre->left=now->left;
					pre->is_threadl=now->is_threadl;
				}
				else{
					pre->right=now->right;
					pre->is_threadr=now->is_threadr;
				}
			}
			else if(now->is_threadl==0&&now->is_threadr==1){ // left child
				if(s<pre->number){ 
					pre->left=now->left;
					pre->is_threadl=now->is_threadl;
					node *temp=now->left;
					while(temp->is_threadr==0) // find big
						temp=temp->right;
					temp->right=now->right;
					temp->is_threadr=now->is_threadr;
				}
				else{
					pre->right=now->left;
					pre->is_threadr=now->is_threadl;
					node *temp=now->left;
					while(temp->is_threadr==0) // find big
						temp=temp->right;
					temp->right=now->right;
					temp->is_threadr=now->is_threadr;
				}
			}
			else if(now->is_threadl==1&&now->is_threadr==0){ // right child
				if(s<pre->number){ // delete small
					pre->left=now->right;
					pre->is_threadl=now->is_threadr;
					node *temp=now->right;
					while(temp->is_threadl==0) // find small
						temp=temp->left;
					temp->left=now->left;
					temp->is_threadl=now->is_threadl;
				}
				else{ // delete big
					pre->right=now->right;
					pre->is_threadr=now->is_threadr;
					node *temp=now->right;
					while(temp->is_threadl==0) // find small
						temp=temp->left;
					temp->left=now->left;
					temp->is_threadl=now->is_threadl;
				}
			}
			else{ // two children
				if(s<pre->number){ // delete small
					if(now->left->is_threadr==1){
						pre->left=now->left;
						now->left->right=now->right;
						now->left->is_threadr=now->is_threadr;
						node *temp=now->right;
						while(temp->is_threadl==0)
							temp=temp->left;
						temp->left=now->left;
						temp->is_threadl=1;
					}
					else if(now->left->is_threadr!=1){ // find the biggest element in now->small tree and put it to now's place
						node *temp=now->left,*pre_temp;
						while(temp->is_threadr==0){
							pre_temp=temp;
							temp=temp->right;
						}
						deletion(temp->number);
						now->number=temp->number;
					}
				}
				else{ // delete big
					if(now->right->is_threadl==1){
						pre->right=now->right;
						now->right->left=now->left;
						now->right->is_threadl=now->is_threadl;
						node *temp=now->left;
						while(temp->is_threadr==0)
							temp=temp->right;
						temp->right=now->right;
						temp->is_threadr=1;
					}
					else if(now->right->is_threadl!=1){ // find the smallest element in now->big tree and put it to now's place
						node *temp=now->right,*pre_temp;
						while(temp->is_threadl==0){
							pre_temp=temp;
							temp=temp->left;
						}
						// 已找到最小 
						deletion(temp->number);
						now->number=temp->number;
					}
				}
			}
		}
	}
	
	// update head
	now=root;
	while(now->left!=NULL)
		now=now->left;
	head->right=now;
	
	// update tail
	now=root;
	while(now->right!=NULL)
		now=now->right;
	tail->left=now;
}

void op_tree_totum::inorder_run(){
	//TODO: travel the tree from the head node to the tail node and output the values
	//You should NOT use stack or recurtion 
	node *now=head->right;
	while(now!=NULL){
		cout << now->number << " ";
		if(now->is_threadr==1||now->right==0)
			now=now->right;
		else{
			now=now->right;
			while(now->is_threadl!=1)
				now=now->left;
		}
	}
	cout << endl;
}

void op_tree_totum::reverseorder_run(){
	//TODO: travel the tree from the tail node to the head node and output the values
	//You should NOT use stack or recurtion 
	node *now=tail->left;
	while(now!=NULL){
		cout << now->number << " ";
		if(now->is_threadl==1||now->left==0)
			now=now->left;
		else{
			now=now->left;
			while(now->is_threadr!=1)
				now=now->right;
		}
	}
	cout << endl;
}

int op_tree_totum::size(){
	return num;
}

