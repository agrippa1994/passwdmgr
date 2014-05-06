#pragma comment(lib, "cryptlib.lib")

#include "crypt.h"
#include "console.h"
#include "entry.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>

std::string g_key;

int main()
{
	std::cout << "Password-Manager" << std::endl;

	std::fstream fs("pw.db");
	if (!fs.is_open())
	{
		std::cout << "Warning: The file \"pw.db\" couldn't be found!" << std::endl;

		while (true)
		{
			std::string keys [] = { input("Enter aes256 key: ", true), input("Enter aes256 key again: ", true) };
			if (keys[0] == keys[1])
			{
				g_key = keys[0];
				break;
			}
			else
			{
				std::cout << "Both keys aren't equal, try again!" << std::endl;
			}
		}
	}
	else
	{
		g_key = input("Enter aes256 key: ", true);

		std::string decrypted_filedata;

		try
		{
			std::stringstream ss;
			ss << fs.rdbuf();

			decrypted_filedata = decode(g_key, ss.str());
		}
		catch (...)
		{
			std::cout << "Error: The file couldn't be decrypted!" << std::endl;

			std::cin.sync();
			std::cin.get();
			return 1;
		}

		try
		{
			std::stringstream ss(decrypted_filedata);
			boost::archive::text_iarchive ia(ss);

			std::string check;
			ia >> check >> g_entries;

			if (check != "ok")
			{
				std::cout << "The AES256 Key seems to be wrong..." << std::endl;
			}
		}
		catch (...)
		{
			std::cout << "The file couldn't be serialized!" << std::endl;

			std::cin.sync();
			std::cin.get();
			return 1;
		}
	}

	fs.close();

	while (1)
	{
		waitForCommand();
	}

	return 0;
}