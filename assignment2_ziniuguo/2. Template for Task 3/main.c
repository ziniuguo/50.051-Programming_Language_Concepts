#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Our Token Types for this LOLBOT language
typedef enum {
    START_TOKEN,
    FORWARD_TOKEN,
    LEFT_TOKEN,
    BACKWARD_TOKEN,
    RIGHT_TOKEN,
    STOP_TOKEN,
    UNKNOWN_TOKEN
} TokenType;


// Our Token struct, as shown in class
typedef struct {
    TokenType tokenType;
    const char* lexeme;
} Token;


// A struct to contain our list of lexemes. Consists of
// - a number of lexemes, called num_lexemes,
// - and as array of string lexemes, called lexemes.
typedef struct {
    int num_lexemes;
    char** lexemes;
} LexemeArray;


// This function should load the content of the file defined
// by the filename string (in our case, probably "source.lolbot").
// It should return a string source_code corresponding to the content
// of the source.lolbot file.
// At the moment it returns 0.
char* read_file(const char* filename) {
    return 0;
}


// This function will split the source_code that has been retrieved from the file
// and will separate all the lexemes using white spaces and \n symbols as separators.
// It should return a struct LexemeArray, described above.
// At the moment it will just create an empty array with 0 lexemes.
LexemeArray split_lexemes(const char* code) {
	
	// Our LexemeArray struct
	// (Currently empty)
    LexemeArray result;
    result.num_lexemes = 0;
    result.lexemes = malloc(0 * sizeof(char*));
    return result;
}

// This function should check for a token type on the given lexeme
// and create a Token object with the correct type and lexeme
Token create_token(const char* lexeme) {
    return 0;
}


// Our function printing a given Token object is missing a few things
void print_token(Token token) {
    printf("TOKEN(Something, Something)");
}


// Assemble everything in the main
int main() {
	
	// Read the file
    const char* filename = "source.lolbot";
    char* code = read_file(filename);
    if (!code) {
        fprintf(stderr, "Error reading the file: %s\n", filename);
        return 1;
    }
	
	// Split the code and produce lexemes array
    LexemeArray lexeme_array = ...
	
	// Read each lexeme in array and create token for given lexeme
	// Print lexeme as well
    for (...) {
        ...
    }
	
    // Free the memory
    ...
	
    return 0;
}
