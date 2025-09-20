/*
 * Copyright (C) Rida Bazzi, 2019
 *
 * Do not share this file with anyone
 */
#ifndef __PARSER_H__
#define __PARSER_H__

#include <string>
#include <vector>
#include <map>
#include "lexer.h"

struct poly_header_t {
  Token name;
  std::vector<Token> id_list;
};

//
using Exponent_map = std::map<std::string, int>;

struct term_t {
  bool parenthesized = false;
  Token add_operator;
  int coefficient;
  Exponent_map exponents; // only applies to monomial lists
  vector<vector<term_t>> parenthesized_list;
};

struct poly_decl_t {
	poly_header_t header;
  std::vector<term_t> term_list;
};

struct poly_exec_t {
  poly_header_t header;
};

class Parser {
  public:
    //void ConsumeAllInput();
    void parse_input();

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
    std::vector<term_t> parse_term_list(Token add_operator);
    void parse_poly_body(poly_decl_t &poly_decl);
    void parse_term(Token add_operator);
    void parse_monomial_list(term_t& term);
    int parse_coefficient();
    void parse_monomial(term_t& term);
    int parse_exponent();
    std::vector<std::vector<term_t>> parse_parenthesized_list(Token add_operator);
    Token parse_add_operator();
    void parse_statement_list();
    void parse_statement();
    void parse_input_statement();
    void parse_output_statement();
    void parse_assign_statement();
    poly_header_t parse_poly_evaluation();
    std::vector<Token> parse_argument_list();
    Token parse_argument();

    std::vector<Token> DMT_12;
    std::vector<Token> IM_4;
    std::vector<Token> AUP_13;
    std::vector<Token> NA_7;
    std::vector<Token> Tasks;
    std::vector<poly_decl_t> Polynomials;

    void display_polynomials();
};

#endif

