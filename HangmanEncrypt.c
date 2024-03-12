#include <stdio.h>
#include <string.h>

int main() {
	FILE *fptr;
    int i, j;
    char words[400][20];
    char words2[400][20];
    int encryptionKey = 90;

    for (i = 0; i < 150; i++) {
        scanf("%s", words[i]);
        while (getchar() != '\n');
    }
    
    for (i = 0; i < 150; i++) {
    	for (j = 0; j < strlen(words[i]); j++) {
            words[i][j] = toupper(words[i][j]);
			words[i][j] += encryptionKey;
		}
	}

	fptr = fopen("HardWord.vin", "wb");
    fwrite(words, sizeof(words[0]), sizeof(words) / sizeof(words[0]), fptr);
    fclose(fptr);
    
	fptr = fopen("HardWord.vin", "rb");
	
	if (fptr == NULL) {
		printf("Error openning file.");
		return -1;
	}
	
	fread(words2, sizeof(words2[0]), sizeof(words2) / sizeof(words2[0]), fptr);
	fclose(fptr);
	
	for (i = 0; i < 150; i++) {
    	for (j = 0; j < strlen(words2[i]); j++) {
			words2[i][j] -= encryptionKey;
		}
	}
	
	for (i = 0; i < 150; i++) {
		printf("\n%s", words2[i]);
	}

	fclose(fptr);

    return 0;
}
