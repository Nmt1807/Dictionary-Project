
#include <iostream>
#include <fstream>
#include <string>
#include <cctype>

using namespace std;

class Node {
public:
    string key;
    Node* parent;
    Node* left;
    Node* right;
    int color;  // 0 for red, 1 for black

    Node(string key) : key(key), parent(NULL), left(NULL), right(NULL), color(0) {}
};

class RedBlackTree {
public:
    
    Node* nil;
    Node* root;
    int number_of_nodes;

    RedBlackTree() : nil(new Node("")), number_of_nodes(0) {
        nil->color = 1;  // The root and the nil are black
        root = nil;
    }

    void readFile(const string& fileName) {
        ifstream file(fileName.c_str());

        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                if (!search(line)) {
                    insert(line);
                }
            }
            file.close();
        }
        else {
            cout << "Unable to open file: " << fileName << endl;
        }
    }

    bool search(string key) {
        Node* node = root;

        while (node != nil) {
            if (node->key == key) {
                return true;
            }
            else if (key < node->key) {
                node = node->left;
            }
            else {
                node = node->right;
            }
        }
        return false;
    }

    void insert(string key) {
        Node* newNode = new Node(key);
        newNode->left = nil;
        newNode->right = nil;
        Node* node = root;
        Node* parent = NULL;

        while (node != nil) {
            parent = node;
            if (key < node->key) {
                node = node->left;
            }
            else {
                node = node->right;
            }
        }
        newNode->parent = parent;

        if (parent == NULL) {
            newNode->color = 1;
            root = newNode;
            number_of_nodes += 1;
            return;
        }
        else if (key < parent->key) {
            parent->left = newNode;
        }
        else {
            parent->right = newNode;
        }

        if (newNode->parent->parent == NULL) {
            number_of_nodes += 1;
            return;
        }

        insertFix(newNode);
        number_of_nodes += 1;
    }

    void insertFix(Node* newNode) {
        while (newNode != root && newNode->parent->color == 0) {
            bool parentIsLeft = false;
            Node* uncle;

            if (newNode->parent == newNode->parent->parent->left) {
                uncle = newNode->parent->parent->right;
                parentIsLeft = true;
            }
            else {
                uncle = newNode->parent->parent->left;
            }

            if (uncle->color == 0) {
                newNode->parent->color = 1;
                uncle->color = 1;
                newNode->parent->parent->color = 0;
                newNode = newNode->parent->parent;
            }
            else {
                if (parentIsLeft && newNode == newNode->parent->right) {
                    newNode = newNode->parent;
                    leftRotate(newNode);
                }
                else if (!parentIsLeft && newNode == newNode->parent->left) {
                    newNode = newNode->parent;
                    rightRotate(newNode);
                }
                if (parentIsLeft) {
                    newNode->parent->color = 1;
                    newNode->parent->parent->color = 0;
                    rightRotate(newNode->parent->parent);
                }
                else {
                    newNode->parent->color = 1;
                    newNode->parent->parent->color = 0;
                    leftRotate(newNode->parent->parent);
                }
            }
        }
        root->color = 1;
    }

    void leftRotate(Node* node) {
        Node* y = node->right;
        node->right = y->left;

        if (y->left != nil) {
            y->left->parent = node;
        }

        y->parent = node->parent;

        if (node->parent == NULL) {
            root = y;
        }
        else if (node == node->parent->left) {
            node->parent->left = y;
        }
        else {
            node->parent->right = y;
        }

        y->left = node;
        node->parent = y;
    }

    void rightRotate(Node* node) {
        Node* y = node->left;
        node->left = y->right;

        if (y->right != nil) {
            y->right->parent = node;
        }

        y->parent = node->parent;

        if (node->parent == NULL) {
            root = y;
        }
        else if (node == node->parent->left) {
            node->parent->left = y;
        }
        else {
            node->parent->right = y;
        }

        y->right = node;
        node->parent = y;
    }

    int heightOfTree(Node* node, int sumval) {
        if (node == nil) {
            return sumval;
        }
        return max(heightOfTree(node->left, sumval + 1), 
            heightOfTree(node->right, sumval + 1));
    }

    int getBlackHeight() {
        Node* node = root;
        int bh = 0;

        while (node != nil) {
            node = node->left;
            if (node->color == 1) {
                bh += 1;
            }
        }
        return bh;
    }

    void printTree() {
        printCall(root, "", true);
    }

private:


    void printCall(Node* node, string indent, bool last) {
        if (node != nil) {
            cout << indent;
            if (last) {
                cout << "R----";
                indent += "     ";
            }
            else {
                cout << "L----";
                indent += "|    ";
            }

            string s_color = (node->color == 0) ? "RED" : "BLACK";
            cout << node->key << "(" << s_color << ")" << endl;
            printCall(node->left, indent, false);
            printCall(node->right, indent, true);
        }
    }
};

int main() {


    RedBlackTree tree;
    string DICTIONARY_NAME = "EN-US-Dictionary.txt";

    while (true) {
        cout << "What do you want to do?" << endl;
        cout << "1- Load \"" << DICTIONARY_NAME << "\"\t2- Print size of the Dictionary\n"
            << "3- Insert Word             \t4- Look-up a Word\n"
            << "5- Print Tree Height       \t6- Print Black Height of the Tree\n"
            << "7- Exit\n"
            << "> ";

        string option;
        getline(cin, option);

        if (option == "1") {
            tree.readFile(DICTIONARY_NAME);
            cout << DICTIONARY_NAME << " loaded successfully!" << endl;
        }
        else if (option == "2") {
            cout << DICTIONARY_NAME << " currently has " << tree.number_of_nodes << " words!" << endl;
        }
        else if (option == "3") {
            string s;
            cout << "Enter the word you want to insert: ";
            getline(cin, s);
            if (tree.search(s)) {
                cout << "\"" << s << "\" is already in the dictionary!" << endl;
            }
            else if (!s.empty() && !isspace(s[0])) {
                tree.insert(s);
                cout << '\"' << s << "\" inserted successfully!" << endl;
            }
            else {
                cout << "Invalid entry" << endl;
            }
        }
        else if (option == "4") {
            string s;
            cout << "Enter the word you want to look-up: ";
            getline(cin, s);
            if (tree.search(s)) {
                cout << "FOUND \"" << s << '\"' << endl;
            }
            else {
                cout << "\"" << s << '\"' << " DOES NOT EXIST IN THE DICTIONARY" << endl;
            }
        }
        else if (option == "5") {
            cout << "Tree Height: " << tree.heightOfTree(tree.root, 0) << endl;
        }
        else if (option == "6") {
            cout << "Black Height of the Tree: " << tree.getBlackHeight() << endl;
        }
        else if (option == "7") {
            cout << "Thank you for using our application! :)" << endl;
            break;
        }

        cout << endl;
    }

    return 0;
}