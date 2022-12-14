#include "hw3_output.hpp"


class Node_Exp_NUM : public Node_Exp{
public:
/////////// Methods ///////////

    Node_Exp_NUM(NodeVector children, Type exp_type): Node_Exp(children, exp_type){
        if (type == Type::BYTE){
            long token_val = std::atoi(std::static_pointer_cast<Node_Token>(children[0])->value.c_str());
            if (token_val > 255){
                throw ByteTooLargeExc(yylineno);
            }
        }
        
    }
    ~Node_Exp_NUM();
    Node_Exp_NUM(Node_Exp_NUM&) = delete;
};

class Node_Exp_Binop : public Node_Exp{
public:
/////////// Methods ///////////

    Node_Exp_Binop(NodeVector children): Node_Exp(children, Type::INT){
        //Todo: if bool, update type
        auto exp1 = std::static_pointer_cast<Node_Exp>(children[0]);
        auto binop = std::static_pointer_cast<Node_Token>(children[1]);
        auto exp2 = std::static_pointer_cast<Node_Exp>(children[2]);

        if (!exp1->typeCheck({Type::INT, Type::BYTE}) || !exp2->typeCheck({Type::INT, Type::BYTE})){
            throw MismatchExc(yylineno);
        }
        if (exp1->type == Type::BYTE && exp2->type == Type::BYTE){
            set_type(Type::BYTE);
        }

    }
    ~Node_Exp_Binop();
    Node_Exp_Binop(Node_Exp_NUM&) = delete;
};

class Node_Exp_Relop : public Node_Exp{
public:
/////////// Methods ///////////

    Node_Exp_Relop(NodeVector children): Node_Exp(children, Type::BOOL){
        auto exp1 = std::static_pointer_cast<Node_Exp>(children[0]);
        auto binop = std::static_pointer_cast<Node_Token>(children[1]);
        auto exp2 = std::static_pointer_cast<Node_Exp>(children[2]);

        if (!exp1->typeCheck({Type::INT, Type::BYTE}) || !exp2->typeCheck({Type::INT, Type::BYTE})){
            throw MismatchExc(yylineno);
        }

    }
    ~Node_Exp_Relop();
    Node_Exp_Relop(Node_Exp_Relop&) = delete;
};

class Node_Exp_Cast : public Node_Exp{
public:
/////////// Methods ///////////

    Node_Exp_Cast(NodeVector children): Node_Exp(children, Type::INT){
        //Todo: if bool, update type
        auto type_node = std::static_pointer_cast<Node_Exp_Type>(children[1]);
        auto exp = std::static_pointer_cast<Node_Exp>(children[3]);

        if (!type_node->typeCheck({Type::INT, Type::BYTE}) || !exp->typeCheck({Type::INT, Type::BYTE})){
            throw MismatchExc(yylineno);
        }
        
        set_type(type_node->type);
    }
    ~Node_Exp_Cast();
    Node_Exp_Cast(Node_Exp_Relop&) = delete;
};

class Node_Exp_Str : public Node_Exp{
public:
/////////// Methods ///////////

    Node_Exp_Str(NodeVector children): Node_Exp(children, Type::STRING){
        
    }
    ~Node_Exp_Str();
    Node_Exp_Str(Node_Exp_Str&) = delete;
};

class Node_Exp_Bool : public Node_Exp{
public:
/////////// Methods ///////////

    Node_Exp_Bool(NodeVector children): Node_Exp(children, Type::BOOL){
        
    }
    ~Node_Exp_Bool();
    Node_Exp_Bool(Node_Exp_Bool&) = delete;
};

class Node_Exp_Type : public Node_Exp{
public:

    Node_Exp_Type(NodeVector children, Type exp_type): Node_Exp(children, exp_type){

    }
    ~Node_Exp_Type();
    Node_Exp_Type(Node_Exp_Type&) = delete;;


};

class Node_Exp_ID : public Node_Exp{
public:

    Node_Exp_ID(NodeVector children): Node_Exp(children, Type::INVALID){
        //TODO: check validity and update frame stack 
        auto node_token_id = std::static_pointer_cast<Node_Token>(children[0]);
        symTableEntry entry = frame_manager.find(node_token_id->value);
        if (!entry.valid){
            throw UndefExc(yylineno);
            //printf();
        }
        set_type(entry.type);
    }
    ~Node_Exp_ID();
    Node_Exp_ID(Node_Exp_ID&) = delete;;

};

class Node_Exp_Call : public Node_Exp{
public:

    Node_Exp_Call(NodeVector children): Node_Exp(children, Type::INVALID){
        ///TODO: check validity and update frame stack 
        auto node_call = std::dynamic_pointer_cast<Node_Call>(children[0]);
        auto func_entry = std::dynamic_pointer_cast<symTableEntryFunc>(frame_manager.find(node_call->ID()));
        assert(func_entry->valid);

        set_type(func_entry->ret_type);
    }
    ~Node_Exp_Call()=default;
    Node_Exp_Call(Node_Exp_Call&) = delete;;

};

class Node_Exp_IfElse : public Node_Exp{
public:

    Node_Exp_IfElse(NodeVector children, Type exp_type): Node_Exp(children, exp_type){
        ///TODO: check validity and update frame stack 
    }
    ~Node_Exp_IfElse();
    Node_Exp_IfElse(Node_Exp_IfElse&) = delete;;

};
