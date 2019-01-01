#include "test_runner.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <set>
#include <utility>
#include <vector>
#include <unordered_set>

using namespace std;

template <typename T>
class PriorityCollection {
public:
	using Id = size_t; //TODO:
	
	PriorityCollection() {};

	//Add zero-priority object
	Id Add(T object) { return 0; }

	template <typename ObjInputIt, typename IdOutputIt>
	void Add(ObjInputIt range_begin, ObjInputIt range_end,
		IdOutputIt ids_begin) {}

	bool IsValid(Id id) const { return false; }

	const T& Get(Id id) const { return data; } //TODO:

	void Promote(Id id) {}

	pair<const T&, int> GetMax() const { return {}; };

	pair<T, int> PopMax() { return {}; }

private:
	set<unordered_set<T>> data;

};


class StringNonCopyable : public string {
public:
	using string::string;  // Allows using string ctor
	StringNonCopyable(const StringNonCopyable&) = delete;
	StringNonCopyable(StringNonCopyable&&) = default;
	StringNonCopyable& operator=(const StringNonCopyable&) = delete;
	StringNonCopyable& operator=(StringNonCopyable&&) = default;
};

void TestNoCopy() {
	PriorityCollection<StringNonCopyable> strings;
	const auto white_id = strings.Add("white");
	const auto yellow_id = strings.Add("yellow");
	const auto red_id = strings.Add("red");

	strings.Promote(yellow_id);
	for (int i = 0; i < 2; ++i) {
		strings.Promote(red_id);
	}
	strings.Promote(yellow_id);
	{
		const auto item = strings.PopMax();
		ASSERT_EQUAL(item.first, "red");
		ASSERT_EQUAL(item.second, 2);
	}
	{
		const auto item = strings.PopMax();
		ASSERT_EQUAL(item.first, "yellow");
		ASSERT_EQUAL(item.second, 2);
	}
	{
		const auto item = strings.PopMax();
		ASSERT_EQUAL(item.first, "white");
		ASSERT_EQUAL(item.second, 0);
	}
}

int main() {
	TestRunner tr;
	RUN_TEST(tr, TestNoCopy);
	return 0;
}