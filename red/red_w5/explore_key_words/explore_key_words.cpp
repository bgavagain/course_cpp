#include "test_runner.h"
#include "profile.h"

#include <map>
#include <string>
#include <string_view>
#include <algorithm>
#include <future>
#include <mutex>

using namespace std;

string ReadBuffer(istream& input);

struct Stats {
	map<string, int> word_frequences;

	void operator += (const Stats& other) {
		for (const auto& e : other.word_frequences) {
			word_frequences[e.first] += e.second;
		}
	}
};

bool CheckWholeWord(size_t pos, size_t len, string_view sv) {
	auto pos_sp = pos + len;
	if (pos > 0 && (sv[pos - 1] != ' ' && sv[pos -1] != '\n')) { return false; }
	if (pos_sp < sv.length() && (sv[pos_sp] != ' ' && sv[pos_sp] != '\n')) { return false; }

	return true;
}

Stats ExploreLine(const set<string>& key_words, string_view str) {
	Stats result;

	for (const auto& word : key_words) {
		string_view sv(str);
		int count = 0;

		for (size_t pos = sv.find(word); pos != sv.npos; pos = sv.find(word, pos + word.length())) {
			if (CheckWholeWord(pos, word.length(), sv)) { ++count; }
		}
		if (count) { result.word_frequences[word] = count; }
	}

	return result;
}

Stats ExploreKeyWordsSingleThread(const set<string>& key_words, istream& input) {
	Stats result;
	for (string line; getline(input, line); ) {
		result += ExploreLine(key_words, line);
	}
	return result;
}

//Your implemenetation of ExploreKeyWords() works for at least 10838 ms in average, 
//but limit is 450 ms.Launch params : 50000 keywords, 50000 lines, 20 words in a line
//(Time used : 61.95 / 100.00, preprocess time used : 0 / None, memory used : 395718656 / 536870912.)

//Stats ProcessChunk(vector<string_view>& v, const set<string>& key_words, size_t start, size_t end) {
//	Stats res;
//	for (auto it = v.begin() + start; it != v.begin() + end; ++it) {
//		res += ExploreLine(key_words, *it);
//	}
//	return res;
//}

Stats ProcessChunk(string_view str, set<string>::iterator begin, set<string>::iterator end) {
	Stats result;

	for (auto it = begin ; it != end; ++it) {
		auto word = *it;
		string_view sv(str);
		int count = 0;		
		for (size_t pos = sv.find(word); pos != sv.npos; pos = sv.find(word, pos + word.length())) {
			if (CheckWholeWord(pos, word.length(), sv)) { ++count; }
		}
		if (count) { result.word_frequences[word] = count; }
	}

	return result;
}

Stats ExploreKeyWords(const set<string>& key_words, istream& input) {
	Stats result;

	string s = ReadBuffer(input);

	const int D_DIVIDE = 8;
	size_t chunk = key_words.size() / D_DIVIDE;
	vector<future<Stats>> vf;

	for (int i = 0; i < D_DIVIDE - 1; ++i) {
		auto start = next(key_words.begin(), i * chunk);
		auto end = next(key_words.begin(), i * chunk + chunk);
		vf.push_back(async(ProcessChunk, s, start, end));
	}

	result += ProcessChunk(s, next(key_words.begin(), (D_DIVIDE - 1) * chunk), key_words.end());

	for (auto& e : vf) {
		result += e.get();
	}

	return result;
}


void TestBasic() {
	const set<string> key_words = { "yangle", "rocks", "sucks", "all" };

	stringstream ss;
	ss << "this new yangle service really rocks\n";
	ss << "It sucks when yangle isn't available\n";
	ss << "10 reasons why yangle is the best IT company\n";
	ss << "yangle rocks others suck\n";
	ss << "Goondex really sucks, but yangle rocks. Use yangle\n";

	const auto stats = ExploreKeyWordsSingleThread(key_words, ss);
	const map<string, int> expected = {
	  {"yangle", 6},
	  {"rocks", 2},
	  {"sucks", 1}
	};
	ASSERT_EQUAL(stats.word_frequences, expected);
}

void TestMultithread() {
	const set<string> key_words = { "yangle", "rocks", "sucks", "all" };

	stringstream ss;
	ss << "this new yangle service really rocks\n";
	ss << "It sucks when yangle isn't available\n";
	ss << "10 reasons why yangle is the best IT company\n";
	ss << "yangle rocks others suck\n";
	ss << "Goondex really sucks, but yangle rocks. Use yangle\n";

	const auto stats = ExploreKeyWords(key_words, ss);
	const map<string, int> expected = {
	  {"yangle", 6},
	  {"rocks", 2},
	  {"sucks", 1}
	};
	ASSERT_EQUAL(stats.word_frequences, expected);
}

void TestMultithreadSimple() {
	const set<string> key_words = { "yangle" };

	stringstream ss;
	ss << "this new yangle service really rocks\n";
	ss << "It sucks when yangle isn't available\n";
	ss << "10 reasons why yangle is the best IT company\n";
	ss << "yangle rocks others suck\n";
	ss << "Goondex really sucks, but yangle rocks. Use yangle\n";

	const auto stats = ExploreKeyWords(key_words, ss);
	const map<string, int> expected = {
	  {"yangle", 6},
	};
	ASSERT_EQUAL(stats.word_frequences, expected);
}

string GenerateText() {
	stringstream ss;
	for (int i = 0; i < 10000; ++i) {
		ss << "this new yangle service really rocks\n";
		ss << "It sucks when yangle isn't available\n";
		ss << "10 reasons why yangle is the best IT company\n";
		ss << "yangle rocks others suck\n";
		ss << "Goondex really sucks, but yangle rocks. Use yangle\n";
	}
	return ss.str();
}

string ReadBuffer(istream& input) {
	input.seekg(0, input.end);
	size_t length = static_cast<size_t>(input.tellg());
	input.seekg(0, input.beg);

	char * buffer = new char[length];
	input.read(buffer, length);
	string str(buffer);
	delete[] buffer;

	return str;
}

int main() {
	TestRunner tr;
	RUN_TEST(tr, TestBasic);
	RUN_TEST(tr, TestMultithreadSimple);
	RUN_TEST(tr, TestMultithread);

	string s = GenerateText();
	const set<string> key_words = { "yangle", "rocks", "sucks", "all",
			"this", "new", "service", "really", "It", "when", "isn't", "available",
			"10", "reasons", "why", "is", "the", "best", "IT", "company", "others",
			"Goondex", "but", "Use" };

	stringstream ss1(s);
	stringstream ss3(s);
	
	Stats res_single;
	{
		LOG_DURATION("single thread");
		res_single += ExploreKeyWordsSingleThread(key_words, ss1);
	}

	Stats res_multi;
	{
		LOG_DURATION("read");
		res_multi += ExploreKeyWords(key_words, ss3);
	}
}