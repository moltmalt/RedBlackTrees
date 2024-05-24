#include "tree.h"
#include <iostream>
using namespace std;
int main() {
    BSTree* tree = new BSTree();

    char op;
    int num;
    do {
        cout << "Operation: ";
        cin >> op;
        switch (op) {
            case 'i':
                cin >> num;
                tree->insert(num);
                break;
            case 'p':
                tree->print();
                break;
            case 'r':
                cin >> num;
                tree->remove(num);
                break;
            default:
                cout << "Invalid operation";
        }
    } while (op != 'x');
    return 0;
};