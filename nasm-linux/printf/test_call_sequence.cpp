char func(int a, char b, char c, const char* d, const char* e, char f, char g, int h, long long i)
{
    c = 5;
    f += 1;

    h *= 2;
    a = b - c;

    return g;
}

int main()
{
    func(22, 2, 2, "Happy", "Birthday", 'u', 'a', -273, ')');

    return 0;
}