#include <stdio.h>
#include "morsetree.h"

int main() {
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
