#include <stdio.h>
#include <string.h>
#define MAX_OUTPUT_SIZE 1000

int my_utf8_encode(unsigned char *input, unsigned char *output);
void test_my_utf8_encode(char *input, char *expected);
// useful func1
int isValid(char *input);
int my_utf8_decode(unsigned char *input, unsigned char *output);
void test_my_utf8_decode(unsigned char *input, char *expected);
int my_utf8_check(unsigned char *string);
void test_my_utf8_check();
int my_utf8_strlen(unsigned char *string);
void test_my_utf8_strlen();
char *my_utf8_charat(unsigned char *string, int index);
char test_my_utf8_charat(char *string, int index, char *expected_result);
int my_utf8_strcmp(unsigned char *string1, unsigned char *string2);
// useful func2
int utf8_char_length(unsigned char byte);


int main() {

    test_my_utf8_encode("\\u05d0 \\u05E8 \\u05D9 \\u05D4\n", "אריה");
    test_my_utf8_encode("\\u0041 \\u0062 \\u0043\n", "AbC");
    test_my_utf8_encode("\n", "");

    test_my_utf8_decode("אריה", "\\u05D0\\u05E8\\u05D9\\u05D4");
    test_my_utf8_decode("AbC" , "\\u0041\\u0062\\u0043\n");
    test_my_utf8_decode("" , "");

    test_my_utf8_check();

    test_my_utf8_strlen();

    test_my_utf8_charat("אריה", 1, "ר");
    test_my_utf8_charat("こんにちは", 2, "に");
    test_my_utf8_charat("你好", 0, "你");

    return 0;
}

char test_my_utf8_charat(char *string, int index, char *expected_result) {

    char passed[] = "Test charat: Passed\n";
    char failed[] = "Test charat: Failed\n";

    unsigned char *result = my_utf8_charat(string, index);  // store the result


    my_utf8_strcmp(result, expected_result) == 1 ? printf("%s", passed) : printf("%s", failed);

}

// func2
int utf8_char_length(unsigned char byte) {
    // Determine the number of bytes in utf8 character (it can only be 1-4)
    if ((byte & 0x80) == 0) {
        return 1;
    } else if ((byte & 0xE0) == 0xC0) {
        return 2;
    } else if ((byte & 0xF0) == 0xE0) {
        return 3;
    } else if ((byte & 0xF8) == 0xF0) {
        return 4;
    } else {
        return 0;
    }
}

char *my_utf8_charat(unsigned char *string, int index) {

    // if we dont have a valid string or index, return null
    if (string == NULL || index < 0) {
        return NULL;
    }

    int i = 0;
    // while we are still in the string and have an index
    // get the length of the character - make sure its not 0
    while (string[i] != '\0' && index > 0) {
        int char_length = utf8_char_length(string[i]);
        if (char_length == 0) {
            return NULL;
        }
        index--;                    //decrement index is its a valid character
        i += char_length;           // Move to the next character in the string
    }

    if (index == 0 && string[i] != '\0') {      // if it found the char, return its starting address
        return &string[i];
    } else {                                    // otherwise, return null
        return NULL;
    }
}


void test_my_utf8_strlen(){

    char passed[] = "Test strlen: Passed\n";
    char failed[] = "Test strlen: Failed\n";

    unsigned char string1[] = "אריה";
    my_utf8_strlen(string1) == 4 ? printf("1 %s", passed) : printf("1 %s", failed);
    unsigned char string2[] = "好你";
    my_utf8_strlen(string2) == 2 ? printf("2 %s\n", passed) : printf("2 %s", failed);
    unsigned char string3[] = "Hello World";
    my_utf8_strlen(string3) == 11 ? printf("3 %s\n", passed) : printf("3 %s", failed);
}

// Returns the number of characters/ symbols in the string.
int my_utf8_strlen(unsigned char *string){
    int strlen = 0;             // initialize to 0
    while (*string != '\0'){    // while we are not at the end
        // find 2 most important bits (leftmost) and if it's not a 10 then it's a new symbol, increment
        if ((*string & 0xC0) != 0x80) {
            strlen++;
        }
        string++;
    }
    return strlen;
}


void test_my_utf8_check(){

    char passed[] = "Test check: Passed\n";
    char failed[] = "Test check: Failed\n";

    // check that it is a valid encoding
    my_utf8_check("Hello, World!") == 1 ? printf("1 %s", passed) : printf("1 %s", failed);
    my_utf8_check("你好，世界！") == 1 ? printf("2 %s", passed) : printf("2 %s", failed);
    my_utf8_check("\xE6\x97\xA5\xE6\x9C\xAC\xE8\xAA\x9E") == 1 ? printf("3 %s", passed) : printf("3 %s\n", failed);
    my_utf8_check("אריה") == 1 ? printf("4 %s", passed) : printf("4 %s", failed);
    my_utf8_check("\xF0\x9F\x98\x8A") == 1 ? printf("5 %s", passed) : printf("5 %s", failed);
    my_utf8_check("\xC3\x28") == 0 ? printf("6 %s", passed) : printf("6 %s", failed);
    unsigned char string7[] = {0xD7, 0x90, 0xD7, 0xA8, 0xD7, 0x99, 0xD7, 0x94, 0x0};
    my_utf8_check(string7) == 1 ? printf("7 %s\n", passed) : printf("7 %s\n", failed);
}

// Validates that the input string is a valid UTF8 encoded string.
int my_utf8_check(unsigned char *string){
    // while we didn't reach the end of the string
    while (*string != '\0'){
        // if it's a single byte char, can go to next one
        if ((*string & 0x80) == 0) {
            string++;
        }
        // if it's a two byte character, make sure its valid, move on
        else if ((*string & 0xE0) == 0xC0) {
            if ((*(string + 1) & 0xC0) != 0x80 || (*(string + 1) & 0xC0) == 0xC0)
                return 0;
            string += 2;
        }
        // if it's a three byte character, make sure its valid, move on
        else if ((*string & 0xF0) == 0xE0) {
            // Three-byte character
            if ((*(string + 1) & 0xC0) != 0x80 || (*(string + 2) & 0xC0) != 0x80)
                return 0;
            string += 3;
        }
        // if it's a four byte character, make sure its valid, move on
        else if ((*string & 0xF8) == 0xF0) {
            if ((*(string + 1) & 0xC0) != 0x80 || (*(string + 2) & 0xC0) != 0x80
                ||(*(string + 3) & 0xC0) != 0x80)
                return 0;
            string += 4;
        }
        // otherwise it is invalid
        else {
            return 0;
        }
    }
    return 1; // at this point it has to be a valid utf8 string
}

void test_my_utf8_decode(unsigned char *input, char *expected){


    char passed[] = "Decode test: Passed\n";
    char failed[] = "Decode test: Failed\n";


    unsigned char array[MAX_OUTPUT_SIZE];
    unsigned char *output = array;
    my_utf8_decode(input, output);
    printf("Decoded input: %s\n", input);
    printf("Decoded output: %s\n", output);
    printf("Decoded expected: %s\n", expected);

    my_utf8_strcmp(output, expected) == 1?  printf("%s", passed) : printf("%s", failed);

}

int my_utf8_decode(unsigned char *input, unsigned char *output) {
    // initialize variables
    unsigned int byte1, byte2, byte3, byte4, unicode;
    unsigned char byteString[7], unicodeString[5];

    while (*input != '\0') {                    // while we are in the string
        byte1 = *input++;                       // get the first char
        *output++ = 0x5C;                       // hardcode a \

        // get right amount of bytes, then add right amount of indecies to output
        if ((byte1 & 0x80) == 0x00) {
            *output++ = 0x75;                   // hard coding a u
            *output++ = 0x30;                   // hard coding a 0
            *output++ = 0x30;                   // hard coding a 0
            sprintf(byteString, "%02X", byte1); // format and add the correct amount of bytes
            *output++ = byteString[0];
            *output++ = byteString[1];
        } else if ((byte1 & 0xE0) == 0xC0) {
            byte2 = *input++;
            *output++ = 0x75;
            *output++ = 0x30;
            unicode = (((byte1 & 0x1F) << 6) | (byte2 & 0x3F));
            sprintf(unicodeString, "%03X", unicode);
            *output++ = unicodeString[0];
            *output++ = unicodeString[1];
            *output++ = unicodeString[2];
        } else if ((byte1 & 0xF0) == 0xE0) {
            byte2 = *input++;
            byte3 = *input++;
            *output++ = 0x75;
            *output++ = 0x30;
            unicode = (((byte1 & 0x0F) << 12) | ((byte2 & 0x3F) << 6) | (byte3 & 0x3F));
            sprintf(unicodeString, "%04X", unicode);
            *output++ = unicodeString[0];
            *output++ = unicodeString[1];
            *output++ = unicodeString[2];
        } else if ((byte1 & 0xF8) == 0xF0) {
            byte2 = *input++;
            byte3 = *input++;
            byte4 = *input++;
            *output++ = 0x55;
            unicode = (((byte1 & 0x07) << 18) | ((byte2 & 0x3F) << 12) | ((byte3 & 0x3F) << 6) | (byte4 & 0x3F));
            sprintf(unicodeString, "%06X", unicode);
            *output++ = unicodeString[0];
            *output++ = unicodeString[1];
            *output++ = unicodeString[2];
        } else {                                // otherwise it is not a valid UTF8 sequence, return
            printf("Invalid UTF8 sequence\n");
            return 0;
        }
    }
    *output = '\0';         // end output with a null , return
    return 1;
}


//Returns whether the two strings are the same (similar result set to strcmp() )
int my_utf8_strcmp(unsigned char *string1, unsigned char *string2){

    while (*string1 != '\0' && *string2 != '\0') {
        if (*string1 != *string2) {
            return 0; // Strings are different
        }
        string1++;
        string2++;
    }

    // Check if one of the strings has more characters
    if (*string1 == '\0' || *string2 == '\0'){
        return 1;
    } else {
        return 0; // Strings are of different lengths
    }
}

// extra function 1
int isValid(char *input) {
    //printf("IsValid: %x\n", *input);
    for (int i = 0; i < 4; i++) {
        if (*input < 0x30 || (*input > 0x39 && *input < 0x41) ||
            (*input > 0x46 && *input< 0x61) || *input > 0x66) {
            //printf("Error: not valid hex value\n");
            return 0;
        }
        input++;
    }
    return 1;
}

void test_my_utf8_encode(char *input, char *expected) {

    char passed[] = "Encode test: Passed\n";
    char failed[] = "Encode test: Failed\n";


    char array[MAX_OUTPUT_SIZE];
    char *output = array;
    my_utf8_encode(input, output);
    printf("Encoded input: %s", input);
    printf("Encoded output: %s\n", output);
    printf("Encoded expected: %s\n", expected);


    my_utf8_strcmp(output, expected) == 1?  printf("%s", passed) : printf("%s", failed);
}

//Encoding a UTF8 string, taking as input an ASCII string, with UTF8
// characters encoded using the Codepoint numbering scheme notation, and returns a UTF8 encoded string.
int my_utf8_encode(unsigned char *input, unsigned char *output) {
    int index = 0;
    while (*input != '\0') {                        // while we haven't reached the end of the input string
        if (*input == '\\') {                       // if there's a \, increment and check for u or U
            input++;
            if (*input == 'u' || *input == 'U') {   // if there is, check if its a valid input
                input++;
                int a = isValid(input);
                if (a == 1) {                       // if it is valid, create a string of the unicode

                    unsigned char unicodeString[5];
                    unsigned int unicodeInt = 0;

                    for (int i = 0; i < 4; i++) {
                        unicodeString[i] = *input;
                        input++;
                    }

                    sscanf((char *)unicodeString, "%x", &unicodeInt);   // transform it to correct format (hex)

                    // with the unicode figure out how many bytes there are and deal with it accordingly
                    if (unicodeInt <= 0x7F) {
                        output[index++] = (char) unicodeInt;
                        output[index] = '\0';
                    } else if (unicodeInt <= 0x7FF) {
                        output[index++] = (char) (0xC0 | ((unicodeInt >> 6) & 0x1F));
                        output[index++] = (char) (0x80 | (unicodeInt & 0x3F));
                        output[index] = '\0';
                    } else if (unicodeInt <= 0xFFFF) {
                        output[index++] = (char) (0xE0 | ((unicodeInt >> 12) & 0x0F));
                        output[index++] = (char) (0x80 | ((unicodeInt >> 6) & 0x3F));
                        output[index++] = (char) (0x80 | (unicodeInt & 0x3F));
                        output[index] = '\0';
                    } else if (unicodeInt <= 0x10FFFF) {
                        output[index++] = (char) (0xF0 | ((unicodeInt >> 18) & 0x07));
                        output[index++] = (char) (0x80 | ((unicodeInt >> 12) & 0x3F));
                        output[index++] = (char) (0x80 | ((unicodeInt >> 6) & 0x3F));
                        output[index++] = (char) (0x80 | (unicodeInt & 0x3F));
                        output[index] = '\0';
                    } else {                                    // error if its not a valid unicode
                        printf("Error: Invalid unicode \n");
                        output[0] = '\0';
                        return 0;
                    }
                }
                    // if its not a valid string, manualy add the \ to output, and decrement to add it to the output
                else{
                    output[index++] = '\\';
                    input = input-1;
                    output[index++] = *input;
                }
            }
        }
            // this means it is just a regular ascii character, so just add it to output
        else {
            output[index++] = *input;
        }
        input++;    // go to next character
    }
    // end the output with a NULL, and return
    output[index] = '\0';
    return 1;
}