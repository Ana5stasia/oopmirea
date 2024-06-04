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
};
int main()
{
    Char a('A');
    Char b('b');
    cout << (a<b) << endl;
    return 0;
}
