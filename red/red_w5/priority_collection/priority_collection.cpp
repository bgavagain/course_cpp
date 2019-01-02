#include "test_runner.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <set>
#include <utility>
#include <vector>
#include <functional>
#include <complex>

using namespace std;

template <typename T>
class PriorityCollection {
public:
	using Id = int;

	PriorityCollection() {};

	Id Add(T object) { 
		++cnt;
		data_[0].insert({ cnt, move(object) });
		idx[cnt] = 0;
		return cnt;
	}

	template <typename ObjInputIt, typename IdOutputIt>
	void Add(ObjInputIt range_begin, ObjInputIt range_end, IdOutputIt ids_begin) {
		for (auto it = range_begin; it != range_end; ++it) {
			Add(*it);
		}
	}

	bool IsValid(Id id) const {
		return idx.count(id);
	}

	const T& Get(Id id) const { 
		return data_[idx.at(id)].at(id);
	}

	void Promote(Id id) {
		auto& prev = data_[idx.at(id)];
		T obj = move(prev.at(id));
		prev.erase(id);
		++idx.at(id);
		auto nprio = idx.at(id);
		data_[nprio].insert({ id, move(obj) });
	}

	pair<const T&, int> GetMax() const {
		auto it = prev(idx.end());
		auto id = (*it).first;
		auto prio = (*it).second;

		return { data_.at(prio).at(id), prio};
	};

	pair<T, int> PopMax() { 
		auto it = prev(idx.end());
		auto id = (*it).first;
		auto prio = (*it).second;
		auto& m = data_[prio];
		T obj = move(m.at(id));

		m.erase(id);
		idx.erase(id);

		return {move(obj), prio};
	}

private:
	int cnt = -1;
	map<int, map<Id, T>> data_; // priority->elements
	map<int, int> idx; //map from cnt to Id
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
		const auto item = strings.GetMax();
		ASSERT_EQUAL(item.first, "red");
		ASSERT_EQUAL(item.second, 2);
	}
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