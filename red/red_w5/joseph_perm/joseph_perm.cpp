#include "test_runner.h"
#include "profile.h"

#include <cstdint>
#include <iterator>
#include <numeric>
#include <vector>
#include <list>
#include <utility>
#include <algorithm>

using namespace std;

//http://school6.tgl.ru/old/made/calk/main.html

template <typename RandomIt>
void MakeJosephusPermutationOrig(RandomIt first, RandomIt last, uint32_t step_size) {
    vector<typename RandomIt::value_type> tmp;
	//{
	//	LOG_DURATION("init orig");
		tmp.reserve(distance(first, last));
		move(first, last, back_inserter(tmp));
//	}

	//{
	//	LOG_DURATION("calc orig");
		size_t cur_pos = 0;
		while (!tmp.empty()) {
			*(first++) = move(tmp[cur_pos]);
			tmp.erase(tmp.begin() + cur_pos);
			if (tmp.empty()) {
				break;
			}
			cur_pos = (cur_pos + step_size - 1) % tmp.size();
		}
//	}
}

template <typename RandomIt>
void MakeJosephusPermutation(RandomIt first, RandomIt last, uint32_t step_size) {
	const uint32_t size = last - first;
	if (size < 2) return;

	vector<uint32_t> apos(size);
	vector<typename RandomIt::value_type> perm;
	iota(begin(apos), end(apos), 1);
	apos.back() = 0;

	uint32_t pos = 0;
	uint32_t preva = size - 1;
	for (uint32_t s = 0; s < size; ++s) {
		perm.push_back(move(*(first + pos)));
		apos[preva] = apos[pos];
		for (uint32_t i = 0; i < step_size; ++i) {
			if (i > 0) { preva = pos; }
			pos = apos[pos];
		}
	}

	move(begin(perm), end(perm), first);
}

vector<int> MakeTestVector(size_t size) {
    vector<int> numbers(size);
    iota(begin(numbers), end(numbers), 0);
    return numbers;
}

void TestIntVectorOrig() {
    const vector<int> numbers = MakeTestVector(10);
    {
        vector<int> numbers_copy = numbers;
        MakeJosephusPermutationOrig(begin(numbers_copy), end(numbers_copy), 1);
        ASSERT_EQUAL(numbers_copy, numbers);
    }
    {
        vector<int> numbers_copy = numbers;
        MakeJosephusPermutationOrig(begin(numbers_copy), end(numbers_copy), 3);
        ASSERT_EQUAL(numbers_copy, vector<int>({ 0, 3, 6, 9, 4, 8, 5, 2, 7, 1 }));
    }
}

void TestIntVector() {
    const vector<int> numbers = MakeTestVector(10);
    {
        vector<int> numbers_copy = numbers;
        MakeJosephusPermutation(begin(numbers_copy), end(numbers_copy), 1);
        ASSERT_EQUAL(numbers_copy, numbers);
    }
    {
        vector<int> numbers_copy = numbers;
        MakeJosephusPermutation(begin(numbers_copy), end(numbers_copy), 3);
        ASSERT_EQUAL(numbers_copy, vector<int>({ 0, 3, 6, 9, 4, 8, 5, 2, 7, 1 }));
    }
}

struct NoncopyableInt {
    int value;

    NoncopyableInt(const NoncopyableInt&) = delete;
    NoncopyableInt& operator=(const NoncopyableInt&) = delete;

    NoncopyableInt(NoncopyableInt&&) = default;
    NoncopyableInt& operator=(NoncopyableInt&&) = default;
};

bool operator == (const NoncopyableInt& lhs, const NoncopyableInt& rhs) {
    return lhs.value == rhs.value;
}

ostream& operator << (ostream& os, const NoncopyableInt& v) {
    return os << v.value;
}

void TestAvoidsCopyingOrig() {
	vector<NoncopyableInt> numbers;
	numbers.push_back({ 1 });
	numbers.push_back({ 2 });
	numbers.push_back({ 3 });
	numbers.push_back({ 4 });
	numbers.push_back({ 5 });

	MakeJosephusPermutationOrig(begin(numbers), end(numbers), 2);

	vector<NoncopyableInt> expected;
	expected.push_back({ 1 });
	expected.push_back({ 3 });
	expected.push_back({ 5 });
	expected.push_back({ 4 });
	expected.push_back({ 2 });

	ASSERT_EQUAL(numbers, expected);
}

void TestAvoidsCopying() {
	vector<NoncopyableInt> numbers;
	numbers.push_back({ 1 });
	numbers.push_back({ 2 });
	numbers.push_back({ 3 });
	numbers.push_back({ 4 });
	numbers.push_back({ 5 });

	MakeJosephusPermutation(begin(numbers), end(numbers), 2);

	vector<NoncopyableInt> expected;
	expected.push_back({ 1 });
	expected.push_back({ 3 });
	expected.push_back({ 5 });
	expected.push_back({ 4 });
	expected.push_back({ 2 });

	ASSERT_EQUAL(numbers, expected);
}


int main() {
    TestRunner tr;
    RUN_TEST(tr, TestIntVectorOrig);
    RUN_TEST(tr, TestAvoidsCopyingOrig);
	RUN_TEST(tr, TestIntVector);
    RUN_TEST(tr, TestAvoidsCopying);


	const vector<int> numbers = MakeTestVector(100000);
	{	LOG_DURATION("int");
		vector<int> numbers_copy = numbers;
		MakeJosephusPermutationOrig(begin(numbers_copy), end(numbers_copy), 3);
	}

	{	LOG_DURATION("int my");
		vector<int> numbers_copy = numbers;
		MakeJosephusPermutation(begin(numbers_copy), end(numbers_copy), 3);
	}

    return 0;
}