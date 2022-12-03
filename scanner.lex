%{

/* Declarations section */
#include <stdio.h>
#include "parser.tab.hpp"
#include "hw3_output.hpp"
#include "iostream"
//printf("FLEX:: %d: %s\n", yylineno, yytext);
#define newToken(token_type) token_value = yytext;return token_type;
#define Comment token_value = "\\\\"; return COMMENT;

using namespace std;

std::string token_value;

std::string tokentype_str[] = {"TEST", "VOID", "INT", "BYTE",
                 "B", "BOOL", "AND", "OR", 
                 "NOT", "TRUE", "FALSE", "RETURN", 
                 "IF", "ELSE", "WHILE", "BREAK", 
                 "CONTINUE", "SC", "COMMA", "LPAREN", 
                 "RPAREN", "LBRACE", "RBRACE", "ASSIGN", 
                 "RELOP", "BINOP", "COMMENT", "ID", 
                 "NUM", "STRING"}; 

void showToken(char* token_name);
void newString(char* token_name);
void printString(char* token_name);
void appendString(int token_name);
void str_es(int escapeSequence_type);


//int newToken(int token_type);

//token_illegal   ("{printable}*\\[{printable}*-[n|r|t|0|x{hexa}{hexa}]]"|"{printable}*"{printable}+|"\\n"|"\\r")
//token_string    ("[{printable}*-[{token_illegal}]]")


%}

%option yylineno
%option noyywrap
digit   		([0-9])
hexa            ([0-9A-Fa-f])
letter  		([a-zA-Z])
alphanumeric    ([a-zA-Z0-9])
whitespace		([\t\n\r ])

token_void      (void)
token_int       (int)
token_byte      (byte)
token_b         (b)
token_bool      (bool)
token_and       (and)
token_or        (or)
token_not       (not)
token_true      (true)
token_false     (false)
token_return    (return)
token_if        (if)
token_else      (else)
token_while     (while)
token_break     (break)
token_continue  (continue)
token_sc        (;)
token_comma     (,)
token_lparen    (\()
token_rparen    (\))
token_lbrace    (\{)
token_rbrace    (\})
token_assign    (=)
token_equeality    (==|!=)
token_relop     (<|>|<=|>=)
token_binop_mul     (\*|\/)
token_binop_add    (\+|\-)
token_comment   (\/\/[^\r\n]*[\r|\n|\r\n]?)
token_id        ([a-zA-Z][a-zA-Z0-9]*)
token_num       (0|[1-9]{digit}*)
token_string    (\"([^\n\r\"\\]|\\[rnt"\\])+\")



%%

{token_void}      return new Node_class(Type::RET_TYPE, Type::VOID);
{token_int}       return new Node_class(Type::INT);
{token_byte}      return new Node_class(Type::BYTE);
{token_b}         newToken(B);
{token_bool}      return new Node_class(Type::BOOL);
{token_and}       newToken(AND);
{token_or}        newToken(OR);
{token_not}       newToken(NOT);
{token_true}      newToken(TRUE);
{token_false}     newToken(FALSE);
{token_return}    newToken(RETURN);
{token_if}        newToken(IF);
{token_else}      newToken(ELSE);
{token_while}     newToken(WHILE);
{token_break}     newToken(BREAK);
{token_continue}  newToken(CONTINUE);
{token_sc}        newToken(SC);
{token_comma}     newToken(COMMA);
{token_lparen}    newToken(LPAREN);
{token_rparen}    newToken(RPAREN);
{token_lbrace}    newToken(LBRACE);
{token_rbrace}    newToken(RBRACE);
{token_assign}    newToken(ASSIGN);
{token_relop}     newToken(RELOP);
{token_equeality}    newToken(EQUALITY);
{token_binop_add}     newToken(BINOP_ADD);
{token_binop_mul}     newToken(BINOP_MUL);

{token_id}        return new Node_class(Type::INVALID, std::string(yytext), "");
{token_num}       return new Node_class(Type::INVALID, , "", std::string(yytext));
{token_string}    return new Node_class(Type::INVALID, , "", std::string(yytext));

{token_comment}              ;
{whitespace}                 ;
.		output::errorLex(yylineno);exit(0);


%%

