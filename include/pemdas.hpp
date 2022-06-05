#include <iostream>  // c++ standard library
#include <vector> // std::vector
#include <map> // std::map
#include <algorithm> // std::find
#include <math.h> // sqrt(), pow()

// if a string is in a list of strings
bool isIn(std::string string, std::vector<std::string> strings){   return std::find(strings.begin(),strings.end(),string).base()->size() == string.size(); } // checks if a string is in a group of strings

enum token{ NUM, OPERATOR }; // types of token
struct Token{token t; std::string data;}; // token struct that says what kinda token it is and the data it holds

double parse(std::string data);

std::map<char,char> lookAlikes = {}; // symbols that might be the same as other ones

std::vector<Token> tokenize(std::string data){ // tokenize the string
    std::vector<Token> tokens; std::string charbuffer=""; // list of tokens and charbuffer for converting char to string
    for (int i=0; i < data.size(); i++){
        if (i+1 < data.size() && charbuffer+data.at(i)+data.at(i+1) == "÷") data.replace(data.begin()+i,data.begin()+i+1,"/"); // replace (string) ÷ with / 
        Token t; if (lookAlikes.find(data.at(i)) != lookAlikes.end()) data.at(i)==lookAlikes.at(data.at(i));
        if (isIn(charbuffer+data.at(i),{"+","-","*","^","/","%","√"}) || data.at(i) == '-' && tokens.size() && tokens.back().t == OPERATOR) tokens.insert(tokens.end(),{OPERATOR, charbuffer+data.at(i)}); // if it's a operator, add it as an operator token
        // if the data at the index is a number, collect data until the number is done
        int b=i; std::string num=""; 
        while (b < data.size() && isIn(charbuffer+data.at(b),{"1","2","3","4","5","6","7","8","9","0","."})){ num += data.at(b); b++;}
        if (num.size()) tokens.insert(tokens.end(),{NUM, num}); // add the number to the tokens as a number token
        // handle parenthesis 
        if (data.at(i) == '('){ 
            int p=1; int j=i+1; std::string col="",before=""; 
            if (tokens.size() && tokens.back().t == NUM) tokens.insert(tokens.end(),{OPERATOR, "*"}); // if there is a number before the parenthesis, add a *. ex : 2(5+1) becomes 2*(5+1)
            
            while (p){ col+=data.at(j);
                j++; if (data.at(j) == ')') p--; else if (data.at(j) == '(') p++; // get whatever is in the parenthesis 
            } data.replace(data.begin()+i,data.begin()+j+1,  before + std::to_string( parse(col) ) ); // parse whatever is in the parenthesis and replace the parenthesis with the answer
        }
    }
    return tokens; // return tokens
}

std::vector<std::vector<std::string>> pemdas = {{"√","^"},{"*","/"},{"+","-"}}; // pemdas symbols in order for parsing

double parse(std::string data){
    double output=0; std::vector<Token> tokens = tokenize(data); // get the tokens
    if (tokens.front().t == NUM) output= std::stod(tokens.front().data); // if the first token is a number, start with the number
    std::string charbuffer="";
    for (int p=0; p < pemdas.size(); p++){ // go through pemdas
        for (int i=0; i < tokens.size(); i++){ // go through the tokens
            if (isIn(charbuffer+tokens.at(i).data,pemdas.at(p))){ // if the symbol is the same one as the current pemdas symbol
                if ( tokens.at(i).data.size() > 1 && charbuffer+tokens.at(i).data.at(0) + tokens.at(i).data.at(1) == "√") tokens.at(i).data.at(0)='q'; // if it's a square root, replace it with q (for the switch)
                switch (tokens.at(i).data.at(0)){
                    case '^': output = pow(output, std::stod(tokens.at(i+1).data)); break; // handle exponents
                    case 'q': output = sqrt(output); break; // handle square roots
                    case '*': output *= std::stod(tokens.at(i+1).data); break; // handle multiplication
                    case '/': output /= std::stod(tokens.at(i+1).data); break; // handle division 
                    case '+': output += std::stod(tokens.at(i+1).data); break; // handle addition
                    case '-': output -= std::stod(tokens.at(i+1).data); break; // handle subtraction
                    default: break;
                } 
            }
        }
    }
    return output; // return the result
}