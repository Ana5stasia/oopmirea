#include <iostream>
#include <iomanip>
#include <vector>
#include <functional>
#include <fstream>
#include <algorithm>

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

    // Добавлены методы filter и find
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

    Element<T>* find(std::function<bool(const T&)> condition) {
        Element<T>* current = head;
        while (current != nullptr) {
            if (condition(current->info)) {
                return current;
            }
            current = current->next;
        }
        return nullptr;
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

template<class T>
class DoublyLinkedList : public LinkedList<T> {
public:
    // Добавлены методы insert и remove
    Element<T>* insert(Element<T>* pos, T value) {
        if (pos == nullptr) {
            return push(value);
        }
        Element<T>* newElement = new Element<T>(pos->next, pos, value);
        if (pos->next != nullptr) {
            pos->next->prev = newElement;
        } else {
            tail = newElement;
        }
        pos->next = newElement;
        count++;
        return newElement;
    }

    Element<T>* remove(Element<T>* pos) {
        if (pos == nullptr || count == 0) {
            return nullptr;
        }
        Element<T>* result = pos;
        if (pos->prev != nullptr) {
            pos->prev->next = pos->next;
        } else {
            head = pos->next;
        }
        if (pos->next != nullptr) {
            pos->next->prev = pos->prev;
        } else {
            tail = pos->prev;
        }
        count--;
        return result;
    }

    Element<T>* pop() override {
        if (tail == nullptr) return nullptr;
        Element<T>* res = tail;
        if (head == tail) {
            head = tail = nullptr;
        } else {
            tail = tail->prev;
            tail->next = nullptr;
        }
        count--;
        return res;
    }

    Element<T>* push(T value) override {
        Element<T>* newElement = new Element<T>(value);
        if (head == nullptr) {
            head = tail = newElement;
        } else {
            tail->next = newElement;
            newElement->prev = tail;
            tail = newElement;
        }
        count++;
        return newElement;
    }

    Element<T>& operator[](int index) override {
        if (index < 0 || index >= count) {
            throw std::out_of_range("Index out of range");
        }
        Element<T>* current;
        if (index < count / 2) {
            current = head;
            for (int i = 0; i < index; i++) {
                current = current->next;
            }
        } else {
            current = tail;
            for (int i = count - 1; i > index; i--) {
                current = current->prev;
            }
        }
        return current->info;
    }

    virtual ~DoublyLinkedList() {
        std::cout << "\nDoublyLinkedList class destructor";
    }
};

// Добавлен класс Team для пункта 6.1
class Team {
public:
    std::string name;
    std::string city;
    int wins;
    int losses;
    int draws;
    int points;

    Team(std::string n, std::string c, int w, int l, int d, int p) : name(n), city(c), wins(w), losses(l), draws(d), points(p) {}

    template<class T1>
    friend std::ostream& operator<<(std::ostream& s, Team& el);
};

template<class T1>
std::ostream& operator<<(std::ostream& s, Team& el) {
    s << el.name << ", " << el.city << ", " << el.wins << ", " << el.losses << ", " << el.draws << ", " << el.points;
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
        if (index < 0 || index >= this->count) {
            throw std::out_of_range("Index out of range");
        }
        Element<T>* current = this->head;
        for (int i = 0; i < index; ++i) {
            current = current->next;
        }
        return current->info;
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

// Добавлены методы для пункта 6.2
template <typename T>
int custom_filter(const T& value, const T& filter_value) {
    return value > filter_value;
}

void dynamic_cast_test(LinkedList<Team>* base_ptr) {
    DoublyLinkedList<team>* derived_ptr = dynamic_cast<DoublyLinkedList<team>*>(base_ptr);
    if (derived_ptr != nullptr) {
        std::cout << "dynamic_cast succeeded\n";
        delete derived_ptr;
    } else {
        std::cout << "dynamic_cast failed\n";
        delete base_ptr;
    }
}

std::ostream& hex_format(std::ostream& s) {
    s << std::hex << std::setw(8) << std::setfill('0');
    return s;
}

int main() {
    // Создание двусвязного списка команд для пункта 6.1
    DoublyLinkedList<Team> teams;
    teams.push(Team("Barcelona", "Barcelona", 26, 3, 5, 85));
    teams.push(Team("Real Madrid", "Madrid", 24, 5, 6, 78));
    teams.push(Team("Atletico Madrid", "Madrid", 19, 6, 10, 67));
    teams.push(Team("Sevilla", "Sevilla", 18, 8, 9, 63));

    // Тестирование фильтрации и поиска для пункта 6.1
    std::vector<Element<Team>*> filtered_teams = teams.filter([](const Team& t) { return t.points > 70; });
    for (Element<Team>* el : filtered_teams) {
        std::cout << *el << "\n";
    }

    Element<Team>* found_team = teams.find([](const Team& t) { return t.name == "Real Madrid"; });
    if (found_team != nullptr) {
        std::cout << *found_team << "\n";
    }

    // Создание списка динамически для пункта 6.2
    LinkedList<Team>* teams_ptr = new DoublyLinkedList<Team>();
    teams_ptr->push(Team("Barcelona", "Barcelona", 26, 3, 5, 85));
    teams_ptr->push(Team("Real Madrid", "Madrid", 24, 5, 6, 78));
    teams_ptr->push(Team("Atletico Madrid", "Madrid", 19, 6, 10, 67));
    teams_ptr->push(Team("Sevilla", "Sevilla", 18, 8, 9, 63));

    // Тестирование фильтрации с произвольным полем для пункта 6.2
    std::vector<Element<Team>*> filtered_teams_ptr = teams_ptr->filter([&](const Team& t) { return custom_filter(t.points, 70); });
    for (Element<Team>* el : filtered_teams_ptr) {
        std::cout << *el << "\n";
    }

    // Тестирование dynamic_cast для пункта 6.2
    dynamic_cast_test(teams_ptr);

    // Тестирование сохранения и загрузки списка из файла для пункта 6.2
    teams.save("teams.dat");
    LinkedList<Team> loaded_teams;
    loaded_teams.load("teams.dat");
    for (int i = 0; i < loaded_teams.count; ++i) {
        std::cout << loaded_teams[i] << "\n";
    }

    return 0;
}
