#pragma once

#include "Globals.h"

namespace JSON {
    class JSONNode;
    using JSONObject = std::map<std::string, std::shared_ptr<JSONNode>>;
    using JSONList = std::vector<std::shared_ptr<JSONNode>>;

    class JSONNode {
        enum class Type { OBJECT, LIST, STRING, NUMBER, BOOLEAN, NULL_TYPE }; // make it private
        union Values {
            JSONObject* object;
            JSONList* list;
            std::string* s;
            float fValue;
            bool bValue;
        } values;
        Type type;

    public:
        JSONNode();
        JSONNode(Type t);

        JSONObject returnObject();
        JSONList returnList();
        std::string returnString();
        float returnFloat();
        bool returnBool();

        void setObject(JSONObject* object);

        void setString(std::string* s);
        void setNumber(float f);
        void setList(JSONList* list);
        void setBoolean(bool v);
        void setNull();

        std::string toString(int indentationLevel);

        void printNode(int indentationLevel) {
            std::cout << toString(indentationLevel);
            std::cout << std::endl;
        }
    };
} // namespace JSON

//aligned union
//compare with actual library
//Constructor - No setter getter
//Inline Functions
//o3 optimizations