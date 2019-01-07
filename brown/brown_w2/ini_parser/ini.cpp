#include "ini.h"

using namespace std;;

std::ostream& operator << (ostream& os, const Ini::Section& s) {
	os << '{';
	for (const auto& e : s) {
		os << e.first << '=' << e.second << ", ";
	}
	os << '}';

	return os;
}

namespace Ini {

Section& Document::AddSection(string name) {
	return sections[name];
}

const Section& Document::GetSection(const string& name) const {
	return sections.at(name);
}

size_t Document::SectionCount() const {
	return sections.size();
}

Document Load(istream& input) {
	Document res;
	Section* section = nullptr;
	for (string s; getline(input, s);) {
		if (s.empty()) continue;

		if (s[0] == '[') {
			section = &res.AddSection(s.substr(1, s.size() - 2));
		} else {
			if (section) {
				auto pos = s.find('=');
				(*section)[s.substr(0, pos)] = s.substr(pos + 1, s.size() - pos - 1);
			}
		}
	}

	return res;
}

}