#include "Grapher.hpp"
#include <stack>

#include <ctype.h>

enum TokenType{
    NUMBER,
    FUNCTION,
    OPERATOR
};

class Token {
    public:
    Token(char* string, int nChars, TokenType tokenTypeIn){
        value = std::string(string, nChars);
        tokenType = tokenTypeIn;
    }

    Token(){
        value = "DEFAULT";
    }

    std::string value;
    TokenType tokenType;
};

/* Returns the number of characters making up the next double in the string, if the next token is a double */
int isNumber(char* input){
    char* restOfString;
    double conversion = strtod(input, &restOfString);

    return (restOfString - input) / (sizeof(char));
}

bool searchFunctions(std::string search){
    std::string validFunctions[3]{
        "sin", "cos", "tan"
    };
    int functionCount = 3;
    bool validFunction = false;
    for (int i = 0; i < functionCount; i++){
        if (search.compare(validFunctions[i])){
            validFunction = true;
            break;
        }
    }

    return validFunction;
}

int isFunction(char* input){
    std::string output;

    /* Keep going over the spaces and letters till we hit a non-letter. Add lowercase versions of the letters to output */
    for(int i = 0; isalpha(input[i]); i++){
        output.push_back(tolower(input[i]));
    }
    
    return searchFunctions(output) ? output.size() : 0;
}

bool searchOperators(char input){
    return input == '+' || input == '*' || input == '-' || input == '/';
}

int isOperator(char* input){
    return searchOperators(input[0]) ? 1 : 0;
}

void advanceString(char** input, int nChars){
    *input = *input + sizeof(char) * nChars;
}

/* Add token to input stack and move along input string */
void addToken(char** inputString, int nChars, std::stack<Token>* outputStack, TokenType tokenType){
    Token newToken(*inputString, nChars, tokenType);
    outputStack->push(newToken);
    advanceString(inputString, nChars);
}

void cleanSpaces(char** input){
    int spaceCount = 0;

    while(isspace(*input[spaceCount])){
        spaceCount++;
    }

    advanceString(input, spaceCount);
}

int precedence(Token token){
    if (token.value.compare("+") || token.value.compare("-")){
        return 2;
    }
    if (token.value.compare("*") || token.value.compare("/")){
        return 3;
    }
    if (token.value.compare("^")){
        return 4;
    }
}

int leftAssociative(Token token){
    return !token.value.compare("^");
}

std::stack<Token> shuntingYard(char* input){
    std::stack<Token> output;
    std::stack<Token> operators;
    Token currentChar;
    Token topChar;
    while(*input){
        cleanSpaces(&input);

        if (isFunction(input)){
            addToken(&input, isFunction(input), &output, TokenType::FUNCTION);
        }
        else if (isOperator(input)){
            currentChar = Token(input, isOperator(input), TokenType::OPERATOR);
            advanceString(&input, isOperator(input));
            if (operators.size() > 0){
            topChar = operators.top();
            }
            while(operators.size() > 0 &&
             (precedence(currentChar) < precedence(topChar) || (precedence(currentChar) < precedence(topChar) && leftAssociative(currentChar))) 
             && !topChar.value.compare("(")){
                operators.pop();
                output.push(topChar);
                topChar = operators.top();
             }
            operators.push(currentChar);
        }
        else if (isNumber(input)){
            addToken(&input, isNumber(input), &output, TokenType::NUMBER);
        }
        else if (input[0] == '('){
            addToken(&input, 1, &operators, TokenType::OPERATOR);
        }
        else if (input[0] == ')'){
            topChar = operators.top();
            int wtf = topChar.value.compare("(");
            int hey = !wtf;
            while(topChar.value.compare("(") != 0){ /* Dunno what is going on here */
                operators.pop();
                wtf = topChar.value.compare("(");
                output.push(topChar);
                topChar = operators.top();
                /* If the stack runs out without finding a left parenthesis, then there are mismatched parentheses. */
            }
            if (topChar.value.compare("(") == 0){
                operators.pop(); /* Discard */
            }
            advanceString(&input, 1);
        }
    }

    /* Clean out operator stack */
    int numOperatorsLeft = operators.size();
    for (int i = 0; i < numOperatorsLeft; i++){
        topChar = operators.top();
        operators.pop();
        output.push(topChar);
    }

    return output;
}

std::stack<Token> reverseStack(std::stack<Token> input){
    std::stack<Token> output;

    while (input.size() > 0){
        output.push(input.top());
        input.pop();
    }

    return output;
}

double evaluateFunction(std::string functionName, double argument){
    if (functionName == "sin"){
        return sin(argument);
    }
    if (functionName == "cos"){
        return cos(argument);
    }
    if (functionName == "tan"){
        return tan(argument);
    }
    throw 2;
}

double evaluateOperator(std::string operatorName, double arg1, double arg2){
    if (operatorName == "+"){
        return arg1 + arg2;
    }
    if (operatorName == "-"){
        return arg1 - arg2;
    }
    if (operatorName == "*"){
        return arg1 * arg2;
    }
    if (operatorName == "^"){
        return pow(arg1, arg2);
    }

}

double eval(std::stack<Token> function){
    function = reverseStack(function);

    Token currentToken;
    double numberStackLag1;
    double numberStackLag2;
    std::stack<double> numberStack;
    while (function.size() > 0){
        currentToken = function.top();
        if (currentToken.tokenType == TokenType::NUMBER){
            numberStack.push(stod(currentToken.value));
        }
        else if(currentToken.tokenType == TokenType::FUNCTION){
            /* Currently unary */
            numberStackLag1 = numberStack.top();
            numberStack.pop();
            numberStack.push(evaluateFunction(currentToken.value, numberStackLag1));
        }
        else if (currentToken.tokenType == TokenType::OPERATOR){
            numberStackLag1 = numberStack.top();
            numberStack.pop();
            numberStackLag2 = numberStack.top();
            numberStack.pop();
            numberStack.push(evaluateOperator(currentToken.value, numberStackLag1, numberStackLag2));
        }
        function.pop();
    }

    if (numberStack.size() != 1){
        throw 2;
    }
    return numberStack.top();
}

int main(){
    char string[]{'(', '5', '+', '6', ')', '*', '2'};
    char* point;
    point = string;
    std::stack<Token> tokens = shuntingYard(point);
    std::cout << eval(tokens);
}