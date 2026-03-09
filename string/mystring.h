#ifndef MYSTRING_H
#define MYSTRING_H

#include <cstring>

class MyString{


private:
    char* data;
    size_t length;
public:
    MyString(const char* str="");
    MyString(const MyString& other);
    MyString& operator=(const MyString& other);
    
    ~MyString();
};

#endif // MYSTRING_H