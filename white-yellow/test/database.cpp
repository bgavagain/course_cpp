#include "database.h"
#include <sstream>

using namespace std;

void Database::Add(const Date& date, const std::string& event) {
    auto& el = db[date];
    if (find(el.begin(), el.end(), event) == el.end()) {
        el.push_back(event);
    }
}

ostream& Database::Print(ostream& os) {
    for (const auto& d : db) {
        for (const auto& e : d.second) {
            os << d.first << " " << e << endl;
        }
    }

    return os;
}

string Database::Last(const Date& date) {
    auto it = db.upper_bound(date);

    if (it == db.begin() || (--it)->second.size() == 0) {
        return "No entries";
    }
    else {
        ostringstream os;
        os << it->first << " " << *it->second.rbegin();
        return os.str();
    }
}
