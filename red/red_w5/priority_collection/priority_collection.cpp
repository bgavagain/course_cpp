#include "test_runner.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <set>
#include <utility>
#include <vector>

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
			*(ids_begin++) = Add(move(*it));
		}
	}

	bool IsValid(Id id) const {
		return idx.count(id);
	}

	const T& Get(Id id) const { 
		return data_.at(idx.at(id)).at(id);
	}

	void Promote(Id id) {
		auto& prev = data_[idx.at(id)];
		T obj = move(prev.at(id));

		prev.erase(id);
		if (prev.size() == 0) { data_.erase(idx.at(id)); }

		++idx.at(id);
		data_[idx.at(id)].insert({ id, move(obj) });
	}

	pair<const T&, int> GetMax() const {
		auto pr_it = prev(data_.end());
		auto it = prev((*pr_it).second.end());

		return { (*it).second, (*pr_it).first };
	};

	pair<T, int> PopMax() { 
		auto pr_it = prev(data_.end());
		auto prio = (*pr_it).first;
		auto& prio_map = (*pr_it).second;
		auto it = prev(prio_map.end());
		auto id = (*it).first;
		T obj = move((*it).second);

		prio_map.erase(it);
		if (prio_map.size() == 0) { data_.erase(pr_it); }
		idx.erase(id);

		return {move(obj), prio };
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

	vector<int>ids(2, 0);
	vector<StringNonCopyable> v;
	StringNonCopyable a = "aaa";
	StringNonCopyable b = "bbb";
	v.push_back(move(a));
	v.push_back(move(b));
	strings.Add(begin(v), end(v), ids.begin());

	const auto white_id = strings.Add("white");
	const auto yellow_id = strings.Add("yellow");
	const auto red_id = strings.Add("red");

	strings.Promote(yellow_id);
	for (int i = 0; i < 2; ++i) {
		strings.Promote(red_id);
	}
	strings.Promote(yellow_id);

	{
		const auto& get = strings.Get(1);
		ASSERT(strings.IsValid(1));
		ASSERT_EQUAL(get, "bbb");
	}
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