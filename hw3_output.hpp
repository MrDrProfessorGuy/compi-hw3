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

enum Type {INVALID=0, INT, BOOL, FUNC, RET_TYPE};

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
    Data(){
        type = Type::INVALID;
        lVAL = false;
        name = "";
        value = "";
    }
    ~Data();

    Data(Data&) = delete;

    Type type;
    bool lVAL;
    std::string name;
    std::string value;
    Type retType;
    
};

class Node_class{
public:

    Node_class(Type type){
        initNodes();
        attributes.type = type;
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
    void initNodes(){
        parent.reset();
        for (int index = 0; index < MAX_CHILDREN; index++){
            children[index].reset();
        }   
    }
    
    Data attributes;

};



#define YYSTYPE Node_class*


#endif

