#ifndef _236360_3_
#define _236360_3_

#include <vector>
#include <string>
#include <memory>
#include <map>
//#include <stack>


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
enum FrameType {FUNC, LOOP, BLOCK};

class symTableEntry{
public:
    std::string name;
    Type type;
    long offset;
    bool valid;

    symTableEntry(std::string entry_name, Type entry_type, long entry_offset, bool entry_valid=true){
        name = entry_name;
        type = entry_type;
        offset = entry_offset;
        valid = entry_valid;
    }
    ~symTableEntry();
    symTableEntry(symTableEntry& entry);
};

typedef std::map<std::string, symTableEntry> dict;


class StackEntry{
public:
    long next_offset;
    FrameType frame_type;
    dict entries;

    StackEntry();
    ~StackEntry();
    StackEntry(StackEntry&) = delete;

    void newEntry(std::string name, Type entry_type){
        symTableEntry entry(name, entry_type, next_offset);
        next_offset++;

        entries.insert({name, entry});
    }
    symTableEntry find(std::string name){
        auto search = entries.find(name);
        if (search == entries.end()){
            return symTableEntry("", Type::INVALID, 0, false);
        }
        return search->second;
    }

};

typedef std::vector<StackEntry> frame;

class Frame_class{
public:
    frame frames;
    
    Frame_class();
    ~Frame_class();
    Frame_class(Frame_class&);
    
    void newEntry(std::string name, Type entry_type){
        symTableEntry entry = find(name);
        if (entry.valid){
            ///TODO: ERROR
            return;
        }

        frames.back().newEntry(name, entry_type);
    }
    symTableEntry find(std::string& name){
        for(auto iter = frames.rbegin(); iter != frames.rend(); ++iter){
            symTableEntry entry = iter->find(name);
            if (entry.valid){
                ///TODO: ERROR
                return entry;
            }
        }
    }

};

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
    ~Node_class();
    Node_class(Node_class&) = delete;

    void create_bool_attributes(bool value){
        attributes = std::make_shared<DataBool>(value);
    }
    void setAttributes(DataP attr);
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
    
    static void type_check(Node node, std::vector<Type> type){
        for (int index = 0; index < type.size(); index++){
            if (node->attributes->type == type[index]){
                return;
            }
        }
        output::errorMismatch(node->attributes->lineno);
    }

    void updateAttrRelop(){
        type_check(children[0], {Type::INT, Type::BYTE});
        type_check(children[2], {Type::INT, Type::BYTE});
        bool val = false;
        std::string relop = std::static_pointer_cast<DataToken>(children[1]->attributes)->value;
    
        long val1 = std::static_pointer_cast<DataNum>(children[0]->attributes)->value;
        long val2 = std::static_pointer_cast<DataNum>(children[3]->attributes)->value;

        if (relop.compare("<")){
            val = val1 < val2;
        }
        else if (relop.compare("<=")){
            val = val1 <= val2;
        }
        else if (relop.compare(">")){
            val = val1 > val2;
        }
        else if (relop.compare(">=")){
            val = val1 >= val2;
        }
        else if (relop.compare("==")){
            val = val1 == val2;
        }
        else if (relop.compare("!=")){
            val = val1 != val2;
        }

        setAttributes(std::allocate_shared<DataBool>(val));   
    }

    void updateAttrBinop(){
        type_check(children[0], {Type::INT, Type::BYTE});
        type_check(children[2], {Type::INT, Type::BYTE});
        
        long val = false;
        std::string binop = std::static_pointer_cast<DataToken>(children[1]->attributes)->value;
    
        long val1 = std::static_pointer_cast<DataNum>(children[0]->attributes)->value;
        long val2 = std::static_pointer_cast<DataNum>(children[3]->attributes)->value;

        if (binop.compare("*")){
            val = val1 * val2;
        }
        else if (binop.compare("/")){
            val = val1 / val2;
        }
        else if (binop.compare("+")){
            val = val1 + val2;
        }
        else if (binop.compare("-")){
            val = val1 - val2;
        }
        if (children[0]->attributes->type == Type::INT || children[2]->attributes->type == Type::INT){
                    setAttributes(std::allocate_shared<DataNum>(Type::INT, val));
        }
        else{
            setAttributes(std::allocate_shared<DataNum>(Type::BYTE, val));
        }
        
    }

    void updateAttrCast(){
        type_check(children[1], {Type::INT, Type::BYTE});
        type_check(children[3], {Type::INT, Type::BYTE});
        
        long val = std::static_pointer_cast<DataNum>(children[3]->attributes)->value;

        if (children[1]->attributes->type == Type::INT){
            setAttributes(std::allocate_shared<DataNum>(Type::INT, (int)val));
        }
        else if (children[1]->attributes->type == Type::BYTE){
            setAttributes(std::allocate_shared<DataNum>(Type::BYTE, (byte)val));
        }

    }

};




#define YYSTYPE Node


#endif

