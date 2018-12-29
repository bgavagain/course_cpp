#pragma once

#include "date.h"
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <set>
#include <algorithm>

class Database {
public:
    void Add(const Date& date, const std::string& event);
    std::string Last(const Date& date) const;
    std::ostream& Print(std::ostream& os) const ;

    template <class Func>
	std::vector<std::string> FindIf(Func predicate) const {
		std::vector<std::string> res;

		for (const auto& d : db) {
			for (const auto& e : d.second) {
				if (predicate(d.first, e)) {
					std::ostringstream os;
					os << d.first << " " << e;
					res.push_back(os.str());
				}
			}
		}

		return res;
	}

	
    template <class Func> 
	int RemoveIf(Func predicate) {
		int res = 0;
		for (auto dbt = db.begin(); dbt != db.end(); ) {
			auto& val = *dbt;
			auto& date = val.first;
			auto& e = val.second;

			auto it = std::stable_partition(e.begin(), e.end(), 
				[predicate, date](const std::string& s) ->bool { return predicate(date, s) == true; });
			res += static_cast<int>(std::distance(e.begin(), it));
			e.erase(e.begin(), it);

			if (e.empty()) dbt = db.erase(dbt);
			else ++dbt;
		}

		for (auto idt = idx.begin(); idt != idx.end(); ) {
			auto& i = *idt;
			for (auto it = i.second.begin(); it != i.second.end(); ) {
				if (predicate(i.first, *it)) it = i.second.erase(it);
				else ++it;
			}
			if (i.second.empty()) idt = idx.erase(idt);
			else ++idt;
		}

		return res;
	}
private:
    std::map<Date, std::vector<std::string>> db;
	std::map<Date, std::set<std::string>>idx;
};