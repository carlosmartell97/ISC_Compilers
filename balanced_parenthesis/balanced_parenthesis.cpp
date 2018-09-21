/*
how to compile this program:
    g++ --std=c++11 -o balanced.exe balanced_parenthesis.cpp

how to run this program:
    ./balanced.exe "(()()(()))"
*/
#include <cstdio>
#include <string>
#include <list>

using namespace std;

int main(int argc, char* argv[]){
  if(argc < 2){
    printf("ERROR. Expected use: program name and parenthesis expression within \"\" quotes\n");
    return 0;
  }
  string input = argv[1];
  printf("examining %s\n...\n", input.c_str());
  list<char> stack;
  for(uint i=0; i<input.size(); i++){
    char read = input[i];
    if(read == '('){
      stack.push_front(read);
    }
    else if(read == ')'){
      if(stack.back() != '('){
        printf("parenthesis are NOT well balanced.\n");
        return 0;
      }
      else {
        stack.pop_back();
      }
    }
    else {
      printf("ERROR. unexpected character '%c'\n", read);
      return 0;
    }
  }
  if(stack.empty()){
    printf("parenthesis ARE well balanced!\n");
  }
  else {
    printf("parenthesis are NOT well balanced.\n");
  }
  return 0;
}
