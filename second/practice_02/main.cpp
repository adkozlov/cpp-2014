#include <iostream>
#include <utility>

#include "file_t.h"

int main()
{
    file_t original_file("test.txt");
    file_t file(std::move(original_file));

    while (!file.eof())
    {
        int result1;
        file >> result1;

        std::cout << result1 << std::endl;
    }
}
