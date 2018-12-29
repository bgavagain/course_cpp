#include "date.h"
#include <exception>
#include <tuple>
#include <iomanip>

using namespace std;

ostream& operator<< (ostream& os, const Date& date) {
    os.fill('0');
    os << setw(4) << date.year_ << "-" 
       << setw(2) << date.month_ << "-" 
       << setw(2) <<date.day_;
    return os;
}

bool operator< (const Date& lhs, const Date& rhs) {
    return tie(lhs.year_, lhs.month_, lhs.day_) < 
        tie(rhs.year_, rhs.month_, rhs.day_);
}

Date ParseDate(istream& is) {
    int y = 0, m = 0, d = 0;
    is >> y;
    is.ignore(1);
    is >> m;
    is.ignore(1);
    is >> d;

    if ((m < 1 || m>12) || (d < 1 || d > 31)) {
        throw invalid_argument("");
    }
    return { y, m, d };
}