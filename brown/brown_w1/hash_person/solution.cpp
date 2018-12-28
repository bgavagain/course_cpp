#include <limits>
#include <random>
#include <unordered_set>

using namespace std;

struct Address {
	string city, street;
	int building;

	bool operator==(const Address& other) const {
		return
			city == other.city &&
			street == other.street &&
			building == other.building;
	}
};

struct Person {
	string name;
	int height;
	double weight;
	Address address;

	bool operator==(const Person& other) const {
		return
			name == other.name &&
			height == other.height &&
			weight == other.weight &&
			address == other.address;
	}
};

struct AddressHasher {
	size_t operator()(const Address& address) const {
		// �������� � �������� ������������ �������� ������� ������� �����;
		// ��������� ���� ����� �������� ������ ����, �.�. �� �� ������
		// �� ��������, � ���� ���������� ������� ������
		const size_t coef = 514'229;

		// std::hash ������ �� ����� ���������, ������� ��������
		// ��������� ���������� �� ��������� � ���������� ���������,
		// � ��������, �������� ����������� ����� �������������� ���
		const hash<int> int_hasher;
		const hash<string> string_hasher;

		return (
			coef * coef * string_hasher(address.city) +
			coef * string_hasher(address.street) +
			int_hasher(address.building)
			);
	}
};

struct PersonHasher {
	size_t operator()(const Person& person) const {
		// �������� � �������� ������������ �������� ������� ������� �����;
		// ��������� ���� ����� �������� ������ ����, �.�. �� �� ������
		// �� ��������, � ���� ���������� ������� ������
		const size_t coef = 39'916'801;

		// std::hash ������ �� ����� ���������, ������� ��������
		// ��������� ���������� �� ��������� � ���������� ���������,
		// � ��������, �������� ����������� ����� �������������� ���
		const hash<int> int_hasher;
		const hash<double> double_hasher;
		const hash<string> string_hasher;
		const AddressHasher address_hasher;

		return (
			coef * coef * coef * string_hasher(person.name) +
			coef * coef * int_hasher(person.height) +
			coef * double_hasher(person.weight) +
			address_hasher(person.address)
			);
	}
};