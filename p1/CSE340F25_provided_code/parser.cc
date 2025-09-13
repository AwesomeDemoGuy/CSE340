/*
 * Copyright (C) Rida Bazzi, 2020
 *
 * Do not share this file with anyone
 *
 * Do not post this file or derivatives of
 * of this file online
 *
 */
#include <iostream>
#include <cstdlib>
#include "parser.h"

using namespace std;

// You should provide the syntax error message for this function
void Parser::syntax_error()
{
    cout << "SYNTAX ERROR !!!!!&%!!!!&%!!!!!!";
    exit(1);
}

// this function gets a token and checks if it is
// of the expected type. If it is, the token is
// returned, otherwise, synatx_error() is generated
// this function is particularly useful to match
// terminals in a right hand side of a rule.
// Written by Mohsen Zohrevandi
Token Parser::expect(TokenType expected_type)
{
    Token t = lexer.GetToken();
    if (t.token_type != expected_type)
        syntax_error();
    return t;
}

// Parsing

// This function is simply to illustrate the GetToken() function.
// You will not need it for your project and you can delete it.
// The function also illustrates the use of peek()
// void Parser::ConsumeAllInput()
// {
//     Token token;
//     int i = 1;

//     token = lexer.peek(i);
//     token.Print();
//     while (token.token_type != END_OF_FILE)
//     {
//         i = i + 1;
//         token = lexer.peek(i);
//         token.Print();
//     }

//     token = lexer.GetToken();
//     token.Print();
//     while (token.token_type != END_OF_FILE)
//     {
//         token = lexer.GetToken();
//         token.Print();
//     }
// }

void Parser::parse_program() {
    // program -> tasks_section poly_section execute_section inputs_section
    parse_tasks_section();
    parse_poly_section();
    parse_execute_section();
    parse_inputs_section();
    expect(END_OF_FILE);
}

void Parser::parse_inputs_section() {
    // inputs_section -> INPUTS num_list
    expect(INPUTS);
    parse_num_list();
}

void Parser::parse_execute_section() {
    // execute_section -> EXECUTE statement_list
    expect(EXECUTE);
    parse_statement_list();
}

void Parser::parse_statement_list() {
    // statement_list -> statement
    // statement_list -> statement statement_list
    parse_statement();
    Token t = lexer.peek(1);
    if (t.token_type == INPUT || t.token_type == OUTPUT || t.token_type == ID) {
        parse_statement_list();
    }
}

void Parser::parse_statement() {
    // statement -> input_statement
    // statement -> output_statement
    // statement -> assign_statement
    Token t = lexer.peek(1);
    if (t.token_type == INPUT) {
        parse_input_statement();
    } else if (t.token_type == OUTPUT) {
        parse_output_statement();
    } else if (t.token_type == ID) {
        parse_assign_statement();
    } else {
        syntax_error();
    }
}

void Parser::parse_input_statement() {
    // input_statement -> INPUT ID SEMICOLON
    expect(INPUT);
    expect(ID);
    expect(SEMICOLON);
}

void Parser::parse_output_statement() {
    // output_statement -> OUTPUT ID SEMICOLON
    expect(OUTPUT);
    expect(ID);
    expect(SEMICOLON);
}

void Parser::parse_assign_statement() {
    // assign_statement -> ID EQUAL poly_evaluation  SEMICOLON
    expect(ID);
    expect(EQUAL);
    parse_poly_evaluation();
    expect(SEMICOLON);
}   

void Parser::parse_poly_evaluation() {
    // poly_evaluation -> poly_name LPAREN argument_list RPAREN
    parse_poly_name();
    expect(LPAREN);
    parse_argument_list();
    expect(RPAREN);
}

void Parser::parse_argument_list() {
    // argument_list -> argument
    // argument_list -> argument COMMA argument_list
    parse_argument();
    Token t = lexer.peek(1);
    if (t.token_type == COMMA) {
        expect(COMMA);
        parse_argument_list();
    }
}

void Parser::parse_argument() {
    // argument -> ID
    // argument -> NUM
    // argument -> poly_evaluation
    Token t = lexer.peek(1);
    if (t.token_type == ID) {
        Token t2 = lexer.peek(2);
        if (t2.token_type == LPAREN) {
            parse_poly_evaluation();
        } else {
            expect(ID);
        }
    } else if (t.token_type == NUM) {
        expect(NUM);
    } else {
        syntax_error();
    }
}

void Parser::parse_tasks_section() {
    // tasks_section -> TASKS num_list
    expect(TASKS);
    parse_num_list();   
}

void Parser::parse_num_list() {
    // num_list -> NUM
    // num_list -> NUM num_list
    expect(NUM);
    Token t = lexer.peek(1);
    if (t.token_type == NUM) {
        parse_num_list();
    }
}

void Parser::parse_poly_section() {
    // poly_section -> POLY poly_decl_list
    expect(POLY);
    parse_poly_decl_list();
}

void Parser::parse_poly_decl_list() {
    // poly_decl_list -> poly_decl
    // poly_decl_list -> poly_decl poly_decl_list
    parse_poly_decl();
    Token t = lexer.peek(1);
    if (t.token_type == ID) {
        parse_poly_decl_list();
    }
}

void Parser::parse_poly_decl() {
    // poly_decl -> poly_header EQUAL poly_body SEMICOLON
    parse_poly_header();
    expect(EQUAL);
    parse_poly_body();
    expect(SEMICOLON);
}

void Parser::parse_poly_body() {
    // poly_body -> term_list
    parse_term_list();
}

void Parser::parse_term_list() {
    // term_list -> term
    // term_list -> term add_operator term_list
    parse_term();
    Token t = lexer.peek(1);
    if (t.token_type == PLUS || t.token_type == MINUS) {
        parse_add_operator();
        parse_term_list();
    }
}

void Parser::parse_add_operator() {
    // add_operator -> PLUS
    // add_operator -> MINUS
    Token t = lexer.peek(1);
    if (t.token_type == PLUS) {
        expect(PLUS);
    } else if (t.token_type == MINUS) {
        expect(MINUS);
    } else {
        syntax_error();
    }
}

void Parser::parse_term() {
    // term -> monomial_list                // ID ID*
    // term -> coefficient monomial_list    // NUM ID ID*
    // term -> coefficient                  // NUM
    // term -> parenthesized_list           // ( term_list ) parenthesized_list*
    Token t = lexer.peek(1);
    if (t.token_type == ID) {
        parse_monomial_list();
    } else if (t.token_type == NUM) {
        parse_coefficient();
        Token t2 = lexer.peek(2);
        if (t2.token_type == ID) {
            parse_monomial_list();
        }
    } else if (t.token_type == LPAREN) {
        parse_parenthesized_list();
    } else {
        syntax_error();
    }
}

void Parser::parse_monomial_list() {
    // monomial_list -> monomial
    // monomial_list -> monomial monomial_list
    parse_monomial();
    Token t = lexer.peek(1);
    if (t.token_type == ID) {
        parse_monomial_list();
    }
}

void Parser::parse_monomial() {
    // monomial -> ID
    // monomial -> ID exponent
    expect(ID);
    Token t = lexer.peek(1);
    if (t.token_type == POWER) {
        parse_exponent();
    }
}

void Parser::parse_exponent() {
    // exponent -> POWER NUM
    expect(POWER);
    expect(NUM);
}

void Parser::parse_parenthesized_list() {
    // parenthesized_list -> LPAREN term_list RPAREN
    // parenthesized_list -> LPAREN term_list RPAREN parenthesized_list
    expect(LPAREN);
    parse_term_list();
    expect(RPAREN);
    Token t = lexer.peek(1);
    if (t.token_type == LPAREN) {
        parse_parenthesized_list();
    }
}

void Parser::parse_coefficient() {
    // coefficient -> NUM
    expect(NUM);
}

void Parser::parse_poly_header() {
    // poly_header -> poly_name
    // poly_header -> poly_name LPAREN id_list RPAREN
    parse_poly_name();
    Token t = lexer.peek(1);
    if (t.token_type == LPAREN) {
        expect(LPAREN);
        parse_id_list();
        expect(RPAREN);
    }
}

void Parser::parse_poly_name() {
    // poly_name -> ID
    expect(ID);
}

void Parser::parse_id_list() {
    // id_list -> ID
    // id_list -> ID COMMA id_list
    expect(ID);
    Token t = lexer.peek(1);
    if (t.token_type == COMMA) {
        expect(COMMA);
        parse_id_list();
    }
}

int main()
{
    // note: the parser class has a lexer object instantiated in it. You should not be declaring
    // a separate lexer object. You can access the lexer object in the parser functions as shown in the
    // example method Parser::ConsumeAllInput
    // If you declare another lexer object, lexical analysis will not work correctly
    Parser parser;

    //parser.ConsumeAllInput();
    parser.parse_program();
}
