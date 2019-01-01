#include <iostream>
#include <string>
#include <tuple>
#include <iomanip>
#include <sstream>
#include <map>
#include <set>
#include <vector>

using namespace std;

class Date {
public:
   Date(const string& date) {
       int y, m, d;
       stringstream ds(date);
       if (!(ds >> y))
           throw invalid_argument("Year not provided"); //TODO
       ds.ignore(1); 
       if (!(ds >> m))
           throw invalid_argument("Month not provided"); //TODO
       ds.ignore(1);
       if (!(ds >> d))
           throw invalid_argument("Day not provided"); //TODO

       if (m < 1 || m > 12)
           throw invalid_argument("Month value is invalid: "); //TODO
       if(d < 1 || d > 31)
           throw invalid_argument("Day value is invalid: "); //TODO

       year = y; month = m; day = d;
   }

   int GetYear() const { return year; }
   int GetMonth() const { return month; }
   int GetDay() const { return day; }
private:
   void SetDate(int y, int m, int d) {

   }

   int year;
   int month;
   int day;
};

bool operator<(const Date& lhs, const Date& rhs) {
   return make_tuple(lhs.GetYear(), lhs.GetMonth(), lhs.GetDay()) <
       make_tuple(rhs.GetYear(), rhs.GetMonth(), rhs.GetDay());
}

ostream& operator<<(ostream& stream, const Date& date) {
   stream.fill('0');
   stream << setw(4) << date.GetYear() << "-" 
          << setw(2) << date.GetMonth() << "-" 
          << setw(2) << date.GetDay();
   return stream;
}

class Database {
public:
   void AddEvent(const Date& date, const string& event) {
       recs[date].insert(event);
   }

   bool DeleteEvent(const Date& date, const string& event) {
       bool res = recs[date].count(event) > 0;
       recs[date].erase(event);
       return res;
   }

   int  DeleteDate(const Date& date) {
       int res = recs[date].size();
       recs.erase(date);
       return res;
   }

   vector<string> Find(const Date& date) const {
       if (recs.count(date) == 0) {
           return {};
       }
       vector<string> res;
       for (const auto& e : recs.at(date))
       {
           res.push_back(e);
       }
       return res;
   }

   void Print() const {
       for (const auto& r : recs) {
           for (const auto& e : r.second) {
               cout << r.first << " " << e << endl;
           }
       }
   }

private:
   map<Date, set<string>> recs;
};

int main() {
   Database db;

   try {
       string command;
       while (getline(cin, command)) {
           stringstream ss(command);
           string cmd, date, event;
           ss >> cmd;

           if (cmd == "Add") { //Add date event
               ss >> date >> event;
               db.AddEvent(date, event);
           }
           else if (cmd == "Del") { //Del date <event>
               ss >> date;
               if (ss >> event) {
                   auto cnt = db.DeleteDate(date);
                   cout << "Deleted " << cnt << " events" << endl;
               }
               else {
                   cout << (db.DeleteEvent(date, event) ? "Deleted successfully" : "Event not found") << endl;
               }
           }
           else if (cmd == "Find") { //Find date
               ss >> date;
               const auto& st = db.Find(date);
               for (const auto& e : st) {
                   cout << date << " " << e << endl;
               }
           }
           else if (cmd == "Print") { //Print
               db.Print();
           }
           else {
               cout << "Unknown command : " << command << endl;
           }
       }
   }
   catch (invalid_argument& e) {
       cout << e.what() << endl;
   }

   return 0;
}