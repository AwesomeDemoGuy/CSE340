/*
 * Copyright (C) Rida Bazzi, 2019
 *
 * Do not share this file with anyone
 */
#ifndef __PARSER_H__
#define __PARSER_H__

#include <string>
#include <vector>
#include "lexer.h"

struct poly_header_t {
  Token name;
  std::vector<Token> id_list;
};

struct poly_decl_t {
	poly_header_t header;
};


class Parser {
  public:
    //void ConsumeAllInput();
    void parse_input();
    std::vector<Token> IM_4;

  private:
    LexicalAnalyzer lexer;
    void syntax_error();
    Token expect(TokenType expected_type);
    void parse_program();
    void parse_tasks_section();
    void parse_poly_section();
    void parse_execute_section();
    void parse_inputs_section();
    void parse_num_list();
    void parse_poly_decl_list();
    poly_decl_t parse_poly_decl();
    poly_header_t parse_poly_header();
    std::vector<Token> make_list(char id_name);
    Token parse_poly_name();
    std::vector<Token> parse_id_list();
    void parse_term_list(poly_decl_t &poly_decl);
    void parse_poly_body(poly_decl_t &poly_decl);
    void parse_term(poly_decl_t &poly_decl);
    void parse_monomial_list(poly_decl_t &poly_decl);
    void parse_coefficient();
    void parse_monomial(poly_decl_t &poly_decl);
    void parse_exponent();
    void parse_parenthesized_list(poly_decl_t &poly_decl);
    void parse_add_operator();
    void parse_statement_list();
    void parse_statement();
    void parse_input_statement();
    void parse_output_statement();
    void parse_assign_statement();
    void parse_poly_evaluation();
    void parse_argument_list();
    void parse_argument();
};

#endif

