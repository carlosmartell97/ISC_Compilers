/*
compile:
    g++ --std=c++11 -o run.exe pre_processor.cpp

execute:
    ./run.exe input.txt
*/
#include <fstream>
#include <string>
#include <unistd.h>
#include <iostream>

using namespace std;

int main(int argc, char* argv[]){
  if(argc != 2){
    printf("ERROR. 2 parameters expected: program name and input file name\n");
    return 0;
  }
  string file_name = argv[1];
  printf("Abriendo archivo: %s\n", file_name.c_str());
  char *path=NULL;
  size_t size;
  path = getcwd(path,size);
  cout << "Path: " << path << endl;
  ifstream input_file(file_name);
  string line;
  bool has_space = false;
  bool first_character = true;
  while(getline(input_file, line, '\0')){
    for(char read : line){
      if(read==' ' || read=='\n'){
        has_space = true;
      }
      else {
        if(has_space){
          if(!first_character){
            cout << " " << flush;
          }
          has_space = false;
        }
        first_character = false;
        cout << read << flush;
        sleep(1);
      }
    }
  }
  printf("\nEOF\n");
  printf("Cerrando Archivo\n");
  return 0;
}
