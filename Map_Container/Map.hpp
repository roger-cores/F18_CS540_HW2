/*
 * Implement your functionality here without changing given test cases
 */

#include <iostream>

using namespace std;

namespace cs540 {

  enum Color {RED, BLACK};

  template <typename Key_T, typename Mapped_T>
  class RBT;

  template <typename Key_T, typename Mapped_T>
  class Node {
    private:
      pair<Key_T, Mapped_T> value;
      bool color;
      Node *l, *r, *p;
      Node() = delete;
      friend class RBT<Key_T, Mapped_T>;
    public:
      Node(const pair<Key_T, Mapped_T> &p) : value(p), l(NULL), r(NULL), p(NULL) { }

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
      void recurInorder(Node<Key_T, Mapped_T> *);
      pair<Key_T, Mapped_T>* recurLookup(const Key_T, Node<Key_T, Mapped_T> *);
    public:
      RBT() : root(NULL) { }
      void insert(const pair<Key_T, Mapped_T> &v);
      void inorder();
      pair<Key_T, Mapped_T>* lookup(const Key_T);
  };

  template <typename Key_T, typename Mapped_T>
  void RBT<Key_T, Mapped_T>::insert(const pair<Key_T, Mapped_T> &v) {
    if(lookup(v.first)!=0) return;
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

  template <typename Key_T, typename Mapped_T>
  void RBT<Key_T, Mapped_T>::fixViolation(Node<Key_T, Mapped_T> *&rt, Node<Key_T, Mapped_T> *&pt) {

    Node<Key_T, Mapped_T> *parent_pt = NULL;
    Node<Key_T, Mapped_T> *grand_parent_pt = NULL;

    while ((pt != rt) && (pt->color != BLACK) &&
           (pt->p->color == RED))
    {
        parent_pt = pt->p;
        grand_parent_pt = pt->p->p;

        /*  Case : A
            Parent of pt is left child of Grand-parent of pt */
        if (parent_pt == grand_parent_pt->l)
        {

            Node<Key_T, Mapped_T> *uncle_pt = grand_parent_pt->r;

            /* Case : 1
               The uncle of pt is also red
               Only Recoloring required */
            if (uncle_pt != NULL && uncle_pt->color == RED)
            {
                grand_parent_pt->color = RED;
                parent_pt->color = BLACK;
                uncle_pt->color = BLACK;
                pt = grand_parent_pt;
            }

            else
            {
                /* Case : 2
                   pt is right child of its parent
                   Left-rotation required */
                if (pt == parent_pt->r)
                {
                    rotateLeft(rt, parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->p;
                }

                /* Case : 3
                   pt is left child of its parent
                   Right-rotation required */
                rotateRight(rt, grand_parent_pt);
                swap(parent_pt->color, grand_parent_pt->color);
                pt = parent_pt;
            }
        }

        /* Case : B
           Parent of pt is right child of Grand-parent of pt */
        else
        {
            Node<Key_T, Mapped_T> *uncle_pt = grand_parent_pt->l;

            /*  Case : 1
                The uncle of pt is also red
                Only Recoloring required */
            if ((uncle_pt != NULL) && (uncle_pt->color == RED))
            {
                grand_parent_pt->color = RED;
                parent_pt->color = BLACK;
                uncle_pt->color = BLACK;
                pt = grand_parent_pt;
            }
            else
            {
                /* Case : 2
                   pt is left child of its parent
                   Right-rotation required */
                if (pt == parent_pt->l)
                {
                    rotateRight(rt, parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->p;
                }

                /* Case : 3
                   pt is right child of its parent
                   Left-rotation required */
                rotateLeft(rt, grand_parent_pt);
                swap(parent_pt->color, grand_parent_pt->color);
                pt = parent_pt;
            }
        }
    }

    rt->color = BLACK;
  }

  template <typename Key_T, typename Mapped_T>
  void RBT<Key_T, Mapped_T>::rotateLeft(Node<Key_T, Mapped_T> *&rt, Node<Key_T, Mapped_T> *&pt) {
    Node<Key_T, Mapped_T> *pt_r = pt->r;
    pt->r = pt_r->l;
    if (pt->r != NULL)
        pt->r->p = pt;
    pt_r->p = pt->p;
    if (pt->p == NULL)
        rt = pt_r;
    else if (pt == pt->p->l)
        pt->p->l = pt_r;
    else
        pt->p->r = pt_r;
    pt_r->l = pt;
    pt->p = pt_r;
  }

  template <typename Key_T, typename Mapped_T>
  void RBT<Key_T, Mapped_T>::rotateRight(Node<Key_T, Mapped_T> *&rt, Node<Key_T, Mapped_T> *&pt) {
    Node<Key_T, Mapped_T> *pt_l = pt->l;
    pt->l = pt_l->r;
    if (pt->l != NULL)
        pt->l->p = pt;
    pt_l->p = pt->p;
    if (pt->p == NULL)
        rt = pt_l;
    else if (pt == pt->p->l)
        pt->p->l = pt_l;
    else
        pt->p->r = pt_l;
    pt_l->r = pt;
    pt->p = pt_l;
  }

  template <typename Key_T, typename Mapped_T>
  void RBT<Key_T, Mapped_T>::recurInorder(Node<Key_T, Mapped_T> *rt) {
    if (rt == NULL)
        return;
    recurInorder(rt->l);
    cout << "{" << (rt->value.first) << "," << (rt->value.second) << "}, ";
    recurInorder(rt->r);
  }

  template <typename Key_T, typename Mapped_T>
  pair<Key_T, Mapped_T>* RBT<Key_T, Mapped_T>::recurLookup(const Key_T key, Node<Key_T, Mapped_T> *rt) {
    if(rt->value.first == key) {
      return &(rt->value);
    } else if(key < rt->value.first) {
      if(rt->l != NULL)
        return recurLookup(key, rt->l);
    } else {
      if(rt->r != NULL)
        return recurLookup(key, rt->r);
    }

    return 0;
  }

  template <typename Key_T, typename Mapped_T>
  pair<Key_T, Mapped_T>* RBT<Key_T, Mapped_T>::lookup(const Key_T key) {
    if(root == NULL) return 0;
    return recurLookup(key, root);
  }

  template <typename Key_T, typename Mapped_T>
  void RBT<Key_T, Mapped_T>::inorder() {
    recurInorder(root);
  }
}
