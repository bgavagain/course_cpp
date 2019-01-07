#include "test_runner.h"

#include <iostream>
#include <map>
#include <string>
#include <unordered_map>

using namespace std;

struct Record {
	string id;
	string title;
	string user;
	int timestamp;
	int karma;
};

struct RecordIdx {
	Record rec;
	multimap<int, Record>::iterator karma;
	multimap<int, Record>::iterator timestamp;
	multimap<string, Record>::iterator user;
};

class Database {
public:
	bool Put(const Record& record) {
		if (data.count(record.id)) { return false; }

		auto karma = karma_idx.insert(make_pair(record.karma, record));
		auto timestamp = timestamp_idx.insert(make_pair(record.timestamp, record));
		auto user = user_idx.insert(make_pair(record.user, record));

		RecordIdx rec = { record, karma, timestamp, user };
		data[record.id] = rec;

		return true;
	}

	const Record* GetById(const string& id) const {
		if (!data.count(id)) { return nullptr; }
		else { return &data.at(id).rec; }
	}

	//template<typename T>
	//void DeleteIdx(multimap<T, Record>& mm, T val, const Record & rec) {
	//	auto r = mm.equal_range(val);
	//	for (auto it = r.first; it != r.second; ++it) {
	//		auto v = *it;
	//		if (v.second.id == rec.id) {
	//			mm.erase(it);
	//			break;
	//		}
	//	}
	//}

	bool Erase(const string& id) { 
		if (!data.count(id)) { return false; }

		auto rec = data[id];
		karma_idx.erase(rec.karma);
		timestamp_idx.erase(rec.timestamp);
		user_idx.erase(rec.user);
		//DeleteIdx(karma_idx, rec.karma, rec);
		//DeleteIdx(timestamp_idx, rec.timestamp, rec);
		//DeleteIdx(user_idx, rec.user, rec);
		data.erase(id);

		return true; 
	}

	template <typename Callback>
	void RangeByTimestamp(int low, int high, Callback callback) const {
		auto l = timestamp_idx.lower_bound(low);
		auto u = timestamp_idx.upper_bound(high);
		for (auto& it = l; it != u; ++it) {
			auto& val = *it;
			if (!callback(val.second)) { break; }
		}
	}

	template <typename Callback>
	void RangeByKarma(int low, int high, Callback callback) const {
		auto l = karma_idx.lower_bound(low);
		auto u = karma_idx.upper_bound(high);
		for (auto& it = l; it != u; ++it) {
			auto& val = *it;
			if (!callback(val.second)) { break; }
		}
	}

	template <typename Callback>
	void AllByUser(const string& user, Callback callback) const {
		auto l = user_idx.lower_bound(user);
		auto u = user_idx.upper_bound(user);
		for (auto& it = l; it != u; ++it) {
			auto& val = *it;
			if (!callback(val.second)) { break; }
		}
	}
private:
	map<string, RecordIdx> data;
	multimap<int, Record> karma_idx;
	multimap<int, Record> timestamp_idx;
	multimap<string, Record> user_idx;
};

void TestRangeBoundaries() {
	const int good_karma = 1000;
	const int bad_karma = -10;

	Database db;
	db.Put({ "id1", "Hello there", "master", 1536107260, good_karma });
	db.Put({ "id2", "O>>-<", "general2", 1536107260, bad_karma });

	int count = 0;
	db.RangeByKarma(bad_karma, good_karma, [&count](const Record&) {
		++count;
		return true;
		});

	ASSERT_EQUAL(2, count);
}

void TestSameUser() {
	Database db;
	db.Put({ "id1", "Don't sell", "master", 1536107260, 1000 });
	db.Put({ "id2", "Rethink life", "master", 1536107260, 2000 });

	int count = 0;
	db.AllByUser("master", [&count](const Record&) {
		++count;
		return true;
		});

	ASSERT_EQUAL(2, count);
}

void TestReplacement() {
	const string final_body = "Feeling sad";

	Database db;
	db.Put({ "id", "Have a hand", "not-master", 1536107260, 10 });
	db.Erase("id");
	db.Put({ "id", final_body, "not-master", 1536107260, -10 });

	auto record = db.GetById("id");
	ASSERT(record != nullptr);
	ASSERT_EQUAL(final_body, record->title);
}

int main() {
	TestRunner tr;
	RUN_TEST(tr, TestRangeBoundaries);
	RUN_TEST(tr, TestSameUser);
	RUN_TEST(tr, TestReplacement);
	return 0;
}