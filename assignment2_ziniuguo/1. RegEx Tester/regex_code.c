#include <stdio.h>
#include <regex.h>

// RegEx helper file for Task #2.B in HW9.
int main() {
	// Input string to test
    char* string = "1001010";
	// The RegEx you would like to test
    char* pattern = "1";
	
	// Compiling and testing the RegEx
    regex_t regex;
    int status = regcomp(&regex, pattern, REG_EXTENDED);
    if (status != 0) {
        printf("Error compiling regex pattern.\n");
        return 1;
    }
    status = regexec(&regex, string, 0, NULL, 0);
    if (status == 0) {
        printf("Pattern matched!\n");
    } else if (status == REG_NOMATCH) {
        printf("Pattern not matched.\n");
    } else {
        printf("Error matching pattern.\n");
        return 1;
    }
    regfree(&regex);
    return 0;
}
