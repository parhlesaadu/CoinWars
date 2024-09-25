#pragma once
#ifndef AVL_H
#define AVL_H
#include <iostream>
using namespace std;

struct Node {
    string object; int quantity;
    Node* left = nullptr, * right = nullptr;
};

string displayInOrder(Node* ptr) {
    string str = "";
    if (ptr) {
        str += displayInOrder(ptr->left);
        str += ptr->object + ": " + to_string(ptr->quantity) + "  ";
        str += displayInOrder(ptr->right);
    }
    return str;
}

int find(Node* ptr, string str) {
    if (ptr) {
        if (ptr->object == str) return ptr->quantity;
        else {
            int l = find(ptr->left, str);
            int r = find(ptr->right, str);
            return l ? l : r;
        }
    }
    else return 0;
}

int height(Node* n) {
    if (!n) return -1;
    return max(height(n->left), height(n->right)) + 1;
}

int getbalance(Node* n) {
    if (!n) return -1;
    return (height(n->left) - height(n->right));
}

Node* rightRotate(Node* node) {
    Node* temp1 = node->left;
    Node* temp2 = temp1->right;

    // Perform rotation
    temp1->right = node;
    node->left = temp2;

    return temp1;
}

Node* leftRotate(Node* node) {
    Node* temp1 = node->right;
    Node* temp2 = temp1->left;

    // Perform rotation
    temp1->left = node;
    node->right = temp2;

    return temp1;
}

Node* insertNode(Node* n, string obj, int num) {
    if (!n) return new Node{ obj, num };
    else if (n->quantity > num) n->left = insertNode(n->left, obj, num);
    else if (n->quantity < num) n->right = insertNode(n->right, obj, num);
    else return n;

    int balance = getbalance(n);
    if (balance > 1 && num < n->left->quantity) return rightRotate(n);
    if (balance < -1 && num > n->right->quantity) return leftRotate(n);

    if (balance > 1 && num > n->left->quantity) {
        n->left = leftRotate(n->left);
        return rightRotate(n);
    }

    if (balance < -1 && num < n->right->quantity) {
        n->right = rightRotate(n->right);
        return leftRotate(n);
    }
    return n;
}

void change(Node* ptr, string s, int i) {
    if (ptr) {
        change(ptr->left, s, i);
        if (ptr->object == s) ptr->quantity = i;
        change(ptr->right, s, i);
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
