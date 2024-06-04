#include <iostream>
#include <locale>
using namespace std;
class MyArrayParent {
    protected:
        int capacity;
        int count;
        double* ptr;
    public:
        explicit MyArrayParent(int dimension=100){
            ptr = new double[dimension];
            capacity=dimension;
            count=0;
        }

        MyArrayParent(const MyArrayParent &a){
            ptr=new double[a.capacity];
            capacity=a.capacity;
            count=a.count;
            for (int i=0; i<count; i++){
                ptr[i]=a.ptr[i];
            }
        }
        ~MyArrayParent(){
            if(ptr!=nullptr){
                delete[] ptr;
                ptr=nullptr;
            }
        }

        MyArrayParent& operator=(const MyArrayParent &a){
            if (this!=&a){
                if(capacity!=a.capacity){
                    delete[] ptr;
                    ptr= new double[a.capacity];
                    capacity=a.capacity;
                }
            }
            count=a.count;
            for(int i=0; i<count; i++)
                ptr[i]=a.ptr[i];
            }
            return* this;
        }

        double& operator[](int index){
            if (index>=0 && index<count){
                return ptr[index];
            }
            throw out_of_range("index out of range");
        }

        int IndexOf(double v, bool fromEnd) const{
            for (int i=0; i<count; i++){
                if (abs(ptr[i]-v)<0.00000001){
                    return i;
                }
            }
            return -1;
        }
            void push(double value) {
                if (count < capacity) {
                    ptr[count++] = value;
            }
                if (count >= capacity) {
                    double* temp = new double[2 * capacity];
                    for (int i = 0; i < count; i++) {
                        temp[i] = ptr[i];
                    }
                    delete[] ptr;
                    ptr = temp;
                    capacity *= 2;
                }
        }

        void RemoveLastValue() {
            if (count > 0) {
                count--;
            }
            else{
                throw std::out_of_range("Index out of range");
            }
        }

        void print() const {
            cout << "Array: [";
            for (int i = 0; i < count; i++) {
                cout << ptr[i];
                if (i != count - 1) {
                    cout << ", ";
                }
            }
            cout << "]" << endl;
        }
};

class MyArrayChild:
    public MyArrayParent {
public:
    void RemoveAt(int x){
        if(x>=0 && x<count){
            for(int i=x; i<count; i++){
                ptr[i]=ptr[i+1];
            }
            count--;
        }
        else{
            throw std::out_of_range("Index out of range");
        }
    }
    void InsertAt(double value, int index) {
        if (count < capacity && index >= 0 && index <= count) {
            for (int i = count; i > index; i--) {
                ptr[i] = ptr[i - 1];
            }
            ptr[index] = value;
            count++;
        }
        else{
            throw std::out_of_range("Index out of range");
        }
    }
    MyArrayChild shift(int n) const{
        MyArrayChild copyy(*this);
        for(int i=0; i<count; i++){
            copyy.ptr[i]+=n;
        }
        return copyy;
    }

    explicit MyArrayChild(int dimension = 100) : MyArrayParent(dimension) {}
    ~MyArrayChild() {}

    MyArrayChild SubSequence(int StartIndex, int Length) {
        MyArrayChild subSequence;
        int endIndex = StartIndex + Length;
        for (int i = StartIndex; i < endIndex && i < count; i++) {
            subSequence.push(ptr[i]);
        }
        return subSequence;
    }

    void operator+(double value) {
        push(value);
    }
};

class MySortedArray :
    public MyArrayChild {
public:
    // Конструкторы и деструкторы
    explicit MySortedArray(int Dimension = 100):  MyArrayChild(Dimension) {
    }
    // Перегрузка операций и другие функции
    void RecursivePush(double value, int left, int right) {
        if (left > right) {
            InsertAt(value, left);
            return;
        }

        int mid = (left + right) / 2;

        if (value < ptr[mid]) {
            RecursivePush(value, left, mid - 1);
        }

        else {
            RecursivePush(value, mid + 1, right);
        }
    }
    // Перегруженный оператор добавления элемента
    void push(double value) {
        if (count >= capacity) {
            double* temp = new double[2 * capacity];
            for (int i = 0; i < count; i++) {
                temp[i] = ptr[i];
            }
            delete[] ptr;
            ptr = temp;
            capacity *= 2;
        }
        RecursivePush(value, 0, count - 1);
    }


    // Переопределение функции поиска
    double Search(double value) {
        int left = 0;
        int right = count - 1;
        while (left <= right) {
            int mid = left + (right - left) / 2;
            if (abs(ptr[mid] - value) < 0.0000001) {
                return mid;
            } else if (ptr[mid] < value) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
        return -1; // Элемент не найден
    }


    // Перегруженный оператор [] для доступа к элементам по индексу
    double& operator[](int index) {
        if (index >= 0 && index < count) {
            return ptr[index];
        } else {
            // Генерируем исключение, если индекс некорректный
            throw std::out_of_range("Index out of range");
        }
}
};

int main() {
    MySortedArray y(2);
    y.push(90);
    y.push(30);
    cout<<y[0];
}
