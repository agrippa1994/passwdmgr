#include "entry.h"

std::vector<stEntry> g_entries;

std::ostream& operator<<(std::ostream& os, stEntry& e)
{
	std::cout << e.description << '\t' << e.account << '\t' << e.password;
	return os;
}