#pragma once

#include "..\Globals.h"

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