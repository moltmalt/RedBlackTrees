#include "node.h"
#include <iostream>
#include <cstdlib>
using namespace std;
class BSTree {
    node* root;
    int size;

private:

    void removeViolations(node* sibling){

        if(sibling == root) return;

        if(sibling){
            cout<<sibling->element<<endl;
            if(!sibling->is_red && (sibling->left || sibling->right) ) {

                bool okay = false;
                if (sibling->left) {
                    if (sibling->left->is_red) {
                        okay = true;
                    }
                } else if (sibling->right) {
                    if (sibling->right->is_red) {
                        okay = true;
                    }
                }
                if (okay) {
                    cout<<"REMOVAL CASE 1"<<endl;
                    node *x = (sibling->left != NULL) ? sibling->left : sibling->right;
                    node *y = sibling;
                    node *z = sibling->parent;
                    bool color = z->is_red;
                    node *res = restructure(x);
                    cout<<res->element<<endl;
                    res->is_red = color;
                    res->left->is_red = false;
                    res->right->is_red = false;
                }
            }else if(!sibling->is_red){
                bool okay2 = false;
                if(sibling->left){
                    if(!sibling->left->is_red){
                        okay2 = true;
                    }
                }else if(sibling->right && okay2){
                    if(!sibling->right->is_red){
                        okay2 = true;
                    }else{
                        okay2 = false;
                    }
                }else if(!sibling->left && !sibling->right){
                    okay2 = true;
                }
                if(okay2){
                    cout<<"REMOVAL CASE 2"<<endl;
                    sibling->is_red = true;
                    if(sibling->parent->is_red){
                        sibling->parent->is_red = false;
                    }else{
                        removeViolations(sibling->parent);
                    }
                }

            }else if(sibling->is_red){
                cout<<"IDK"<<endl;
            }
        }else{
            return;
        }
    }
    void violationCheck(node* n){
        while(n != root && n->parent->is_red){

            if(getSibling(n->parent) && getSibling(n->parent)->is_red){
                node* uncle = getSibling(n->parent);

                if(uncle != NULL && uncle->is_red){
                    cout << "INSERTION Violation: Case 2"<<endl;
                    n->parent->is_red = false;
                    uncle->is_red = false;
                    if(n->parent->parent != root){
                        n->parent->parent->is_red = true;
                    }
                    violationCheck(n->parent->parent);
                }
            }else{
                cout << "INSERTION Violation: Case 1"<<endl;
                node* b = restructure(n);

                //cout<<b->element<<endl;
                b->is_red = false;

                b->left->is_red = true;
                b->right->is_red = true;

                break;
            }
        }
    }

    node* getSibling(node* n){
        if(!n->parent) return NULL;
        node* par = n->parent;
        if(par->left == n){
            return par->right;
        }

        return par->left;
    }

    node* restructure(node* child){
        node* par = child->parent;
        node* gp = par->parent;
        node* back = NULL;

        bool gtop_right = gp->right == par;
        bool ptoc_right = par->right == child;

        if(gtop_right && ptoc_right){
            cout<<"ZIGLEFT"<<endl;
            back = zigleft(par);
        }else if(gtop_right && !ptoc_right){
            cout<<"ZIGZAGLEFT"<<endl;
            zigright(child);
            back = zigleft(child);
        }else if(!gtop_right && !ptoc_right){
            cout<<"ZIGRIGHT"<<endl;
            back = zigright(par);
        }else{
            cout<<"ZIGZAGRIGHT"<<endl;
            zigleft(child);
            back = zigright(child);
        }

        return back;
    }

    node* zigleft(node* curr){
        node* par = curr->parent;
        node* gp = par->parent;
        node* shared = curr->left;
        curr->left = par;
        par->parent = curr;
        par->right = shared;

        if(shared){
            shared->parent = par;
        }

        if(par == root){
            root = curr;
            curr->parent = NULL;
        }else{
            curr->parent = gp;
            if(gp->left == par){
                gp->left = curr;
            }else{
                gp->right = curr;
            }
        }
        return curr;
    }

    node* zigright(node* curr){
        node* par = curr->parent;
        node* gp = par->parent;
        node* shared = curr->right;
        curr->right = par;
        par->parent = curr;
        par->left = shared;

        if(shared){
            shared->parent = par;
        }

        if(par == root){
            root = curr;
            curr->parent = NULL;
        }else{
            curr->parent = gp;
            if(gp->left == par){
                gp->left = curr;
            }else{
                gp->right = curr;
            }
        }

        return curr;
    }




public:
    BSTree() {
        root = NULL;
        size = 0;
    }

    bool insert(int num) {
        node* newest = new node;
        newest->parent = NULL;
        newest->right = NULL;
        newest->left = NULL;
        newest->element = num;
        newest->is_red = true;

        if(!root){
            root = newest;
            newest->is_red = false;
            size++;
            return true;
        }else{
            node* curr = root;
            node* par = NULL;

            while(curr != NULL){
                par = curr;
                if(num < curr->element){
                    curr = curr->left;
                }else if(num > curr->element){
                    curr = curr->right;
                }
            }

            newest->parent = par;
            if(num < par->element){
                par->left = newest;
            }else{
                par->right = newest;
            }
            size++;
        }

        violationCheck(newest);

        return true;
    }

    node* search(int num) {
        node* curr = root;
        while (curr != NULL) {
            if (num == curr->element) {
                return curr;
            } else if (num < curr->element) {
                curr = curr->left;
            } else {
                curr = curr->right;
            }
        }
        return NULL;
    }

    void remove(int num) {
        node* nodeToDelete = search(num);
        node* sibling = NULL;
        if (nodeToDelete == NULL){
            cout<<"HUH"<<endl;
            return;
        }  // Node not found

        // Case 1: Node to delete is a leaf
        if (nodeToDelete->left == NULL && nodeToDelete->right == NULL) {
            cout<<"CASE 1"<<endl;
            if (nodeToDelete->parent == NULL) {
                root = NULL; // The tree had only one node
            } else {
                sibling = getSibling(nodeToDelete);
                if (nodeToDelete == nodeToDelete->parent->left) {
                    nodeToDelete->parent->left = NULL;
                } else {
                    nodeToDelete->parent->right = NULL;
                }
            }
            free (nodeToDelete);
        }
            // Case 2: Node to delete has one child
        else if (nodeToDelete->left == NULL || nodeToDelete->right == NULL) {
            cout<<"CASE 2"<<endl;
            node* child = (nodeToDelete->left != NULL) ? nodeToDelete->left : nodeToDelete->right;
            if (nodeToDelete->parent == NULL) {
                root = child;
            } else {
                sibling = getSibling(nodeToDelete);
                if (nodeToDelete == nodeToDelete->parent->left) {
                    nodeToDelete->parent->left = child;
                } else {
                    nodeToDelete->parent->right = child;
                }
            }
            child->parent = nodeToDelete->parent;
            delete nodeToDelete;
        }
            // Case 3: Node to delete has two children
        else {
            node* successor = findMin(nodeToDelete->right);
            sibling = getSibling(successor);
            nodeToDelete->element = successor->element;
            deleteNodeWithSingleChild(successor);
        }
        size--;

        removeViolations(sibling);
    }


    void deleteNodeWithSingleChild(node* nodeToDelete) {
        node* child = (nodeToDelete->left != NULL) ? nodeToDelete->left : nodeToDelete->right;
        if (nodeToDelete->parent == NULL) {
            root = child;
        } else {
            if (nodeToDelete == nodeToDelete->parent->left) {
                nodeToDelete->parent->left = child;
            } else {
                nodeToDelete->parent->right = child;
            }
        }
        if (child != NULL) {
            child->parent = nodeToDelete->parent;
        }
        delete nodeToDelete;
    }



    node* findMin(node* curr) {
        while (curr->left != NULL) {
            curr = curr->left;
        }
        return curr;
    }



    // WARNING. Do not modify these methods below.
    // Doing so will nullify your score for this activity.
    void print() {
        if (isEmpty()) {
            cout << "EMPTY" << endl;
            return;
        }
        cout << "PRE-ORDER: ";
        print_preorder(root);
        cout << endl << "IN-ORDER: ";
        print_inorder(root);
        cout << endl << "POST-ORDER: ";
        print_postorder(root);
        cout << endl << "STATUS: " << check_health(root, NULL) << endl;
    }

    bool isEmpty() {
        return size == 0;
    }

    void print_preorder(node* curr) {
        cout << curr->element;
        if (curr->is_red) {
            cout << "(R) ";
        } else {
            cout << "(B) ";
        }
        if (curr->left != NULL) {
            print_preorder(curr->left);
        }
        if (curr->right != NULL) {
            print_preorder(curr->right);
        }
    }

    void print_inorder(node* curr) {
        if (curr->left != NULL) {
            print_inorder(curr->left);
        }
        cout << curr->element;
        if (curr->is_red) {
            cout << "(R) ";
        } else {
            cout << "(B) ";
        }
        if (curr->right != NULL) {
            print_inorder(curr->right);
        }
    }

    void print_postorder(node* curr) {
        if (curr->left != NULL) {
            print_postorder(curr->left);
        }
        if (curr->right != NULL) {
            print_postorder(curr->right);
        }
        cout << curr->element;
        if (curr->is_red) {
            cout << "(R) ";
        } else {
            cout << "(B) ";
        }
    }

    // WARNING. Do not modify this method.
    // Doing so will nullify your score for this activity.
    bool check_health(node* curr, node* parent) {
        bool health = curr->parent == parent;
        if (curr->left != NULL) {
            health &= check_health(curr->left, curr);
        }
        if (curr->right != NULL) {
            health &= check_health(curr->right, curr);
        }
        return health;
    }
};