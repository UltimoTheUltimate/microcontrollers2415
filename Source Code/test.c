#include <stdio.h>
#include "morsetree.h"
#include <assert.h>

/*
void unitTestDecodeMorse() {
    // Test on small Morse tree
    Node* root = createNode();
    insertMorse(root, ".", 'E');
    insertMorse(root, "-", 'T');

    char result;

    // Valid test case: Decoding a single Morse code symbol
    result = decodeMorse(root, ".");
    assert(result == 'E');  // Expecting 'E' for Morse code "."

    result = decodeMorse(root, "-");
    assert(result == 'T');  // Expecting 'T' for Morse code "-"

    // Invalid test case: Decoding an invalid Morse code
    result = decodeMorse(root, "..");  // This should reach a dead end in the tree, would be "i" in a full tree
    assert(result == '?');  // Expecting '?' because ".." is not mapped

    result = decodeMorse(root, "-.-.");  // Invalid Morse sequence, would be "c" in a full tree
    assert(result == '?');  // Expecting '?' due to no mapping

    printf("All test cases passed!\n");
}
*/

/*
int main() {
    unitTestDecodeMorse();
    
    Node* morseTree = buildMorseTree();

    // Test decoding                                                                         should return:
    printf("Morse '.' -> %c\n", decodeMorse(morseTree, "."));                        // e
    printf("Morse '-' -> %c\n", decodeMorse(morseTree, "-"));                        // t
    printf("Morse '. -' -> %c\n", decodeMorse(morseTree, ". -"));                    // a
    printf("Morse '- . . .' -> %c\n", decodeMorse(morseTree, "- . . ."));            // b
    printf("Morse '- . - .' -> %c\n", decodeMorse(morseTree, "- . - ."));            // c
    printf("Morse '. - - - -' -> %c\n", decodeMorse(morseTree, ". - - - -"));        // 1
    printf("Morse '. . - - -' -> %c\n", decodeMorse(morseTree, ". . - - -"));        // 2
    printf("Morse '. . . - -' -> %c\n", decodeMorse(morseTree, ". . . - -"));        // 3
    printf("Morse '- - -' -> %c\n", decodeMorse(morseTree, "- - -"));                // o

    return 0;
}
*/

int main() {
    Node* root = buildMorseTree();

    printf("Morse '.' -> %c\n", decodeMorse(root, (int[]){0}, 1));                             // 0 (.) should print e
    printf("Morse '-' -> %c\n", decodeMorse(root, (int[]){1}, 1));                         // 1 (-) should print t

    printf("Morse '. .' -> %c\n", decodeMorse(root, (int[]){0,0}, 2));                 // 00 (. .) should print i
    printf("Morse '. -' -> %c\n", decodeMorse(root, (int[]){0,1}, 2));                 // 01 (. -) should print a

    printf("Morse '. . .' -> %c\n", decodeMorse(root, (int[]){0,0,0}, 3));         // 000 (. . .) should print s
    printf("Morse '- - -' -> %c\n", decodeMorse(root, (int[]){1,1,1}, 3));         // 111 (- - -) should print o

    printf("Morse '- . . -' -> %c\n", decodeMorse(root, (int[]){1,0,0,1}, 4)); // 1001 (- . . -) should print x
    printf("Morse '7 . . -' -> %c\n", decodeMorse(root, (int[]){7,0,0,1}, 4)); // 7001 (7 . . -) should print ? via error

    return 0;
}
