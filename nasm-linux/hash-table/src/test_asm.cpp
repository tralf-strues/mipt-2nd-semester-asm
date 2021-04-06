// #include <stdio.h>
// #include <stdint.h>

extern unsigned int getMurmur3Hash(const char* string);

// 0 2 3

int main()
{ 
    const char* string  = "Hello, World!\n";
    const char* string1 = "Hello, World1!\n";
    const char* string2 = "Hello, World2!\n";
    const char* string3 = "Hello, World3!\n";
    const char* string4 = "Hello, World4!\n";

    unsigned int hash  = getMurmur3Hash(string);
    unsigned int hash1 = getMurmur3Hash(string1);
    unsigned int hash2 = getMurmur3Hash(string2);
    unsigned int hash3 = getMurmur3Hash(string3);
    unsigned int hash4 = getMurmur3Hash(string4);
}