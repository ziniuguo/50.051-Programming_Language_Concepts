#include <iostream>
#include <unordered_map>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include "part2.h"

using namespace std;

JSONValue JSONParser::parse() {
    skipWhiteSpace();

    char firstChar = getCurrentChar();

    if (firstChar == 'n') {
        expect("null");
        return {};
    } else if (firstChar == 't') {
        expect("true");
        return {true};
    } else if (firstChar == 'f') {
        expect("false");
        return {false};
    } else if (firstChar == '\"') {
        return parseString();
    } else if (isdigit(firstChar) || firstChar == '-') {
        return parseNumber();
    } else if (firstChar == '[') {
        return parseArray();
    } else if (firstChar == '{') {
        return parseObject();
    }

    throw invalid_argument("Invalid JSON string");
}

void JSONParser::skipWhiteSpace() {
    while (index < jsonString.size() && isspace(jsonString[index])) {
        ++index;
    }
}

char JSONParser::getCurrentChar() {
    return jsonString[index];
}

void JSONParser::expect(const string &expected) {
    size_t length = expected.length();

    if (jsonString.substr(index, length) == expected) {
        index += length;
        skipWhiteSpace();
    } else {
        throw invalid_argument("Invalid JSON string");
    }
}

string JSONParser::parseString() {
    expect("\"");

    string result;
    char currentChar = getCurrentChar();

    while (currentChar != '\"' && index < jsonString.size()) {
        if (currentChar == '\\') {
            ++index;
            currentChar = getCurrentChar();

            switch (currentChar) {
                case '\"':
                    result += '\"';
                    break;
                case '\\':
                    result += '\\';
                    break;
                case '/':
                    result += '/';
                    break;
                case 'b':
                    result += '\b';
                    break;
                case 'f':
                    result += '\f';
                    break;
                case 'n':
                    result += '\n';
                    break;
                case 'r':
                    result += '\r';
                    break;
                case 't':
                    result += '\t';
                    break;
                case 'u': {
                    string hexString = jsonString.substr(index + 1, 4);
                    int codePoint = stoi(hexString, nullptr, 16);

                    if (codePoint < 0xD800 || codePoint > 0xDFFF) {
                        result += static_cast<char>(codePoint);
                        index += 5;
                    } else {
                        throw invalid_argument("Invalid JSON string");
                    }
                    break;
                }
                default:
                    throw invalid_argument("Invalid JSON string");
                    break;
            }
        } else {
            result += currentChar;
        }

        ++index;
        currentChar = getCurrentChar();
    }

    if (currentChar == '\"') {
        ++index;
        skipWhiteSpace();
        return result;
    } else {
        throw invalid_argument("Invalid JSON string");
    }
}

double JSONParser::parseNumber() {
    size_t startIndex = index;

    if (getCurrentChar() == '-') {
        ++index;
    }

    while (index < jsonString.size() && isdigit(getCurrentChar())) {
        ++index;
    }

    if (getCurrentChar() == '.') {
        ++index;

        while (index < jsonString.size() && isdigit(getCurrentChar())) {
            ++index;
        }
    }

    if (getCurrentChar() == 'e' || getCurrentChar() == 'E') {
        ++index;

        if (getCurrentChar() == '-' || getCurrentChar() == '+') {
            ++index;
        }

        while (index < jsonString.size() && isdigit(getCurrentChar())) {
            ++index;
        }
    }

    string numberString = jsonString.substr(startIndex, index - startIndex);
    skipWhiteSpace();
    return stod(numberString);
}

vector <JSONValue> JSONParser::parseArray() {
    vector <JSONValue> result;

    expect("[");

    while (getCurrentChar() != ']') {
        result.push_back(parse());

        if (getCurrentChar() == ',') {
            ++index;
            skipWhiteSpace();
        } else if (getCurrentChar() != ']') {
            throw invalid_argument("Invalid JSON string");
        }
    }

    ++index;
    skipWhiteSpace();
    return result;
}

unordered_map <string, JSONValue> JSONParser::parseObject() {
    unordered_map <string, JSONValue> result;

    expect("{");

    while (getCurrentChar() != '}') {
        string key = parseString();
        expect(":");
        JSONValue value = parse();

        result.emplace(std::move(key), std::move(value));

        if (getCurrentChar() == ',') {
            ++index;
            skipWhiteSpace();
        } else if (getCurrentChar() != '}') {
            throw invalid_argument("Invalid JSON string");
        }
    }

    ++index;
    skipWhiteSpace();
    return result;
}

/* Generate the header file for a class */
string generate_impl_file(JSONValue inJsonValue, ofstream &header_file, ofstream &cpp_file) {
    string className = inJsonValue.objectValue["Class"].stringValue;
    /* write to header file */
    header_file << "class " << className << " {\n";
    header_file << "private:\n";
    /* write to cpp file */
    cpp_file << className << "::" << className << "(";

    string headerParamStr;
    string consParamStr;
    string consInitStr;
    string objInstStr;
    unordered_map <string, JSONValue> jsonMap = inJsonValue.objectValue;

    for (std::pair <std::string, JSONValue> element: jsonMap) {
        if (element.first.find("Field") != string::npos) {
            string index = element.first.substr(5);
            string valueKey = "Value" + index;
            /* check value type */
            JSONValue value = jsonMap[valueKey];
            consInitStr += ("this->" + element.second.stringValue + " = " + element.second.stringValue + ";\n");
            if (value.type == JSONValueType::String) {
                headerParamStr += ("string " + element.second.stringValue + ";\n");
                consParamStr += ("string " + element.second.stringValue);
                objInstStr += ("\"" + value.stringValue + "\"");
            } else if (value.type == JSONValueType::Number) {
                if (ceil(value.numberValue) == floor(value.numberValue)) {
                    headerParamStr += ("int " + element.second.stringValue + ";\n");
                    consParamStr += ("int " + element.second.stringValue);
                    objInstStr += (to_string(int(value.numberValue)));
                } else {
                    headerParamStr += ("float " + element.second.stringValue + ";\n");
                    consParamStr += ("float " + element.second.stringValue);
                    string numStr = to_string((value.numberValue));
                    /* it won't trim 440.0 to 440. though it's not required (as 440 is int and checked above) */
                    numStr.erase(numStr.find_last_not_of('0') + 1, std::string::npos);
                    numStr.erase(numStr.find_last_not_of('.') + 1, std::string::npos);
                    objInstStr += (numStr + "f");
                }
            } else if (value.type == JSONValueType::Boolean) {
                /* not needed actually */
                headerParamStr += ("bool " + element.second.stringValue + ";\n");
                consParamStr += ("bool " + element.second.stringValue);
                objInstStr += (to_string(value.booleanValue));
            } else if (value.type == JSONValueType::Array || value.type == JSONValueType::Object) {
                /* not needed either , so not implemented*/
                throw invalid_argument("Unsupported JSON value type: Array");
            } else {
                throw invalid_argument(
                        R"(Unsupported JSON value type. Either "Value" is not supported, or "Value" of corresponding "Field" is missing)");
            }
            consParamStr += ", ";
            objInstStr += ", ";
        } else if ((element.first.find("Value") == string::npos) && (element.first.find("Class") == string::npos) &&
                   (element.first.find("Instance") == string::npos)) {
            /* ignore unsupported non-Value/Class/Instance */
            cout << "Unsupported JSON entry: " << element.first << "! Ignore it...\n";
        }
    }

    consParamStr.erase(consParamStr.length() - 2); // removes last 3 characters
    objInstStr.erase(objInstStr.length() - 2); // which is ", "

    header_file << headerParamStr;

    header_file << "public:\n";
    header_file << className << "(";

    header_file << consParamStr;
    header_file << ");\n";

    header_file << "protected: \n";
    header_file << "};\n";

    cpp_file << consParamStr << ") {\n";
    cpp_file << consInitStr << "}\n";


    string outStr =
            className + " " + inJsonValue.objectValue["Instance"].stringValue + " = " + className + "(" + objInstStr +
            ");\n";
    return outStr;
}

void generate_file(const JSONValue &inJsonValue, ofstream &header_file, ofstream &cpp_file, const string &fileName) {
    /* write to header file */
    string capFileName = fileName;
    transform(capFileName.begin(), capFileName.end(), capFileName.begin(), ::toupper);
    header_file << "#ifndef " << capFileName << "_H\n";
    header_file << "#define " << capFileName << "_H\n";
    header_file << "#include <string>" << "\n";
    header_file << "using namespace std;\n";

    /* write to cpp file */
    cpp_file << "#include <iostream>\n";
    cpp_file << "#include \"" << fileName << ".h\"\n";
    cpp_file << "using namespace std;\n";

    string mainStr;
    if (inJsonValue.arrayValue.empty()) {
        mainStr += generate_impl_file(inJsonValue, header_file, cpp_file);
    } else {
        for (const JSONValue &parsedJsonValue: inJsonValue.arrayValue) {
            mainStr += generate_impl_file(parsedJsonValue, header_file, cpp_file);
        }
    }

    header_file << "#endif\n";

    cpp_file << "int main(int argc, char *argv[]) {\n";
    cpp_file << mainStr;
    cpp_file << "return 0;\n}\n";
}


int main() {
    string fileName;
    cout << "Input file name (without .json): ";
    cin >> fileName;
    ifstream inJsonFile(fileName + ".json");
    string jsonString((istreambuf_iterator<char>(inJsonFile)), istreambuf_iterator<char>());

    try {
        JSONParser parser(jsonString);
        JSONValue parsedJsonValue = parser.parse();
        ofstream header_file(fileName + ".h");
        ofstream cpp_file(fileName + ".cpp");
        generate_file(parsedJsonValue, header_file, cpp_file, fileName);
    } catch (const exception &e) {
        cerr << "Failed to parse JSON string: " << e.what() << endl;
    }

    return 0;
}