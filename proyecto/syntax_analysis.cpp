#include "lexical_analysis.h"
// #include <fstream>
using namespace std;

ofstream tabla_de_simbolos_sintactico_csv;

void syntax_error_found(int n_linea, int n_caracter, string cadena){
  printf(RED_TEXT);
  printf("ERROR. wrong syntax for \"%s\" at line %d, character %d\n",
    cadena.c_str(),
    n_linea,
    n_caracter
  );
  printf(DEFAULT);
  tabla_de_simbolos_sintactico_csv.close();
  exit(EXIT_SUCCESS);
}

void start_lexical_analysis(vector<Simbolo*> tabla_de_simbolos){
  printf(BLUE_TEXT);
  printf("starting syntax analysis...\n");
  printf(DEFAULT);
  tabla_de_simbolos_sintactico_csv.open("tabla_de_simbolos_sintactico.csv");
  // tabla_de_simbolos_lexico_csv << "cadena,tipo,n_linea,n_caracter,valor,identificador_funcion_o_variable\n";

  string cadena, tipo, valor, identificador_funcion_o_variable;
  int n_linea, n_caracter;
  for(uint i=0; i<tabla_de_simbolos.size(); i++){
    cadena = tabla_de_simbolos[i]->cadena.c_str();
    tipo = tabla_de_simbolos[i]->tipo.c_str();
    n_linea = tabla_de_simbolos[i]->n_linea;
    n_caracter = tabla_de_simbolos[i]->n_caracter;
    printf("symbol %s : %s, line:%d character:%d\n",
      cadena.c_str(),
      tipo.c_str(),
      n_linea,
      n_caracter
    );
    if(cadena == "="){
      int traverse_position = i+1;
      int traverse_character = n_caracter+1;
      string asignacion = "";
      if(tabla_de_simbolos[i-1]->tipo != "identificador"){
        syntax_error_found(n_linea, traverse_character, tabla_de_simbolos[i-1]->cadena);
      }
      while(true){
        if(tabla_de_simbolos[traverse_position]->cadena==";") break;
        if((tabla_de_simbolos[traverse_position]->tipo!="separador" &&
          tabla_de_simbolos[traverse_position]->tipo!="separador (" &&
          tabla_de_simbolos[traverse_position]->tipo!="separador )" &&
          tabla_de_simbolos[traverse_position]->tipo!="separador +" &&
          tabla_de_simbolos[traverse_position]->tipo!="separador -" &&
          tabla_de_simbolos[traverse_position]->tipo!="separador *" &&
          tabla_de_simbolos[traverse_position]->tipo!="separador /" &&
          tabla_de_simbolos[traverse_position]->tipo!="separador ^" &&
          tabla_de_simbolos[traverse_position]->tipo!="separador ==" &&
          tabla_de_simbolos[traverse_position]->tipo!="separador <" &&
          tabla_de_simbolos[traverse_position]->tipo!="separador >" &&
          tabla_de_simbolos[traverse_position]->tipo!="separador &" &&
          tabla_de_simbolos[traverse_position]->tipo!="separador |" &&
          tabla_de_simbolos[traverse_position]->tipo!="identificador" &&
          tabla_de_simbolos[traverse_position]->tipo!="numero"
        )){
          syntax_error_found(n_linea, traverse_character, tabla_de_simbolos[traverse_position]->cadena);
        }
        asignacion += tabla_de_simbolos[traverse_position]->cadena;
        traverse_position++;
        n_caracter++;
      }
      printf(YELLOW_TEXT);
      printf("new value for %s -> \"%s\"\n",
        tabla_de_simbolos[i-1]->cadena.c_str(),
        asignacion.c_str()
      );
      printf(DEFAULT);
    }
    // tabla_de_simbolos_lexico_csv<<cadena<<","<<tipo<<","<<n_linea<<","<<n_caracter<<",,\n";
  }
  tabla_de_simbolos_sintactico_csv.close();
}
