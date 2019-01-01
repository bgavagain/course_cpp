//#include <iostream>
//#include <exception>
//using namespace std;
//
//int GreatestCommonDivisor(int a, int b) {
//    if (b == 0) {
//        return a;
//    }
//    else {
//        return GreatestCommonDivisor(b, a % b);
//    }
//}
//
//class Rational {
//public:
//    Rational() {  
//        numerator = 0;
//        denominator = 1;
//    }
//    Rational(int new_numerator, int new_denominator) {
//        if (new_denominator == 0)
//            throw invalid_argument("New denominator is zero");
//
//        const int gcd = GreatestCommonDivisor(new_numerator, new_denominator);
//        numerator = new_numerator / gcd;
//        denominator = new_denominator / gcd;
//
//        if (denominator < 0) {
//            denominator = -denominator;
//            numerator = -numerator;
//        }
//    }
//
//    int Numerator() const {
//        return numerator;
//    }
//
//    int Denominator() const {
//        return denominator;
//    }
//
//private:
//    int numerator;
//    int denominator;
//};
//
//
//bool operator == (const Rational& lhs, const Rational& rhs) {
//    return lhs.Numerator() == rhs.Numerator() &&
//        lhs.Denominator() == rhs.Denominator();
//}
//
//
//Rational operator + (const Rational& lhs, const Rational& rhs) {
//    return {
//        lhs.Numerator() * rhs.Denominator() + rhs.Numerator() * lhs.Denominator(),
//        lhs.Denominator() * rhs.Denominator()
//    };
//}
//
//Rational operator - (const Rational& lhs, const Rational& rhs) {
//    return {
//        lhs.Numerator() * rhs.Denominator() - rhs.Numerator() * lhs.Denominator(),
//        lhs.Denominator() * rhs.Denominator()
//    };
//}
//
//Rational operator * (const Rational& lhs, const Rational& rhs) {
//    return {
//        lhs.Numerator() * rhs.Numerator(),
//        lhs.Denominator() * rhs.Denominator()
//    };
//}
//
//Rational operator / (const Rational& lhs, const Rational& rhs) {
//    if (rhs.Numerator() == 0)
//        throw domain_error("Divider is zero");
//       
//    return lhs * Rational(rhs.Denominator(), rhs.Numerator());
//}
//
//istream& operator >> (istream& is, Rational& r) {
//    int n, d;
//    char c;
//    is >> n >> c >> d;
//    if (is && c == '/') {
//        r = Rational(n, d);
//    }
//    return is;
//}
//
//ostream& operator << (ostream& os, const Rational& r) {
//    return os << r.Numerator() << '/' << r.Denominator();
//}
//
//
//bool operator < (const Rational& lhs, const Rational& rhs) {
//    return (lhs - rhs).Numerator() < 0;
//}
//
//
//int main() {
//    int n1, d1, n2, d2;
//    char op;
//
//    cin >> n1; cin.ignore(1); cin >> d1;
//    cin >> op;
//    cin >> n2; cin.ignore(1); cin >> d2;
//
//    try {
//        Rational r1(n1, d1);
//        Rational r2(n2, d2);
//
//        if (op == '+') {
//            cout << (r1 + r2) << endl;
//        }
//        else if (op == '-') {
//            cout << (r1 - r2) << endl;
//        }
//        else if (op == '*') {
//            cout << (r1 * r2) << endl;
//        }
//        else if (op == '/') {
//            cout << (r1 / r2) << endl;
//        }
//    }
//    catch (invalid_argument&) {
//        cout << "Invalid argument" << endl;
//    }
//    catch (domain_error&) {
//        cout << "Division by zero" << endl;
//    }
//
//    return 0;
//}