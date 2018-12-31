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
	{
		LOG_DURATION("init orig");
		tmp.reserve(distance(first, last));
		move(first, last, back_inserter(tmp));
	}

	{
		LOG_DURATION("calc orig");
		size_t cur_pos = 0;
		while (!tmp.empty()) {
			*(first++) = move(tmp[cur_pos]);
			tmp.erase(tmp.begin() + cur_pos);
			if (tmp.empty()) {
				break;
			}
			cur_pos = (cur_pos + step_size - 1) % tmp.size();
		}
	}
}

template <typename RandomIt>
void MakeJosephusPermutation(RandomIt first, RandomIt last, size_t step_size) {
	vector<typename RandomIt::value_type> tmp;
	vector<size_t>ptr;
	size_t size = distance(first, last);
//	list<int> killed(size);
	//{
	//	LOG_DURATION("init my");
		tmp.reserve(size);
		ptr.resize(size);
		move(first, last, back_inserter(tmp));
		iota(begin(ptr), end(ptr), 1);
		ptr.back() = 1;
	//}

	//{
	//	LOG_DURATION("calc my");

		first++;
		size_t pos = 0;
		for (size_t i = 1; i < size; ++i) {
			size_t cnt = 0;
			while (cnt < step_size -1) {
				pos = ptr[pos];
				++cnt;
			}
			auto val = ptr[pos];
			*(first++) = move(tmp[val]);
			ptr[pos] = ptr[val];
		}

	//	size_t pos = 0;
	//	auto it = killed.begin();
	//	while (!killed.empty()) {
	//		*(first++) = move(tmp[*it]);
	//		it = killed.erase(it); --size;
	//		if (killed.empty()) break;

	//		size_t next = pos + step_size - 1;
	//		size_t new_pos = next % size;
	//		if (new_pos > pos) {
	//			advance(it, new_pos - pos - 1);
	//		}
	//		else {
	//			it = killed.begin();
	//			advance(it, new_pos);
	//		}
	//		pos = new_pos;
	//	}
	//}
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