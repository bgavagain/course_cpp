#pragma once

#include "date.h"
#include <string>
#include <vector>
#include <map>

class Database {
public:
    void Add(const Date& date, const std::string& event);
    std::string Last(const Date& date);
    std::ostream& Print(std::ostream& os);

    template <class Func>
    std::vector<std::string> FindIf(Func predicate) { return {}; } //TODO:

    template <class Func> 
    int RemoveIf(Func predicate) { return -1; } //TODO:
private:
    std::map<Date, std::vector<std::string>> db;
};