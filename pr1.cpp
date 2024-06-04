#include <iostream>
#include <locale>
using namespace std;

class Char {
    private:
     char letter;

    public:
     Char(char element)
     {
     letter=element;
     }
     char getLetter(){return letter;}
     void setLetter(char a){letter=a;}
     bool operator< (Char a)
     {
        return tolower(letter, locale())<tolower(a.letter, locale());
     }
     // Friend function for comparison with integer
     friend bool operator< (int a, Char b)
     {
        return a < static_cast<int>(b.letter);
     }
};

int main()
{
    Char a('A');
    Char b('b');
    cout << (a<b) << endl;
    cout << (65 < a) << endl; // 65 is the ASCII value for 'A'
    return 0;
}

