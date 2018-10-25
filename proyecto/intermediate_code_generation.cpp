/*
  How to compile:
      g++ --std=c++11 -o run.exe intermediate_code_generation.cpp

  How to run:
      ./run.exe c_minus.txt
      ./run.exe <name_of_file_with_ASCII_text_that_has_no_CRLF_line_terminators>
*/

#include "intermediate_code_generation.h"

using namespace std;

ofstream codigo_de_3_direcciones;
bool ignore_comments = false;

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

void write_to_txt(string replacement, string cadena, string tipo, string COLOR){
  string output;
  if(replacement == ""){
    output = tipo;
  }
  else if(replacement == "cadena"){
    output = cadena;
  } else {
    output = replacement;
  }
  printf("%s", COLOR.c_str());
  printf("%s ", output.c_str());
  printf(DEFAULT);
  codigo_de_3_direcciones << output << " " << flush;
}

void grammar_error_found(int n_linea, int n_caracter, string cadena, string expected){
  printf(RED_TEXT);
  printf("ERROR. \"%s\" was expected to be a %s at line %d, character %d\n",
    cadena.c_str(),
    expected.c_str(),
    n_linea,
    n_caracter
  );
  printf(DEFAULT);
  codigo_de_3_direcciones.close();
  exit(EXIT_FAILURE);
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
  vector<string> palabras_reservadas = { "while", "if", "return"};
  vector<string> funciones_reservadas = {"read", "write"};
  vector<string> type_specifiers = {"int", "bool", "true", "false", "void"};
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
          if(ignore_comments){
            printf(BLACK_TEXT);
            printf("word %s ignored\n", read_word.c_str());
            printf(DEFAULT);
          }
          else if(is_in(read_word, palabras_reservadas)){    // antes del separador, hay una palabra reservada
            tabla_de_simbolos.push_back(
              new Simbolo(n_linea, n_caracter, read_word, "palabra reservada "+read_word)
            );
          }
          else if(is_in(read_word, funciones_reservadas)){    // antes del separador, hay una palabra reservada
            tabla_de_simbolos.push_back(
              new Simbolo(n_linea, n_caracter, read_word, "funcion reservada")
            );
          }
          else if(is_in(read_word, type_specifiers)){    // antes del separador, hay una palabra reservada
            tabla_de_simbolos.push_back(
              new Simbolo(n_linea, n_caracter, read_word, "type specifier")
            );
          }
          else if(is_identificador(read_word)){
            // printf("%s is identificador.\n", read_word.c_str());
            tabla_de_simbolos.push_back(
                new Simbolo(n_linea, n_caracter, read_word, "identificador")
              );
          }
          else if(is_number(read_word)){
            // printf("%s is number.\n", read_word.c_str());
            tabla_de_simbolos.push_back(
              new Simbolo(n_linea, n_caracter, read_word, "numero")
            );
          }
          else if(read_word=="=" || read_word=="<" || read_word==">" || read_word=="==" || read_word=="<=" || read_word==">=" || read_word=="!="){
            tabla_de_simbolos.push_back(
              new Simbolo(n_linea, n_caracter, read_word, "separador "+read_word)
            );
          }
          else {
            printf(RED_TEXT); printf("error: %s\n", read_word.c_str());
            printf(DEFAULT);
          }
        }
        read_word = "";
        if(read!=' ' && read!='\0' && read!='\t'){
          tabla_de_simbolos.push_back(
            new Simbolo(n_linea, n_caracter, read_string, "separador "+read_string)
          );
        }
        else {
          printf("character .%c. ignored.\n", read);
        }
        if(read=='*' && previous_separador=='/'){
          ignore_comments = true;
          tabla_de_simbolos.pop_back();
          tabla_de_simbolos.pop_back();
        }
        if(read=='/' && previous_separador=='*'){
          ignore_comments = false;
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
  for(uint i=0; i<tabla_de_simbolos.size(); i++){
    Simbolo* symb = tabla_de_simbolos[i];
    if(symb->tipo.substr(0,17)=="palabra reservada" || symb->tipo=="funcion reservada" || symb->tipo=="type specifier") printf(GREEN_TEXT);
    else if(symb->tipo=="identificador") printf(YELLOW_TEXT);
    else if(symb->tipo=="numero") printf(DEFAULT);
    else if(symb->tipo.substr(0,9)=="separador") printf(BLUE_TEXT);
    else printf(RED_TEXT);
    printf("symbol %s : %s, line:%d character:%d\n",
      symb->cadena.c_str(),
      symb->tipo.c_str(),
      symb->n_linea,
      symb->n_caracter
    );
    printf(DEFAULT);
  }
  string estado = "START";
  uint pos = 0;
  printf("\nnow processing symbols...\n");
  map<string, Estado*> estados;
  estados["START"] = new Estado("START", GREEN_TEXT, "entry", {make_pair("type specifier","declaracion")});
  estados["declaracion"] = new Estado("declaracion", DEFAULT, "cadena", {make_pair("identificador","tipo declaracion")});

  estados["tipo declaracion"] = new Estado("tipo declaracion", BLUE_TEXT, " ", {make_pair("separador ;","START"), make_pair("separador [", "array tamano"), make_pair("separador (","funcion parametros")});
  estados["array tamano"] = new Estado("array tamano", BLUE_TEXT, "", {make_pair("numero","array cerrar tamano")});
  estados["array cerrar tamano"] = new Estado("array cerrar tamano", BLUE_TEXT, "", {make_pair("separador ]","array declaracion")});
  estados["array declaracion"] = new Estado("array declaracion", BLUE_TEXT, "", {make_pair("separador ;","START")});

  estados["funcion parametros"] = new Estado("funcion parametros", YELLOW_TEXT, " ", {make_pair("separador )","compound-stmt"), make_pair("type specifier","param identificador")});
  estados["param identificador"] = new Estado("param identificador", YELLOW_TEXT, " ", {make_pair("identificador","params-list")});
  estados["params-list"] = new Estado("params-list", YELLOW_TEXT, " ", {make_pair("separador )","compound-stmt"), make_pair("separador ,","param secundario"), make_pair("separador [","parametro array")});
  estados["parametro array"] = new Estado("parametro array", YELLOW_TEXT, "", {make_pair("separador ]","params-list")});
  estados["param secundario"] = new Estado("params secundarios", YELLOW_TEXT, " ", {make_pair("type specifier","param identificador")});

  estados["compound-stmt"] = new Estado("compound-stmt", BLUE_TEXT, " ", {make_pair("separador {","var-declaration")});
  estados["var-declaration"] = new Estado("var-declaration", YELLOW_TEXT, " ", {make_pair("type specifier","declaracion en f"), make_pair("separador }","START"),
                                                                                      make_pair("palabra reservada if","selection-stmt"), make_pair("palabra reservada return","return-stmt")});
  estados["declaracion en f"] = new Estado("declaracion en f", DEFAULT, "cadena", {make_pair("identificador","tipo declaracion en f")});

  estados["tipo declaracion en f"] = new Estado("tipo declaracion en f", BLUE_TEXT, " ", {make_pair("separador ;","var-declaration"), make_pair("separador [","array tamano en f"), make_pair("separador (","funcion parametros en f")});
  estados["array tamano en f"] = new Estado("array tamano en f", BLUE_TEXT, "", {make_pair("numero","array cerrar tamano en f")});
  estados["array cerrar tamano en f"] = new Estado("array cerrar tamano en f", BLUE_TEXT, "", {make_pair("separador ]","array declaracion en f")});
  estados["array declaracion en f"] = new Estado("array declaracion en f", BLUE_TEXT, "", {make_pair("separador ;","var-declaration")});

  // llamada a función
  estados["funcion parametros en f"] = new Estado("funcion parametros en f", YELLOW_TEXT, " ", {make_pair("separador )","llamada funcion"), make_pair("type specifier","param identificador en f"), make_pair("identificador","funcion parametros en f")});
  estados["param identificador en f"] = new Estado("param identificador en f", DEFAULT, " ", {make_pair("identificador","params-list en f")});
  estados["params-list en f"] = new Estado("params-list en f", DEFAULT, " ", {make_pair("separador )","llamada funcion")});
  estados["llamada funcion"] = new Estado("llamada funcion", GREEN_TEXT, "call ", {make_pair("separador ;","var-declaration")});

  // llamada a if()
  estados["selection-stmt"] = new Estado("selection-stmt", BLUE_TEXT, " ", {make_pair("separador (","expression en if")});
  estados["expression en if"] = new Estado("expression en if", YELLOW_TEXT, " ", {make_pair("identificador","id dentro de if"), make_pair("numero","numero dentro de if")});
  estados["id dentro de if"] = new Estado("id dentro de if", DEFAULT, " ", {make_pair("separador =","asignacion dentro de if"), make_pair("separador [","array dentro de if"),
                                                                          make_pair("separador <","relop"), make_pair("separador >","relop"), make_pair("separador <=","relop"),
                                                                          make_pair("separador >=","relop"), make_pair("separador ==","relop"), make_pair("separador !=","relop")});
  estados["asignacion dentro de if"] = new Estado("asignacion dentro de if", DEFAULT, " ", {make_pair("identificador","id dentro de if"), make_pair("numero","numero dentro de if")});

  estados["array dentro de if"] = new Estado("array dentro de if", DEFAULT, " ", {make_pair("separador [","array tamano dentro if")});
  estados["array tamano dentro if"] = new Estado("array tamano dentro if", DEFAULT, " ", {make_pair("numero","array cerrar tamano dentro if")});
  estados["array cerrar tamano dentro if"] = new Estado("array cerrar tamano dentro if", DEFAULT, " ", {make_pair("separador ]","id dentro de if")});

  estados["relop"] = new Estado("relop", DEFAULT, " ", {make_pair("identificador","id dentro de if"), make_pair("numero","numero dentro de if")});
  estados["numero dentro de if"] = new Estado("numero dentro de if", DEFAULT, " ", {make_pair("separador <","relop"), make_pair("separador >","relop"), make_pair("separador <=","relop"),
                                                                                    make_pair("separador >=","relop"), make_pair("separador ==","relop"), make_pair("separador !=","relop"),
                                                                                    make_pair("separador )","var-declaration")});

  Simbolo* symb = tabla_de_simbolos[pos];
  Estado* estado_actual = estados["START"];
  string  temp_operando_1="?",
          temp_operando_2="?",
          temp_operador="?";
  int temp_t_count=1, temp_L_count=1;
  while(estado_actual->name != "END"){
    symb = tabla_de_simbolos[pos];
    // printf("estado:<%s>, cadena:<%s>, symbol:<%s>\n", estado_actual->name.c_str(), symb->cadena.c_str(), symb->tipo.c_str());
    if(estado_actual->name=="expression en if" || estado_actual->name=="relop"){
      if(temp_operando_1 == "?"){
        temp_operando_1 = symb->cadena;
      } else {
        write_to_txt(boost::str(boost::format("\nt%d = %s %s %s") % temp_t_count % temp_operando_1 % temp_operador % symb->cadena), symb->cadena, symb->tipo, YELLOW_TEXT);
        write_to_txt(boost::str(boost::format("\nif false t%d goto L%d") % temp_t_count % temp_L_count), symb->cadena, symb->tipo, BLUE_TEXT);
        temp_operando_1 = "?";
        temp_operador = "?";
        temp_t_count++;
        temp_L_count++;
      }
    }
    else if(estado_actual->name=="id dentro de if"){
      temp_operador = symb->cadena;
    }
    if(estado_actual->tokens_expected_next.find(symb->tipo) != estado_actual->tokens_expected_next.end()){
      if(pos >= tabla_de_simbolos.size()-1){
        // string s;
        // s= = accumulate(estado_actual->tokens_expected_next.begin(), estado_actual->tokens_expected_next.end(), s);
        // grammar_error_found(symb->n_linea, symb->n_caracter, "EOF", s);
        grammar_error_found(symb->n_linea, symb->n_caracter, "EOF", "{more stuff}");
      }
      else if(estados.find(estado_actual->tokens_expected_next[symb->tipo]) == estados.end()){
        grammar_error_found(symb->n_linea, symb->n_caracter, estado_actual->tokens_expected_next[symb->tipo], "*Estado in map*");
      }
      else {
        write_to_txt(estado_actual->output_3_direcciones, symb->cadena, symb->tipo, estado_actual->COLOR);
        estado_actual = estados[estado_actual->tokens_expected_next[symb->tipo]];
        pos++;
      }
    } else {
      // string s;
      // s = accumulate(estado_actual->tokens_expected_next.begin(), estado_actual->tokens_expected_next.end(), s);
      // grammar_error_found(symb->n_linea, symb->n_caracter, symb->cadena, s);
      grammar_error_found(symb->n_linea, symb->n_caracter, symb->cadena, "{something}");
    }

    // if(estado=="START"){
    //   if(symb->tipo=="type specifier"){
    //     write_to_txt("entry", GREEN_TEXT);
    //     if(pos >= tabla_de_simbolos.size()-1){
    //       grammar_error_found(symb->n_linea, symb->n_caracter, symb->cadena, "type specifier");
    //     }
    //     else {
    //       estado = "identificador funcion";
    //       pos++;
    //     }
    //   } else {
    //     grammar_error_found(symb->n_linea, symb->n_caracter, symb->cadena, "type specifier");
    //   }
    // }
    // else if(estado == "identificador funcion"){
    //   if(symb->tipo=="identificador" || symb->cadena=="main"){
    //     write_to_txt(symb->cadena, DEFAULT);
    //     if(pos >= tabla_de_simbolos.size()-1){
    //       grammar_error_found(symb->n_linea, symb->n_caracter, symb->cadena, "identificador");
    //     }
    //     else {
    //       estado = "tipo declaracion";
    //       pos++;
    //     }
    //   }
    //   else {
    //     grammar_error_found(symb->n_linea, symb->n_caracter, symb->cadena, "identificador");
    //   }
    // }
    // else if(estado == "tipo declaracion"){
    //   estado = "END"; printf("\n"); codigo_de_3_direcciones.close();
    // }
  }
  return 0;
}
