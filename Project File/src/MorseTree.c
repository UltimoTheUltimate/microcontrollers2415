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

// Morse code arrays moved to file scope for C89 compliance
static int e[] = {0};
static int t[] = {1};
static int i_[] = {0,0};
static int a[] = {0,1};
static int n[] = {1,0};
static int m[] = {1,1};
static int s[] = {0,0,0};
static int u[] = {0,0,1};
static int r[] = {0,1,0};
static int w[] = {0,1,1};
static int d[] = {1,0,0};
static int k[] = {1,0,1};
static int g[] = {1,1,0};
static int o[] = {1,1,1};
static int h[] = {0,0,0,0};
static int v[] = {0,0,0,1};
static int f[] = {0,0,1,0};
static int l[] = {0,1,0,0};
static int p[] = {0,1,1,0};
static int j[] = {0,1,1,1};
static int b[] = {1,0,0,0};
static int x[] = {1,0,0,1};
static int c[] = {1,0,1,0};
static int y[] = {1,0,1,1};
static int z[] = {1,1,0,0};
static int q[] = {1,1,0,1};
static int five[] = {0,0,0,0,0};
static int four[] = {0,0,0,0,1};
static int understood[] = {0,0,0,1,0};
static int three[] = {0,0,0,1,1};
static int two[] = {0,0,1,1,1};
static int wait[] = {0,1,0,0,0};
static int plus[] = {0,1,0,1,0};
static int one[] = {0,1,1,1,1};
static int six[] = {1,0,0,0,0};
static int equal[] = {1,0,0,0,1};
static int slash[] = {1,0,0,1,0};
static int start[] = {1,0,1,0,1};
static int open_brack[] = {1,0,1,1,0};
static int seven[] = {1,1,0,0,0};
static int eight[] = {1,1,1,0,0};
static int nine[] = {1,1,1,1,0};
static int zero[] = {1,1,1,1,1};
static int end_work[] = {0,0,0,1,0,1};
static int question[] = {0,0,1,1,0,0};
static int quote[] = {0,1,0,0,1,0};
static int period[] = {0,1,0,1,0,1};
static int at[] = {0,1,1,0,1,0};
static int apostrophe[] = {0,1,1,1,1,0};
static int dash[] = {1,0,0,0,0,1};
static int close_brack[] = {1,0,1,1,0,1};
static int comma[] = {1,1,0,0,1,1};
static int colon[] = {1,1,1,0,0,0};
static int error[] = {0,0,0,0,0,0,0,0};
Node* root;
// Create a new node using static pool
Node* createNode() {
    Node* newNode;
    if (nodeIndex >= MAX_NODES) {
        return NULL_PTR;
    }
    newNode = &nodePool[nodeIndex++];
    newNode->letter = '\0';
    newNode->dot = NULL_PTR;
    newNode->dash = NULL_PTR;
    return newNode;
}

// Function to insert a character into the Morse tree
void insertMorse(Node* root, const int* morse, int length, char letter) {
    Node* current = root;
    int i; // declaration moved to top
    for (i = 0; i < length; i++) {
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
    
    root = createNode();  // root is empty

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
    insertMorse(root, e, 1, 'e');
    insertMorse(root, t, 1, 't');

    // 2 symbol characters, len = 2, 4 characters
    insertMorse(root, i_, 2, 'i');
    insertMorse(root, a, 2, 'a');
    insertMorse(root, n, 2, 'n');
    insertMorse(root, m, 2, 'm');

    // 3 symbol characters, len = 3, 8 characters
    insertMorse(root, s, 3, 's');
    insertMorse(root, u, 3, 'u');
    insertMorse(root, r, 3, 'r');
    insertMorse(root, w, 3, 'w');
    insertMorse(root, d, 3, 'd');
    insertMorse(root, k, 3, 'k');
    insertMorse(root, g, 3, 'g');
    insertMorse(root, o, 3, 'o');

    // 4 symbol characters, len = 4, 12 characters
    insertMorse(root, h, 4, 'h');
    insertMorse(root, v, 4, 'v');
    insertMorse(root, f, 4, 'f');
    insertMorse(root, l, 4, 'l');
    insertMorse(root, p, 4, 'p');
    insertMorse(root, j, 4, 'j');
    insertMorse(root, b, 4, 'b');
    insertMorse(root, x, 4, 'x');  // is also multiplication sign
    insertMorse(root, c, 4, 'c');
    insertMorse(root, y, 4, 'y');
    insertMorse(root, z, 4, 'z');
    insertMorse(root, q, 4, 'q');

    // Numbers and special characters

    // 5 symbol characters, len = 5, 17 characters
    insertMorse(root, five, 5, '5');
    insertMorse(root, four, 5, '4');
    insertMorse(root, understood, 5, '~');  // understood
    insertMorse(root, three, 5, '3');
    insertMorse(root, two, 5, '2');
    insertMorse(root, wait, 5, '&');  // wait
    insertMorse(root, plus, 5, '+');
    insertMorse(root, one, 5, '1');
    insertMorse(root, six, 5, '6');
    insertMorse(root, equal, 5, '=');
    insertMorse(root, slash, 5, '/');
    insertMorse(root, start, 5, '#');  // starting signal
    insertMorse(root, open_brack, 5, '(');
    insertMorse(root, seven, 5, '7');
    insertMorse(root, eight, 5, '8');
    insertMorse(root, nine, 5, '9');
    insertMorse(root, zero, 5, '0');

    // 6 symbol characters, len = 6, 10 characters
    insertMorse(root, end_work, 6, '|');  // end of work
    insertMorse(root, question, 6, '?');
    insertMorse(root, quote, 6, '"');
    insertMorse(root, period, 6, '.');
    insertMorse(root, at, 6, '@');
    insertMorse(root, apostrophe, 6, '\'');
    insertMorse(root, dash, 6, '-');
    insertMorse(root, close_brack, 6, ')');
    insertMorse(root, comma, 6, ',');
    insertMorse(root, colon, 6, ':');

    // 8 symbol characters, len = 8, 1 character
    insertMorse(root, error, 8, '^');  // 8 dots (error)

    return root;
}

// Function to decode a Morse code sequence
char decodeMorse(Node* root, const int* morse, int length) {
    Node* current = root;
    int i; // declaration moved to top
    for (i = 0; i < length; i++) {
        if (morse[i] == 0) {
            if (current->dot == NULL_PTR) return '?';
            current = current->dot;
        } else if (morse[i] == 1) {
            if (current->dash == NULL_PTR) return '?';
            current = current->dash;
        } else {
            // Invalid input: only 0 or 1 allowed
            return '?';
        }
    }
    return (current && current->letter != '\0') ? current->letter : '?';
}
