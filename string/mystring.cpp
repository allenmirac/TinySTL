#include "mystring.h"

MyString::MyString(const char* str){
    length = std::strlen(str);
    data = new char[length + 1];
    std::strcpy(data, str);
}