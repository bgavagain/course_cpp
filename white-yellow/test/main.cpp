#include "database.h"
#include "date.h"
#include "condition_parser.h"
#include "node.h"
#include "test_runner.h"

#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;

string ParseEvent(istream& is) {
    string res;

    while (is.peek() == ' ') { is.ignore(1); }
    getline(is, res);
    
    return res;
}

void TestAll();

int main() {
    TestAll();

    Database db;

    for (string line; getline(cin, line); ) {
        istringstream is(line);

        string command;
        is >> command;
        if (command == "Add") {
            const auto date = ParseDate(is);
            const auto event = ParseEvent(is);
            db.Add(date, event);
        }
        else if (command == "Print") {
            db.Print(cout);
        }
        else if (command == "Del") {
            auto condition = ParseCondition(is);
            auto predicate = [condition](const Date& date, const string& event) {
                return condition->Evaluate(date, event);
            };
            int count = db.RemoveIf(predicate);
            cout << "Removed " << count << " entries" << endl;
        }
        else if (command == "Find") {
            auto condition = ParseCondition(is);
            auto predicate = [condition](const Date& date, const string& event) {
                return condition->Evaluate(date, event);
            };

            const auto entries = db.FindIf(predicate);
            for (const auto& entry : entries) {
                cout << entry << endl;
            }
            cout << "Found " << entries.size() << " entries" << endl;
        }
        else if (command == "Last") {
            try {
                cout << db.Last(ParseDate(is)) << endl;
            }
            catch (invalid_argument&) {
                cout << "No entries" << endl;
            }
        }
        else if (command == "Exit") {
            break;
        }
        else if (command.empty()) {
            continue;
        }
        else {
            throw logic_error("Unknown command: " + command);
        }
    }

    return 0;
}

void TestParseEvent() {
    {
        istringstream is("event");
        AssertEqual(ParseEvent(is), "event", "Parse event without leading spaces");
    }
    {
        istringstream is("   sport event ");
        AssertEqual(ParseEvent(is), "sport event ", "Parse event with leading spaces");
    }
    {
        istringstream is("  first event  \n  second event");
        vector<string> events;
        events.push_back(ParseEvent(is));
        events.push_back(ParseEvent(is));
        AssertEqual(events, vector<string>{"first event  ", "second event"}, "Parse multiple events");
    }
}

void TestAddLast() {
    Database db;
    {
        istringstream is("1-1-1    asdf");
        const auto date = ParseDate(is);
        const auto event = ParseEvent(is);
        db.Add(date, event); 
    }
    {
        istringstream is("0-1-1");
        AssertEqual(db.Last(ParseDate(is)), "No entries", "No last date");
    }
    {
        istringstream is("1-1-1");
        AssertEqual(db.Last(ParseDate(is)), "0001-01-01 asdf", "Exact last date");
    }
    {
        istringstream is("2-1-1");
        AssertEqual(db.Last(ParseDate(is)), "0001-01-01 asdf", "Smaller last date");
    }
}

void TestAll() {
    TestRunner tr;
    tr.RunTest(TestParseEvent, "TestParseEvent");

    tr.RunTest(TestAddLast, "TestAddLast");

    tr.RunTest(TestParseCondition, "TestParseCondition");
}