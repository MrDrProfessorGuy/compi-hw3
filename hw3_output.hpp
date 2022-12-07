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
class Data;

typedef std::shared_ptr<Node_class> Node;
typedef std::vector<Node>   NodeVector;
typedef std::shared_ptr<Data> DataP;

enum Type {INVALID=0, INT, BYTE, BOOL, STRING, FUNC, TOKEN, RET_TYPE};

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

class DataID : public Data{
public:
    std::string name;
    symTableEntry* id_entry;
    
    DataID(Type data_type, std::string name): Data(data_type){
        this->name = name;
        /// TODO: fill id_entry
    }
    ~DataID();
    DataID(DataID&) = delete;
};

class DataType : public Data{
public:
    DataType(Type d_type): Data(d_type){}
    ~DataType();
    DataType(DataType&)=delete;
};
class DataToken : public Data{
public:
    std::string value;
    DataToken(std::string token_value): Data(Type::TOKEN){
        value = token_value;
        switch(type){}
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

    DataBool(bool val): Data(Type::BOOL){
        value = val;
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


std::vector<Node> TreeNodes;
Node createNode(){
    int index = TreeNodes.size();
    TreeNodes.push_back(std::make_shared<Node_class>(index));
    return TreeNodes.back();
}

class Node_class{
public:
    Node parent;
    //Node children[MAX_CHILDREN];
    NodeVector children;
    DataP attributes;
    int node_index;

/////////// Methods ///////////

    Node_class(){
        //initNodes();
    }
    Node_class(int node_idx){
        //initNodes();
        node_index = node_idx;
    }
    Node_class(Type type){
        //initNodes();
        if (type == Type::INT){
            attributes = std::make_shared<DataNum>(type);
        }
        else if (type == Type::BYTE){
            attributes = std::make_shared<DataNum>(type);
        }
        else if (type == Type::BOOL){
            attributes = std::make_shared<DataBool>();
        }
        else if (type == Type::STRING){
            attributes = std::make_shared<DataStr>(type);
        }
        else if (type == Type::TOKEN){
            attributes = std::make_shared<DataToken>(type);
        }
        //attributes.type = type;
    }
    Node_class(Type type, std::string name){
        //initNodes();
        if (type == Type::TOKEN){
            attributes = std::make_shared<DataToken>(name);
        }
        else{
            exit(1);
        }
        
    }
    Node_class(Type type, std::string name, std::string value){
        //initNodes();
        attributes.type = type;
        attributes.name = name;
        attributes.value = value;
    }

    ~Node_class();
    Node_class(Node_class&) = delete;

    void create_bool_attributes(bool value){
        attributes = std::make_shared<DataBool>(value);
    }
    void setAttributes(Data attr);
    void initNodes(){
        parent.reset();
        for (int index = 0; index < MAX_CHILDREN; index++){
            children[index].reset();
        }   
    }
    void setChildren(NodeVector vector){
        for (int index = 0; index < vector.size(); index++){
            children.push_back(vector[index]);
            children[index]->setParent(get());
        }
    }
    void setParent(Node parent){
        this->parent = parent;
    }
    Node get(){
        return TreeNodes[node_index];
    }

};




#define YYSTYPE Node


#endif

