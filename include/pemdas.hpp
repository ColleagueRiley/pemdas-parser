#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <math.h>

bool isIn(std::string string, std::vector<std::string> strings){   return std::find(strings.begin(),strings.end(),string).base()->size() == string.size(); } // checks if a string is in a group of strings

enum token{NUM, OPERATOR };
struct Token{token t; std::string data;};

double parse(std::string data);

std::map<char,char> lookAlikes = {};

std::vector<Token> tokenize(std::string data){
    std::vector<Token> tokens; std::string charbuffer="";
    for (int i=0; i < data.size(); i++){
        if (i+1 < data.size() && charbuffer+data.at(i)+data.at(i+1) == "÷") data.replace(data.begin()+i,data.begin()+i+1,"/");
        Token t; if (lookAlikes.find(data.at(i)) != lookAlikes.end()) data.at(i)==lookAlikes.at(data.at(i));
        if (isIn(charbuffer+data.at(i),{"+","-","*","^","/","%","√"})) tokens.insert(tokens.end(),{OPERATOR, charbuffer+data.at(i)}); 
        int b=i; std::string num="";
        while (b < data.size() && isIn(charbuffer+data.at(b),{"1","2","3","4","5","6","7","8","9","0","."})){ num += data.at(b); b++;}
        if (num.size()) tokens.insert(tokens.end(),{NUM, num}); 

        if (data.at(i) == '('){
            int p=1; int j=i+1; std::string col="",before="";
             tokens.insert(tokens.end(),{OPERATOR, "*"});
            if (tokens.size()) before+= tokens.back().data; 
            while (p){ col+=data.at(j);
                j++; if (data.at(j) == ')') p--; else if (data.at(j) == '(') p++; 
            } data.replace(data.begin()+i,data.begin()+j+1,  before + std::to_string( parse(col) ) );
        }
    }
    return tokens;
}

std::vector<std::vector<std::string>> pemdas = {{"√","^"},{"*","/"},{"+","-"}};

double parse(std::string data){
    double output=0; std::vector<Token> tokens = tokenize(data);
    if (tokens.front().t == NUM) output= std::stod(tokens.front().data);
    std::string charbuffer="";
    for (int i=0; i < tokens.size(); i++){
        for (int p=0; p < pemdas.size(); p++){
            if (isIn(charbuffer+tokens.at(i).data,pemdas.at(p))){
                if ( tokens.at(i).data.size() > 1 && charbuffer+tokens.at(i).data.at(0) + tokens.at(i).data.at(1) == "√") tokens.at(i).data.at(0)='q';
                switch (tokens.at(i).data.at(0)){
                    case '^': output = pow(output, std::stod(tokens.at(i+1).data)); break;
                    case 'q': output = sqrt(output); break;
                    case '*': output *= std::stod(tokens.at(i+1).data); break;
                    case '/': output /= std::stod(tokens.at(i+1).data); break;
                    case '+': output += std::stod(tokens.at(i+1).data); break;
                    case '-': output -= std::stod(tokens.at(i+1).data); break;
                    default: break;
                }
            }
        }
    }
    return output;
}