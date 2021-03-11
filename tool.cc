#include <iostream>
#include <string>

std::string translate(std::string line, std::string argument){
  std::string result = "";

//omgedraaid(argument = 1)
  if(argument == "1"){
    for(int i = line.length(); i>=0; i--){
      result.push_back(line[i]);
    }
  }
  //Hoofdletter(argument = 2)
  if(argument == "2"){
    for(char c : line){
      if(c < 123 && c > 96){
      c = c - 32;
      result.push_back(c);
      }else
      if(c < 93 && c > 64){
      c = c + 32;
      result.push_back(c);
      }else{
      result.push_back(c);
      }
      

    }
  }


  return result; }

int main(int argc, char *argv[])
{ std::string line;

  if(argc != 2)
  { std::cerr << "Deze functie heeft exact 1 argument nodig" << std::endl;
    return -1; }

  while(std::getline(std::cin, line))
  { std::cout << translate(line, argv[1]) << std::endl; } 

  return 0; }
