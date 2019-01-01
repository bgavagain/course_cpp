#include "test_runner.h"

#include <iostream>
#include <deque>

using namespace std;


int main() {
    int n, k;
    cin >> n >> k;

    deque<string> deq;
    deq.push_back(to_string(n));
    string last_op;

    for (int i = 0; i < k; ++i) {
        string op; int z;
        cin >> op >> z;
        if ((last_op == "-" || last_op == "+") && (op == "*" || op == "/")) {
            deq.push_front("(");
            deq.push_back(")");
        }
        deq.push_back(op);
        deq.push_back(to_string(z));
        last_op = op;
    }

    for (const auto& e : deq) {
        string space = (e == "*" || e == "/" || e == "-" || e == "+") ? " " : "";
        cout << space << e << space;
    }
    cout << endl;
}