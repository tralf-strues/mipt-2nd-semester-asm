extern "C" int _writeFormatted(const char* format, ...);

int main()
{
    _writeFormatted("I %s %x %s %s %d%%%c%b", "love", 3802, "and", "you", 100, 33, 255);
}