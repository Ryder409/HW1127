#include <iostream>
#include <cmath>
using namespace std;

class Polynomial;

class Term {
    friend class Polynomial;
private:
    float coef;
    int exp;

public:
    float getCoef() const { return coef; }
    int getExp() const { return exp; }
    void setCoef(float c) { coef = c; }
    void setExp(int e) { exp = e; }
};

class Polynomial {
private:
    Term* termArray;
    int capacity;
    int terms;

    void Resize(int newCapacity) {
        Term* newArray = new Term[newCapacity];
        for (int i = 0; i < terms; ++i) {
            newArray[i] = termArray[i];
        }
        delete[] termArray;
        termArray = newArray;
        capacity = newCapacity;
    }

    void SortTerms() {
        for (int i = 0; i < terms - 1; ++i) {
            for (int j = i + 1; j < terms; ++j) {
                if (termArray[i].exp < termArray[j].exp) {
                    swap(termArray[i], termArray[j]);
                }
            }
        }
    }

public:
    Polynomial() : termArray(new Term[10]), capacity(10), terms(0) {}
    ~Polynomial() { delete[] termArray; }

    void AddTerm(float coef, int exp) {
        for (int i = 0; i < terms; ++i) {
            if (termArray[i].getExp() == exp) {
                termArray[i].setCoef(termArray[i].getCoef() + coef);
                if (fabs(termArray[i].getCoef()) < 1e-6) {
                    for (int j = i; j < terms - 1; ++j) {
                        termArray[j] = termArray[j + 1];
                    }
                    --terms;
                }
                SortTerms();
                return;
            }
        }
        if (terms == capacity) {
            Resize(2 * capacity);
        }
        termArray[terms].setCoef(coef);
        termArray[terms].setExp(exp);
        ++terms;
        SortTerms();
    }

    Polynomial operator+(const Polynomial& other) const {
        Polynomial result;

        int i = 0, j = 0;
        while (i < terms || j < other.terms) {
            if (i < terms && (j >= other.terms || termArray[i].getExp() > other.termArray[j].getExp())) {
                result.AddTerm(termArray[i].getCoef(), termArray[i].getExp());
                ++i;
            }
            else if (j < other.terms && (i >= terms || termArray[i].getExp() < other.termArray[j].getExp())) {
                result.AddTerm(other.termArray[j].getCoef(), other.termArray[j].getExp());
                ++j;
            }
            else {
                result.AddTerm(termArray[i].getCoef() + other.termArray[j].getCoef(), termArray[i].getExp());
                ++i;
                ++j;
            }
        }

        return result;
    }

    friend istream& operator>>(istream& in, Polynomial& poly);
    friend ostream& operator<<(ostream& out, const Polynomial& poly);
};

istream& operator>>(istream& in, Polynomial& poly) {
    cout << "Enter the number of terms: ";
    int termCount;
    in >> termCount;

    for (int i = 0; i < termCount; ++i) {
        float coef;
        int exp;
        cout << "Enter the coefficient and exponent for term " << (i + 1) << ": ";
        in >> coef >> exp;
        poly.AddTerm(coef, exp);
    }

    return in;
}

ostream& operator<<(ostream& out, const Polynomial& poly) {
    if (poly.terms == 0) {
        out << "0";
        return out;
    }

    for (int i = 0; i < poly.terms; ++i) {
        float coef = poly.termArray[i].getCoef();
        int exp = poly.termArray[i].getExp();

        if (i == 0) {
            if (coef < 0) out << "-";
            if (fabs(coef) != 1 || exp == 0) out << fabs(coef);
        }
        else {
            if (coef > 0) out << " + ";
            else out << " - ";
            if (fabs(coef) != 1 || exp == 0) out << fabs(coef);
        }

        if (exp > 0) {
            out << "x";
            if (exp > 1) out << "^" << exp;
        }
    }

    return out;
}

int main() {
    Polynomial p1, p2;

    cout << "Enter polynomial 1:" << endl;
    cin >> p1;

    cout << "Enter polynomial 2:" << endl;
    cin >> p2;

    Polynomial p3 = p1 + p2;

    cout << "Polynomial 1: " << p1 << endl;
    cout << "Polynomial 2: " << p2 << endl;
    cout << "Sum of polynomials: " << p3 << endl;

    return 0;
}
