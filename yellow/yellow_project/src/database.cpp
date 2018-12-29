#include "database.h"
#include <sstream>

using namespace std;

void Database::Add(const Date& date, const std::string& event) {
	auto& id = idx[date];
	if(!id.count(event)) {
		auto& el = db[date];
		db[date].push_back(event);
		id.insert(event);
	}
}

ostream& Database::Print(ostream& os) const {
    for (const auto& d : db) {
        for (const auto& e : d.second) {
            os << d.first << " " << e << endl;
        }
    }

    return os;
}

string Database::Last(const Date& date) const {
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
