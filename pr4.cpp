#include <iostream>
#include <cstring> 
using namespace std;

class BaseString
{
protected:
    char* p;
    int len;
    int capacity;

public:
    BaseString(char* ptr)
    {
        cout<<"\nBase Constructor 1\n";
        len = strlen(ptr) + 1;
        capacity = 256;
        p = new char[capacity];
        for(int i=0; i<len; i++)
        {
            p[i] = ptr[i];
        }
        p[len] = '\0';
    }

    BaseString(int Capacity = 256)
    {
        cout<<"\nBase Constructor 0\n";
        capacity = Capacity;
        p = new char[capacity];
        len = 0;
    }

    BaseString(const BaseString& other) // Copy constructor
    {
        cout << "\nBase Copy Constructor\n";
        len = other.len;
        capacity = other.capacity;
        p = new char[capacity];
        for(int i = 0; i < len; i++)
        {
            p[i] = other.p[i];
        }
        p[len] = '\0';
    }

    ~BaseString() // Destructor
    {
        cout << "\nBase Destructor\n";
        delete[] p;
    }

    BaseString& operator=(const BaseString& other) // Assignment operator
    {
        cout << "\nBase Assignment Operator\n";
        if (this != &other)
        {
            delete[] p;
            len = other.len;
            capacity = other.capacity;
            p = new char[capacity];
            for(int i = 0; i < len; i++)
            {
                p[i] = other.p[i];
            }
            p[len] = '\0';
        }
        return *this;
    }

    int compare(BaseString other)
    {
        char* ptr1 = p;
        char* ptr2 = other.p;
        while(*ptr1 != '\0' && *ptr2 != '\0')
        {
            if(*ptr1 != *ptr2)
                return 0;
            ptr1++;
            ptr2++;
        }
        if(*ptr1 == '\0' && *ptr2 == '\0')
            return 1;
        else
            return 0;
    }
};

class DerivedString : public BaseString
{
public:
    DerivedString(char* ptr) : BaseString(ptr)
    {
        cout << "\nDerived Constructor 1\n";
    }

    DerivedString(int Capacity = 256) : BaseString(Capacity)
    {
        cout << "\nDerived Constructor 0\n";
    }

    DerivedString(const DerivedString& other) : BaseString(other) // Copy constructor
    {
        cout << "\nDerived Copy Constructor\n";
    }

    ~DerivedString() // Destructor
    {
        cout << "\nDerived Destructor\n";
    }

    DerivedString& operator=(const DerivedString& other) // Assignment operator
    {
        cout << "\nDerived Assignment Operator\n";
        BaseString::operator=(other);
        return *this;
    }
};

int main()
{
    BaseString bs1("Hello");
    BaseString bs2("World");
    DerivedString ds1("Hello");
    DerivedString ds2("World");

    cout << "\nComparing bs1 and bs2: " << bs1.compare(bs2);
    cout << "\nComparing ds1 and ds2: " << ds1.compare(ds2);

    return 0;
}
