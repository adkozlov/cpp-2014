#include <fstream>
#include <vector>

typedef std::vector<int> ints;

void merge(ints& a, size_t left, size_t mid, size_t right)
{
    size_t i = 0;
    size_t j = 0;
    int result[right - left];

    while (left + i < mid && mid + j < right)
    {
        if (a[left + i] < a[mid + j])
        {
            result[i + j] = a[left + i];
            i++;
        }
        else
        {
            result[i + j] = a[mid + j];
            j++;
        }
    }

    while (left + i < mid)
    {
        result[i + j] = a[left + i];
        i++;
    }

    while (mid + j < right)
    {
        result[i + j] = a[mid + j];
        j++;
    }

    for (size_t k = 0; k < i + j; k++)
    {
        a[left + k] = result[k];
    }
}

void mergesort(ints& a, size_t left, size_t right)
{
    if (left + 1 >= right)
    {
        return;
    }

    size_t mid = (left + right) / 2;
    mergesort(a, left, mid);
    mergesort(a, mid, right);

    merge(a, left, mid, right);
}

void mergesort(ints& a)
{
    mergesort(a, 0, a.size());
}

int main()
{
    int n;
    ints numbers;

    std::ifstream in("input.txt");
    while (in >> n)
    {
        numbers.push_back(n);
    }

    mergesort(numbers);

    std::ofstream out("output.txt");
    for (size_t i = 0; i < numbers.size(); i++)
    {
        out << numbers[i] << " ";
    }
    out << std::endl;

    return 0;
}
