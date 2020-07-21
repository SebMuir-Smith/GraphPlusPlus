#include <stack>
#include <string>
#include<ctype.h>

class Parser {
    std::stack<int> integers;
};

class Token {
    public:
    Token(char* string, int nChars){
        value = std::string(string, nChars);
    }

    Token(){
        value = "DEFAULT";
    }

    std::string value;
};

/* Returns the number of characters making up the next double in the string, if the next token is a double */
int isNumber(char* input){
    char* restOfString;
    double conversion = strtod(input, &restOfString);

    return (input - restOfString) / (sizeof(char));
}

bool searchFunctions(std::string search){
    std::string validFunctions[] {
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

/* Add token to input stack and move along input string */
void addToken(char** inputString, int nChars, std::stack<Token>* outputStack){
    Token newToken(*inputString, nChars);
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

void advanceString(char** input, int nChars){
    *input = *input + sizeof(char) * nChars;
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

std::string shuntingYard(char* input, int nChars){
    std::stack<Token> output;
    std::stack<Token> operators;
    Token currentChar;
    Token topChar;
    for (int i = 0; i < nChars; i++){
        cleanSpaces(&input);

        if (isNumber(input)){
            addToken(&input, isNumber(input), &output);
        }
        else if (isFunction(input)){
            addToken(&input, isFunction(input), &output);
        }
        else if (isOperator(input)){
            advanceString(&input, isOperator(input));
            currentChar = Token(input, isOperator(input));
            topChar = operators.top();
            while(operators.size() > 0 &&
             (precedence(currentChar) < precedence(topChar) || (precedence(currentChar) < precedence(topChar) && leftAssociative(currentChar))) 
             && !topChar.value.compare("(")){
                operators.pop();
                output.push(topChar);
                topChar = operators.top();
             }
            operators.push(currentChar);
        }
        else if (topChar.value.compare("(")){
            addToken(&input, 1, &output);
        }
        else if (topChar.value.compare(")")){
            topChar = operators.top();
            while(!topChar.value.compare("(")){
                operators.pop();
                output.push(topChar);
                topChar = operators.top();
                /* If the stack runs out without finding a left parenthesis, then there are mismatched parentheses. */
            }
            if (topChar.value.compare("(")){
                operators.pop(); /* Discard */
            }
        }
    }

    int numOperatorsLeft = operators.size();
    for (int i = 0; i < numOperatorsLeft; i++){
        topChar = operators.top();
        operators.pop();
        output.push(topChar);
    }
}