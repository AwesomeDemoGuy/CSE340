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
#include <algorithm>
#include "parser.h"

using namespace std;

// You should provide the syntax error message for this function
void Parser::syntax_error()
{
    cout << "SYNTAX ERROR !!!!!&%!!!!&%!!!!!!\n";
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

void Parser::parse_input() {
    parse_program();
    expect(END_OF_FILE);

    sort(DMT_12.begin(), DMT_12.end(), [](Token a, Token b) {
        return a.line_no < b.line_no;
    });
    if (DMT_12.size() > 0) {
        cout << "Semantic Error Code DMT-12:";
        for (int i = 0; i < DMT_12.size(); i++) {
            cout << " ";
            cout << DMT_12[i].line_no;
        }
        cout << "\n";
        exit(1);
    }
    
    sort(IM_4.begin(), IM_4.end(), [](Token a, Token b) {
        return a.line_no < b.line_no;
    });
    if (IM_4.size() > 0) {
        cout << "Semantic Error Code IM-4:";
        for (int i = 0; i < IM_4.size(); i++) {
            cout << " ";
            cout << IM_4[i].line_no;
        }
        cout << "\n";
        exit(1);
    }

    sort(AUP_13.begin(), AUP_13.end(), [](Token a, Token b) {
        return a.line_no < b.line_no;
    });
    if (AUP_13.size() > 0) {
        cout << "Semantic Error Code AUP-13:";
        for (int i = 0; i < AUP_13.size(); i++) {
            cout << " ";
            cout << AUP_13[i].line_no;
        }
        cout << "\n";
        exit(1);
    }

    sort(NA_7.begin(), NA_7.end(), [](Token a, Token b) {
        return a.line_no < b.line_no;
    });
    if (NA_7.size() > 0) {
        cout << "Semantic Error Code NA-7:";
        for (int i = 0; i < NA_7.size(); i++) {
            cout << " ";
            cout << NA_7[i].line_no;
        }
        cout << "\n";
        exit(1);
    }

    for (Token t : Tasks) {
        if (t.lexeme == "3") {
            display_polynomials();
            break;
        }
    }
}

void Parser::parse_program() {
    // program -> tasks_section poly_section execute_section inputs_section
    parse_tasks_section();
    parse_poly_section();
    parse_execute_section();
    parse_inputs_section();
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

poly_header_t Parser::parse_poly_evaluation() {
    // poly_evaluation -> poly_name LPAREN argument_list RPAREN
    poly_header_t header;
    header.name = parse_poly_name();
    expect(LPAREN);
    header.id_list = parse_argument_list();
    expect(RPAREN);

    bool found = false;
    for (size_t i = 0; i < Polynomials.size(); i++) {
        if (Polynomials[i].header.name.lexeme == header.name.lexeme) {
            found = true;
            break;
        }
    }
    if (!found) {
        AUP_13.push_back(header.name);
    } else {
        // check number of arguments
    }

    for (size_t i = 0; i < Polynomials.size(); i++) {
        if (Polynomials[i].header.name.lexeme == header.name.lexeme) {
            if (Polynomials[i].header.id_list.size() != header.id_list.size()) {
                NA_7.push_back(header.name);
            }
            break;
        }
    }

    return header;
}

vector<Token> Parser::parse_argument_list() {
    // argument_list -> argument
    // argument_list -> argument COMMA argument_list
    vector<Token> args;
    // ad parse_argument to args
    args.push_back(parse_argument());
    Token t = lexer.peek(1);
    if (t.token_type == COMMA) {
        expect(COMMA);
        vector<Token> more_args = parse_argument_list();
        args.insert(args.end(), more_args.begin(), more_args.end());
    }
    return args;
}

Token Parser::parse_argument() {
    // argument -> ID
    // argument -> NUM
    // argument -> poly_evaluation
    Token arg;
    Token t = lexer.peek(1);
    if (t.token_type == ID) {
        Token t2 = lexer.peek(2);
        if (t2.token_type == LPAREN) {
            arg = parse_poly_evaluation().name;
        } else {
            arg = expect(ID);
        }
    } else if (t.token_type == NUM) {
        arg = expect(NUM);
    } else {
        syntax_error();
    }
    return arg;
}

void Parser::parse_tasks_section() {
    // tasks_section -> TASKS num_list
    expect(TASKS);
    parse_num_list();   
}

void Parser::parse_num_list() {
    // num_list -> NUM
    // num_list -> NUM num_list
    Token t = expect(NUM);
    Tasks.push_back(t);
    Token t2 = lexer.peek(1);
    if (t2.token_type == NUM) {
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
    poly_decl_t poly_decl = parse_poly_decl();
    if (Polynomials.size() > 0) {
        for (int i = 0; i < Polynomials.size(); i++) {
            if (Polynomials[i].header.name.lexeme == poly_decl.header.name.lexeme) {
                DMT_12.push_back(poly_decl.header.name);
                break;
            }
        }
    }
    Polynomials.push_back(poly_decl);
    Token t = lexer.peek(1);
    if (t.token_type == ID) {
        parse_poly_decl_list();
    }
}

poly_decl_t Parser::parse_poly_decl() {
    // poly_decl -> poly_header EQUAL poly_body SEMICOLON

    poly_decl_t decl;
    decl.header = parse_poly_header();
    expect(EQUAL);
    parse_poly_body(decl);
    expect(SEMICOLON);
    return decl;
}

void Parser::parse_poly_body(poly_decl_t &poly_decl) {
    // poly_body -> term_list  
    Token add_operator;
    add_operator.lexeme = "";
    add_operator.token_type = PLUS; // default to PLUS for the first term
    parse_term_list(add_operator);
}

vector<term_t> Parser::parse_term_list(Token add_operator) {
    // term_list -> term
    // term_list -> term add_operator term_list
    vector<term_t> term_list;
    parse_term(add_operator);
    Token t = lexer.peek(1);
    if (t.token_type == PLUS || t.token_type == MINUS) {
        Token add_operator = parse_add_operator();
        // Store the add_operator in the current (last) term
        
        parse_term_list(add_operator);
    }
    return term_list;
}

Token Parser::parse_add_operator() {
    // add_operator -> PLUS
    // add_operator -> MINUS
    Token t = lexer.peek(1);
    if (t.token_type == PLUS) {
        return expect(PLUS);
    } else if (t.token_type == MINUS) {
        return expect(MINUS);
    } else {
        syntax_error();
    }
    // to suppress compiler warning
    Token dummy;
    return dummy;
}

void Parser::parse_term(Token add_operator) {
    // term -> monomial_list                // ID ID*
    // term -> coefficient monomial_list    // NUM ID ID*
    // term -> coefficient                  // NUM
    // term -> parenthesized_list           // ( term_list ) parenthesized_list*
    Token t = lexer.peek(1);
    term_t term;
    term.add_operator = add_operator;
    if (t.token_type == ID) {
        for (auto id : Parser::Polynomials.back().header.id_list) {
            term.exponents[id.lexeme] = 0;
        }
        term.coefficient = 1;
        parse_monomial_list(term);
    } else if (t.token_type == NUM) {
        for (auto id : Parser::Polynomials.back().header.id_list) {
            term.exponents[id.lexeme] = 0;
        }
        term.coefficient = parse_coefficient();
        term.add_operator = add_operator;
        Token t2 = lexer.peek(1);
        if (t2.token_type == ID) {
            parse_monomial_list(term);
        }
    } else if (t.token_type == LPAREN) {
        term.parenthesized = true;
        term.parenthesized_list = parse_parenthesized_list(add_operator);
    } else {
        syntax_error();
    }
}

void Parser::parse_monomial_list(term_t& term) {
    // monomial_list -> monomial
    // monomial_list -> monomial monomial_list
    parse_monomial(term);
    Token t = lexer.peek(1);
    if (t.token_type == ID) {
        parse_monomial_list(term);
    }
}

void Parser::parse_monomial(term_t& term) {
    // monomial -> ID
    // monomial -> ID exponent
    Token t = expect(ID);

    bool found = false;
    for (int i = 0; i < Parser::Polynomials.back().header.id_list.size(); i++) {
        if (t.lexeme == Parser::Polynomials.back().header.id_list[i].lexeme) {
            found = true;
            break;
        }
    }
    if (!found) {
        IM_4.push_back(t);
    }

    int exp = 1;
    Token t2 = lexer.peek(1);
    if (t2.token_type == POWER) {
        exp = parse_exponent();
    }

    term.exponents[t.lexeme] += exp;
}

int Parser::parse_exponent() {
    // exponent -> POWER NUM
    expect(POWER);
    Token t = expect(NUM);
    return stoi(t.lexeme);
}

vector<vector<term_t>> Parser::parse_parenthesized_list(Token add_operator) {
    // parenthesized_list -> LPAREN term_list RPAREN
    // parenthesized_list -> LPAREN term_list RPAREN parenthesized_list
    vector<vector<term_t>> parenthesized_list;

    expect(LPAREN);
    vector<term_t> term_list = parse_term_list(add_operator);
    parenthesized_list.push_back(term_list);

    expect(RPAREN);

    Token t = lexer.peek(1);
    if (t.token_type == LPAREN) {
        vector<vector<term_t>> more_parenthesized = parse_parenthesized_list(add_operator);
        parenthesized_list.insert(parenthesized_list.end(), more_parenthesized.begin(), more_parenthesized.end());
    }

    return parenthesized_list;
}

int Parser::parse_coefficient() {
    // coefficient -> NUM
    return stoi(expect(NUM).lexeme);
}

poly_header_t Parser::parse_poly_header() {
    // poly_header -> poly_name
    // poly_header -> poly_name LPAREN id_list RPAREN
    poly_header_t header;
    header.name = parse_poly_name();
    Token t = lexer.peek(1);
    if (t.token_type == LPAREN) {
        expect(LPAREN);
        header.id_list = parse_id_list();
        expect(RPAREN);
    } else {
        header.id_list = make_list('x');
    }
    return header;
}

vector<Token> Parser::make_list(char id_name) {
    vector<Token> ids;
    Token t;
    t.lexeme = id_name;
    t.token_type = ID;
    ids.push_back(t);
    return ids;
}

Token Parser::parse_poly_name() {
    // poly_name -> ID
    return expect(ID);
}

vector<Token> Parser::parse_id_list() {
    // id_list -> ID
    // id_list -> ID COMMA id_list
    vector<Token> ids;
    ids.push_back(expect(ID));
    Token t = lexer.peek(1);
    if (t.token_type == COMMA) {
        expect(COMMA);
        vector<Token> more_ids = parse_id_list();
        ids.insert(ids.end(), more_ids.begin(), more_ids.end());
    }
    return ids;
}

void Parser::display_polynomials() {
    for (auto poly : Polynomials) {
        std::cout << poly.header.name.lexeme;
        if (!(poly.header.id_list.size() == 1 && poly.header.id_list[0].lexeme == "x")) {
            std::cout << "(";
            for (int i = 0; i < poly.header.id_list.size(); i++) {
                std::cout << poly.header.id_list[i].lexeme;
                if (i != poly.header.id_list.size() - 1) {
                    std::cout << ",";
                }
            }
            std::cout << ")";
        }
        std::cout << " =";

        for (auto &term : poly.term_list) {
            if (term.parenthesized) {
                std::cout << " (";
            }
            if (term.coefficient != 0) {
                // Check if the coefficient is 1 by iteself or 1 followed by monomial
                if (term.coefficient == 1) {
                    bool is_only_one = true;
                    for (const auto &exp_pair : term.exponents) {
                        if (exp_pair.second != 0) {
                            is_only_one = false;
                            break;
                        }
                    }
                    if (is_only_one) {
                        std::cout << " " << "1";
                    }
                } else {
                    std::cout << " " << term.coefficient;
                }
            }
            for (const auto &exp_pair : term.exponents) {
                if (exp_pair.second > 0) {
                    std::cout << " " << exp_pair.first;
                    if (exp_pair.second > 1) {
                        std::cout << "^" << exp_pair.second;
                    }
                }
            }
            if (term.ends_paren_group) {
                std::cout << " )";
            }
            if (term.add_operator.token_type == PLUS) {
                std::cout << " +";
            } else if (term.add_operator.token_type == MINUS) {
                std::cout << " -";
            }
        }

        /* 
        for (int i = 0; i < poly.term_list.size(); i++) {
            if (poly.term_list[i].starts_paren_group) {
                std::cout << " (";
            } else {
                std::cout << " ";
            }

            if (poly.term_list[i].coefficient.lexeme != "0") {
                std::cout << poly.term_list[i].coefficient.lexeme;
            }
            
            // Display variables in header order with their exponents
            for (int j = 0; j < poly.header.id_list.size(); j++) {
                std::string var = poly.header.id_list[j].lexeme;
                int exp = poly.term_list[i].exponents[var];
                if (exp > 0) {
                    // Add space only if coefficient was not printed or was "0"
                    if (poly.term_list[i].coefficient.lexeme == "0" || poly.term_list[i].coefficient.lexeme.empty()) {
                         if (j == 0 || poly.term_list[i].exponents[poly.header.id_list[j-1].lexeme] == 0) {
                             // no space if it's the very first part of the term
                         } else {
                            std::cout << " ";
                         }
                    } else {
                        std::cout << " ";
                    }
                    std::cout << var;
                    if (exp > 1) {
                        std::cout << "^" << exp;
                    }
                }
            }
            
            if (poly.term_list[i].ends_paren_group) {
                std::cout << ")";
            }

            // Display add_operator if it exists and this isn't the last term
            if (poly.term_list[i].add_operator.token_type == PLUS) {
                std::cout << " +";
            } else if (poly.term_list[i].add_operator.token_type == MINUS) {
                std::cout << " -";
            }
        }
        */
        std::cout << "\n";
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
    parser.parse_input();
}
