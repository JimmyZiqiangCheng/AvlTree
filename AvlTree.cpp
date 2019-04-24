/*
AvlTree implementation in c++ with search, insert, remove functions

Author: Jimmy Ziqiang Cheng
Date: 24, April, 2019
*/

#include <iostream>
#include <typeinfo>
#include <algorithm>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// node struct which stores the value, left child, right child, parent, as well as the balance factor
struct Node{
	// constructors
	Node(int value): val(value), left(nullptr), right(nullptr), parent(nullptr), balance(0){}
	Node(int value, Node *parentNode): val(value), left(nullptr), right(nullptr), parent(parentNode), balance(0){}
	// attributes
	int val;
	Node *left;
	Node *right;
	Node *parent;
	int balance;
};

class AvlTree{
public:
	// constructor & destructor
	AvlTree();
	~AvlTree();
	// interfaces for user
	void destroyTree();
	void insert(int value);
	void remove(int value);
	void inorderTraversal();
	void postorderTraversal();
	void preorderTraversal();
	void search(int value);

private:
	// metheds implementation
	void destroyTree(Node *node);
	void insert(int value, Node *node);
	void remove(int value, Node *node);
	Node *search(int value, Node *node);
	void inorderTraversal(Node *node);
	void postorderTraversal(Node *node);
	void preorderTraversal(Node *node);
	int  findRightMost(Node *node);
	int getHeight(Node *node);
	Node *updateBalance(Node *node);
	Node *rebalance(Node *node);
	void traverseUp(Node *node);
	Node *rotateLeft(Node *node);
	Node *rotateRight(Node *node);
	Node *rotateRightLeft(Node *node);
	Node *rotateLeftRight(Node *node);
	// attribute
	Node *root;
};
// constructors and destructors	
AvlTree::AvlTree(): root(nullptr){}
AvlTree::~AvlTree(){
	destroyTree();
}

// method for deleting the whole tree
void AvlTree::destroyTree(){
	destroyTree(root);
}
void AvlTree::destroyTree(Node *node){
	if(node != nullptr){
		destroyTree(node->left);
		destroyTree(node->right);
		delete node;
	}
}

// method for getting the height of a given node
int AvlTree::getHeight(Node *node){
	if (node == nullptr){
		return -1;
	}else{
		return 1+ max(getHeight(node -> left), getHeight(node -> right));
	}
}

// method for updating the balance of a given node
Node *AvlTree::updateBalance(Node* node){
	int leftHeight = 0;
	int rightHeight = 0;
	if (node -> left != nullptr){
		leftHeight = 1+getHeight(node -> left);
	}
	if (node -> right != nullptr){
		rightHeight = 1+getHeight(node -> right);
	}
	node -> balance = rightHeight - leftHeight;
	return node;
}

// when the right branch of the right child is causing imbalance
Node *AvlTree::rotateLeft(Node* node){
	//cout << "come here" << endl;
	Node *x = node -> right;
	node -> right = x -> left;
	if (x -> left != nullptr){
		x -> left -> parent = node;
	}
	x -> left = node;
	x -> parent = node -> parent;
	if (node -> parent == nullptr){
		root = x;
	}else{
		if (node -> parent -> left != nullptr && node -> parent -> left -> val == node -> val){
			node -> parent -> left = x;
		}else{
			node -> parent -> right = x;
		}
	}
	node -> parent = x;
	traverseUp(node);
	return x;
}

// when the left branch of the left child is causing imbalance(mirror image of above)
Node *AvlTree::rotateRight(Node* node){
	//cout << "come here" << endl;
	Node *x = node -> left;
	node -> left = x -> right;
	if (x -> right != nullptr){
		x -> right -> parent = node;
	}
	x -> right = node;
	x -> parent = node -> parent;
	if (node -> parent == nullptr){
		root = x;
	}else{
		if (node -> parent -> left != nullptr && node -> parent -> left -> val == node -> val){
			node -> parent -> left = x;			
		}else{
			node -> parent -> right = x;
		}
	}
	node -> parent = x;
	traverseUp(node);
	return x;
}

// when the left branch of the right child is causing imbalance
Node *AvlTree::rotateRightLeft(Node* node){
	//cout << "come here" << endl;
	Node *x = node -> right;
	Node *w = x -> left;
	node -> right = w -> left;
	if (w->left != nullptr){
		w -> left -> parent = node;
	}
	x -> left = w -> right;
	if (w-> right != nullptr){
		w -> right -> parent = x;
	}
	w -> left = node;
	w -> parent = node -> parent;
	if (node -> parent == nullptr){
		root = w;
	}else{
		if (node -> parent -> left != nullptr && node -> parent -> left -> val == node -> val){
			node -> parent -> left = w;
		}else{
			node -> parent -> right = w;
		}
	}
	node -> parent = w;
	w -> right = x;
	x -> parent = w;
	traverseUp(x);
	traverseUp(node);
	return w;
}

// when the right branch of the left child is causing imbalance (mirror image of above)
Node *AvlTree::rotateLeftRight(Node* node){
	//cout << "come here" << endl;
	Node *x = node -> left;
	Node *w = x -> right;
	node -> left = w -> right;
	if (w->right != nullptr){
		w -> right -> parent = node;
	}
	x -> right = w -> left;
	if (w->left != nullptr){
		w -> left -> parent = x;
	}
	w -> right = node;
	w -> parent = node -> parent;
	if (node -> parent == nullptr){
		root = w;
	}else{
		if (node -> parent -> left != nullptr && node -> parent -> left -> val == node -> val){
			node -> parent -> left = w;
		}else{
			node -> parent -> right = w;
		}
	}
	node -> parent = w;
	w -> left = x;
	x -> parent = w;
	traverseUp(x);
	traverseUp(node);
	return w;
}

// method for rebalancing the tree, check which rotation method to take
Node *AvlTree::rebalance(Node *node){
	if (node -> balance == 2){
		// right right imbalance, single left rotation
		if (node -> right -> balance == 1 || node -> right -> balance == 0){
			node = rotateLeft(node);
		}
		// right left imbalance, double rotation
		else{
			node = rotateRightLeft(node);
		}
	}
	// mirror image of the above
	if (node -> balance == -2){
		if (node -> left -> balance == -1 || node -> left -> balance == 0){
			node = rotateRight(node);
		}else{
			node = rotateLeftRight(node);
		}
	}

	return node;
}

// a recursive method to traverse upwards to keep updating/rebalancing the nodes
void AvlTree::traverseUp(Node *node){
	if (node != nullptr){
		node = updateBalance(node);
		if (node -> balance == 2 || node -> balance == -2){
			node = rebalance(node);
		}
		if (node -> parent != nullptr){
			traverseUp(node->parent);
		}
	}
}

// method for searching a value in the tree
void AvlTree::search(int value){
	if (root != nullptr){
		search(value, root);
	}
}
Node* AvlTree::search(int value, Node *node){

	if(value < node->val){
		if(node->left != nullptr){
			return search(value, node->left);
		}else{
			return nullptr;
		}
	}else if(value > node->val){
		if(node->right != nullptr){
			return search(value, node->right);
		}else{
			return nullptr;
		}
	}else{
		return node;
	}
}

// method for inserting a value into the tree, updating balance and rebalance after insertion
void AvlTree::insert(int value){
	if (root != nullptr){
		insert(value, root);
	}else{
		root = new Node(value);
	}
}
void AvlTree::insert(int value, Node *node){

	if(value < node->val){
		if(node->left != nullptr){
			insert(value, node->left);
		}else{
			node -> left = new Node(value, node);
			traverseUp(node);
		}
	}else if(value > node->val){
		if(node->right != nullptr){
			insert(value, node->right);
		}else{
			node -> right = new Node(value, node);
			traverseUp(node);
		}
	}
}

// method for removing a value from the tree, updating balance and rebalance after deletion
void AvlTree::remove(int value){
	if (root != nullptr){
		remove(value, root);
	}
}
void AvlTree::remove(int value, Node *node){
	if(node != nullptr){
		if(value < node->val){ 
	        remove(value, node->left);
	    }else if (value > node->val){
	        remove(value, node->right);
	    }else{
	    	Node *temp = node -> parent;
	        //No child
	        if(node->right == nullptr && node->left == nullptr){
	        	if (temp != nullptr){
		            if (temp -> left != nullptr && temp -> left -> val == node -> val){
		            	temp -> left = nullptr;
		            } else{
		            	temp -> right = nullptr;
		            }
		            traverseUp(temp);
		        }else{
		        	root = temp;
		        }
	        }
	        //One child 
	        else if(node->right == nullptr){
	        	node -> left -> parent = temp;
	        	if (temp != nullptr){
		        	if (temp -> left != nullptr && temp -> left -> val == node -> val){
		        		temp -> left = node -> left;
		        	}else{
		        		temp -> right = node -> left;
		        	}
		        	traverseUp(temp);
		        }
	        }else if(node->left == nullptr){
	        	node -> right -> parent = temp;
	        	if (temp != nullptr){
		        	if (temp -> left != nullptr && temp -> left -> val == node -> val){
		        		temp -> left = node -> right;
		        	}else{
		        		temp -> right = node -> right;
		        	}
		        	traverseUp(temp);
		        }
	        }
	        //Two children
	        else{
	            int rightMostValue = findRightMost(node->left);
	            node->val = rightMostValue;
	            remove(rightMostValue, node->left);
	        }
	    }
	}
}

// method to find the right most (largest) value in the branch
int AvlTree::findRightMost(Node *node)
{
    if (node -> right != nullptr){
    	return findRightMost(node -> right);
    }else{
    	return node -> val;
    }
}

// mothed for printing out values of the tree inorder
void AvlTree::inorderTraversal(){
	if (root != nullptr){
		inorderTraversal(root);
		cout<<"\n";
	}else{
		cout<< "EMPTY" << endl;
	}
}
void AvlTree::inorderTraversal(Node *node){
	if (node->left != nullptr){
		inorderTraversal(node->left);
	}
	cout << node->val <<" ";
	if (node->right != nullptr){
		inorderTraversal(node->right);
	}
}

// mothed for printing out values of the tree in postorder
void AvlTree::postorderTraversal(){
	if (root != nullptr){
		postorderTraversal(root);
		cout<<"\n";
	}else{
		cout<< "EMPTY" << endl;
	}
}
void AvlTree::postorderTraversal(Node *node){
	if (node->left != nullptr){
		postorderTraversal(node->left);
	}
	if (node->right != nullptr){
		postorderTraversal(node->right);
	}
	cout << node->val <<" ";
}

// mothed for printing out values of the tree in preorder
void AvlTree::preorderTraversal(){
	if (root != nullptr){
		preorderTraversal(root);
		cout<<"\n";
	}else{
		cout<< "EMPTY" << endl;
	}
}
void AvlTree::preorderTraversal(Node *node){
	cout << node->val <<" ";
	if (node->left != nullptr){
		preorderTraversal(node->left);
	}
	if (node->right != nullptr){
		preorderTraversal(node->right);
	}
}