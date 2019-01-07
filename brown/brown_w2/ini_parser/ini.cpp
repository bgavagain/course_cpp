#include "ini.h"

using namespace std;;

namespace Ini {

Section& Document::AddSection(string name) {
	return sections.at(name);
}

const Section& Document::GetSection(const string& name) const {
	return sections.at(name);
}

size_t Document::SectionCount() const {
	return 0;
}

Document Load(istream& input) {
	return {};
}

}