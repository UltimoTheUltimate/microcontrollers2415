#include <stdio.h>
#include "morsetree.h"
#include <assert.h>

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
