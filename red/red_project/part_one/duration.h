#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <chrono>

using namespace std;
using namespace chrono;

struct TotalDuration {
	string message;
	steady_clock::duration value;
	explicit TotalDuration(const string& msg = "")
		: message(msg + ": ")
		, value(0)
	{
	}
	~TotalDuration() {
		ostringstream os;
		os << message
			<< duration_cast<milliseconds>(value).count()
			<< " ms" << endl;
		cerr << os.str();
	}
};

class AddDuration {
public:
	explicit AddDuration(steady_clock::duration& dest)
		: add_to(dest)
		, start(steady_clock::now())
	{
	}
	explicit AddDuration(TotalDuration& dest)
		: AddDuration(dest.value)
	{
	}
	~AddDuration() {
		add_to += steady_clock::now() - start;
	}
private:
	steady_clock::duration& add_to;
	steady_clock::time_point start;
};

#define UNIQ_ID_IMPL_2(lineno) _a_local_var_##lineno
#define UNIQ_ID_IMPL(lineno) UNIQ_ID_IMPL_2(lineno)
#define UNIQ_ID UNIQ_ID_IMPL(__LINE__)

#define ADD_DURATION(value) \
	AddDuration UNIQ_ID{value};

//// Usage examples
//istream& ReadLine(istream& input, string& s, TotalDuration& dest) {
//	ADD_DURATION(dest);
//	return getline(input, s);
//}
//
//int main() {
//	TotalDuration read("Total read");
//	TotalDuration parse("Total parse");
//	for (string line; ReadLine(cin, line, read); ) {
//		ADD_DURATION(parse);
//		const auto words = SplitIntoWordsView(line);
//	}