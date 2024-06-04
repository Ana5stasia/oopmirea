#include <iostream>

using namespace std;

class A1 {
protected:
	int a1;
public:
	A1(int x) : a1(x) {}
	A1() : a1(0) {}

	virtual void print() {
		cout << "A1" << ";\n";
	}

	virtual void show() {
		cout << "a1 = " << a1 << ";\n";
	}
};

class A2 {
protected:
	int a2;
public:
	A2(int x) : a2(x) {}
	A2() : a2(0) {}

	virtual void print() {
		cout << "A2" << ";\n";
	}

	virtual void show() {
		cout << "a2 = " << a2 << ";\n";
	}
};

class B1 : public virtual A1, public virtual A2 {
protected:
	int b1;
public:
	B1(int b, int a1, int a2) : b1(b), A1(a1), A2(a2) {}

	virtual void print() {
		cout << "B1" << ";\n";
	}

	virtual void show() {
		cout << "a1 = " << a1 << " a2 = " << a2 << " b1 = " << b1 << ";\n";
	}
};

class B2 : public virtual A1, public virtual A2 {
protected:
	int b2;
public:
	B2(int b, int a1, int a2) : b2(b), A1(a1), A2(a2) {}

	virtual void print() {
		cout << "B2" << ";\n";
	}

	virtual void show() {
		cout << "a1 = " << a1 << " a2 = " << a2 << " b2 = " << b2 << ";\n";
	}
};

class B3 : public virtual A1, public virtual A2 {
protected:
	int b3;
public:
	B3(int b, int a1, int a2) : b3(b), A1(a1), A2(a2) {}

	virtual void print() {
		cout << "B3" << ";\n";
	}

	virtual void show() {
		cout << "a1 = " << a1 << " a2 = " << a2 << " b3 = " << b3 << ";\n";
	}
};

class C1 : public virtual B1, public virtual B2, public virtual B3 {
protected:
	int c1;
public:
	C1(int c, int b1, int b2, int b3) : c1(c), B1(b1, a1, a2), B2(b2, a1+1, a2+2), B3(b3, a1+3, a2+4) {}

	virtual void print() {
		cout << "C1";
	}

	virtual void show() {
		cout << "a1 = " << a1 << " a2 = " << a2 << " b1 = " << b1 << " b2 = " << b2 << " b3 = " << b3 << " c1 = " << c1 <<  ";\n";
	}
};

int main() {
	A1* v = new C1(1, 2, 3, 4);
	v->show();
	cout << " ";
	v->print();
	cout << "\n";

	v = new B1(2, 4, 5);
	v->show();
	cout << " ";
	v->print();
	cout << "\n";

	v = new B2(3, 4, 5);
	v->show();
	cout << " ";
	v->print();
	cout << "\n";
}
