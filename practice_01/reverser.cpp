#include <string>
#include <iostream>
#include <sstream>

int main(int argc, char** argv)
{
    std::string in = argv[1];
    
    std::stringstream out("");
    for (int i = 1; i <= in.size(); ++i)
    {
        out << in[in.size() - i];
    }

    std::cout << out.str() << std::endl;
    
    return 0;
}
