/*
 * Implement your functionality here without changing given test cases
 */

#include <iostream>

using namespace std;

namespace cs540 {

  enum Color {RED, BLACK};

  template <typename Key_T, typename Mapped_T>
  class Node {
    private:
      std::pair<Key_T, Mapped_T> value;
      bool color;
      Node *l, *r, *p;
      Node() = delete;
    public:
      Node(const std::pair<Key_T, Mapped_T> &p) : value(p), l(NULL), r(NULL), p(NULL) { }

  };

  template <typename Key_T, typename Mapped_T>
  class RBT {
    private:
      Node<Key_T, Mapped_T> *root;
    protected:
      void rotateLeft(Node<Key_T, Mapped_T> *&, Node<Key_T, Mapped_T> *&);
      void rotateRight(Node<Key_T, Mapped_T> *&, Node<Key_T, Mapped_T> *&);
      void fixViolation(Node<Key_T, Mapped_T> *&, Node<Key_T, Mapped_T> *&);
      Node<Key_T, Mapped_T>* recurInsert(Node<Key_T, Mapped_T> *, Node<Key_T, Mapped_T> *);
    public:
      RBT() : root(NULL) { }
      void insert(const std::pair<Key_T, Mapped_T> &v);
  };

  template <typename Key_T, typename Mapped_T>
  void RBT<Key_T, Mapped_T>::insert(const std::pair<Key_T, Mapped_T> &v) {
    Node<Key_T, Mapped_T> *pt = new Node<Key_T, Mapped_T>(v);
    root = recurInsert(root, pt);
    fixViolation(root, pt);
  }

  template <typename Key_T, typename Mapped_T>
  Node<Key_T, Mapped_T>* RBT<Key_T, Mapped_T>::recurInsert(Node<Key_T, Mapped_T> *rt, Node<Key_T, Mapped_T> *pt) {
    if(rt == NULL) return pt;
    if(pt->value.first < rt->value.first) {
      rt->l = recurInsert(rt->l, pt);
      rt->l->p = rt;
    } else if(!(pt->value.first == rt->value.first)) { //key is greater that that of rt
      rt->r = recurInsert(rt->r, pt);
      rt->r->p = rt;
    }

    return rt;
  }

  void RBT<Key_T, Mapped_T>::fixViolation(Node<Key_T, Mapped_T> *&rt, Node<Key_T, Mapped_T> *&pt) {
    Node *parent_pt = NULL;
    Node *grand_parent_pt = NULL;


  }

  void RBT<Key_T, Mapped_T>::rotateLeft(Node<Key_T, Mapped_T> *&rt, Node<Key_T, Mapped_T> *&pt) {

  }

  void RBT<Key_T, Mapped_T>::rotateRight(Node<Key_T, Mapped_T> *&rt, Node<Key_T, Mapped_T> *&pt) {

  }
}
