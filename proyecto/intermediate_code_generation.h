#ifndef LEXICAL_ANALYSIS_H_
#define LEXICAL_ANALYSIS_H_

#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <numeric>
#include <boost/format.hpp>

#define BLACK_TEXT	"\e[30;1m"
#define RED_TEXT	"\e[31;1m"
#define GREEN_TEXT	"\e[32;1m"
#define YELLOW_TEXT	"\e[33;1m"
#define BLUE_TEXT	"\e[34;1m"
#define DEFAULT		"\e[0m"

using namespace std;

class Simbolo {
public:
  int n_linea, n_caracter;
  string cadena, tipo, valor, identificador_funcion_o_variable;
  Simbolo(int n_linea, int n_caracter, string cadena, string tipo){
    this->n_linea=n_linea; this->n_caracter=n_caracter;
    this->cadena=cadena; this->tipo=tipo;
    this->valor=""; this->identificador_funcion_o_variable="";
  }
};

class Estado {
public:
  string name;
  map<string, string> tokens_expected_next;
  string COLOR;
  string output_3_direcciones;
  Estado(string name, const string COLOR, string output_3_direcciones, const initializer_list< pair<string,string> > &tokens_expected_next){
    this->name = name;
    this->COLOR = COLOR;
    this->output_3_direcciones = output_3_direcciones;
    for(pair<string,string> next : tokens_expected_next) this->tokens_expected_next[next.first]=next.second;
  }
};

#endif
