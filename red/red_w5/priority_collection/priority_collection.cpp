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
	using Id = complex<uint32_t>;
	
	PriorityCollection() {};

	Id Add(T object) { 
		data_[0].push_back(move(object));
		return { 0, data_[0].size() -1 };
	}

	template <typename ObjInputIt, typename IdOutputIt>
	void Add(ObjInputIt range_begin, ObjInputIt range_end, IdOutputIt ids_begin) {
		for (auto it = range_begin; it != range_end; ++it) {
			Add(*it);
		}
	}

	bool IsValid(Id id) const {
		return data_.count(id.real) && id.imag < data_.at(id.real).size();
	}

	const T& Get(Id id) const { 
		return data_[id.real][id.imag];
	}

	void Promote(Id id) {
		auto p = id.real();
		auto i = id.imag();

		auto& vp = data_.at(p);
		T obj = move(vp[i]);
		vp.erase(vp.begin() + id.imag());
		if (vp.size() == 0) { data_.erase(it); }

		//auto itn = data[id.real + 1];
		//auto& vp = (*itp).second;
		//T obj = move(vp[id.imag]);
		//vp.erase(vp.begin() + id.imag);
		//if (vp.size() == 0) { data_.erase(itp); }
	}

	pair<const T&, int> GetMax() const {
		auto it = prev(data_.end());
		auto i = (*it).first;
		auto& v = (*it).second;

		return { v.back(), i };
	};

	pair<T, int> PopMax() { 
		auto it = prev(data_.end());
		auto i = (*it).first;
		auto& v = (*it).second;

		T obj = move(v.back());
		v.pop_back();
		if (v.size() == 0) { data_.erase(it); }

		return { move(obj), i };
	}

private:
	uint32_t idx = 0;
	T d;
	map<uint32_t, vector<T>> data_;
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