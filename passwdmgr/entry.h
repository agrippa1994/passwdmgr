#pragma once
#include <string>
#include <vector>
#include <iostream>

struct stEntry
{
	std::string description;
	std::string account;
	std::string password;
};

extern std::vector<stEntry> g_entries;

template<typename T> void serialize(T& ar, stEntry& e, const unsigned int version)
{
	ar & e.description;
	ar & e.account;
	ar & e.password;
}

std::ostream& operator<<(std::ostream& os, stEntry& e);