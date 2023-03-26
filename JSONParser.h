#ifndef JSONPARSER_HEADER
#define JSONPARSER_HEADER

#include "JSONNode.h"
#include "Tokenizer.h"
#include <memory>

class JSONParser {
public:
    std::fstream file;
    //Use C files
    //Minimize shared pointers
    std::shared_ptr<JSON::JSONNode> root;
    std::unique_ptr<JSON::JSONNode> current;
    Tokenizer tokenizer;

    JSONParser(const std::string filename);

    void parse();

    std::shared_ptr<JSON::JSONNode> parseObject();
    std::shared_ptr<JSON::JSONNode> parseString();
    std::shared_ptr<JSON::JSONNode> parseNumber();
    std::shared_ptr<JSON::JSONNode> parseList();
    std::shared_ptr<JSON::JSONNode> parseBoolean();
    std::shared_ptr<JSON::JSONNode> parseNull();
};

#endif