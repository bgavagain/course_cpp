//#include <iostream>
//#include <sstream>
//
//using namespace std;
//
//class Rational {
//public:
//    Rational() {
//        p = 0; q = 1;
//    }
//
//    Rational(int numerator, int denominator) {
//        if (numerator == 0) {
//            denominator = 1;
//        }
//        else if (numerator < 0 && denominator < 0) {
//            numerator = abs(numerator); denominator = abs(denominator);
//        }
//        else if (numerator >= 0 && denominator < 0) {
//            numerator = -numerator;  denominator = abs(denominator);
//        }
//
//        p = numerator;
//        q = denominator;
//        while (p > 0 && q > 0) {
//            if (p > q) {
//                p %= q;
//            }
//            else {
//                q %= p;
//            }
//        }
//
//        int div = p + q;
//        p = numerator / div;
//        q = denominator / div;
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
//    int den = lhs.Denominator() * rhs.Denominator();
//    int num = lhs.Numerator() * rhs.Numerator();
//    return Rational(num, den);
//}
//
//Rational operator*(const Rational& lhs, const Rational& rhs) {
//    int den = lhs.Denominator() * rhs.Denominator();
//    int num = lhs.Numerator() * rhs.Numerator();
//    return Rational(num, den);
//}
//
//Rational operator/(const Rational& lhs, const Rational& rhs) {
//    int den = lhs.Denominator() * rhs.Numerator();
//    int num = lhs.Numerator() * rhs.Denominator();
//    return Rational(num, den);
//}
//
//ostream& operator<<(ostream& stream, const Rational& rational) {
//    stream << rational.Numerator() << "/" << rational.Denominator();
//    return stream;
//}
//
//istream& operator>>(istream& stream, Rational& rational) {
//    int p, q;
//    if (!(stream >> p))
//        return stream;
//    stream.ignore(1); // '7/9'
//    if (!(stream >> q))
//        return stream;
//    rational = Rational(p, q);
//    return stream;
//}
//
//int main() {
//    {
//        ostringstream output;
//        output << Rational(-6, 8);
//        if (output.str() != "-3/4") {
//            cout << "Rational(-6, 8) should be written as \"-3/4\"" << endl;
//            return 1;
//        }
//    }
//
//    {
//        istringstream input("5/7");
//        Rational r;
//        input >> r;
//        bool equal = r == Rational(5, 7);
//        if (!equal) {
//            cout << "5/7 is incorrectly read as " << r << endl;
//            return 2;
//        }
//    }
//
//    {
//        istringstream input("5/7 10/8");
//        Rational r1, r2;
//        input >> r1 >> r2;
//        bool correct = r1 == Rational(5, 7) && r2 == Rational(5, 4);
//        if (!correct) {
//            cout << "Multiple values are read incorrectly: " << r1 << " " << r2 << endl;
//            return 3;
//        }
//
//        input >> r1;
//        input >> r2;
//        correct = r1 == Rational(5, 7) && r2 == Rational(5, 4);
//        if (!correct) {
//            cout << "Read from empty stream shouldn't change arguments: " << r1 << " " << r2 << endl;
//            return 4;
//        }
//    }
//
//    cout << "OK" << endl;
//    return 0;
//}