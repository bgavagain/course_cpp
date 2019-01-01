#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <fstream>

using namespace std;

int main() {
    ifstream in("input.txt");
    ofstream out("output.txt");
    if (in.is_open() && out.is_open()) {
        string line;
        while (getline(in, line)) {
            out << line << endl;
        }
    }

    return 0;
}