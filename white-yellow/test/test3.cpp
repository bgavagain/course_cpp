#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>

using namespace std;

template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) {
    if ((range_end - range_begin) < 2)
        return;

    int step = (range_end - range_begin) / 3;
    RandomIt m1 = range_begin + step;
    RandomIt m2 = m1 + step;
    
    vector<typename RandomIt::value_type> lhs(range_begin, m1);
    vector<typename RandomIt::value_type> mhs(m1, m2);
    vector<typename RandomIt::value_type> rhs(m2, range_end);
    vector<typename RandomIt::value_type> tmp(range_begin, range_begin);
    MergeSort(lhs.begin(), lhs.end());
    MergeSort(mhs.begin(), mhs.end());
    MergeSort(rhs.begin(), rhs.end());

    merge(lhs.begin(), lhs.end(), mhs.begin(), mhs.end(), back_inserter(tmp));
    merge(tmp.begin(), tmp.end(), rhs.begin(), rhs.end(), range_begin);
}


int main() {
    vector<int> v = { 6, 4, 7, 6, 4, 4, 0, 1, 5 };
    MergeSort(begin(v), end(v));
    for (int x : v) {
        cout << x << " ";
    }
    cout << endl;
    return 0;
}