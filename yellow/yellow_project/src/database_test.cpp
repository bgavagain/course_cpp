#include "test_runner.h"
#include "date.h"
#include "database.h"
#include "condition_parser.h"

#include <sstream>
using namespace std;

void PrintVector(const vector<string>& entries) {
	for (const auto& entry : entries) {
		cout << entry << endl;
	}
	cout << "Found " << entries.size() << " entries" << endl;
}

void Add(Database& db, const string& date, const string& event) {
	istringstream is{ date };
	db.Add(ParseDate(is), event);
}

vector<string> Find(Database& db, const string& cond) {
	istringstream is{ cond };
	auto condition = ParseCondition(is);
	auto predicate = [condition](const Date& date, const string& event) {
		return condition->Evaluate(date, event);
	};
	return db.FindIf(predicate);
}

int Del(Database& db, const string& cond) {
	istringstream is{ cond };
	auto condition = ParseCondition(is);
	auto predicated = [condition](const Date& date, const string& event) {
		return condition->Evaluate(date, event);
	};
	return db.RemoveIf(predicated);
}

void TestDatabase() {
	Database db;
	Add(db, "2017-01-01", "Holiday");
	Add(db, "2017-01-01", "Holiday");
	Add(db, "2017-03-08", "Holiday");
	Add(db, "2017-01-01", "New Year");

	PrintVector(Find(db, "event != \"working day\""));
	cout << "---" << endl;

	Add(db, "2017-05-09", "Holiday");
	db.Print(cout);
	cout << "---" << endl;

	PrintVector(Find(db, "event == \"Holiday\""));
	cout << "---" << endl;

	cout << "empty condition below" << endl;
	PrintVector(Find(db, ""));
	cout << "---" << endl;

	cout << "Removed date AND event: " << Del(db, "date == 2017-03-08 AND event == \"Holiday\"") << endl;
	db.Print(cout);

	cout << db.Last({ 2017, 3, 8 }) << endl;

	//cout << "Removed event only: " << Del(db, "event == \"Holiday\"") << endl;
	//db.Print(cout);

	cout << "Removed all: " << Del(db, "") << endl;
	db.Print(cout);
}