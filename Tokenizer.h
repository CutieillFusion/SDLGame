#ifndef TOKENIZER_HEADER
#define TOKENIZER_HEADER

#include <fstream>
#include <vector>
#include <map>
#include <stdexcept>
#include <stack>
#include <iostream>

enum class TOKEN
{
    CURLY_OPEN,
    CURLY_CLOSE,
    COLON,
    STRING,
    NUMBER,
    ARRAY_OPEN,
    ARRAY_CLOSE,
    COMMA,
    BOOLEAN,
    NULL_TYPE
};

struct Token
{
    std::string value;
    TOKEN type;
    std::string toString();
};

class Tokenizer
{
    std::fstream file;
    size_t prevPos;

public:
    Tokenizer(std::string fileName);

    char getWithoutWhiteSpace();
    Token getToken();
    bool hasMoreTokens();
    void rollBackToken();

    /*
        "": String begin?
        {}: Object
        []: List
        num: number

     */
};

#endif