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
	tmp.reserve(distance(first, last));
	{
		LOG_DURATION("init orig");
		move(first, last, back_inserter(tmp));
	}

	auto f = &*first;
	auto c = &*tmp.begin();
	cout << f << " " << c << endl;

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
	list<typename RandomIt::value_type> tmp;
	{
		LOG_DURATION("init my");
		move(first, last, back_inserter(tmp));
	}
	auto it = tmp.begin();
	size_t pos = 0;
	size_t size = tmp.size();

	{
		LOG_DURATION("calc my");
		while (!tmp.empty()) {
			*(first++) = move(*it);
			it = tmp.erase(it); --size;
			if (tmp.empty()) break;

			size_t next = pos + step_size - 1;
			if (next < size) {
				pos = next;
				advance(it, step_size - 1);
			}
			else {
				pos = next % size;
				it = tmp.begin();
				advance(it, pos);
			}
		}
	}

	


 //   size_t size = distance(first, last);
	//vector<typename RandomIt::value_type> tmp;
	//move(first, last, back_inserter(tmp));
	//vector<size_t> lst(size);
	//iota(lst.begin(), lst.end(), 0);

	//auto it = lst.begin();
	//while (!lst.empty()) {
	//	*(first++) = move(tmp[*it]);
	//	it = lst.erase(it);
	//	if (lst.empty()) break;
	//	size_t d = distance(lst.begin(), it);

	//	if (lst.size() - d > step_size)
	//		it = next(it, step_size - 1);
	//	else {
	//		size_t offset = (d + step_size - 1) % lst.size();
	//		it = next(lst.begin(), offset);
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
    //{
    //    vector<int> numbers_copy = numbers;
    //    MakeJosephusPermutation(begin(numbers_copy), end(numbers_copy), 1);
    //    ASSERT_EQUAL(numbers_copy, numbers);
    //}
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
 //   RUN_TEST(tr, TestIntVectorOrig);
 //   RUN_TEST(tr, TestAvoidsCopyingOrig);
	RUN_TEST(tr, TestIntVector);
 //   RUN_TEST(tr, TestAvoidsCopying);


	const vector<int> numbers = MakeTestVector(100000);
	{	LOG_DURATION("int");
		vector<int> numbers_copy = numbers;
		MakeJosephusPermutationOrig(begin(numbers_copy), end(numbers_copy), 100);
	}

	{	LOG_DURATION("int my");
		vector<int> numbers_copy = numbers;
		MakeJosephusPermutation(begin(numbers_copy), end(numbers_copy), 100);
	}

    return 0;
}