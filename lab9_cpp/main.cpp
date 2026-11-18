#include <iostream>
#include <queue>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

// УЗЕЛ ДЕРЕВА
struct Node {
    int data;
    Node* left;
    Node* right;
    Node(int val) : data(val), left(nullptr), right(nullptr) {}
};

// ЧАСТЬ 1: БИНАРНОЕ ДЕРЕВО С ОБХОДАМИ
class BinaryTree {
private:
    Node* root;

    // Прямой обход: Корень → Лево → Право
    void preOrder(Node* node) {
        if (node == nullptr) return;
        cout << node->data << " ";
        preOrder(node->left);
        preOrder(node->right);
    }

    // Симметричный обход: Лево → Корень → Право
    void inOrder(Node* node) {
        if (node == nullptr) return;
        inOrder(node->left);
        cout << node->data << " ";
        inOrder(node->right);
    }

    // Обратный обход: Лево → Право → Корень
    void postOrder(Node* node) {
        if (node == nullptr) return;
        postOrder(node->left);
        postOrder(node->right);
        cout << node->data << " ";
    }

    // Уничтожение дерева (для деструктора)
    void destroy(Node* node) {
        if (node == nullptr) return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

public:
    BinaryTree() : root(nullptr) {}
    ~BinaryTree() { destroy(root); }

    // Тестовое дерево
    void buildSample() {
        root = new Node(10);
        root->left = new Node(5);
        root->right = new Node(15);
        root->left->left = new Node(3);
        root->left->right = new Node(7);
        root->right->right = new Node(20);
        root->left->right->left = new Node(6);
        root->right->right->left = new Node(18);
    }

    void preOrder() { preOrder(root); cout << endl; }
    void inOrder() { inOrder(root); cout << endl; }
    void postOrder() { postOrder(root); cout << endl; }

    // Уровневый обход (через очередь)
    void levelOrder() {
        if (root == nullptr) return;
        queue<Node*> q;
        q.push(root);
        while (!q.empty()) {
            Node* current = q.front();
            q.pop();
            cout << current->data << " ";
            if (current->left) q.push(current->left);
            if (current->right) q.push(current->right);
        }
        cout << endl;
    }

    // ЗАДАЧА 7: среднее арифметическое всех элементов
    int sum(Node* node) {
        if (node == nullptr) return 0;
        return node->data + sum(node->left) + sum(node->right);
    }

    int countNodes(Node* node) {
        if (node == nullptr) return 0;
        return 1 + countNodes(node->left) + countNodes(node->right);
    }

    double average() {
        int total = countNodes(root);
        if (total == 0) return 0;
        return (double)sum(root) / total;
    }
};

// ЧАСТЬ 2: БИНАРНОЕ ДЕРЕВО ПОИСКА (BST) + ВИЗУАЛИЗАЦИЯ
class BST {
private:
    Node* root;

    Node* insert(Node* node, int key) {
        if (node == nullptr) return new Node(key);
        if (key < node->data) node->left = insert(node->left, key);
        else if (key > node->data) node->right = insert(node->right, key);
        return node;
    }

    Node* search(Node* node, int key) {
        if (node == nullptr || node->data == key) return node;
        if (key < node->data) return search(node->left, key);
        return search(node->right, key);
    }

    Node* findMin(Node* node) {
        while (node->left != nullptr) node = node->left;
        return node;
    }

    Node* remove(Node* node, int key) {
        if (node == nullptr) return nullptr;
        if (key < node->data) node->left = remove(node->left, key);
        else if (key > node->data) node->right = remove(node->right, key);
        else {
            // Случай 1: лист
            if (node->left == nullptr && node->right == nullptr) {
                delete node;
                return nullptr;
            }
            // Случай 2: один потомок
            else if (node->left == nullptr) {
                Node* temp = node->right;
                delete node;
                return temp;
            }
            else if (node->right == nullptr) {
                Node* temp = node->left;
                delete node;
                return temp;
            }
            // Случай 3: два потомка
            else {
                Node* minNode = findMin(node->right);
                node->data = minNode->data;
                node->right = remove(node->right, minNode->data);
            }
        }
        return node;
    }

    void inOrder(Node* node) {
        if (node == nullptr) return;
        inOrder(node->left);
        cout << node->data << " ";
        inOrder(node->right);
    }

    void destroy(Node* node) {
        if (node == nullptr) return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

    int height(Node* node) {
        if (node == nullptr) return 0;
        return 1 + max(height(node->left), height(node->right));
    }

    // ЗАДАЧА 7: визуализация дерева в консоли
    void printTree(Node* node, int space) {
        if (node == nullptr) return;
        space += 5;
        printTree(node->right, space);
        cout << endl;
        for (int i = 5; i < space; i++) cout << " ";
        cout << node->data << endl;
        printTree(node->left, space);
    }

public:
    BST() : root(nullptr) {}
    ~BST() { destroy(root); }

    void insert(int key) { root = insert(root, key); }
    bool search(int key) { return search(root, key) != nullptr; }
    void remove(int key) { root = remove(root, key); }
    void print() { inOrder(root); cout << endl; }
    int getHeight() { return height(root); }
    void visualize() { printTree(root, 0); }
};

int main() {
    setlocale(LC_ALL, "ru");

    cout << "ЛР №9: Бинарное дерево и BST\n";

    cout << "\n1. Бинарное дерево\n";
    BinaryTree tree;
    tree.buildSample();
    cout << "Прямой обход (pre-order):    ";
    tree.preOrder();
    cout << "Симметричный (in-order):     ";
    tree.inOrder();
    cout << "Обратный (post-order):       ";
    tree.postOrder();
    cout << "Уровневый (level-order):     ";
    tree.levelOrder();
    cout << "Среднее арифметическое: " << tree.average() << endl;

    cout << "\n2. BST\n";
    BST bst;
    bst.insert(50);
    bst.insert(30);
    bst.insert(70);
    bst.insert(20);
    bst.insert(40);
    bst.insert(60);
    bst.insert(80);

    cout << "Симметричный обход BST: ";
    bst.print();
    cout << "Высота дерева: " << bst.getHeight() << endl;
    cout << "Поиск 40: " << (bst.search(40) ? "найден" : "не найден") << endl;
    cout << "Поиск 100: " << (bst.search(100) ? "найден" : "не найден") << endl;

    cout << "\nВизуализация BST:\n";
    bst.visualize();

    cout << "\nУдаление 30...\n";
    bst.remove(30);
    cout << "После удаления 30: ";
    bst.print();

    cout << "\n3. Применение BST (20 случайных чисел)\n";
    srand(time(0));
    BST randomTree;
    for (int i = 0; i < 20; i++) {
        randomTree.insert(rand() % 100 + 1);
    }

    cout << "Отсортированная последовательность: ";
    randomTree.print();
    cout << "Высота дерева: " << randomTree.getHeight() << endl;

    return 0;
}