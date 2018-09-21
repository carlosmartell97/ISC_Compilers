#include <iostream>
#include <list>
#include <cstdbool>
#include <string>
#include <sstream>
#include <map>

using namespace std;

void print_queue(list<char> q){
  printf("queue: ");
  for (const char & c : q){
    printf("%c", c);
  }
  printf("\n");
}

int main(){
  char read;
  list<char> q;
  //string e1 = "2+3*4-6^1/2";
  string e1 = "6+2*3/2^2-4";
  string e2 = "";
  map<char, int> importancia;
  importancia['^'] = 3;
  importancia['*'] = 2;
  importancia['/'] = 2;
  importancia['+'] = 1;
  importancia['-'] = 1;
  importancia['1'] = 0;
  importancia['2'] = 0;
  importancia['3'] = 0;
  importancia['4'] = 0;
  importancia['5'] = 0;
  importancia['6'] = 0;
  importancia['7'] = 0;
  importancia['8'] = 0;
  importancia['9'] = 0;
  importancia['0'] = 0;
  char top;
  for(int i=0; i<e1.size(); i++){
    read = e1[i];
    printf("\n\nSTEP #%d:--------\nread: %c \n", i+1, read);
    // printf("impor read %c: %d\n", read, importancia[read]);
    if(read=='1' || read=='2' || read=='3' || read=='4' || read=='5'
       || read=='6' || read=='7' || read=='8' || read=='9'|| read=='0'){
      // printf("\tprimero\n");
      stringstream ss;
      string read_string;
      ss << read;
      ss >> read_string;
      e2.append(read_string);
      // printf("\tprimero_end\n");
    }
    else {
      if(q.empty()){
        // printf("\tempty\n");
        q.push_back(read);
      } else {
        top = q.back();
        // printf("impor top %c: %d\n", top, importancia[top]);
        // q.pop_back();
        if(importancia[read] == importancia[top]){
          printf("\tigual\n");
          stringstream ss;
          string read_string;
          ss << top;
          ss >> read_string;
          e2.append(read_string);
          q.pop_back();
          q.push_back(read);
        }
        else if(importancia[read] > importancia[top]){
          // printf("\tmayor\n");
          // print_queue(q);
          printf("pushing back %c\n", read);
          q.push_back(read);
        }
        else if(importancia[read] < importancia[top]){
          // printf("\tmenor\n");
          stringstream ss;
          string read_string;
          ss << top;
          ss >> read_string;
          e2.append(read_string);
          q.pop_back();
          while(true){
            if(q.empty()){
              break;
            }
            top = q.back();
            if(importancia[read] > importancia[top]){
              break;
            }
            if(importancia[read] <= importancia[top]){
              stringstream ss;
              string read_string;
              ss << top;
              ss >> read_string;
              e2.append(read_string);
              q.pop_back();
            }
          }
          q.push_back(read);
        }
        else {
          printf("ERROR.\n");
        }
      }
    }
    printf("e1 = %s\n", e1.c_str());
    printf("e2 = %s\n", e2.c_str());
    print_queue(q);
    printf("\n");
  }
  printf("finished going through e1.\n");
  printf("now emptying queue, if not empty already.\n\n");
  while(!q.empty()){
    top = q.back();
    q.pop_back();
    stringstream ss;
    string read_string;
    ss << top;
    ss >> read_string;
    e2.append(read_string);
    printf("e1 = %s\n", e1.c_str());
    printf("e2 = %s\n", e2.c_str());
    print_queue(q);
    printf("\n");
  }
  return 0;
}
