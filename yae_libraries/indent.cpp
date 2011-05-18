#include <indent.h>

std::string IndentFacet::indent = "";

std::ostream& indent(std::ostream& os)
{
	os << std::flush;
	IndentFacet::indent += '\t';
	return os;
}

std::ostream& unindent(std::ostream& os)
{
	os << std::flush;
	IndentFacet::indent.erase(IndentFacet::indent.length() - 1);
	return os;
}
