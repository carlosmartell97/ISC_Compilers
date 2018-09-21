#ifndef LEXICAL_ANALYSIS_H_
#define LEXICAL_ANALYSIS_H_

#include <fstream>
#include <string>
// #include <unistd.h>
#include <iostream>
#include <vector>
#include <string>
// #include <map>
// #include <algorithm>
#include <sstream>

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
    printf(GREEN_TEXT);
    printf("new symbol %s : %s, line:%d character:%d\n",
      cadena.c_str(),
      tipo.c_str(),
      n_linea,
      n_caracter
    );
    printf(DEFAULT);
  }
};

#endif
