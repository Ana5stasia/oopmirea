#include <iostream>
#include <string>
#include <algorithm>

template<class T>
class Node {
protected:
    T data;
    Node<T>* left;
    Node<T>* right;
    Node<T>* parent;
    int height;

public:
    virtual void setData(T d) { data = d; }
    virtual T getData() { return data; }
    int getHeight() { return height; }
    virtual Node<T>* getLeft() { return left; }
    virtual Node<T>* getRight() { return right; }
    virtual Node<T>* getParent() { return parent; }
    virtual void setLeft(Node<T>* N) { left = N; }
    virtual void setRight(Node<T>* N) { right = N; }
    virtual void setParent(Node<T>* N) { parent = N; }

    Node<T>(T n) {
        data = n;
        left = right = parent = NULL;
        height = 1;
    }

    Node<T>() {
        left = NULL;
        right = NULL;
        parent = NULL;
        data = 0;
        height = 1;
    }

    virtual void print() {
        std::cout << "\n" << data;
    }

    virtual void setHeight(int h) {
        height = h;
    }

    template<class T>
    friend std::ostream& operator<<(std::ostream& stream, Node<T>& N);
};

template<class T>
std::ostream& operator<<(std::ostream& stream, Node<T>& N) {
    stream << "\nNode data: " << N.data << ", height: " << N.height;
    return stream;
}

template<class T>
void print(Node<T>* N) {
    std::cout << "\n" << N->getData();
}

template<class T>
class Tree {
protected:
    Node<T>* root;

public:
    virtual Node<T>* getRoot() { return root; }

    Tree<T>() { root = NULL; }

    virtual Node<T>* Add_R(Node<T>* N) {
        return Add_R(N, root);
    }

    virtual Node<T>* Add_R(Node<T>* N, Node<T>* Current) {
        if (N == NULL) return NULL;
        if (root == NULL) {
            root = N;
            return N;
        }

        if (Current->getData() > N->getData()) {
            if (Current->getLeft() != NULL)
                Current->setLeft(Add_R(N, Current->getLeft()));
            else
                Current->setLeft(N);
            Current->getLeft()->setParent(Current);
        }

        if (Current->getData() < N->getData()) {
            if (Current->getRight() != NULL)
                Current->setRight(Add_R(N, Current->getRight()));
            else
                Current->setRight(N);
            Current->getRight()->setParent(Current);
        }

        if (Current->getData() == N->getData()) {
            // Нашли совпадение
        }

        return Current;
    }

    virtual void Add(T n) {
        Node<T>* N = new Node<T>;
        N->setData(n);
        Add_R(N);
    }

    virtual Node<T>* operator[](const T& key) {
        return Find(key, root);
    }

    virtual Node<T>* Min(Node<T>* Current = NULL) {
        if (root == NULL) return NULL;
        if (Current == NULL)
            Current = root;
        while (Current->getLeft() != NULL)
            Current = Current->getLeft();
        return Current;
    }

    virtual Node<T>* Max(Node<T>* Current = NULL) {
        if (root == NULL) return NULL;
        if (Current == NULL)
            Current = root;
        while (Current->getRight() != NULL)
            Current = Current->getRight();
        return Current;
    }

    virtual Node<T>* Find(const T& data, Node<T>* Current) {
        if (Current == NULL) return NULL;
        if (Current->getData() == data) return Current;
        if (Current->getData() > data) return
            Find(data, Current->getLeft());
        if (Current->getData() < data) return Find(data,
                                                     Current->getRight());
    }

    virtual void PreOrder(Node<T>* N, void
