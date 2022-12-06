#ifndef _236360_3_
#define _236360_3_

#include <vector>
#include <string>
#include <memory>
#include <map>
#include <stack>
using namespace std;

#define MAX_CHILDREN 8


namespace output{
    void endScope();
    void printID(const string& id, int offset, const string& type);

    /* Do not save the string returned from this function in a data structure
        as it is not dynamically allocated and will be destroyed(!) at the end of the calling scope.
    */
    string makeFunctionType(const string& retType, vector<string>& argTypes);

    void errorLex(int lineno);
    void errorSyn(int lineno);
    void errorUndef(int lineno, const string& id);
    void errorDef(int lineno, const string& id);
    void errorUndefFunc(int lineno, const string& id);
    void errorMismatch(int lineno);
    void errorPrototypeMismatch(int lineno, const string& id, vector<string>& argTypes);
    void errorUnexpectedBreak(int lineno);
    void errorUnexpectedContinue(int lineno);
    void errorMainMissing();
    void errorByteTooLarge(int lineno, const string& value);

}

class Node_class;
typedef std::shared_ptr<Node_class> Node;

enum Type {INVALID=0, INT, BYTE, BOOL, STRING, FUNC, RET_TYPE};

struct symTableEntry{
    std::string name;
    Type type;
    long offset;
};

typedef std::map<std::string, symTableEntry> dict;
typedef std::stack<dict> frame;
typedef std::stack<long> offset_frame;

class Data{
public:
    const Type type;
    bool lVAL;

    Data(Type data_type) : type(data_type){
        lVAL = false;
    }
    ~Data();
    Data(Data&) = delete;

    
};
class DataToken : public Data{
public:
    std::string value;
    DataToken(std::string token_value){
        value = token_value;
    }
    ~DataToken();
    DataToken(DataToken&)=delete;

};
class DataNum : public Data{
public:
    int value;
    DataNum(Type num_type) : Data(num_type){
        value = 0;
    }   
    DataNum(Type num_type, int num_value): Data(num_type){
        value = num_value;
    }
    ~DataNum();
    DataNum(DataNum&)=delete;

};
class DataBool : public Data{
public:
    bool value;

    DataBool(): Data(Type::BOOL){
        value = false;
    }
    ~DataBool();
    DataBool(DataBool&)=delete;
};
class DataStr : public Data{
public:
    std::string value;

    DataStr(std::string str_value): Data(Type::STRING){
        value = str_value;
    }
    ~DataStr();
    DataStr(DataStr&)=delete;
};


class DataID : public DataNum{
public:
    DataID(std::string id_name){
        name = id_name;
        value = "";
    }
    ~DataID();
    DataID(DataID&)=delete;

    // id
    std::string name;
    //std::string value;

};

class DataFunc : public Data{
public:
    std::string name;
    Type retType;

    DataFunc(Type ret_type): Data(ret_type){
        name = "";
    }
    ~DataFunc();
    DataFunc(DataFunc&)=delete;


};

class Node_class{
public:

    Node_class(Type type){
        initNodes();
        if (type == Type::INT){
            attributes = std::make_shared<DataNum>(type);
        }
        else if (type == Type::BYTE){
            attributes = std::make_shared<DataNum>(type);
        }
        else if (type == Type::BOOL){
            attributes = std::make_shared<DataBool>(type);
        }
        else if (type == Type::STRING){
            attributes = std::make_shared<DataStr>(type);
        }
        //attributes.type = type;
    }
    Node_class(Type type, Type retType){
        initNodes();
        attributes.type = type;
        attributes.retType = retType;
    }
    Node_class(Type type, std::string name, std::string value){
        initNodes();
        attributes.type = type;
        attributes.name = name;
        attributes.value = value;
    }

    ~Node_class();
    
    Node_class(Node_class&) = delete;

    void create_int_attributes();

    Node parent;
    Node children[MAX_CHILDREN];
    std::shared_ptr<Data> attributes;
    
    
    void initNodes(){
        parent.reset();
        for (int index = 0; index < MAX_CHILDREN; index++){
            children[index].reset();
        }   
    }
    
    

};



#define YYSTYPE Node_class*


#endif

