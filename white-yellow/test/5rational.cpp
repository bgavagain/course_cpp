//#include <iostream>
//#include <map>
//#include <set>
//#include <vector>
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
//bool operator<(const Rational& lhs, const Rational& rhs) {
//    int ln = lhs.Numerator() * rhs.Denominator();
//    int rn = rhs.Numerator() * lhs.Denominator();
//    return ln < rn;
//}
//
//int main() {
//    {
//        const set<Rational> rs = { {1, 2}, {1, 25}, {3, 4}, {3, 4}, {1, 2} };
//        if (rs.size() != 3) {
//            cout << "Wrong amount of items in the set" << endl;
//            return 1;
//        }
//
//        vector<Rational> v;
//        for (auto x : rs) {
//            v.push_back(x);
//        }
//        if (v != vector<Rational>{ {1, 25}, { 1, 2 }, { 3, 4 }}) {
//            cout << "Rationals comparison works incorrectly" << endl;
//            return 2;
//        }
//    }
//
//    {
//        map<Rational, int> count;
//        ++count[{1, 2}];
//        ++count[{1, 2}];
//
//        ++count[{2, 3}];
//
//        if (count.size() != 2) {
//            cout << "Wrong amount of items in the map" << endl;
//            return 3;
//        }
//    }
//
//    cout << "OK" << endl;
//    return 0;
//}