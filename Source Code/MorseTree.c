#include <stdio.h>
#include <stdlib.h>
#include "morsetree.h"

// Function to create a new node
Node* createNode() {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->letter = '\0';  // Empty by default
    newNode->dot = NULL;
    newNode->dash = NULL;
    return newNode;
}

// Function to insert a character into the Morse tree
void insertMorse(Node* root, const char* morse, char letter) {
    Node* current = root;

    while (*morse) {
        if (*morse == '.') {
            if (!current->dot) current->dot = createNode();  // Create node if missing
            current = current->dot;
        } else if (*morse == '-') {
            if (!current->dash) current->dash = createNode();
            current = current->dash;
        }
        morse++;
    }

    current->letter = letter;  // Store the character at the leaf node
}

// Function to build the complete Morse Code tree
Node* buildMorseTree() {
    Node* root = createNode();  // Root is empty

    /*
    RULES:
    - A dash is equal to 3 dots
    - Space between signals forming the same letter is equal to 1 dot
    - Space between letters is equal to 3 dots
    - Space between words is equal to 7 dots
    */

    // Characters are added to the tree in the order they would appear in a breadth-first traversal
    // Characters with a comment next to them are custom characters to depict a particular meaning or characters with a double meaning such as 'x' being a letter and multiplication symbol, or 8 dots being "error"
    // Based on Morse Code document linked in assignment handbook
    // Document link: https://www.itu.int/rec/R-REC-M.1677-1-200910-I/

    // Letters

    // 1 symbol characters, 2 characters
    insertMorse(root, ".", 'e');
    insertMorse(root, "-", 't');

    // 2 symbol characters, 4 characters
    insertMorse(root, ". .", 'i');
    insertMorse(root, ". -", 'a');
    insertMorse(root, "- .", 'n');
    insertMorse(root, "- -", 'm');

    // 3 symbol characters, 8 characters
    insertMorse(root, ". . .", 's');
    insertMorse(root, ". . -", 'u');
    insertMorse(root, ". - .", 'r');
    insertMorse(root, ". - -", 'w');
    insertMorse(root, "- . .", 'd');
    insertMorse(root, "- . -", 'k');  // is also invitation to transmit
    insertMorse(root, "- - .", 'g');
    insertMorse(root, "- - -", 'o');

    // 4 symbol characters, 12 characters
    insertMorse(root, ". . . .", 'h');
    insertMorse(root, ". . . -", 'v');
    insertMorse(root, ". . - .", 'f');
    insertMorse(root, ". - . .", 'l');
    insertMorse(root, ". - - .", 'p');
    insertMorse(root, ". - - -", 'j');
    insertMorse(root, "- . . .", 'b');
    insertMorse(root, "- . . -", 'x');  // is also multiplication sign
    insertMorse(root, "- . - .", 'c');
    insertMorse(root, "- . - -", 'y');
    insertMorse(root, "- - . .", 'z');
    insertMorse(root, "- - . -", 'q');

    // Numbers and special characters

    // 5 symbol characters, 17 characters
    insertMorse(root, ". . . . .", '5');
    insertMorse(root, ". . . . -", '4');
    insertMorse(root, ". . . - .", '~');  // understood
    insertMorse(root, ". . . - -", '3');
    insertMorse(root, ". . - - -", '2');
    insertMorse(root, ". - . . .", '&');  // wait
    insertMorse(root, ". - . - .", '+');
    insertMorse(root, ". - - - -", '1');
    insertMorse(root, "- . . . .", '6');
    insertMorse(root, "- . . . -", '=');
    insertMorse(root, "- . . - .", '/');
    insertMorse(root, "- . - . -", '#');  // starting signal
    insertMorse(root, "- . - - .", '(');
    insertMorse(root, "- - . . .", '7');
    insertMorse(root, "- - - . .", '8');
    insertMorse(root, "- - - - .", '9');
    insertMorse(root, "- - - - -", '0');

    // 6 symbol characters, 10 characters
    insertMorse(root, ". . . - . -", '|');  // end of work
    insertMorse(root, ". . - - . .", '?');
    insertMorse(root, ". - . . - .", '"');
    insertMorse(root, ". - . - . -", '.');
    insertMorse(root, ". - - . - .", '@');
    insertMorse(root, ". - - - - .", '\'');
    insertMorse(root, "- . . . . -", '-');
    insertMorse(root, "- . - - . -", ')');
    insertMorse(root, "- - . . - -", ',');
    insertMorse(root, "- - - . . .", ':');

    // 8 symbol characters, 1 character
    insertMorse(root, ". . . . . . . .", '^');  // error (8 dots)

    return root;
}

// Function to decode a Morse code sequence
char decodeMorse(Node* root, const char* morse) {
    Node* current = root;

    while (*morse) {
        if (*morse == '.') {
            if (!current->dot) return '?';  // Invalid Morse sequence
            current = current->dot;
        } else if (*morse == '-') {
            if (!current->dash) return '?';
            current = current->dash;
        }
        morse++;
    }

    return (current->letter != '\0') ? current->letter : '?';  // Return character if valid
}

// Function to free memory allocated for tree
void freeMorseTree(Node* root) {
    if (root == NULL) return;
    freeMorseTree(root->dot);
    freeMorseTree(root->dash);
    free(root);
}
