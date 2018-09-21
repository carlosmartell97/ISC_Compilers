/*
  How to compile:
      g++ --std=c++11 -o run.exe lexical_analysis.cpp

  How to run:
      ./run.exe archivo_fuente.txt
      ./run.exe <name_of_file_with_ASCII_text_that_has_no_CRLF_line_terminators>

  If there are no lexical errors in the input file, a file 'tabla_de_simbolos_lexico.csv'
  will be created for all tokens and their properties, and the symbols table will be
  automatically given to the syntax analyzer to continue processing the input
*/

#include "lexical_analysis.h"
#include "syntax_analysis.cpp"

using namespace std;

ofstream tabla_de_simbolos_lexico_csv;

string char_to_string(char c){
  stringstream ss;
  string s;
  ss << c;
  ss >> s;
  return s;
}

bool is_in(string cadena, vector<string> cadenas){
  for(uint i=0; i<cadenas.size(); i++){
    if(cadena == cadenas[i]) return true;
  }
  return false;
}

bool is_number(string cadena){
  for(uint i=0; i<cadena.size(); i++){
    int ascii_code = (int) cadena[i];
    if(ascii_code<48 || ascii_code>57) return false;
  }
  return true;
}

bool is_identificador(string cadena){
  int ascii_code = (int) cadena[0];
  // printf("finding out if %s is an identificador. First char, %c -> %d\n",
  //   cadena.c_str(),
  //   cadena[1],
  //   ascii_code
  // );
  if(!(ascii_code>96&&ascii_code<123)) return false;   // primer caracter no está dentro de a-z
  for(uint i=1; i<cadena.size(); i++){
    ascii_code = (int) cadena[i];
    // printf("char #%d, %c -> %d\n",
    //   i,
    //   cadena[1],
    //   ascii_code
    // );
    if(!((ascii_code>96&&ascii_code<123) || (ascii_code>64&&ascii_code<91) || (ascii_code>47&&ascii_code<58))) {     // un caracter después del primero no está en a-z, ni A-Z, ni 0-9
      return false;
    }
  }
  return true;
}

void lexical_error_found(int n_linea, int n_caracter, string cadena){
  printf(RED_TEXT);
  printf("ERROR. No pattern found for \"%s\" at line %d, character %d\n",
    cadena.c_str(),
    n_linea,
    n_caracter
  );
  printf(DEFAULT);
  tabla_de_simbolos_lexico_csv.close();
  exit(EXIT_SUCCESS);
}

void write_to_csv(int n_linea, int n_caracter, string cadena, string tipo){
  // printf(YELLOW_TEXT);
  // printf("cadena: .%s.\n", cadena.c_str());
  if(cadena==",") cadena="coma";    // escribir 'coma' en vez de ',' en el csv
  if(cadena==";") cadena="punto_y_coma";    // escribir 'punto_y_coma' en vez de ';' en el csv
  // printf("writing .%s. to csv\n", cadena.c_str());
  // printf(DEFAULT);
  tabla_de_simbolos_lexico_csv<<cadena<<","<<tipo<<","<<n_linea<<","<<n_caracter<<",,\n";    // dejar valor e identificador_funcion_o_variable vacíos
}

int main(int argc, char* argv[]){
  if(argc != 2){
    printf("ERROR. 2 parameters expected: program name and input file name\n");
    return 0;
  }
  string file_name = argv[1];
  printf("\nAbriendo archivo: %s\n", file_name.c_str());
  ifstream input_file(file_name);
  tabla_de_simbolos_lexico_csv.open("tabla_de_simbolos_lexico.csv");
  tabla_de_simbolos_lexico_csv << "cadena,tipo,n_linea,n_caracter,valor,identificador_funcion_o_variable\n";
  string line;

  string separadores = "(){};,&<>+-/^|! \t\0";
  vector<string> palabras_reservadas = {"entero", "real", "logico", "mientras", "si"
                                  "regresa", "verdadero", "principal", "falso"};
  string numeros = "0123456789";

  string read_word = "";
  int n_linea = 0;
  int n_caracter = 0;
  vector<Simbolo*> tabla_de_simbolos;
  while(getline(input_file, line, '\n')){
    // printf("line: %s\n", line.c_str());
    // replace(line.begin(), line.end(), '\n', ' ');
    // printf("\nline:%s-\n", line.c_str());

    // printf("<");
    n_linea++;
    n_caracter = 0;
    for(uint i=0; i<line.size(); i++){
      n_caracter++;
      char read = line[i];
      string read_string = char_to_string(read);
      // printf("%c", read);

      // printf("reading %c\n", read);
      size_t found = separadores.find_first_of(read_string);
      if(found!=string::npos || read==' ' || read=='\t'){    // es un separador
        if(read_word!=""){    // había algo antes
          // printf("new token: .%s.\n", read_word.c_str());
          if(is_in(read_word, palabras_reservadas)){    // antes del separador, hay una palabra reservada
            tabla_de_simbolos.push_back(
              new Simbolo(n_linea, n_caracter, read_word, "palabra reservada")
            ); write_to_csv(n_linea, n_caracter, read_word, "palabra reservada");
          }
          else if(is_identificador(read_word)){
            // printf("%s is identificador.\n", read_word.c_str());
            tabla_de_simbolos.push_back(
                new Simbolo(n_linea, n_caracter, read_word, "identificador")
              ); write_to_csv(n_linea, n_caracter, read_word, "identificador");
          }
          else if(is_number(read_word)){
            // printf("%s is number.\n", read_word.c_str());
            tabla_de_simbolos.push_back(
              new Simbolo(n_linea, n_caracter, read_word, "numero")
            ); write_to_csv(n_linea, n_caracter, read_word, "numero");
          }
          else if(read_word == "="){
            tabla_de_simbolos.push_back(
              new Simbolo(n_linea, n_caracter, read_word, "separador "+read_word)
            ); write_to_csv(n_linea, n_caracter, read_word, "separador "+read_word);
          }
          else if(read_word == "=="){
            tabla_de_simbolos.push_back(
              new Simbolo(n_linea, n_caracter, read_word, "separador "+read_word)
            ); write_to_csv(n_linea, n_caracter, read_word, "separador "+read_word);
          }
          else {
            printf("%s is wrong.\n", read_word.c_str());
            lexical_error_found(n_linea, n_caracter, read_word);
          }
        }
        read_word = "";
        if(read!=' ' && read!='\0' && read!='\t'){
          tabla_de_simbolos.push_back(
            new Simbolo(n_linea, n_caracter, read_string, "separador")
          ); if(read_string==",")read_string="coma"; if(read_string==";")read_string="punto_y_coma";
          write_to_csv(n_linea, n_caracter, read_string, "separador "+read_string);
        }
        else {
          printf("character .%c. ignored.\n", read);
        }
      }
      else {    // no es un separador
        read_word += read;
      }
    }
    // printf(">\n");

  }
  printf("\nEOF\n");
  printf("cerrando archivo de entrada\n");
  tabla_de_simbolos_lexico_csv.close();
  start_lexical_analysis(tabla_de_simbolos);
  return 0;
}
