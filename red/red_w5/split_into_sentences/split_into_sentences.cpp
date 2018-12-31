#include "test_runner.h"

#include <vector>
#include <string>

using namespace std;

template <typename Token>
using Sentence = vector<Token>;


template <typename Token>
vector<Sentence<Token>> SplitIntoSentences(vector<Token> tokens) {
	vector<Sentence<Token>> res;

	Sentence<Token> sen;
	bool eol = false;
	for (auto& t : tokens) {
		if (t.IsEndSentencePunctuation()) {
			eol = true;
		}
		else {
			if (eol) {
				eol = false;
				res.push_back(move(sen));
				sen.clear();
			}
		}
		sen.push_back(move(t));
	}

	if(!sen.empty()) res.push_back(move(sen));

	return move(res);
}


struct TestToken {
	string data;
	bool is_end_sentence_punctuation = false;

	bool IsEndSentencePunctuation() const {
		return is_end_sentence_punctuation;
	}
	bool operator==(const TestToken& other) const {
		return data == other.data && is_end_sentence_punctuation == other.is_end_sentence_punctuation;
	}
};

ostream& operator<<(ostream& stream, const TestToken& token) {
	return stream << token.data;
}

void TestSplitting() {
	ASSERT_EQUAL(
		SplitIntoSentences(vector<TestToken>({ {"Split"}, {"into"}, {"sentences"}, {"!"} })),
		vector<Sentence<TestToken>>({
			{{"Split"}, {"into"}, {"sentences"}, {"!"}}
			})
	);

	ASSERT_EQUAL(
		SplitIntoSentences(vector<TestToken>({ {"Split"}, {"into"}, {"sentences"}, {"!", true} })),
		vector<Sentence<TestToken>>({
			{{"Split"}, {"into"}, {"sentences"}, {"!", true}}
			})
	);

	auto v = vector<Sentence<TestToken>>({
			{{"Split"}, {"into"}, {"sentences"}, {"!", true}, {"!", true}},
			{{"Without"}, {"copies"}, {".", true}},
		});

	ASSERT_EQUAL(
		SplitIntoSentences(vector<TestToken>({ {"Split"}, {"into"}, {"sentences"}, {"!", true}, {"!", true}, {"Without"}, {"copies"}, {".", true} })),
		v);
}

int main() {
	TestRunner tr;
	RUN_TEST(tr, TestSplitting);
	return 0;
}