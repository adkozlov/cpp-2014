#include <iostream>

#include "au_list.h"

using namespace containers;

int main()
{
	au_list<int> list;
	list.insert(list.begin(), 1);

	std::cout << "finished" << std::endl;
	return 0;
}