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

// empty if unknown
string FindNameByYear(const map<int, string>& names, int year) {
    string name;

    for (const auto& item : names) {
        if (item.first <= year) {
            name = item.second;
        }
        else {
            break;
        }
    }

    return name;
}

class Person {
public:
    void ChangeFirstName(int year, const string& first_name) {
        first_names[year] = first_name;
    }
    void ChangeLastName(int year, const string& last_name) {
        last_names[year] = last_name;
    }
    string GetFullName(int year) {
        const string first_name = FindNameByYear(first_names, year);
        const string last_name = FindNameByYear(last_names, year);

        if (first_name.empty() && last_name.empty()) {
            return "Incognito";
        }
        else if (first_name.empty()) {
            return last_name + " with unknown first name";
        }
        else if (last_name.empty()) {
            return first_name + " with unknown last name";
        }
        else {
            return first_name + " " + last_name;
        }
    }

private:
    map<int, string> first_names;
    map<int, string> last_names;
};

void TestIncognito() {
    Person p;
    AssertEqual(p.GetFullName(1999), "Incognito", "assert Incognito");
}

void TestFirstNameOnly() {
    Person p;
    p.ChangeFirstName(1995, "Klava");
    AssertEqual(p.GetFullName(1999), "Klava with unknown last name", "assert first name only");
}

void TestLastNameOnly() {
    Person p;
    p.ChangeLastName(1995, "Boobls");
    AssertEqual(p.GetFullName(1999), "Boobls with unknown first name", "assert last name only");
}

void TestBothNames() {
    Person p;
    p.ChangeLastName(1995, "Klava");
    p.ChangeLastName(1996, "Boobls");
    AssertEqual(p.GetFullName(1999), "Klava Boobls", "assert both names");
}

void TestFirstNameChange() {
    Person p;
    p.ChangeFirstName(1995, "Klava");
    p.ChangeLastName(1996, "Boobls");
    p.ChangeFirstName(1999, "Sveta");
    AssertEqual(p.GetFullName(1999), "Sveta Boobls", "assert first name change");
}

void TestLastNameChange() {
    Person p;
    p.ChangeFirstName(1995, "Klava");
    p.ChangeLastName(1996, "Boobls");
    p.ChangeLastName(1999, "Sveta");
    AssertEqual(p.GetFullName(1999), "Sveta Sveta", "assert last name change");
}

void TestBothNamesChange() {
    Person p;
    p.ChangeFirstName(1995, "Klava");
    p.ChangeLastName(1996, "Boobls");
    p.ChangeFirstName(1998, "Klavdia");
    p.ChangeLastName(1999, "Sveta");
    p.ChangeLastName(2000, "Booblsova");
    AssertEqual(p.GetFullName(1999), "Klavdia Sveta", "assert both names change");
}

int main3() {
    TestRunner runner;
    runner.RunTest(TestIncognito, "TestIncognito");
    runner.RunTest(TestFirstNameOnly, "TestFirstNameOnly");
    runner.RunTest(TestLastNameOnly, "TestLastNameOnly");
    runner.RunTest(TestBothNames, "TestBothNames");
    runner.RunTest(TestFirstNameChange, "TestFirstNameChange");
    runner.RunTest(TestLastNameChange, "TestLastNameChange");
    runner.RunTest(TestBothNamesChange, "TestBothNamesChange");

    return 0;
}