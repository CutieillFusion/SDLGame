#include "JSONNode.h"

JSON::JSONNode::JSONNode()
{
}

JSON::JSONNode::JSONNode(Type t)
{
	type = t;
}

JSON::JSONObject JSON::JSONNode::returnObject()
{
    if (type == Type::OBJECT) {
        return *values.object;
    }
    throw std::logic_error("Improper return");
}

JSON::JSONList JSON::JSONNode::returnList()
{
    if (type == Type::LIST) {
        return *values.list;
    }
    throw std::logic_error("Improper return");
}

std::string JSON::JSONNode::returnString()
{
    if (type == Type::STRING) {
        return *values.s;
    }
    throw std::logic_error("Improper return");
}

float JSON::JSONNode::returnFloat()
{
    if (type == Type::NUMBER) {
        return values.fValue;
    }
    throw std::logic_error("Improper return");
}

void JSON::JSONNode::setObject(JSONObject* object)
{
    this->values.object = object;
    type = Type::OBJECT;
}

void JSON::JSONNode::setString(std::string* s)
{
    this->values.s = s;
    type = Type::STRING;
}

void JSON::JSONNode::setNumber(float f)
{
    this->values.fValue = f;
    type = Type::NUMBER;
}

void JSON::JSONNode::setList(JSONList* list)
{
    this->values.list = list;
    type = Type::LIST;
}

void JSON::JSONNode::setBoolean(bool v)
{
    this->values.bValue = v;
    type = Type::BOOLEAN;
}

void JSON::JSONNode::setNull()
{
    type = Type::NULL_TYPE;
}

std::string JSON::JSONNode::toString(int indentationLevel)
{
    std::string spaceString = std::string(indentationLevel, ' ');

    std::string outputString = "";

    switch (type) 
    {
    case Type::STRING: {
        outputString += spaceString + *values.s;
        break; 
    }
    case Type::NUMBER: {
        outputString += spaceString + std::to_string(values.fValue);
        break;
    }
    case Type::BOOLEAN: {
        outputString += spaceString + (values.bValue ? "true" : "false");
        break; 
    }
    case Type::NULL_TYPE: {
        outputString += spaceString + "null";
        break;
    }
    case Type::LIST: {
        outputString += "\n[\n";
        int index = 0;
        for (auto node : (*values.list)) {
            outputString += node->toString(indentationLevel + 1);
            if (index < (*values.list).size() - 1) {
                outputString += spaceString + ", ";
            }
            index++;
        }
        outputString += spaceString + "]\n";
        break; 
    }
    case Type::OBJECT: {
        outputString += "{\n";
        for (JSONObject::iterator i = (*values.object).begin();
            i != (*values.object).end(); i++) {
            outputString += spaceString + i->first + ": ";
            outputString += i->second->toString(indentationLevel + 1);
            JSONObject::iterator next = i;
            next++;
            if ((next) != (*values.object).end()) {
                outputString += spaceString + ",";
            }
            outputString += spaceString + "\n";
        }
        outputString += "}\n";
        return outputString;
    }
    default:
        std::cout << "ERROR" << std::endl;
        return "ERROR";
    }
    return outputString;
}
