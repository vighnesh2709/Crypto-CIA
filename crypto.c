#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include<string.h>
#define SIZE 2
#define MAX_GRAM_LEN 10
#define MAX_NGRAMS 100

void caesersCipher(char *Input, bool august){
    int val;
    if (august){
    	val = 1;
    }else {
    	printf("Enter the shift");
    	scanf("%d",&val);
    }
    
    for (int i = 0; Input[i] != '\0'; i++) {
        char ch = Input[i];
	char base;
        if (isalpha(ch)) {
        
            if(isupper(ch)){
            	base = 'A';
            }else{
            	base = 'a';
            }
    
            Input[i] = (ch - base +  val) % 26 + base;
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

void affine(char *text) {
    int a;
    int b;
    printf("Enter a: ");
    scanf("%d",&a);
    printf("Enter b: ");
    scanf("%d",&b);
    
    for (int i = 0; text[i] != '\0'; i++) {
        char ch = text[i];

        if (isalpha(ch)) {
            char base = isupper(ch) ? 'A' : 'a';
            int x = ch - base;
            text[i] = ((a * x + b) % 26) + base;
        }
    }
}

void vigenere(char *text, bool beaufort) {
    char key[100];
    printf("Enter the Key: ");
    fgets(key, sizeof(key), stdin);

    size_t keyLength = strlen(key);
    if (key[keyLength - 1] == '\n') {
        key[keyLength - 1] = '\0'; 
        keyLength--;
    }

    int textLength = strlen(text);
    int keyIndex = 0;

    for (int i = 0; i < textLength; i++) {
        char ch = text[i];

        if (isalpha(ch)) {
            char base = isupper(ch) ? 'A' : 'a';
            int shift = tolower(key[keyIndex % keyLength]) - 'a';

            if (beaufort) {
                // Beaufort uses: C = (K - P + 26) % 26
                text[i] = ((shift - (ch - base) + 26) % 26) + base;
            } else {
                // Vigenere uses: C = (P + K) % 26
                text[i] = ((ch - base + shift) % 26) + base;
            }

            keyIndex++;
        }
    }
}
void gronsfeld(char *text) {
    char key[100];


    printf("Enter the numeric key: ");
    fgets(key, sizeof(key), stdin);

    size_t len = strlen(key);
    if (len > 0 && key[len - 1] == '\n') {
        key[len - 1] = '\0';
    }

    int textLength = strlen(text);
    int keyLength = strlen(key);
    int keyIndex = 0;

    for (int i = 0; i < textLength; i++) {
        char ch = text[i];

        if (isalpha(ch)) {
            char base = isupper(ch) ? 'A' : 'a';
            int shift = key[keyIndex % keyLength] - '0';  
            text[i] = ((ch - base + shift) % 26) + base;

            keyIndex++;
        }
    }
}

void autokeyEncrypt(char *text) {
    char key[100];
    printf("Enter the initial Key (a word): ");
    fgets(key, sizeof(key), stdin);

    size_t keyLength = strlen(key);
    if (key[keyLength - 1] == '\n') {
        key[keyLength - 1] = '\0'; 
        keyLength--;
    }

    int textLength = strlen(text);
    char runningKey[200];

    // Build running key: initial key + text
    strcpy(runningKey, key);
    strncat(runningKey, text, textLength - keyLength);

    int keyIndex = 0;

    for (int i = 0; i < textLength; i++) {
        char ch = text[i];

        if (isalpha(ch)) {
            char base = isupper(ch) ? 'A' : 'a';
            int shift = tolower(runningKey[keyIndex]) - 'a';
            text[i] = ((ch - base + shift) % 26) + base;
            keyIndex++;
        }
    }
}

typedef struct {
    char gram[MAX_GRAM_LEN];
    int count;
} NGram;

void toLowerCase(char *str) {
    for (int i = 0; str[i]; i++)
        str[i] = tolower(str[i]);
}

int compareNGrams(char *a, char *b) {
    return strncmp(a, b, MAX_GRAM_LEN);
}

void generateNGrams(char *text, int n) {
    NGram ngrams[MAX_NGRAMS];
    int total = 0;

    int len = strlen(text);

    for (int i = 0; i <= len - n; i++) {
        char temp[MAX_GRAM_LEN] = {0};
        strncpy(temp, &text[i], n);

        
        int found = 0;
        for (int j = 0; j < total; j++) {
            if (compareNGrams(temp, ngrams[j].gram) == 0) {
                ngrams[j].count++;
                found = 1;
                break;
            }
        }

        
        if (!found && total < MAX_NGRAMS) {
            strncpy(ngrams[total].gram, temp, n);
            ngrams[total].count = 1;
            total++;
        }
    }

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

        printf("%c%c", c1 + 'A', c2 + 'A');
    }

    printf("\n");
}

void railFenceEncrypt(char *text, int rails) {
    int len = strlen(text);
    char rail[rails][len];

   
    for (int i = 0; i < rails; i++)
        for (int j = 0; j < len; j++)
            rail[i][j] = '\n';

    int row = 0, dirDown = 0;


    for (int i = 0; i < len; i++) {
        rail[row][i] = text[i];

        
        if (row == 0 || row == rails - 1)
            dirDown = !dirDown;

        row += dirDown ? 1 : -1;
    }

    
    printf("Encrypted text: ");
    for (int i = 0; i < rails; i++)
        for (int j = 0; j < len; j++)
            if (rail[i][j] != '\n')
                printf("%c", rail[i][j]);

    printf("\n");
}


void routeCipherEncrypt(char *text, int rows, int cols) {
    char matrix[rows][cols];
    int len = strlen(text);
    int index = 0;

    
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
            if (key[j] < key[i])
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
            getchar(); 
            caesersCipher(input, special);
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
        	fgets(key,sizeof(key),stdin);
        	key[strcspn(key,"\n")]=0;
        	myszkowskiEncrypt(input,key);
        }
        default:
            printf("Invalid choice.\n");
            break;
    }

    return 0;
}


