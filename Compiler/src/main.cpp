#include "Compiler.h"
#include <fstream>
#include <iostream>
void readfile(std::string& s, std::ifstream& is) {
	s.erase();
	if (is.bad()) return;
	//
	// attempt to grow string buffer to match file size,
	// this doesn't always work...
	s.reserve(is.rdbuf()->in_avail());
	char c;
	while (is.get(c))
	{
		// use logarithmic growth stategy, in case
		// in_avail (above) returned zero:
		if (s.capacity() == s.size())
			s.reserve(s.capacity() * 3);
		s.append(1, c);
	}
}

int main()
{
	std::string str;
	Compiler compiler;
	std::ifstream is("C:\\Users\\en9er\\OneDrive\\Рабочий стол\\test.txt");
	readfile(str, is);
	compiler.Compile(str);

	if (!compiler.errorOccured)
	{
		for (auto byte : compiler.resultBinary)
		{
			std::cout << std::hex << (int)byte << " ";
		}
	}
}