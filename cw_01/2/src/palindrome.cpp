#include <fstream>
#include <string>

using std::string;

bool is_palindrome(const string& str, const size_t left, const size_t right)
{
    for (size_t i = left; i < (right - left) / 2; i++)
    {
        if (str[left + i] != str[right - i - 1])
        {
            return false;
        }
    }
    
    return true;
}

string palindrome(const string& str)
{
    string result;

    if (!str.empty())
    {
        size_t max = 0;
        for (size_t i = 0; i < str.length() - 1; i++)
        {
            for (size_t j = i; j < str.length(); j++)
            {
                if (is_palindrome(str, i, j) && (j - i) > max)
                {
                    result = str.substr(i, j - i);
                    max = j - i;
                }
            }
        }
    }

    return result;
}
int main()
{
    string str;

    std::ifstream in("input.txt");
    in >> str;

    std::ofstream out("output.txt");
    out << palindrome(str) << std::endl;

    return 0;
}
