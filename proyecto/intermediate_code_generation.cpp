/*
  How to compile:
      g++ --std=c++11 -o run.exe lexical_analysis.cpp

  How to run:
      ./run.exe archivo_fuente.txt
      ./run.exe <name_of_file_with_ASCII_text_that_has_no_CRLF_line_terminators>
*/

#include "intermediate_code_generation.h"

using namespace std;

ofstream codigo_de_3_direcciones;
bool ignore = false;

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

void write_to_txt(string output, string COLOR){
  if(ignore){
    printf(BLACK_TEXT);
    printf("word %s ignored\n", output.c_str());
    printf(DEFAULT);
    return;
  }
  // printf("%s", COLOR.c_str());
  // printf("%s\n", output.c_str());
  // printf(DEFAULT);
  // codigo_de_3_direcciones << output << endl;
}

void lexical_error_found(int n_linea, int n_caracter, string cadena){
  printf(RED_TEXT);
  printf("ERROR. No pattern found for \"%s\" at line %d, character %d\n",
    cadena.c_str(),
    n_linea,
    n_caracter
  );
  printf(DEFAULT);
  codigo_de_3_direcciones.close();
  exit(EXIT_SUCCESS);
}

int main(int argc, char* argv[]){
  if(argc != 2){
    printf("ERROR. 2 parameters expected: program name and input file name\n");
    return 0;
  }
  string file_name = argv[1];
  printf("\nAbriendo archivo: %s\n", file_name.c_str());
  ifstream input_file(file_name);
  codigo_de_3_direcciones.open("codigo_de_3_direcciones.txt");
  string line;

  string separadores = "(){};,&<>+-*/^|! \t\0";
  vector<string> palabras_reservadas = {"int", "bool", "while", "if", "return",
                                        "true", "false", "main", "read", "write"};
  string numeros = "0123456789";
  char previous_separador = ' ';

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
          if(ignore){
            printf(BLACK_TEXT);
            printf("word %s ignored\n", read_word.c_str());
            printf(DEFAULT);
          }
          else if(is_in(read_word, palabras_reservadas)){    // antes del separador, hay una palabra reservada
            tabla_de_simbolos.push_back(
              new Simbolo(n_linea, n_caracter, read_word, "palabra reservada")
            ); write_to_txt("palabra reservada: "+read_word, GREEN_TEXT);
          }
          else if(is_identificador(read_word)){
            // printf("%s is identificador.\n", read_word.c_str());
            tabla_de_simbolos.push_back(
                new Simbolo(n_linea, n_caracter, read_word, "identificador")
              ); write_to_txt("identificador: "+read_word, YELLOW_TEXT);
          }
          else if(is_number(read_word)){
            // printf("%s is number.\n", read_word.c_str());
            tabla_de_simbolos.push_back(
              new Simbolo(n_linea, n_caracter, read_word, "numero")
            ); write_to_txt("numero: "+read_word, DEFAULT);
          }
          else if(read_word == "="){
            tabla_de_simbolos.push_back(
              new Simbolo(n_linea, n_caracter, read_word, "separador "+read_word)
            ); write_to_txt("separador: "+read_word, BLUE_TEXT);
          }
          else if(read_word == "=="){
            tabla_de_simbolos.push_back(
              new Simbolo(n_linea, n_caracter, read_word, "separador "+read_word)
            ); write_to_txt("separador: "+read_word, BLUE_TEXT);
          }
          else {
            write_to_txt("error: "+read_word, RED_TEXT);
          }
        }
        read_word = "";
        if(read!=' ' && read!='\0' && read!='\t'){
          tabla_de_simbolos.push_back(
            new Simbolo(n_linea, n_caracter, read_string, "separador")
          ); write_to_txt("separador: "+read_string, BLUE_TEXT);
        }
        else {
          printf("character .%c. ignored.\n", read);
        }
        if(read=='*' && previous_separador=='/'){
          ignore = true;
          tabla_de_simbolos.pop_back();
          tabla_de_simbolos.pop_back();
        }
        if(read=='/' && previous_separador=='*'){
          ignore = false;
          tabla_de_simbolos.pop_back();
          tabla_de_simbolos.pop_back();
        }
      }
      else {    // no es un separador
        read_word += read;
      }
      previous_separador = read;
    }
    // printf(">\n");
  }
  printf("\nEOF\n");
  printf("cerrando archivo de entrada\n");
  codigo_de_3_direcciones.close();
  for(uint i=0; i<tabla_de_simbolos.size(); i++){
    Simbolo* symb = tabla_de_simbolos[i];
    if(symb->tipo=="palabra reservada") printf(GREEN_TEXT);
    else if(symb->tipo=="identificador") printf(YELLOW_TEXT);
    else if(symb->tipo=="numero") printf(DEFAULT);
    else if(symb->tipo=="separador" || symb->tipo=="separador =" || symb->tipo=="separador ==") printf(BLUE_TEXT);
    else printf(RED_TEXT);
    printf("symbol %s : %s, line:%d character:%d\n",
      symb->cadena.c_str(),
      symb->tipo.c_str(),
      symb->n_linea,
      symb->n_caracter
    );
    printf(DEFAULT);
  }
  return 0;
}
