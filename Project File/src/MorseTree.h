#ifndef MORSETREE_H
#define MORSETREE_H

// Morse tree node structure
typedef struct Node {
    char letter;
    struct Node* dot;
    struct Node* dash;
} Node;

// Function declarations
Node* createNode();
void insertMorse(Node* root, const int* morse, int length, char letter);
Node* buildMorseTree();
char decodeMorse(Node* root, const int* morse, int length);

#endif
