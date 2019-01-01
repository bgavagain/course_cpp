#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

template <class T>
ostream& operator << (ostream& os, const vector<T>& s) {
    os << "{";
    bool first = true;
    for (const auto& x : s) {
        if (!first) {
            os << ", ";
        }
        first = false;
        os << x;
    }
    return os << "}";
}

template <class T>
ostream& operator << (ostream& os, const set<T>& s) {
    os << "{";
    bool first = true;
    for (const auto& x : s) {
        if (!first) {
            os << ", ";
        }
        first = false;
        os << x;
    }
    return os << "}";
}

template <class K, class V>
ostream& operator << (ostream& os, const map<K, V>& m) {
    os << "{";
    bool first = true;
    for (const auto& kv : m) {
        if (!first) {
            os << ", ";
        }
        first = false;
        os << kv.first << ": " << kv.second;
    }
    return os << "}";
}

template<class T, class U>
void AssertEqual(const T& t, const U& u, const string& hint = {}) {
    if (t != u) {
        ostringstream os;
        os << "Assertion failed: " << t << " != " << u;
        if (!hint.empty()) {
            os << " hint: " << hint;
        }
        throw runtime_error(os.str());
    }
}

void Assert(bool b, const string& hint) {
    AssertEqual(b, true, hint);
}

class TestRunner {
public:
    template <class TestFunc>
    void RunTest(TestFunc func, const string& test_name) {
        try {
            func();
            cerr << test_name << " OK" << endl;
        }
        catch (exception& e) {
            ++fail_count;
            cerr << test_name << " fail: " << e.what() << endl;
        }
        catch (...) {
            ++fail_count;
            cerr << "Unknown exception caught" << endl;
        }
    }

    ~TestRunner() {
        if (fail_count > 0) {
            cerr << fail_count << " unit tests failed. Terminate" << endl;
            exit(1);
        }
    }

private:
    int fail_count = 0;
};

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

void TestDefault() {
    Rational r;
    Assert(r.Numerator() == 0 && r.Denominator() == 1, "assert default");
 
    r = { 4, 8 };
    Assert(r.Numerator() == 1 && r.Denominator() == 2, "assert contract");

    r = { 2, -9 };
    Assert(r.Numerator() < 0 && r.Denominator() > 0, "assert negative");

    r = { -2, -9 };
    Assert(r.Numerator() > 0 && r.Denominator() > 0, "assert positive");
    r = { 2, 9 };
    Assert(r.Numerator() > 0 && r.Denominator() > 0, "assert positive 2");

    r = { 0, 9 };
    Assert(r.Numerator() == 0 && r.Denominator() == 1, "assert zero numerator");
}

int main() {
    TestRunner runner;
    runner.RunTest(TestDefault, "TestDefault");
    return 0;
}