#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>

class Exception : public std::exception {
protected:
    std::string str;
public:
    Exception(const std::string& s) : str(s) {}
    Exception(const Exception& e) : str(e.str) {}
    virtual const char* what() const noexcept override {
        return str.c_str();
    }
};

class InvalidOperationException : public Exception {
public:
    InvalidOperationException(const std::string& s) : Exception(s) {}
};

class IndexOutOfBoundsException : public Exception {
public:
    IndexOutOfBoundsException(const std::string& s) : Exception(s) {}
};

class WrongSizeException : public Exception {
public:
    WrongSizeException(const std::string& s) : Exception(s) {}
};

class NonPositiveSizeException : public Exception {
public:
    NonPositiveSizeException(const std::string& s) : Exception(s) {}
};

class TooLargeSizeException : public Exception {
public:
    TooLargeSizeException(const std::string& s) : Exception(s) {}
};

template<typename T>
class BaseMatrix {
protected:
    T** ptr;
    int height;
    int width;
public:
    BaseMatrix(int Height = 2, int Width = 2) {
        if (Height <= 0 || Width <= 0)
            throw NonPositiveSizeException("Non-positive size of matrix");
        height = Height;
        width = Width;
        ptr = new T* [height];
        for (int i = 0; i < height; i++)
            ptr[i] = new T[width];
    }
    BaseMatrix(const BaseMatrix& M) {
        height = M.height;
        width = M.width;
        ptr = new T* [height];
        for (int i = 0; i < height; i++) {
            ptr[i] = new T[width];
            for (int j = 0; j < width; j++)
                ptr[i][j] = M.ptr[i][j];
        }
    }
    ~BaseMatrix() {
        if (ptr != NULL) {
            for (int i = 0; i < height; i++)
                delete[] ptr[i];
            delete[] ptr;
            ptr = NULL;
        }
    }
    void print() {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++)
                std::cout << ptr[i][j] << " ";
            std::cout << "\n";
        }
    }
    T& operator()(int row, int column) {
        if(row<0 || column<0 || row>=height || column>=width)
            throw IndexOutOfBoundsException("Index is out of bounds");
        return ptr[row][column];
    }
    friend std::ostream& operator << (std::ostream& ustream, BaseMatrix<T> obj);
    friend std::istream& operator >> (std::istream& ustream, BaseMatrix<T>& obj);
    BaseMatrix& operator=(const BaseMatrix& M) {
        if (this == &M)
            return *this;
        height = M.height;
        width = M.width;
        for (int i = 0; i < height; i++)
            delete[] ptr[i];
        delete[] ptr;
        ptr = new T* [height];
        for (int i = 0; i < height; i++) {
            ptr[i] = new T[width];
            for (int j = 0; j < width; j++)
                ptr[i][j] = M.ptr[i][j];
        }
        return *this;
    }
};

template<typename T>
std::ostream& operator << (std::ostream& ustream, BaseMatrix<T> obj)
{
    ustream << obj.height << " " << obj.width << "\n";
    for (int i = 0; i < obj.height; i++) {
        for (int j = 0; j < obj.width; j++)
            ustream << obj.ptr[i][j] << "\n";
    }
    return ustream;
}

template<typename T>
std::istream& operator >> (std::istream& ustream, BaseMatrix<T>& obj)
{
    if (typeid(ustream) == typeid(std::ifstream))
        ustream >> obj.height >> obj.width;
    for (int i = 0; i < obj.height; i++)
        for (int j = 0; j < obj.width; j++)
            ustream >> obj.ptr[i][j];
    return ustream;
}

class DerivedMatrix : public BaseMatrix<double> {
public:
    DerivedMatrix(int Height = 2, int Width = 2) : BaseMatrix(Height, Width) {}
    DerivedMatrix(double** arr, int Height, int Width) : BaseMatrix(Height, Width) {
        for (int i = 0; i < Height; i++)
            for (int j = 0; j < Width; j++)
                ptr[i][j] = arr[i][j];
    }
    std::vector<double> alternateRows() {
        std::vector<double> result(height, 0);
        for (int i = 0; i < height; i++) {
            bool prev
= false;
            for (int j = 0; j < width; j++) {
                if (ptr[i][j] == 0) {
                    if (prev) {
                        result[i] = 1;
                        break;
                    }
                    prev = true;
                } else {
                    prev = false;
                }
            }
        }
        return result;
    }
};

int main() {
    try {
        DerivedMatrix Wrong(-2, 0);
    }
    catch (Exception& e) {
        std::cout << "\nException has been caught: " << e.what();
    }
    std::cout << "\n";
    double arr[2][3] = {{1, 0, 1}, {0, 1, 0}};
    DerivedMatrix M(arr, 2, 3);
    std::ofstream fout("out.txt");
    if (fout.is_open()) {
        fout << M;
        fout.close();
    }
    std::ifstream fin("out.txt");
    DerivedMatrix M1;
    if (fin) {
        fin >> M1;
        fin.close();
    }
    std::cout << M1;
    std::vector<double> result = M1.alternateRows();
    std::cout << "Alternate rows: ";
    for (int i = 0; i < result.size(); i++)
        std::cout << result[i] << " ";
    std::cout << "\n";
    DerivedMatrix M2(3, 3);
    std::cin >> M2;
    std::ofstream fout2("out2.txt");
    if (fout2.is_open()) {
        fout2 << M2;
        fout2.close();
    }
    std::ifstream fin2("out2.txt");
    DerivedMatrix M3(2, 2);
    if (fin2) {
        try {
            fin2 >> M3;
            fin2.close();
        }
        catch (Exception& e) {
            std::cout << "Exception while reading from file: " << e.what();
            fin2.close();
        }
    }
    std::cout << M3;
    char c1;
    std::cin >> c1;
    return 0;
}
