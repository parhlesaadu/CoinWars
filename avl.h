#pragma once
#ifndef AVL_H
#define AVL_H
#include <iostream>
using namespace std;

struct Node {
    string object; int quantity;
    Node* llink = nullptr, * rlink = nullptr;
};

string displayInOrder(Node* ptr) {
    string str = "";
    if (ptr) {
        str += displayInOrder(ptr->llink);
        str += ptr->object + ": " + to_string(ptr->quantity) + "  ";
        str += displayInOrder(ptr->rlink);
    }
    return str;
}

int find(Node* ptr, string str) {
    if (ptr) {
        if (ptr->object == str) return ptr->quantity;
        else {
            int l = find(ptr->llink, str);
            int r = find(ptr->rlink, str);
            return l ? l : r;
        }
    }
    else return 0;
}

int height(Node* n) {
    if (!n) return -1;
    return max(height(n->llink), height(n->rlink)) + 1;
}

int getbalance(Node* n) {
    if (!n) return -1;
    return (height(n->llink) - height(n->rlink));
}

Node* rightRotate(Node* node) {
    Node* temp1 = node->llink;
    Node* temp2 = temp1->rlink;

    // Perform rotation
    temp1->rlink = node;
    node->llink = temp2;

    return temp1;
}

Node* leftRotate(Node* node) {
    Node* temp1 = node->rlink;
    Node* temp2 = temp1->llink;

    // Perform rotation
    temp1->llink = node;
    node->rlink = temp2;

    return temp1;
}

Node* insertNode(Node* n, string obj, int num) {
    if (!n) return new Node{ obj, num };
    else if (n->quantity > num) n->llink = insertNode(n->llink, obj, num);
    else if (n->quantity < num) n->rlink = insertNode(n->rlink, obj, num);
    else return n;

    int balance = getbalance(n);
    if (balance > 1 && num < n->llink->quantity) return rightRotate(n);
    if (balance < -1 && num > n->rlink->quantity) return leftRotate(n);

    if (balance > 1 && num > n->llink->quantity) {
        n->llink = leftRotate(n->llink);
        return rightRotate(n);
    }

    if (balance < -1 && num < n->rlink->quantity) {
        n->rlink = rightRotate(n->rlink);
        return leftRotate(n);
    }
    return n;
}

void change(Node* ptr, string s, int i) {
    if (ptr) {
        change(ptr->llink, s, i);
        if (ptr->object == s) ptr->quantity = i;
        change(ptr->rlink, s, i);
    }
}

class Inventory {
    Node* root;
public:
    Inventory() : root(nullptr) {}
    void set(string str, int i) {
        change(root, str, i);
    }
    int get(string key) {
        return find(root, key);
    }
    string display() {
        return displayInOrder(root) + "\n";
    }
    void insert(string obj, int num) {
        root = insertNode(root, obj, num);
    }
};
#endif
