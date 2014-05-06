#include "console.h"
#include "entry.h"
#include "crypt.h"
#include "main.h"

#include <conio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include <boost/lexical_cast.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>

std::string input(const std::string & text, bool passwd)
{
	std::cout << text;

	std::string retn;

	char ch = '\0';
	while ((ch = _getch()) != 13)
	{
		std::cout << (char) ((passwd) ? '*' : ch);
		retn += ch;
	}
	std::cout << std::endl;
	return retn;
}

void commandHandler(const std::string& str)
{
	if (str == "p") // print
	{
		int idx = 0;
		for (auto & i : g_entries)
			std::cout << (idx++) << '\t' << i << std::endl;

	}
	else if (str == "a") // add
	{
		stEntry e;
		e.description = input("Enter description: ", false);
		e.account = input("Enter nickname / email: ", false);
		e.password = input("Enter password: ", true);

		g_entries.push_back(e);
	}

	else if (str == "r") // remove
	{
		commandHandler("p");
		std::cout << std::endl;

		try
		{
			auto idx = boost::lexical_cast<int>(input("Enter index, which should be removed: ", false));

			if (idx < 0 || idx >= g_entries.size())
				std::cout << "This index was invalid!" << std::endl;
			else
				g_entries.erase(g_entries.begin() + idx);
		}
		catch (...)
		{
			std::cout << "This index was invalid!" << std::endl;
		}

	}
	else if (str == "w")
	{
		std::fstream fs("pw.db", std::ios_base::out);

		std::stringstream ss;
		boost::archive::text_oarchive oa(ss);
		oa << std::string("ok") << g_entries;

		fs << encode(g_key, ss.str());
		fs.close();
	}
	else if (str == "q")
		exit(0);

	else if (str == "?")
	{
		std::cout << 'p' << '\t' << "Print all entries." << std::endl;
		std::cout << 'a' << '\t' << "Append an entry." << std::endl;
		std::cout << 'r' << '\t' << "Remove an entry." << std::endl;
		std::cout << 'w' << '\t' << "Write the changes to the file." << std::endl;
		std::cout << 'q' << '\t' << "Quit" << std::endl;
		std::cout << '?' << '\t' << "Show this message." << std::endl;
	}
	else
		std::cout << "Unknown Command, enter '?' to show basic commands" << std::endl;
}


void waitForCommand()
{
	std::cout << "pwmgr> ";

	char szIn[256] = { 0 };
	std::cin.getline(szIn, sizeof(szIn) -1);

	commandHandler(szIn);
}