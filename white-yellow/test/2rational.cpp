//#include <iostream>
//using namespace std;
//
//class Rational {
//public:
//    Rational() {
//        p = 0; q = 1;
//    }
//
//    static int nod(int p, int q) {
//        while (p > 0 && q > 0) {
//            if (p > q) {
//                p %= q;
//            }
//            else {
//                q %= p;
//            }
//        }
//
//        return p + q;
//    }
//
//    Rational(int numerator, int denominator) {
//        int nd = nod(abs(numerator), abs(denominator));
//
//        p = numerator / nd;
//        q = denominator / nd;
//
//        if (p < 0 && q < 0) {
//            p = abs(p); q = abs(q);
//        }
//        else if (p >= 0 && q < 0) {
//            p = -p; q = abs(q);
//        }
//    }
//
//    int Numerator() const {
//        return p;
//    }
//
//    int Denominator() const {
//        return q;
//    }
//
//private:
//    int p;
//    int q;
//};
//
//bool operator==(const Rational& lhs, const Rational& rhs) {
//    int ln = lhs.Numerator(); int rn = rhs.Numerator();
//    int ld = lhs.Denominator(); int rd = rhs.Denominator();
//    return (ln == rn && ld == rd);
//}
//
//Rational operator+(const Rational& lhs, const Rational& rhs) {
//    int den = lhs.Denominator() * rhs.Denominator();
//    int num = lhs.Numerator() * rhs.Denominator() + rhs.Numerator() * lhs.Denominator();
//    return Rational(num, den);
//}
//
//Rational operator-(const Rational& lhs, const Rational& rhs) {
//    //int den = lhs.Denominator() * rhs.Denominator();
//    int num = lhs.Numerator() * rhs.Denominator() - rhs.Numerator() * lhs.Denominator();
//    return Rational(num, den);
//}
//
//int main() {
//    {
//        Rational r1(4, 6);
//        Rational r2(2, 3);
//        bool equal = r1 == r2;
//        if (!equal) {
//            cout << "4/6 != 2/3" << endl;
//            return 1;
//        }
//    }
//
//    {
//        Rational a(2, 3);
//        Rational b(4, 3);
//        Rational c = a + b;
//        bool equal = c == Rational(2, 1);
//        if (!equal) {
//            cout << "2/3 + 4/3 != 2" << endl;
//            return 2;
//        }
//    }
//
//    {
//        Rational a(5, 7);
//        Rational b(2, 9);
//        Rational c = a - b;
//        bool equal = c == Rational(31, 63);
//        if (!equal) {
//            cout << "5/7 - 2/9 != 31/63" << endl;
//            return 3;
//        }
//    }
//
//    cout << "OK" << endl;
//    return 0;
//}