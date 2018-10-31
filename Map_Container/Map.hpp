#include <iostream>
#include <queue>



using namespace std;

namespace cs540 {
  enum COLOR { RED, BLACK };

  template <typename Key_T, typename Mapped_T>
  class RBTree;

  template <typename Key_T, typename Mapped_T>
  class Map;

  /**
  I have used the red-black tree implementation over here:
  https://www.geeksforgeeks.org/red-black-tree-set-3-delete-2/
  And modified it to support the Map parameters

  TODO Map Implementation is after line number 488!
  **/
  template <typename Key_T, typename Mapped_T>
  class Node {
    private:
    pair<Key_T, Mapped_T> val;
    COLOR color;
    Node *left, *right, *parent;
    friend class RBTree<Key_T, Mapped_T>;
    friend class Map<Key_T, Mapped_T>;


    public:
    Node(const pair<Key_T, Mapped_T> &p) : val(p), left(NULL), right(NULL), parent(NULL), color(RED) { }

    // returns pointer to uncle
    Node *uncle() {
    	// If no parent or grandparent, then no uncle
    	if (parent == NULL or parent->parent == NULL)
    	return NULL;

    	if (parent->isOnLeft())
    	// uncle on right
    	return parent->parent->right;
    	else
    	// uncle on left
    	return parent->parent->left;
    }

    // check if node is left child of parent
    bool isOnLeft() const { return this == parent->left; }

    bool isOnRight() const { return this == parent->right; }

    // returns pointer to sibling
    Node *sibling() {
    	// sibling null if no parent
    	if (parent == NULL)
    	return NULL;

    	if (isOnLeft())
    	return parent->right;

    	return parent->left;
    }

    // moves node down and moves given node in its place
    void moveDown(Node *nParent) {
    	if (parent != NULL) {
    	if (isOnLeft()) {
    		parent->left = nParent;
    	} else {
    		parent->right = nParent;
    	}
    	}
    	nParent->parent = parent;
    	parent = nParent;
    }

    bool hasRedChild() {
    	return (left != NULL and left->color == RED) or
    		(right != NULL and right->color == RED);
    }
  };

  template <typename Key_T, typename Mapped_T>
  class RBTree {
    Node<Key_T, Mapped_T> *root;
    friend class Map<Key_T, Mapped_T>;
    // left rotates the given node
    void leftRotate(Node<Key_T, Mapped_T> *x) {
    	// new parent will be node's right child
    	Node<Key_T, Mapped_T> *nParent = x->right;

    	// update root if current node is root
    	if (x == root)
    	root = nParent;

    	x->moveDown(nParent);

    	// connect x with new parent's left element
    	x->right = nParent->left;
    	// connect new parent's left element with node
    	// if it is not null
    	if (nParent->left != NULL)
    	nParent->left->parent = x;

    	// connect new parent with x
    	nParent->left = x;
    }

    void rightRotate(Node<Key_T, Mapped_T> *x) {
    	// new parent will be node's left child
    	Node<Key_T, Mapped_T> *nParent = x->left;

    	// update root if current node is root
    	if (x == root)
    	root = nParent;

    	x->moveDown(nParent);

    	// connect x with new parent's right element
    	x->left = nParent->right;
    	// connect new parent's right element with node
    	// if it is not null
    	if (nParent->right != NULL)
    	nParent->right->parent = x;

    	// connect new parent with x
    	nParent->right = x;
    }

    void swapColors(Node<Key_T, Mapped_T> *x1, Node<Key_T, Mapped_T> *x2) {
    	COLOR temp;
    	temp = x1->color;
    	x1->color = x2->color;
    	x2->color = temp;
    }

    void swapValues(Node<Key_T, Mapped_T> *u, Node<Key_T, Mapped_T> *v) {
    	// pair<Key_T, Mapped_T> temp;
    	// temp = u->val;
    	// u->val = v->val;
    	// v->val = temp; TODO
    }

    // fix red red at given node
    void fixRedRed(Node<Key_T, Mapped_T> *x) {
    	// if x is root color it black and return
    	if (x == root) {
    	x->color = BLACK;
    	return;
    	}

    	// initialize parent, grandparent, uncle
    	Node<Key_T, Mapped_T> *parent = x->parent, *grandparent = parent->parent,
    		*uncle = x->uncle();

    	if (parent->color != BLACK) {
    	if (uncle != NULL && uncle->color == RED) {
    		// uncle red, perform recoloring and recurse
    		parent->color = BLACK;
    		uncle->color = BLACK;
    		grandparent->color = RED;
    		fixRedRed(grandparent);
    	} else {
    		// Else perform LR, LL, RL, RR
    		if (parent->isOnLeft()) {
    		if (x->isOnLeft()) {
    			// for left right
    			swapColors(parent, grandparent);
    		} else {
    			leftRotate(parent);
    			swapColors(x, grandparent);
    		}
    		// for left left and left right
    		rightRotate(grandparent);
    		} else {
    		if (x->isOnLeft()) {
    			// for right left
    			rightRotate(parent);
    			swapColors(x, grandparent);
    		} else {
    			swapColors(parent, grandparent);
    		}

    		// for right right and right left
    		leftRotate(grandparent);
    		}
    	}
    	}
    }

    // find node that do not have a left child
    // in the subtree of the given node
    Node<Key_T, Mapped_T> *successor(Node<Key_T, Mapped_T> *x) {
    	Node<Key_T, Mapped_T> *temp = x;

    	while (temp->left != NULL)
    	temp = temp->left;

    	return temp;
    }

    // find node that replaces a deleted node in BST
    Node<Key_T, Mapped_T> *BSTreplace(Node<Key_T, Mapped_T> *x) {
    	// when node have 2 children
    	if (x->left != NULL and x->right != NULL)
    	return successor(x->right);

    	// when leaf
    	if (x->left == NULL and x->right == NULL)
    	return NULL;

    	// when single child
    	if (x->left != NULL)
    	return x->left;
    	else
    	return x->right;
    }

    // deletes the given node
    void deleteNode(Node<Key_T, Mapped_T> *v) {
    	Node<Key_T, Mapped_T> *u = BSTreplace(v);

    	// True when u and v are both black
    	bool uvBlack = ((u == NULL or u->color == BLACK) and (v->color == BLACK));
    	Node<Key_T, Mapped_T> *parent = v->parent;

    	if (u == NULL) {
    	// u is NULL therefore v is leaf
    	if (v == root) {
    		// v is root, making root null
    		root = NULL;
    	} else {
    		if (uvBlack) {
    		// u and v both black
    		// v is leaf, fix double black at v
    		fixDoubleBlack(v);
    		} else {
    		// u or v is red
    		if (v->sibling() != NULL)
    			// sibling is not null, make it red"
    			v->sibling()->color = RED;
    		}

    		// delete v from the tree
    		if (v->isOnLeft()) {
    		parent->left = NULL;
    		} else {
    		parent->right = NULL;
    		}
    	}
    	delete v;
    	return;
    	}

    	if (v->left == NULL or v->right == NULL) {
    	// v has 1 child
    	if (v == root) {
    		// v is root, assign the value of u to v, and delete u
    		// v->val = u->val;

    		v->left = v->right = NULL;
    		delete u;
    	} else {
    		// Detach v from tree and move u up
    		if (v->isOnLeft()) {
    		parent->left = u;
    		} else {
    		parent->right = u;
    		}
    		delete v;
    		u->parent = parent;
    		if (uvBlack) {
    		// u and v both black, fix double black at u
    		fixDoubleBlack(u);
    		} else {
    		// u or v red, color u black
    		u->color = BLACK;
    		}
    	}
    	return;
    	}

    	// v has 2 children, swap values with successor and recurse
    	swapValues(u, v);
    	deleteNode(u);
    }

    void fixDoubleBlack(Node<Key_T, Mapped_T> *x) {
    	if (x == root)
    	// Reached root
    	return;

    	Node<Key_T, Mapped_T> *sibling = x->sibling(), *parent = x->parent;
    	if (sibling == NULL) {
    	// No sibiling, double black pushed up
    	fixDoubleBlack(parent);
    	} else {
    	if (sibling->color == RED) {
    		// Sibling red
    		parent->color = RED;
    		sibling->color = BLACK;
    		if (sibling->isOnLeft()) {
    		// left case
    		rightRotate(parent);
    		} else {
    		// right case
    		leftRotate(parent);
    		}
    		fixDoubleBlack(x);
    	} else {
    		// Sibling black
    		if (sibling->hasRedChild()) {
    		// at least 1 red children
    		if (sibling->left != NULL and sibling->left->color == RED) {
    			if (sibling->isOnLeft()) {
    			// left left
    			sibling->left->color = sibling->color;
    			sibling->color = parent->color;
    			rightRotate(parent);
    			} else {
    			// right left
    			sibling->left->color = parent->color;
    			rightRotate(sibling);
    			leftRotate(parent);
    			}
    		} else {
    			if (sibling->isOnLeft()) {
    			// left right
    			sibling->right->color = parent->color;
    			leftRotate(sibling);
    			rightRotate(parent);
    			} else {
    			// right right
    			sibling->right->color = sibling->color;
    			sibling->color = parent->color;
    			leftRotate(parent);
    			}
    		}
    		parent->color = BLACK;
    		} else {
    		// 2 black children
    		sibling->color = RED;
    		if (parent->color == BLACK)
    			fixDoubleBlack(parent);
    		else
    			parent->color = BLACK;
    		}
    	}
    	}
    }

    // prints level order for given node
    void levelOrder(Node<Key_T, Mapped_T> *x) {
    	if (x == NULL)
    	// return if node is null
    	return;

    	// queue for level order
    	queue<Node<Key_T, Mapped_T> *> q;
    	Node<Key_T, Mapped_T> *curr;

    	// push x
    	q.push(x);

    	while (!q.empty()) {
    	// while q is not empty
    	// dequeue
    	curr = q.front();
    	q.pop();

    	// print node value
    	cout << curr->val << " ";

    	// push children to queue
    	if (curr->left != NULL)
    		q.push(curr->left);
    	if (curr->right != NULL)
    		q.push(curr->right);
    	}
    }

    // prints inorder recursively
    void inorder(Node<Key_T, Mapped_T> *x) {
    	if (x == NULL)
    	return;
    	inorder(x->left);
    	cout << (x->val.first) << " ";
    	inorder(x->right);
    }

    public:
    // constructor
    // initialize root
    RBTree() { root = NULL; }

    Node<Key_T, Mapped_T> *getRoot() { return root; }

    // searches for given value
    // if found returns the node (used for delete)
    // else returns the last node while traversing (used in insert)
    Node<Key_T, Mapped_T> *search(const Key_T key) const {
    	Node<Key_T, Mapped_T> *temp = root;
    	while (temp != NULL) {
      	if (key < temp->val.first) {
      		if (temp->left == NULL)
      		  break;
      		else
      		temp = temp->left;
      	} else if (key == temp->val.first) {
      		  break;
      	} else {
      		if (temp->right == NULL)
      		  break;
      		else
      		temp = temp->right;
      	}
    	}

      return temp;
    }

    // inserts the given value to tree
    bool insert(const pair<Key_T, Mapped_T> &n) {
    	Node<Key_T, Mapped_T> *newNode = new Node<Key_T, Mapped_T>(n);
    	if (root == NULL) {
    	// when root is null
    	// simply insert value at root
    	newNode->color = BLACK;
    	root = newNode;
    	} else {
    	Node<Key_T, Mapped_T> *temp = search(n.first);

    	if (temp != NULL && temp->val.first == n.first) {
    		// return if value already exists
    		return 0;
    	}

    	// if value is not found, search returns the node
    	// where the value is to be inserted

    	// connect new node to correct node
    	newNode->parent = temp;

    	if (n.first < temp->val.first)
    		temp->left = newNode;
    	else
    		temp->right = newNode;

    	// fix red red voilaton if exists
    	fixRedRed(newNode);
    	}
      return 1;
    }

    // utility function that deletes the node with given value
    void deleteByVal(Key_T key) {
    	if (root == NULL)
    	// Tree is empty
    	return;

    	Node<Key_T, Mapped_T> *v = search(key), *u;

    	if (v->val.first != key) {
    	cout << "No node found to delete with value:" << key << endl;
    	return;
    	}

    	deleteNode(v);
    }

    // prints inorder of the tree
    void printInOrder() {
    	cout << "Inorder: " << endl;
    	if (root == NULL)
    	cout << "Tree is empty" << endl;
    	else
    	inorder(root);
    	cout << endl;
    }

    // prints level order of the tree
    void printLevelOrder() {
    	cout << "Level order: " << endl;
    	if (root == NULL)
    	cout << "Tree is empty" << endl;
    	else
    	levelOrder(root);
    	cout << endl;
    }
  };

  template<typename Key_T, typename Mapped_T>
  class Map {
    private:
      RBTree<Key_T, Mapped_T> tree;
      size_t length = 0;

    public:
      Map() {} //creates an empty map

      Map(const Map &m) {
        auto it = m.begin();
        while(it != m.end()){
          this->tree.insert(pair<Key_T, Mapped_T>(Key_T(it->first), Mapped_T(it->second)));
          ++it;
        }
        length = m.length;
      }

      Map &operator=(const Map &m) { //TODO assignment
        auto it = m.begin();
        while(it != m.end()){
          this->tree.insert(pair<Key_T, Mapped_T>(Key_T(it->first), Mapped_T(it->second)));
          ++it;
        }
        length = m.length;
      }

      Map(initializer_list<pair<const Key_T, Mapped_T>> list) { //creates from initializer list
        for(auto val : list) {
          tree.insert(val);
          ++length;
        }
      }

      ~Map() { //Destructor

      }

      size_t size() const {
        return length;
      }

      Mapped_T &operator[](const Key_T &key) {
        Node<Key_T, Mapped_T> *node = tree.search(key);
        if(node != NULL && node->val.first == key) return node->val.second;
        else {
          tree.insert(pair<Key_T, Mapped_T>(key, Mapped_T()));
          return at(key);
        }
      }

      void insert(const pair<Key_T, Mapped_T> &val) {
        if(tree.insert(val)) {
          ++length;
        }
      }

      Mapped_T &at(const Key_T &key) const {
        Node<Key_T, Mapped_T> *node = tree.search(key);
        if(node != NULL && node->val.first == key) return node->val.second;
        else throw std::out_of_range("index is out of range");
      }

      // const Mapped_T &at(const Key_T &key) const {
      //   Node<Key_T, Mapped_T> *node = tree.search(key);
      //   if(node != NULL && node->val.first == key) return node->val.second;
      //   else throw std::out_of_range("index is out of range");
      // }

      friend bool operator==(const Map &m1, const Map &m2) {
        if(m1.size() != m2.size()) return false;

        auto it1 = m1.begin();
        auto it2 = m2.begin();

        while(it1 != m1.end()) { //same size, no need to check more bounds
          if(!(it1->first == it2->first && it1->second == it2->second)) return false;
          ++it1;
          ++it2;
        }

        return true;
      }

      friend bool operator!=(const Map &m1, const Map &m2) {
        return !(m1==m2);
      }

      friend bool operator<(const Map &m1, const Map &m2) {
        auto it1 = m1.begin();
        auto it2 = m2.begin();

        while(it1 != m1.end()) { //same size, no need to check more bounds
          if(!(it1->first < it2->first && it1->second < it2->second)) return false;
          ++it1;
          ++it2;
        }

        if(m1.size() < m2.size()) return true;

        return false;
      }

      class BaseIterator {
        protected:
          Node<Key_T, Mapped_T>* current;
          bool end;
          bool start;
          BaseIterator(Node<Key_T, Mapped_T>* root, bool end = 0, bool start = 0) : current(root), end(end), start(start) { } //point to root
          friend class Map;

          void goRight() { //increment in-order
            if(start == 1) {
              start = 0;
              return;
            }
            if(current->right != NULL) {
              current = current->right;
              while(current->left != NULL) {
                current = current->left;
              }
            } else {
              Node<Key_T, Mapped_T>* temp = current;
              while(temp->parent != NULL && temp->isOnRight()) {
                temp = temp->parent;
              }
              if(temp->parent != NULL && temp->isOnLeft())
                current = temp->parent;
              else end = 1;
            }
          }

          void goLeft() { //decrement in-order
            if(end == 1) {
              end = 0;
              return;
            }
            if(current->left != NULL) {
              current = current->left;
              while(current->right != NULL) {
                current = current->right;
              }
            } else {
              Node<Key_T, Mapped_T>* temp = current;
              while(temp->parent != NULL && temp->isOnLeft()) {
                temp = temp->parent;
              }
              if(temp->parent != NULL && temp->isOnRight())
                current = temp->parent;
              else start = 1;
            }
          }

        public:
          friend bool operator==(const BaseIterator &it1, const BaseIterator &it2) {
            cout << endl << "{" << it1.end << "," << it2.end << "} " << "{" << it1.start << "," << it2.start << "}" << endl;
            return it1.end == it2.end && it1.start == it2.start && it1.current == it2.current;
          }

          friend bool operator!=(const BaseIterator &it1, const BaseIterator &it2) {
            return it1.end != it2.end || it1.start != it2.start || it1.current != it2.current;
          }

      };

      class ConstIterator : public BaseIterator {
        protected:
          ConstIterator(Node<Key_T, Mapped_T>* root, bool end = 0, bool start = 0) : BaseIterator(root, end, start) { }
          friend class Map;
        public:
          ConstIterator &operator++() { //prefix
            this->goRight();
            return *this;
          }

          ConstIterator operator++(int) { //postfix
            ConstIterator temp = *(new ConstIterator(*this));
            this->goRight();
            return temp;
          }

          ConstIterator &operator--() { //prefix
            this->goLeft();
            return *this;
          }

          ConstIterator operator--(int) { //postfix
            ConstIterator temp = *(new ConstIterator(*this));
            this->goLeft();
            return temp;
          }

          const pair<Key_T, Mapped_T> &operator*() const {
            return this->current->val;
          }

          const pair<Key_T, Mapped_T> *operator->() const {
            return &(this->current->val);
          }
      };

      class Iterator : public BaseIterator {
        protected:
          Iterator(Node<Key_T, Mapped_T>* root, bool end = 0, bool start = 0) : BaseIterator(root, end, start) { }
          friend class Map;
        public:
          Iterator &operator++() { //prefix
            this->goRight();
            return *this;
          }

          Iterator operator++(int) { //postfix
            Iterator temp = *(new Iterator(*this));
            this->goRight();
            return temp;
          }

          Iterator &operator--() { //prefix
            this->goLeft();
            return *this;
          }

          Iterator operator--(int) { //postfix
            Iterator temp = *(new Iterator(*this));
            this->goLeft();
            return temp;
          }

          pair<Key_T, Mapped_T> &operator*() const {
            return this->current->val;
          }

          pair<Key_T, Mapped_T> *operator->() const {
            return &(this->current->val);
          }
      };

      class ReverseIterator : public BaseIterator {
        protected:
          ReverseIterator(Node<Key_T, Mapped_T>* root, bool end = 0, bool start = 0) : BaseIterator(root, end, start) { }
          friend class Map;
        public:
          ReverseIterator &operator++() { //prefix
            this->goLeft();
            return *this;
          }

          ReverseIterator operator++(int) { //postfix
            ReverseIterator temp = *(new ReverseIterator(*this));
            this->goLeft();
            return temp;
          }

          ReverseIterator &operator--() { //prefix
            this->goRight();
            return *this;
          }

          ReverseIterator operator--(int) { //postfix
            ReverseIterator temp = *(new ReverseIterator(*this));
            this->goRight();
            return temp;
          }

          pair<Key_T, Mapped_T> &operator*() const {
            return this->current->val;
          }

          pair<Key_T, Mapped_T> *operator->() const {
            return &(this->current->val);
          }
      };

      Iterator begin() {
        Node<Key_T, Mapped_T> *curr = tree.root;
        while(curr->left != NULL) curr = curr->left;
        return *(new Iterator(curr));
      }

      ConstIterator begin() const {
        Node<Key_T, Mapped_T> *curr = tree.root;
        while(curr->left != NULL) curr = curr->left;
        return *(new ConstIterator(curr));
      }

      ConstIterator end() const {
        Node<Key_T, Mapped_T> *curr = tree.root;
        while(curr->right != NULL) curr = curr->right;
        return *(new ConstIterator(curr, 1, 0));
      }

      Iterator end() {
        Node<Key_T, Mapped_T> *curr = tree.root;
        while(curr->right != NULL) curr = curr->right;
        return *(new Iterator(curr, 1, 0));
      }

      ReverseIterator rend() const {
        Node<Key_T, Mapped_T> *curr = tree.root;
        while(curr->left != NULL) curr = curr->left;
        return *(new ReverseIterator(curr, 0, 1));
      }

      ReverseIterator rbegin() const {
        Node<Key_T, Mapped_T> *curr = tree.root;
        while(curr->right != NULL) curr = curr->right;
        return *(new ReverseIterator(curr));
      }

      Iterator find(const Key_T &key) {
        Node<Key_T, Mapped_T> *node = tree.search(key);
        if(!(node->val.first == key)) {
          return end();
        } else {
          return *(new Iterator(node));
        }
      }

      ConstIterator find(const Key_T &key) const {
        Node<Key_T, Mapped_T> *node = tree.search(key);
        if(!(node->val.first == key)) {
          return end();
        } else {
          return *(new ConstIterator(node));
        }
      }

      void erase(Iterator pos) {
        tree.deleteNode(pos.current);
      }
      void erase(const Key_T &key) {
        Node<Key_T, Mapped_T> *node = tree.search(key);
        if(!(node->val.first == key)) {
          throw std::out_of_range("index is out of range");
        } else {
          tree.deleteNode(node);
        }
      }



  };
}
