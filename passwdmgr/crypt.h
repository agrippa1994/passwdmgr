#pragma once
#include <string>

std::string encode(const std::string& key, const std::string& plain);
std::string decode(const std::string& key, const std::string& cipher);