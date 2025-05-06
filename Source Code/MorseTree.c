#define MAX_NODES 100
#define NULL_PTR ((void*)0)  // NULL replacement

typedef struct Node {
    char letter;
    struct Node* dot;
    struct Node* dash;
} Node;

// Static memory pool
static Node nodePool[MAX_NODES];
static int nodeIndex = 0;

// Create a new node using static pool
Node* createNode() {
    if (nodeIndex >= MAX_NODES) {
        return NULL_PTR;
    }
    Node* newNode = &nodePool[nodeIndex++];
    newNode->letter = '\0';
    newNode->dot = NULL_PTR;
    newNode->dash = NULL_PTR;
    return newNode;
}

// Function to insert a character into the Morse tree
void insertMorse(Node* root, const int* morse, int length, char letter) {
    Node* current = root;
    for (int i = 0; i < length; i++) {
        if (morse[i] == 0) {
            if (current->dot == NULL_PTR) current->dot = createNode();
            current = current->dot;
        } else if (morse[i] == 1) {
            if (current->dash == NULL_PTR) current->dash = createNode();
            current = current->dash;
        }
    }
    current->letter = letter;
}

// Function to build complete Morse Code tree
Node* buildMorseTree() {
    nodeIndex = 0;  // reset pool
    Node* root = createNode();  // root is empty

    /*
    RULES:
     * A dash is equal to 3 dots
     * Space between signals forming the same letter is equal to 1 dot
     * Space between letters is equal to 3 dots
     * Space between words is equal to 7 dots
    */

    /*
     * Characters are added to the tree in the order they would appear in a breadth-first traversal
     * Characters with a comment next to them are custom characters to depict a particular meaning
       or characters with a double meaning, such as 'x' being a letter and multiplication symbol,
       or 8 dots being "error"
     * Based on Morse Code document linked in assignment handbook
     * Document link: https://www.itu.int/rec/R-REC-M.1677-1-200910-I/
    */

    // Character translations

    // 1 symbol characters, len = 1, 2 characters
    int e[] = {0}; insertMorse(root, e, 1, 'e');
    int t[] = {1}; insertMorse(root, t, 1, 't');

    // 2 symbol characters, len = 2, 4 characters
    int i[] = {0,0}; insertMorse(root, i, 2, 'i');
    int a[] = {0,1}; insertMorse(root, a, 2, 'a');
    int n[] = {1,0}; insertMorse(root, n, 2, 'n');
    int m[] = {1,1}; insertMorse(root, m, 2, 'm');

    // 3 symbol characters, len = 3, 8 characters
    int s[] = {0,0,0}; insertMorse(root, s, 3, 's');
    int u[] = {0,0,1}; insertMorse(root, u, 3, 'u');
    int r[] = {0,1,0}; insertMorse(root, r, 3, 'r');
    int w[] = {0,1,1}; insertMorse(root, w, 3, 'w');
    int d[] = {1,0,0}; insertMorse(root, d, 3, 'd');
    int k[] = {1,0,1}; insertMorse(root, k, 3, 'k');  // is also invitation to transmit
    int g[] = {1,1,0}; insertMorse(root, g, 3, 'g');
    int o[] = {1,1,1}; insertMorse(root, o, 3, 'o');

    // 4 symbol characters, len = 4, 12 characters
    int h[] = {0,0,0,0}; insertMorse(root, h, 4, 'h');
    int v[] = {0,0,0,1}; insertMorse(root, v, 4, 'v');
    int f[] = {0,0,1,0}; insertMorse(root, f, 4, 'f');
    int l[] = {0,1,0,0}; insertMorse(root, l, 4, 'l');
    int p[] = {0,1,1,0}; insertMorse(root, p, 4, 'p');
    int j[] = {0,1,1,1}; insertMorse(root, j, 4, 'j');
    int b[] = {1,0,0,0}; insertMorse(root, b, 4, 'b');
    int x[] = {1,0,0,1}; insertMorse(root, x, 4, 'x');  // is also multiplication sign
    int c[] = {1,0,1,0}; insertMorse(root, c, 4, 'c');
    int y[] = {1,0,1,1}; insertMorse(root, y, 4, 'y');
    int z[] = {1,1,0,0}; insertMorse(root, z, 4, 'z');
    int q[] = {1,1,0,1}; insertMorse(root, q, 4, 'q');

    // Numbers and special characters

    // 5 symbol characters, len = 5, 17 characters
    int five[] = {0,0,0,0,0}; insertMorse(root, five, 5, '5');
    int four[] = {0,0,0,0,1}; insertMorse(root, four, 5, '4');
    int understood[] = {0,0,0,1,0}; insertMorse(root, understood, 5, '~');  // understood
    int three[] = {0,0,0,1,1}; insertMorse(root, three, 5, '3');
    int two[] = {0,0,1,1,1}; insertMorse(root, two, 5, '2');
    int wait[] = {0,1,0,0,0}; insertMorse(root, wait, 5, '&');  // wait
    int plus[] = {0,1,0,1,0}; insertMorse(root, plus, 5, '+');
    int one[] = {0,1,1,1,1}; insertMorse(root, one, 5, '1');
    int six[] = {1,0,0,0,0}; insertMorse(root, six, 5, '6');
    int equal[] = {1,0,0,0,1}; insertMorse(root, equal, 5, '=');
    int slash[] = {1,0,0,1,0}; insertMorse(root, slash, 5, '/');
    int start[] = {1,0,1,0,1}; insertMorse(root, start, 5, '#');  // starting signal
    int open_brack[] = {1,0,1,1,0}; insertMorse(root, open_brack, 5, '(');
    int seven[] = {1,1,0,0,0}; insertMorse(root, seven, 5, '7');
    int eight[] = {1,1,1,0,0}; insertMorse(root, eight, 5, '8');
    int nine[] = {1,1,1,1,0}; insertMorse(root, nine, 5, '9');
    int zero[] = {1,1,1,1,1}; insertMorse(root, zero, 5, '0');

    // 6 symbol characters, len = 6, 10 characters
    int end_work[] = {0,0,0,1,0,1}; insertMorse(root, end_work, 6, '|');  // end of work
    int question[] = {0,0,1,1,0,0}; insertMorse(root, question, 6, '?');
    int quote[] = {0,1,0,0,1,0}; insertMorse(root, quote, 6, '"');
    int period[] = {0,1,0,1,0,1}; insertMorse(root, period, 6, '.');
    int at[] = {0,1,1,0,1,0}; insertMorse(root, at, 6, '@');
    int apostrophe[] = {0,1,1,1,1,0}; insertMorse(root, apostrophe, 6, '\'');
    int dash[] = {1,0,0,0,0,1}; insertMorse(root, dash, 6, '-');
    int close_brack[] = {1,0,1,1,0,1}; insertMorse(root, close_brack, 6, ')');
    int comma[] = {1,1,0,0,1,1}; insertMorse(root, comma, 6, ',');
    int colon[] = {1,1,1,0,0,0}; insertMorse(root, colon, 6, ':');

    // 8 symbol characters, len = 8, 1 character
    int error[] = {0,0,0,0,0,0,0,0}; insertMorse(root, error, 8, '^');  // 8 dots (error)

    return root;
}

// Function to decode a Morse code sequence
char decodeMorse(Node* root, const int* morse, int length) {
    Node* current = root;
    for (int i = 0; i < length; i++) {
        if (morse[i] == 0) {
            if (current->dot == NULL_PTR) return '?';
            current = current->dot;
        } else if (morse[i] == 1) {
            if (current->dash == NULL_PTR) return '?';
            current = current->dash;
        }
    }
    return (current && current->letter != '\0') ? current->letter : '?';
}
