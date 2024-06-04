#include <iostream>
#include <iomanip>
#include <vector>
#include <functional>
#include <fstream>

template<class T>
class Element {
protected:
    Element* next;
    Element* prev;
    T info;

public:
    Element(T data) : next(nullptr), prev(nullptr), info(data) {}
    Element(Element* Next, Element* Prev, T data) : next(Next), prev(Prev), info(data) {}
    Element(const Element& el) : next(el.next), prev(el.prev), info(el.info) {}

    template<class T1>
    friend std::ostream& operator<<(std::ostream& s, Element<T1>& el);
};

template<class T1>
std::ostream& operator<<(std::ostream& s, Element<T1>& el) {
    s << el.info;
    return s;
}

template<class T>
class LinkedList {
protected:
    Element<T>* head;
    Element<T>* tail;
    int count;

public:
    LinkedList() : head(nullptr), tail(nullptr), count(0) {}

    virtual ~LinkedList() {
        while (head != nullptr) {
            Element<T>* temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
        count = 0;
        std::cout << "\nBase class destructor";
    }

    virtual Element<T>* pop() = 0;
    virtual Element<T>* push(T value) = 0;
    virtual Element<T>& operator[](int index) = 0;
    virtual bool isEmpty() { return count == 0; }

    std::vector<Element<T>*> filter(std::function<bool(const T&)> condition) {
        std::vector<Element<T>*> result;
        Element<T>* current = head;
        while (current != nullptr) {
            if (condition(current->info)) {
                result.push_back(current);
            }
            current = current->next;
        }
        return result;
    }

    void save(const std::string& filename) {
        std::ofstream file(filename, std::ios::binary);
        if (!file) {
            throw std::runtime_error("Failed to open file for writing");
        }

        Element<T>* current = head;
        while (current != nullptr) {
            file.write(reinterpret_cast<char*>(&current->info), sizeof(T));
            current = current->next;
        }
        file.close();
    }

    void load(const std::string& filename) {
        std::ifstream file(filename, std::ios::binary);
        if (!file) {
            throw std::runtime_error("Failed to open file for reading");
        }

        T value;
        while (file.read(reinterpret_cast<char*>(&value), sizeof(T))) {
            push(value);
        }
        file.close();
    }

    template<class T1>
    friend std::ostream& operator<<(std::ostream& s, LinkedList<T1>& el);
};

template<class T1>
std::ostream& operator<<(std::ostream& s, LinkedList<T1>& el) {
    Element<T1>* current = el.head;
    while (current != nullptr) {
        s << current->info << " ";
        current = current->next;
    }
    return s;
}

template<class T, int N = 20>
class Stack : public LinkedList<T> {
public:
    Stack() {
        if (N > 0) {
            for (int i = 0; i < N; ++i) {
                this->push(0);
            }
        }
    }

    virtual Element<T>* push(T value) override {
        Element<T>* newElement = new Element<T>(value);
        if (this->head == nullptr) {
            this->head = this->tail = newElement;
        } else {
            this->tail->next = newElement;
            newElement->prev = this->tail;
            this->tail = newElement;
        }
        this->count++;
        return this->tail;
    }

    virtual Element<T>* pop() override {
        if (this->tail == nullptr) return nullptr;
        Element<T>* res = this->tail;
        if (this->head == this->tail) {
            this->head = this->tail = nullptr;
        } else {
            this->tail = this->tail->prev;
            this->tail->next = nullptr;
        }
        this->count--;
        return res;
    }

    virtual ~Stack() {
        std::cout << "\nStack class destructor";
    }
};

template<class T, int N = 20>
class Queue : public LinkedList<T> {
public:
    Queue() {
        if (N > 0) {
            for (int i = 0; i < N; ++i) {
                this->push(0);
            }
        }
    }

    virtual Element<T>* push(T value) override {
        Element<T>* newElement = new Element<T>(value);
        if (this->head == nullptr) {
            this->head = this->tail = newElement;
        } else {
            this->tail->next = newElement;
            newElement->prev = this->tail;
            this->tail = newElement;
        }
        this->count++;
        return this->tail;
    }

    virtual Element<T>* pop() override {
        if (this->head == nullptr) return nullptr;
        Element<T>* res = this->head;
        if (this->head == this->tail) {
            this->head = this->tail = nullptr;
        } else {
            this->head = this->head->next;
            this->head->prev = nullptr;
        }
        this->count--;
        return res;
    }

    virtual ~Queue() {
        std::cout << "\nQueue class destructor";
    }
};

template<class T, int N = 20>
class StackQueue : protected Stack<T, N>, protected Queue<T, N> {
public:
    void push_back(T value) {
        Stack<T, N>::push(value);
    }

    void push_front(T value) {
        Queue<T, N>::push(value);
    }

    Element<T>* pop_back() {
        return Stack<T, N>::pop();
    }

    Element<T>* pop_front() {
        return Queue<T, N>::pop();
    }

    virtual Element<T>* push(T value) override {
        return push_back(value);
    }

    virtual Element<T>* pop() override {
        return pop_back();
    }

    virtual Element<T>& operator[](int index) override {
        Element<T>* current = this->head;
        for (int i = 0; i < index && current != nullptr; ++i) {
            current = current->next;
        }
        if (current == nullptr) {
            throw std::out_of_range("Index out of range");
        }
        return *current;
    }

    friend std::ostream& operator<<(std::ostream& s, StackQueue<T, N>& sq) {
        Element<T>* current = sq.head;
        while (current != nullptr) {
            s << current->info << " ";
            current = current->next;
        }
        return s;
    }
};

std::ostream& hex_format(std::ostream& s) {
    s << std::hex << std::setw(8) << std::setfill('0');
    return s;
}

int main() {
    StackQueue<double> sq;
    for (int i = 0; i < 10; ++i) {
        sq.push_back(i * 1.1);
    }
    std::cout << std::setfill(' ') << std::setw(5) << sq << std::endl;

    Element<double>* el = sq.pop_back();
    if (el != nullptr) {
        std::cout << "Popped from back: " << el->info << std::endl;
        delete el;
    }

    el = sq.pop_front();
    if (el != nullptr) {
        std::cout << "Popped from front: " << el->info << std::endl;
        delete el;
    }

    // Save and load example
    sq.save("list.dat");
    StackQueue<double> loadedSq;
    loadedSq.load("list.dat");
    std::cout << loadedSq << std::endl;

    // Filter example
    auto filtered = loadedSq.filter([](const double& value) { return value > 5.0; });
    for (auto elem : filtered) {
        std::cout << *elem << " ";
    }
    std::cout << std::endl;

    return 0;
}
