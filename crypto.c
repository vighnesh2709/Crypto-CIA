#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define SIZE 2
#define MAX_GRAM_LEN 10
#define MAX_NGRAMS 100

// Caesar Cipher
void caesarCipher(char *Input, bool august, bool decrypt){
    int val;
    if (august){
        val = 1;
    } else {
        printf("Enter the shift: ");
        scanf("%d",&val);
    }
    
    // For decryption, negate the shift value
    if (decrypt) {
        val = -val;
    }
    
    for (int i = 0; Input[i] != '\0'; i++) {
        char ch = Input[i];
        char base;
        if (isalpha(ch)) {
            if(isupper(ch)){
                base = 'A';
            } else {
                base = 'a';
            }
            
            Input[i] = (ch - base + val) % 26;
            if (Input[i] < 0) Input[i] += 26;
            Input[i] += base;
        }
    }
}

// Atbash Cipher (same for encryption and decryption)
void atbash(char *Input){
    for (int i = 0; Input[i] != '\0'; i++) {
        char ch = Input[i];
        char base;

        if (isalpha(ch)) {
            if (isupper(ch)) {
                base = 'A';
            } else {
                base = 'a';
            }
            Input[i] = base + (25 - (ch - base));
        }
    }
}

int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Find modular multiplicative inverse
int modInverse(int a, int m) {
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 1) {
            return x;
        }
    }
    return -1; // Should not happen if a and m are coprime
}

// Affine Cipher
void affine(char *text, bool decrypt) {
    int a, b;
    
    do {
        printf("Enter a (must be coprime with 26): ");
        scanf("%d",&a);
        if (gcd(a, 26) != 1) {
            printf("Error: 'a' must be coprime with 26. Try again.\n");
        }
    } while (gcd(a, 26) != 1);
    
    printf("Enter b: ");
    scanf("%d",&b);
    
    int a_inverse = -1;
    if (decrypt) {
        a_inverse = modInverse(a, 26);
        if (a_inverse == -1) {
            printf("Error: Couldn't find modular inverse.\n");
            return;
        }
    }
    
    for (int i = 0; text[i] != '\0'; i++) {
        char ch = text[i];

        if (isalpha(ch)) {
            char base = isupper(ch) ? 'A' : 'a';
            int x = ch - base;
            int result;
            
            if (decrypt) {
                // Decrypt: (a^-1 * (y - b)) mod 26
                result = (a_inverse * (x - b)) % 26;
            } else {
                // Encrypt: (ax + b) mod 26
                result = (a * x + b) % 26;
            }
            
            if (result < 0) result += 26;  
            text[i] = result + base;
        }
    }
}

// Vigenere/Beaufort Cipher
void vigenere(char *text, bool beaufort, bool decrypt) {
    char key[100];
    
    printf("Enter the Key: ");
    getchar();  
    fgets(key, sizeof(key), stdin);
    key[strcspn(key, "\n")] = '\0';  
    
    int keyLen = strlen(key);
    int keyIndex = 0;
    
    for (int i = 0; text[i] != '\0'; i++) {
        if (isalpha(text[i])) {
            char base = isupper(text[i]) ? 'A' : 'a';
            int textVal = text[i] - base;
            int keyVal = tolower(key[keyIndex % keyLen]) - 'a';
            
            if (beaufort) {
                // Beaufort is its own inverse, so encryption = decryption
                text[i] = ((keyVal - textVal + 26) % 26) + base;
            } else {
                // Vigenere
                if (decrypt) {
                    // Decryption: (c - k + 26) % 26
                    text[i] = ((textVal - keyVal + 26) % 26) + base;
                } else {
                    // Encryption: (p + k) % 26
                    text[i] = ((textVal + keyVal) % 26) + base;
                }
            }
            
            keyIndex++;
        }
    }
}

// Gronsfeld Cipher
void gronsfeld(char *text, bool decrypt) {
    char key[100];
    
    printf("Enter the numeric key: ");
    getchar();  
    fgets(key, sizeof(key), stdin);
    key[strcspn(key, "\n")] = '\0';  
    
    int keyLen = strlen(key);
    int keyIndex = 0;
    
    for (int i = 0; text[i] != '\0'; i++) {
        if (isalpha(text[i])) {
            char base = isupper(text[i]) ? 'A' : 'a';
            int textVal = text[i] - base;
            int keyVal = key[keyIndex % keyLen] - '0'; 
            
            if (decrypt) {
                // Decryption: (c - k + 26) % 26
                text[i] = ((textVal - keyVal + 26) % 26) + base;
            } else {
                // Encryption: (p + k) % 26
                text[i] = ((textVal + keyVal) % 26) + base;
            }
            
            keyIndex++;
        }
    }
}

// Autokey Cipher
void autokey(char *text, bool decrypt) {
    char key[100];
    
    printf("Enter the initial Key (a word): ");
    getchar();  
    fgets(key, sizeof(key), stdin);
    key[strcspn(key, "\n")] = '\0';  
    
    int keyLen = strlen(key);
    int textLen = strlen(text);
    int keyIndex = 0;
    
    if (decrypt) {
        // Need to keep track of decrypted characters to use as subsequent keys
        char decrypted[1000];
        strcpy(decrypted, "");
        
        for (int i = 0; i < textLen; i++) {
            if (isalpha(text[i])) {
                char base = isupper(text[i]) ? 'A' : 'a';
                int textVal = text[i] - base;
                int keyVal;
                
                if (keyIndex < keyLen) {
                    // Use original key
                    keyVal = tolower(key[keyIndex]) - 'a';
                } else {
                    // Use previously decrypted character
                    keyVal = tolower(decrypted[keyIndex - keyLen]) - 'a';
                }
                
                // Decryption: (c - k + 26) % 26
                char decryptedChar = ((textVal - keyVal + 26) % 26) + base;
                text[i] = decryptedChar;
                
                // Append to decrypted string
                decrypted[keyIndex] = tolower(decryptedChar);
                decrypted[keyIndex + 1] = '\0';
                
                keyIndex++;
            }
        }
    } else {
        // Encryption (original function)
        char plaintext[1000]; 
        strcpy(plaintext, text);  
        
        int plaintextIndex = 0;
        
        for (int i = 0; i < textLen; i++) {
            if (isalpha(text[i])) {
                char base = isupper(text[i]) ? 'A' : 'a';
                int textVal = text[i] - base;
                int keyVal;
                
                if (keyIndex < keyLen) {
                    keyVal = tolower(key[keyIndex]) - 'a';
                } else {
                    while (plaintextIndex < textLen && !isalpha(plaintext[plaintextIndex])) {
                        plaintextIndex++;
                    }
                    keyVal = tolower(plaintext[plaintextIndex]) - 'a';
                    plaintextIndex++;
                }
                
                text[i] = ((textVal + keyVal) % 26) + base;
                keyIndex++;
            }
        }
    }
}

typedef struct {
    char gram[MAX_GRAM_LEN + 1];  
    int count;
} NGram;

void toLowerCase(char *str) {
    for (int i = 0; str[i]; i++)
        str[i] = tolower(str[i]);
}

int compareNGrams(const void *a, const void *b) {
    NGram *ngramA = (NGram *)a;
    NGram *ngramB = (NGram *)b;
    return strcmp(ngramA->gram, ngramB->gram);
}

void generateNGrams(char *text, int n) {
    NGram ngrams[MAX_NGRAMS];
    int total = 0;

    int len = strlen(text);
    
    char cleanText[1000];
    int cleanLen = 0;
    
    for (int i = 0; i < len; i++) {
        if (isalpha(text[i])) {
            cleanText[cleanLen++] = tolower(text[i]);
        }
    }
    cleanText[cleanLen] = '\0';

    for (int i = 0; i <= cleanLen - n; i++) {
        char temp[MAX_GRAM_LEN + 1] = {0};
        strncpy(temp, &cleanText[i], n);
        temp[n] = '\0';  
        
        int found = 0;
        for (int j = 0; j < total; j++) {
            if (strcmp(temp, ngrams[j].gram) == 0) {
                ngrams[j].count++;
                found = 1;
                break;
            }
        }
        
        if (!found && total < MAX_NGRAMS) {
            strncpy(ngrams[total].gram, temp, n);
            ngrams[total].gram[n] = '\0';
            ngrams[total].count = 1;
            total++;
        }
    }

    qsort(ngrams, total, sizeof(NGram), compareNGrams);

    printf("\nN-Grams of size %d:\n", n);
    for (int i = 0; i < total; i++) {
        printf("%s: %d\n", ngrams[i].gram, ngrams[i].count);
    }
}

// Determinant of a 2x2 matrix
int det2x2(int matrix[SIZE][SIZE]) {
    return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
}

// Find modular multiplicative inverse of determinant
int detModInverse(int det, int mod) {
    det = det % mod;
    if (det < 0) det += mod;
    
    for (int i = 1; i < mod; i++) {
        if ((det * i) % mod == 1) {
            return i;
        }
    }
    return -1;
}

// Hill Cipher
void hill(char *text, bool decrypt) {
    int key[SIZE][SIZE];
    
    printf("Enter 2x2 Hill matrix key row-wise:\n");
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            scanf("%d", &key[i][j]);
        }
    }
    getchar();
    
    if (decrypt) {
        // For decryption, we need to find the inverse of the key matrix
        int det = det2x2(key);
        int detInv = detModInverse(det, 26);
        
        if (detInv == -1) {
            printf("Error: The matrix is not invertible modulo 26.\n");
            return;
        }
        
        // Adjugate matrix (for 2x2)
        int adj[SIZE][SIZE] = {
            {key[1][1], -key[0][1]},
            {-key[1][0], key[0][0]}
        };
        
        // Inverse matrix = adjugate * detInv (mod 26)
        int inv[SIZE][SIZE];
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                inv[i][j] = (adj[i][j] * detInv) % 26;
                if (inv[i][j] < 0) inv[i][j] += 26;
            }
        }
        
        // Replace key with inverse
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                key[i][j] = inv[i][j];
            }
        }
    }
    
    int len = strlen(text);
    char cleanText[100];
    int cleanLen = 0;

    for (int i = 0; i < len; i++) {
        if (isalpha(text[i])) {
            cleanText[cleanLen++] = toupper(text[i]);
        }
    }

    // Pad if necessary
    if (cleanLen % 2 != 0) {
        cleanText[cleanLen++] = 'X';
    }
    cleanText[cleanLen] = '\0';

    char result[100];
    int resultIdx = 0;

    for (int i = 0; i < cleanLen; i += 2) {
        int a = cleanText[i] - 'A';
        int b = cleanText[i + 1] - 'A';

        int c1 = (key[0][0] * a + key[0][1] * b) % 26;
        int c2 = (key[1][0] * a + key[1][1] * b) % 26;
        
        // Handle negative modulo
        if (c1 < 0) c1 += 26;
        if (c2 < 0) c2 += 26;

        result[resultIdx++] = c1 + 'A';
        result[resultIdx++] = c2 + 'A';
    }
    
    result[resultIdx] = '\0';
    
    printf("%s text: %s\n", decrypt ? "Decrypted" : "Encrypted", result);
}

// Rail Fence Cipher
void railFence(char *text, int rails, bool decrypt) {
    int len = strlen(text);
    
    if (decrypt) {
        // Decryption
        char rail[rails][len];
        
        // Initialize the rail matrix
        for (int i = 0; i < rails; i++) {
            for (int j = 0; j < len; j++) {
                rail[i][j] = '\0';
            }
        }
        
        // Mark the rail pattern with special marker
        int row = 0;
        bool dirDown = true;
        for (int i = 0; i < len; i++) {
            rail[row][i] = '*';
            
            if (row == 0)
                dirDown = true;
            else if (row == rails - 1)
                dirDown = false;
                
            row = dirDown ? row + 1 : row - 1;
        }
        
        // Fill the rail matrix with ciphertext
        int index = 0;
        for (int i = 0; i < rails; i++) {
            for (int j = 0; j < len; j++) {
                if (rail[i][j] == '*' && index < len) {
                    rail[i][j] = text[index++];
                }
            }
        }
        
        // Read off the original text
        char result[len + 1];
        index = 0;
        row = 0;
        dirDown = true;
        
        for (int i = 0; i < len; i++) {
            result[i] = rail[row][i];
            
            if (row == 0)
                dirDown = true;
            else if (row == rails - 1)
                dirDown = false;
                
            row = dirDown ? row + 1 : row - 1;
        }
        
        result[len] = '\0';
        printf("Decrypted text: %s\n", result);
    } else {
        // Original encryption code
        char rail[rails][len];

        for (int i = 0; i < rails; i++)
            for (int j = 0; j < len; j++)
                rail[i][j] = '\0';

        int row = 0;
        bool dirDown = true; 

        for (int i = 0; i < len; i++) {
            rail[row][i] = text[i];
            
            if (row == 0)
                dirDown = true;
            else if (row == rails - 1)
                dirDown = false;
                
            row = dirDown ? row + 1 : row - 1;
        }

        printf("Encrypted text: ");
        for (int i = 0; i < rails; i++)
            for (int j = 0; j < len; j++)
                if (rail[i][j] != '\0')
                    printf("%c", rail[i][j]);

        printf("\n");
    }
}

// Route Cipher
void routeCipher(char *text, int rows, int cols, bool decrypt) {
    if (decrypt) {
        int len = strlen(text);
        char matrix[rows][cols];
        int index = 0;
        
        // Fill the matrix column by column (reverse of encryption)
        for (int j = 0; j < cols; j++) {
            for (int i = 0; i < rows; i++) {
                if (index < len) {
                    matrix[i][j] = text[index++];
                } else {
                    matrix[i][j] = 'X';
                }
            }
        }
        
        // Read the matrix row by row
        printf("Decrypted text: ");
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                printf("%c", matrix[i][j]);
            }
        }
        printf("\n");
    } else {
        // Original encryption code
        char matrix[rows][cols];
        int len = strlen(text);
        int index = 0;

        // Fill the matrix row by row
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (index < len)
                    matrix[i][j] = text[index++];
                else
                    matrix[i][j] = 'X'; 
            }
        }

        printf("Encrypted text: ");
        for (int j = 0; j < cols; j++) {
            for (int i = 0; i < rows; i++) {
                printf("%c", matrix[i][j]);
            }
        }
        printf("\n");
    }
}

void getKeyOrder(const char *key, int *order) {
    int len = strlen(key);
    
    for (int i = 0; i < len; i++) {
        order[i] = 1;
        for (int j = 0; j < len; j++) {
            if (key[j] < key[i] || (key[j] == key[i] && j < i))
                order[i]++;
        }
    }
}

// Myszkowski Transposition Cipher
void myszkowski(const char *text, const char *key, bool decrypt) {
    int keyLen = strlen(key);
    int textLen = strlen(text);
    int rows = (textLen + keyLen - 1) / keyLen;

    if (decrypt) {
        int order[keyLen];
        getKeyOrder(key, order);
        
        // Calculate how many characters go in each column
        int colSize[keyLen];
        for (int i = 0; i < keyLen; i++) {
            colSize[i] = rows;
        }
        
        // Create a matrix to hold the decrypted text
        char matrix[rows][keyLen];
        
        // Fill the matrix column by column according to key order
        int index = 0;
        for (int rank = 1; rank <= keyLen; rank++) {
            for (int col = 0; col < keyLen; col++) {
                if (order[col] == rank) {
                    for (int row = 0; row < rows && index < textLen; row++) {
                        matrix[row][col] = text[index++];
                    }
                }
            }
        }
        
        // Read the matrix row by row
        printf("Decrypted text: ");
        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < keyLen; col++) {
                if (row * keyLen + col < textLen) {
                    printf("%c", matrix[row][col]);
                }
            }
        }
        printf("\n");
    } else {
        // Original encryption code
        char matrix[rows][keyLen];
        int index = 0;
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < keyLen; j++)
                matrix[i][j] = (index < textLen) ? text[index++] : 'X';

        int order[keyLen];
        getKeyOrder(key, order);

        printf("Encrypted text: ");
        for (int rank = 1; rank <= keyLen; rank++) {
            for (int col = 0; col < keyLen; col++) {
                if (order[col] == rank) {
                    for (int row = 0; row < rows; row++) {
                        printf("%c", matrix[row][col]);
                    }
                }
            }
        }
        printf("\n");
    }
}

int main() {
    int choice, mode;
    char input[200];
    char key[100];
    bool special = false;

    printf("Enter the text: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0'; 
    
    printf("\nChoose mode:\n");
    printf("1. Encryption\n");
    printf("2. Decryption\n");
    printf("Enter choice: ");
    scanf("%d", &mode);
    
    bool decrypt = (mode == 2);

    printf("\nChoose a cipher method:\n");
    printf("1. Caesar Cipher\n");
    printf("2. Atbash Cipher\n");
    printf("3. Affine Cipher\n");
    printf("4. Vigenère Cipher\n");
    printf("5. Beaufort Cipher\n");
    printf("6. Gronsfeld Cipher\n");
    printf("7. Autokey Cipher\n");
    printf("8. Hill Cipher\n");
    printf("9. Rail Fence Cipher\n");
    printf("10. Route Cipher\n");
    printf("11. N-Gram Analysis\n");
    printf("12. Myszkowski Transposition\n");
    printf("13. Augusts");
    printf("Enter choice: ");
    scanf("%d", &choice);
    getchar(); 

    switch (choice) {
        case 1:
            if (!decrypt) {
                printf("Use default shift of 1? (0 = No, 1 = Yes): ");
                scanf("%d", (int*)&special);
                getchar(); // Consume newline
            }
            caesarCipher(input, special, decrypt);
            printf("%s Text: %s\n", decrypt ? "Decrypted" : "Encrypted", input);
            break;
        case 2:
            // Atbash is its own inverse, so encryption = decryption
            atbash(input);
            printf("%s Text: %s\n", decrypt ? "Decrypted" : "Encrypted", input);
            break;
        case 3:
            affine(input, decrypt);
            printf("%s Text: %s\n", decrypt ? "Decrypted" : "Encrypted", input);
            break;
        case 4:
            vigenere(input, false, decrypt);
            printf("%s Text: %s\n", decrypt ? "Decrypted" : "Encrypted", input);
            break;
        case 5:
            // Beaufort is its own inverse, so encryption = decryption
            vigenere(input, true, false);
            printf("%s Text: %s\n", decrypt ? "Decrypted" : "Encrypted", input);
            break;
        case 6:
            gronsfeld(input, decrypt);
            printf("%s Text: %s\n", decrypt ? "Decrypted" : "Encrypted", input);
            break;
        case 7:
            autokey(input, decrypt);
            printf("%s Text: %s\n", decrypt ? "Decrypted" : "Encrypted", input);
            break;
        case 8:
            hill(input, decrypt);
            break;
        case 9: {
            int rails;
            printf("Enter number of rails: ");
            scanf("%d", &rails);
            getchar();
            railFence(input, rails, decrypt);
            break;
        }
        case 10: {
            int rows, cols;
            printf("Enter number of rows and columns for route cipher: ");
            scanf("%d %d", &rows, &cols);
            getchar(); 
            routeCipher(input, rows, cols, decrypt);
            break;
        }
        case 11: {
            int n;
            printf("Enter the value of n for n-gram analysis: ");
            scanf("%d", &n);
            getchar(); 
            generateNGrams(input, n);
            break;
        }
        case 12: {
            printf("Enter the key: ");
            fgets(key, sizeof(key), stdin);
            key[strcspn(key, "\n")] = 0;
            myszkowski(input, key, decrypt);
            break; 
        }
        default:
            printf("Invalid choice.\n");
            break;
    }

    return 0;
}