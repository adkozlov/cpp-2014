void foo(const long long n)
{
    foo(n);
}

int main()
{
    foo(0);

    return 0;
}
