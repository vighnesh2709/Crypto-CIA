#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define SIZE 2
#define MAX_GRAM_LEN 10
#define MAX_NGRAMS 100


void caesarCipher(char *Input, bool august){
    int val;
    if (august){
        val = 1;
    } else {
        printf("Enter the shift: ");
        scanf("%d",&val);
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

void affine(char *text) {
    int a;
    int b;
    
    do {
        printf("Enter a (must be coprime with 26): ");
        scanf("%d",&a);
        if (gcd(a, 26) != 1) {
            printf("Error: 'a' must be coprime with 26. Try again.\n");
        }
    } while (gcd(a, 26) != 1);
    
    printf("Enter b: ");
    scanf("%d",&b);
    
    for (int i = 0; text[i] != '\0'; i++) {
        char ch = text[i];

        if (isalpha(ch)) {
            char base = isupper(ch) ? 'A' : 'a';
            int x = ch - base;
            int result = (a * x + b) % 26;
            if (result < 0) result += 26;  
            text[i] = result + base;
        }
    }
}


void vigenere(char *text, bool beaufort) {
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
                
                text[i] = ((keyVal - textVal + 26) % 26) + base;
            } else {
                
                text[i] = ((textVal + keyVal) % 26) + base;
            }
            
            keyIndex++;
        }
    }
}


void gronsfeld(char *text) {
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
            
            
            text[i] = ((textVal + keyVal) % 26) + base;
            
            keyIndex++;
        }
    }
}


void autokeyEncrypt(char *text) {
    char key[100];
    char plaintext[1000]; 
    
  
    printf("Enter the initial Key (a word): ");
    getchar();  
    fgets(key, sizeof(key), stdin);
    key[strcspn(key, "\n")] = '\0';  
    
    strcpy(plaintext, text);  
    
    int keyLen = strlen(key);
    int textLen = strlen(text);
    int keyIndex = 0;
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
            ngrams[total].gram[n] = '\0'; =
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

void hillEncrypt(char *plaintext, int key[SIZE][SIZE]) {
    int len = strlen(plaintext);
    char cleanText[100];
    int cleanLen = 0;

    for (int i = 0; i < len; i++) {
        if (isalpha(plaintext[i])) {
            cleanText[cleanLen++] = toupper(plaintext[i]);
        }
    }

    
    if (cleanLen % 2 != 0) {
        cleanText[cleanLen++] = 'X';
    }
    cleanText[cleanLen] = '\0';

    printf("Encrypted text: ");

    for (int i = 0; i < cleanLen; i += 2) {
        int a = cleanText[i] - 'A';
        int b = cleanText[i + 1] - 'A';

        int c1 = (key[0][0] * a + key[0][1] * b) % 26;
        int c2 = (key[1][0] * a + key[1][1] * b) % 26;
        
        // Handle negative modulo
        if (c1 < 0) c1 += 26;
        if (c2 < 0) c2 += 26;

        printf("%c%c", c1 + 'A', c2 + 'A');
    }

    printf("\n");
}

void railFenceEncrypt(char *text, int rails) {
    int len = strlen(text);
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

void routeCipherEncrypt(char *text, int rows, int cols) {
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

void myszkowskiEncrypt(const char *text, const char *key) {
    int keyLen = strlen(key);
    int textLen = strlen(text);
    int rows = (textLen + keyLen - 1) / keyLen;

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

int main() {
    int choice;
    char input[200];
    char key[100];
    bool special = false;

    printf("Enter the plaintext: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0'; 

    printf("\nChoose an encryption method:\n");
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
    printf("Enter choice: ");
    scanf("%d", &choice);
    getchar(); 

    switch (choice) {
        case 1:
            printf("Use default shift of 1? (0 = No, 1 = Yes): ");
            scanf("%d", (int*)&special);
            getchar(); // Consume newline
            caesarCipher(input, special);
            printf("Encrypted Text: %s\n", input);
            break;
        case 2:
            atbash(input);
            printf("Encrypted Text: %s\n", input);
            break;
        case 3:
            affine(input);
            printf("Encrypted Text: %s\n", input);
            break;
        case 4:
            vigenere(input, false);
            printf("Encrypted Text: %s\n", input);
            break;
        case 5:
            vigenere(input, true);
            printf("Encrypted Text: %s\n", input);
            break;
        case 6:
            gronsfeld(input);
            printf("Encrypted Text: %s\n", input);
            break;
        case 7:
            autokeyEncrypt(input);
            printf("Encrypted Text: %s\n", input);
            break;
        case 8: {
            int key[SIZE][SIZE];
            printf("Enter 2x2 Hill matrix key row-wise:\n");
            for (int i = 0; i < SIZE; i++) {
                for (int j = 0; j < SIZE; j++) {
                    scanf("%d", &key[i][j]);
                }
            }
            getchar(); 
            hillEncrypt(input, key);
            break;
        }
        case 9: {
            int rails;
            printf("Enter number of rails: ");
            scanf("%d", &rails);
            getchar();
            railFenceEncrypt(input, rails);
            break;
        }
        case 10: {
            int rows, cols;
            printf("Enter number of rows and columns for route cipher: ");
            scanf("%d %d", &rows, &cols);
            getchar(); 
            routeCipherEncrypt(input, rows, cols);
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
            myszkowskiEncrypt(input, key);
            break; 
        }
        default:
            printf("Invalid choice.\n");
            break;
    }

    return 0;
}