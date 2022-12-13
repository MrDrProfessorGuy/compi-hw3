#include "hw3_output.hpp"


class Node_Statement_Block : public Node_Statement{
public:
/////////// Methods ///////////
    Node_Statement_Block(NodeVector children): Node_Statement(children){
        //TODO: nothing
    }
    ~Node_Statement_Block();
    Node_Statement_Block(Node_Statement_Block&) = delete;

};

class Node_Statement_ID_Decl : public Node_Statement{
public:

/////////// Methods ///////////
    Node_Statement_ID_Decl(NodeVector children): Node_Statement(children){
        //TODO: check validity and update symTable. 
        //      check for assignment and validity in this case
    }
    ~Node_Statement_ID_Decl();
    Node_Statement_ID_Decl(Node_Statement_ID_Decl&) = delete;

};

class Node_Statement_ID_Assign : public Node_Statement{
public:

/////////// Methods ///////////
    Node_Statement_ID_Assign(NodeVector children): Node_Statement(children){
        //TODO: check validity and update symTable. 
        //      check for assignment validity
    }
    ~Node_Statement_ID_Assign();
    Node_Statement_ID_Assign(Node_Statement_ID_Assign&) = delete;

};

class Node_Statement_Call : public Node_Statement{
public:

/////////// Methods ///////////
    Node_Statement_Call(NodeVector children): Node_Statement(children){
        //TODO: ?
    }
    ~Node_Statement_Call();
    Node_Statement_Call(Node_Statement_Call&) = delete;

};

class Node_Statement_Ret : public Node_Statement{
public:
/////////// Methods ///////////
    Node_Statement_Ret(NodeVector children): Node_Statement(children){
        //TODO: check for return type and such
    }
    ~Node_Statement_Ret();
    Node_Statement_Ret(Node_Statement_Ret&) = delete;

};

class Node_Statement_IF : public Node_Statement{
public:
/////////// Methods ///////////
    Node_Statement_IF(NodeVector children): Node_Statement(children){
        //TODO: check for exp type validity
    }
    ~Node_Statement_IF();
    Node_Statement_IF(Node_Statement_IF&) = delete;

};

class Node_Statement_While : public Node_Statement{
public:
/////////// Methods ///////////
    Node_Statement_While(NodeVector children): Node_Statement(children){
        //TODO: check for exp type validity
    }
    ~Node_Statement_While();
    Node_Statement_While(Node_Statement_While&) = delete;

};

class Node_Statement_LoopMod : public Node_Statement{
public:
/////////// Methods ///////////
    Node_Statement_LoopMod(NodeVector children): Node_Statement(children){
        //TODO: check we are currently in a loop..
    }
    ~Node_Statement_LoopMod();
    Node_Statement_LoopMod(Node_Statement_LoopMod&) = delete;

};
