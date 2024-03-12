#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Structure
typedef struct 
{
    char username[100];
    char password[100];
    int easyHighScore;
    int mediumHighScore;
    int hardHighScore;
    int allHighScore;
    int encryptionKey;
} Player;

typedef struct 
{
    bool loginStatus;
    char username[100];
} LoginCache;

// Fungtion Prototype
void hangmanDisplay(int mistakes);
bool checkLogin(Player *players, char playerUsername[], char playerPassword[], int playerCount);
void encryptPassword(char * password, int encryptionKey);
void gameDisplay(int score, int highScore, int mistakes, char * mistake, int wordLength, char * displayWord, int difficulty);
void getUser(Player *players, char playerUsername[], int playerCount);

// UserIndex
int userIndex = 0;

int main() {
    // Random
    srand(time(NULL));

    // File
    FILE *fptr;

    // Structure
    Player players[100];
    LoginCache loginCache;

    // Global
    int i, j;
    int playerCount = 0;

    // Login
    int loginOption;
    char playerUsername[100];
    char playerPassword[100];
    bool validLogin = false;
    bool usernameExist = false;
    int encryptionKey;

    // Game
    char easyWords[400][20];
    char mediumWords[400][20];
    char hardWords[400][20];

    char word[20];
    int wordIndex;
    int wordLength;
    char guess;
    char displayWord[20];
    bool duplicateWords = false;
    int easyUsedWords[100];
    int easyUsedWordsCount = 0;
    int mediumUsedWords[100];
    int mediumUsedWordsCount = 0;
    int hardUsedWords[100];
    int hardUsedWordsCount = 0;
    bool duplicateWord = false;
    int totalWords = 150;

    bool validGuess;
    int mistakes = 6;
    int corrects;
    int menuOption;
    int difficulty;
    int score = 0;
    int highScore = 0;
    char mistake[6];

    // Words Read
    fptr = fopen("EasyWord.vin", "rb");
	
	if (fptr == NULL) {
		printf("Error openning file.");
		return -1;
	}
	
	fread(easyWords, sizeof(easyWords[0]), sizeof(easyWords) / sizeof(easyWords[0]), fptr);
	fclose(fptr);
	
	for (i = 0; i < totalWords; i++) {
    	for (j = 0; j < strlen(easyWords[i]); j++) {
			easyWords[i][j] -= 90;
		}
	}
    fclose(fptr);

    fptr = fopen("MediumWord.vin", "rb");
	
	if (fptr == NULL) {
		printf("Error openning file.");
		return -1;
	}
	
	fread(mediumWords, sizeof(mediumWords[0]), sizeof(mediumWords) / sizeof(mediumWords[0]), fptr);
	fclose(fptr);
	
	for (i = 0; i < totalWords; i++) {
    	for (j = 0; j < strlen(mediumWords[i]); j++) {
			mediumWords[i][j] -= 90;
		}
	}
    fclose(fptr);

    fptr = fopen("HardWord.vin", "rb");
	
	if (fptr == NULL) {
		printf("Error openning file.");
		return -1;
	}
	
	fread(hardWords, sizeof(hardWords[0]), sizeof(hardWords) / sizeof(hardWords[0]), fptr);
	fclose(fptr);
	
	for (i = 0; i < totalWords; i++) {
    	for (j = 0; j < strlen(hardWords[i]); j++) {
			hardWords[i][j] -= 90;
		}
	}
    fclose(fptr);

    // Login Read
    fptr = fopen("UserData.vin", "rb");
    
    if (fptr == NULL) {
        fclose(fptr);
		printf("There is no file detected. Creating new file.");
        getchar();
        fptr = fopen("UserData.vin", "wb+");
	}	

    while (!feof(fptr)) {
        fread(&players[playerCount], sizeof(players[0]), 1, fptr);
        
        if (!feof(fptr)) {
            playerCount++;
        }
    }
    
    fclose(fptr);

    // Auto Login
    fptr = fopen("LoginCache.vin", "ab+");
	
	if (fptr == NULL) {
		printf("Error openning file.");
		return -1;
	}

    fread(&loginCache, sizeof(loginCache), 1, fptr);
    fclose(fptr);

    do
    {
        if (loginCache.loginStatus == true) {
            getUser(players, loginCache.username, playerCount);
            validLogin = true;
        }

        while (validLogin == false)
        {
            menuOption = 0;

            hangmanDisplay(mistakes);
            printf("\nWelcome To Hangman\n\n");
            printf("1. Login\n");
            printf("2. Register\n");
            printf("3. Exit Game\n\n");

            printf("Choose an option: ");
            scanf("%d", &loginOption);
            while (getchar() != '\n');

            if (loginOption == 1) {
                do
                {
                    hangmanDisplay(mistakes);

                    printf("\n       L O G I N\n");
                    printf("-----------------------\n\n");

                    printf("Username: ");
                    scanf("%s", playerUsername);
                    while (getchar() != '\n');

                    printf("Password: ");
                    scanf("%s", playerPassword);
                    while (getchar() != '\n');

                    validLogin = checkLogin(players, playerUsername, playerPassword, playerCount);

                    if (validLogin == false) {
                        printf("\nLogin is incorrect! please try again!");
                        getchar();
                        break;
                    }

                    strcpy(loginCache.username, playerUsername);
                    loginCache.loginStatus = true;

                    fptr = fopen("LoginCache.vin", "wb");
                    fwrite(&loginCache, sizeof(loginCache), 1, fptr);
                    
                    fclose(fptr);

                }
                while (validLogin == false);
            }
            else if (loginOption == 2) {
                do 
                {
                    hangmanDisplay(mistakes);

                    printf("\n    R E G I S T E R\n");
                    printf("-----------------------\n\n");

                    printf("Username: ");
                    scanf("%s", playerUsername);
                    while (getchar() != '\n');

                    usernameExist = false;

                    for (i = 0; i < playerCount; i++) {
                        if (strcmp(players[i].username, playerUsername) == 0) {
                            printf("\nUsername has been used! Please try another one!");
                            getchar();
                            usernameExist = true;
                        }
                    }
                } 
                while (usernameExist == true);

                printf("Password: ");
                scanf("%s", playerPassword);
                while (getchar() != '\n');

                encryptionKey = rand() % 88 + 3;
                encryptPassword(playerPassword, encryptionKey);

                strcpy(players[playerCount].username, playerUsername);
                strcpy(players[playerCount].password, playerPassword);
                players[playerCount].easyHighScore = 0;
                players[playerCount].mediumHighScore = 0;
                players[playerCount].hardHighScore = 0;
                players[playerCount].allHighScore = 0;
                players[playerCount].encryptionKey = encryptionKey;

                fptr = fopen("UserData.vin", "ab");
                fwrite(&players[playerCount], sizeof(players[0]), 1, fptr);
                
                fclose(fptr);

                strcpy(loginCache.username, playerUsername);
                loginCache.loginStatus = true;

                fptr = fopen("LoginCache.vin", "wb");
                fwrite(&loginCache, sizeof(loginCache), 1, fptr);
                
                fclose(fptr);

                playerCount++;

                validLogin = true;
                userIndex = playerCount - 1;

                printf("User registered! Press 'Enter' to return to main menu");
                getchar();
            }
            else if (loginOption == 3) {
                break;
            }
        } 

        while (validLogin == true && menuOption != 4 && menuOption != 5)
        {
            hangmanDisplay(mistakes);
            printf("\nH A N G M A N\n");
            printf("Welcome, %s\n\n", players[userIndex].username);
            printf("1. Play!\n");
            printf("2. Stats\n");
            printf("3. Instruction\n");
            printf("4. Log Out\n");
            printf("5. Exit\n");
            
            printf("\nChoose an option: "); 
            scanf("%d", &menuOption);
            while (getchar() != '\n');

            if (menuOption == 1) {
                hangmanDisplay(mistakes);
                printf("\nH A N G M A N\n\n");
                printf("1. Easy\n");
                printf("2. Medium\n");
                printf("3. Hard\n");
                printf("4. All\n");

                printf("\nChoose a difficulty: ");
                scanf("%d", &difficulty);
                while (getchar() != '\n');

                score = 0;
                if (difficulty == 1) {
                    highScore = players[userIndex].easyHighScore;
                }
                else if (difficulty == 2) {
                    highScore = players[userIndex].mediumHighScore;
                }
                else if (difficulty == 3) {
                    highScore = players[userIndex].hardHighScore;
                }
                else {
                    highScore = players[userIndex].allHighScore;
                }

                do
                {
                    do
                    {
                        duplicateWord = false;
                        wordIndex = rand() % totalWords;
                        if (difficulty == 1) {
                            for (i = 0; i < easyUsedWordsCount; i++) {
                                if (easyUsedWords[i] == wordIndex) {
                                    duplicateWord = true;
                                    break;
                                }
                            }

                            if (duplicateWord == false) {
                                strcpy(word, easyWords[wordIndex]);
                                easyUsedWords[easyUsedWordsCount] = wordIndex;
                                easyUsedWordsCount++;

                                if (easyUsedWordsCount == totalWords) {
                                    easyUsedWordsCount = 0;
                                }
                            }
                        }
                        else if (difficulty == 2) {
                            for (i = 0; i < mediumUsedWordsCount; i++) {
                                if (mediumUsedWords[i] == wordIndex) {
                                    duplicateWord = true;
                                    break;
                                }
                            }

                            if (duplicateWord == false) {
                                strcpy(word, mediumWords[wordIndex]);
                                mediumUsedWords[mediumUsedWordsCount] = wordIndex;
                                mediumUsedWordsCount++;

                                if (mediumUsedWordsCount == totalWords) {
                                    mediumUsedWordsCount = 0;
                                }
                            }
                        }
                        else if (difficulty == 3) {
                           for (i = 0; i < hardUsedWordsCount; i++) {
                                if (hardUsedWords[i] == wordIndex) {
                                    duplicateWord = true;
                                    break;
                                }
                            }

                            if (duplicateWord == false) {
                                strcpy(word, hardWords[wordIndex]);
                                hardUsedWords[hardUsedWordsCount] = wordIndex;
                                hardUsedWordsCount++;

                                if (hardUsedWordsCount == totalWords) {
                                    hardUsedWordsCount = 0;
                                }
                            }
                        }
                        else {
                            if (score % 9 <= 4) {
                                for (i = 0; i < easyUsedWordsCount; i++) {
                                    if (easyUsedWords[i] == wordIndex) {
                                        duplicateWord = true;
                                        break;
                                    }
                                }

                                if (duplicateWord == false) {
                                    strcpy(word, easyWords[wordIndex]);
                                    easyUsedWords[easyUsedWordsCount] = wordIndex;
                                    easyUsedWordsCount++;

                                    if (easyUsedWordsCount == totalWords) {
                                        easyUsedWordsCount = 0;
                                    }
                                }
                            }
                            else if (score % 9 == 8) {
                                for (i = 0; i < mediumUsedWordsCount; i++) {
                                    if (mediumUsedWords[i] == wordIndex) {
                                        duplicateWord = true;
                                        break;
                                    }
                                }

                                if (duplicateWord == false) {
                                    strcpy(word, mediumWords[wordIndex]);
                                    mediumUsedWords[mediumUsedWordsCount] = wordIndex;
                                    mediumUsedWordsCount++;

                                    if (mediumUsedWordsCount == totalWords) {
                                        mediumUsedWordsCount = 0;
                                    }
                                }
                            }
                            else {
                                for (i = 0; i < hardUsedWordsCount; i++) {
                                    if (hardUsedWords[i] == wordIndex) {
                                        duplicateWord = true;
                                        break;
                                    }
                                }

                                if (duplicateWord == false) {
                                    strcpy(word, hardWords[wordIndex]);
                                    hardUsedWords[hardUsedWordsCount] = wordIndex;
                                    hardUsedWordsCount++;

                                    if (hardUsedWordsCount == totalWords) {
                                        hardUsedWordsCount = 0;
                                    }
                                }
                            }
                        }
                    }
                    while (duplicateWord == true);

                    wordLength = strlen(word);
                    validGuess = true;
                    mistakes = 0;
                    corrects = 0;

                    for (i = 0; i < wordLength; i++) {
                        displayWord[i] = '_';
                    }

                    do 
                    {
                        gameDisplay(score, highScore, mistakes, mistake, wordLength, displayWord, difficulty);

                        // printf("\n%s", word);

                        printf("\n\nInput Character: ");
                        scanf("%c", &guess);
                        while (getchar() != '\n');

                        guess = toupper(guess);
                        
                        if (!isalpha(guess)) {
                            printf("Incorrect Input!");
                            getchar();
                            continue;
                        }
                        
                        validGuess = true;
                        for (i = 0; i < mistakes; i++) {
                            if (guess == mistake[i])
                                validGuess = false;
                        }

                        for (i = 0; i < wordLength; i++) {
                            if (guess == displayWord[i])
                                validGuess = false;
                        }

                        if (validGuess == false) {
                            printf("You've already input this character!");
                            getchar();
                            continue;
                        }

                        validGuess = false;
                        for (i = 0; i < wordLength; i++) {
                            if (guess == word[i]) {
                                validGuess = true;
                                corrects++;
                                displayWord[i] = guess;
                            }
                        }

                        if (validGuess == false) {
                            mistake[mistakes] = guess;
                            mistakes++;
                        }
                    }
                    while (mistakes < 6 && corrects != wordLength);

                    gameDisplay(score, highScore, mistakes, mistake, wordLength, displayWord, difficulty);

                    if (corrects == wordLength) {
                        printf("\n\nC O R R E C T !");
                        score++;
                    }
                    else {
                        printf("\nAnswer = %s\n\n", word);
                        printf("G A M E  O V E R\n");
                        printf("=============================\n");
                        printf("Final Score : %d\n", score);
                        printf("Previous Best Score : %d\n", highScore);

                        if (score > highScore) {
                            printf("\nCONGRATULATIONS!!\n");
                            printf("NEW PERSONAL RECORD");

                            if (difficulty == 1) {
                                players[userIndex].easyHighScore = score;
                            }
                            else if (difficulty == 2) {
                                players[userIndex].mediumHighScore = score;
                            }
                            else if (difficulty == 3) {
                                players[userIndex].hardHighScore = score;
                            }
                            else {
                                players[userIndex].allHighScore = score;
                            }
                            
                            fptr = fopen("UserData.vin", "wb");

                            fwrite(players, sizeof(players[0]), playerCount, fptr);

                            fclose(fptr);
                        }
                    }
                    
                    printf("\nPress 'Enter' to continue");
                    getchar();
                } 
                while (corrects == wordLength);
            }
            else if (menuOption == 2) {
                hangmanDisplay(mistakes);
                printf("\nBest Score (easy)   : %d\n", players[userIndex].easyHighScore);
                printf("Best Score (medium) : %d\n", players[userIndex].mediumHighScore);
                printf("Best Score (hard)   : %d\n", players[userIndex].hardHighScore);
                printf("Best Score (all)    : %d\n", players[userIndex].allHighScore);
                
                printf("\nPress 'Enter' to exit.");
                getchar();
            }
            else if (menuOption == 3) {
                hangmanDisplay(mistakes);
                printf("\nHangman Instructions\n");
                printf("1. Choose Menu Option (input number 1/2/3/4/5 to proceed)\n");
                printf("     a) Play Option\n");
                printf("        > Choose a difficulty\n");
                printf("           - Easy, the program will generate easy words\n");
                printf("           - Medium, the program will generate medium words\n");
                printf("           - Hard, the program will generate hard words\n");
                printf("           - All, the program will generate 5 easy words, 3 medium words, and 1 hard words repeatedly\n");
                printf("         // Mistake tolerance is set to 6 for every difficulty\n");
                printf("     b) Stats Option\n");
                printf("           You can see your best score in every difficulty\n");
                printf("     c) Instruction Option\n");
                printf("           Shows game instruction\n");
                printf("     d) Log Out Option\n");
                printf("           You log out from your account and go back to login menu\n");
                printf("     e) Exit Option\n");
                printf("              Exit the program\n");

                printf("\nPress 'Enter' to exit.");
                getchar();
            }
            else if (menuOption == 4) {
                strcpy(loginCache.username, "");
                loginCache.loginStatus = false;

                fptr = fopen("LoginCache.vin", "wb");
                fwrite(&loginCache, sizeof(loginCache), 1, fptr);
                
                fclose(fptr);

                validLogin = false;
            }
        }

    } 
    while (loginOption != 3 && menuOption != 5);

    printf("Thank you for playing!\n");
    printf("Made by Elvin");
    getchar();

    return 0;
}

void hangmanDisplay(int mistakes) {
    if (mistakes == 6) {
        system("cls");
        printf("      ______________\n");
        printf("      |            |\n");
        printf("      /\\           |\n");
        printf("      \\/           |\n");
        printf("     /||\\          |\n");
        printf("    / || \\         |\n");
        printf("      /\\           |\n");
        printf("     /  \\          |\n");
        printf("                   |\n");
        printf("  ====================\n");
    }
    else if (mistakes == 0) {
        system("cls");
        printf("      ______________\n");
        printf("      |            |\n");
        printf("                   |\n");
        printf("                   |\n");
        printf("                   |\n");
        printf("                   |\n");
        printf("                   |\n");
        printf("                   |\n");
        printf("                   |\n");
        printf("  ====================\n");
    }
    else if (mistakes == 1) {
        system("cls");
        printf("      ______________\n");
        printf("      |            |\n");
        printf("      /\\           |\n");
        printf("      \\/           |\n");
        printf("                   |\n");
        printf("                   |\n");
        printf("                   |\n");
        printf("                   |\n");
        printf("                   |\n");
        printf("  ====================\n");
    }
    else if (mistakes == 2) {
        system("cls");
        printf("      ______________\n");
        printf("      |            |\n");
        printf("      /\\           |\n");
        printf("      \\/           |\n");
        printf("      ||           |\n");
        printf("      ||           |\n");
        printf("                   |\n");
        printf("                   |\n");
        printf("                   |\n");
        printf("  ====================\n");
    }
    else if (mistakes == 3) {
        system("cls");
        printf("      ______________\n");
        printf("      |            |\n");
        printf("      /\\           |\n");
        printf("      \\/           |\n");
        printf("     /||           |\n");
        printf("    / ||           |\n");
        printf("                   |\n");
        printf("                   |\n");
        printf("                   |\n");
        printf("  ====================\n");
    }
    else if (mistakes == 4) {
        system("cls");
        printf("      ______________\n");
        printf("      |            |\n");
        printf("      /\\           |\n");
        printf("      \\/           |\n");
        printf("     /||\\          |\n");
        printf("    / || \\         |\n");
        printf("                   |\n");
        printf("                   |\n");
        printf("                   |\n");
        printf("  ====================\n");
    }
    else if (mistakes == 5) {
        system("cls");
        printf("      ______________\n");
        printf("      |            |\n");
        printf("      /\\           |\n");
        printf("      \\/           |\n");
        printf("     /||\\          |\n");
        printf("    / || \\         |\n");
        printf("      /            |\n");
        printf("     /             |\n");
        printf("                   |\n");
        printf("  ====================\n");
    }
}

bool checkLogin(Player *players, char playerUsername[], char playerPassword[], int playerCount) {
    int i;

    for (i = 0; i < playerCount; i++) {
        if (strcmp(players[i].username, playerUsername) == 0) {

            encryptPassword(playerPassword, players[i].encryptionKey);

            if (strcmp(players[i].password, playerPassword) == 0) {
                userIndex = i;
                return true;
            }
        }
    }

    return false;
}

void encryptPassword(char * password, int encryptionKey) {
    int i;

    for (i = 0; i < strlen(password); i++) {
        password[i] += encryptionKey;
    }

}

void getUser(Player *players, char playerUsername[], int playerCount) {
    int i;

    for (i = 0; i < playerCount; i++) {
        if (strcmp(players[i].username, playerUsername) == 0) {
            userIndex = i;      
        }
    }
}

void gameDisplay(int score, int highScore, int mistakes, char * mistake, int wordLength, char * displayWord, int difficulty) {
    int i;
    char difficultyName[10];

    if (difficulty == 1) {
        strcpy(difficultyName, "Easy");
    }
    else if (difficulty == 2) {
        strcpy(difficultyName, "Medium");
    }
    else if (difficulty == 3) {
        strcpy(difficultyName, "Hard");
    }
    else {
        strcpy(difficultyName, "All");
    }

    hangmanDisplay(mistakes);
    printf("\nScore      : %7d   | Best Score : %5d\n", score, highScore);
    printf("Difficulty : %7s   | Mistake(s) : ", difficultyName);

    for (i = 0; i < mistakes; i++) {
        printf("%c ", mistake[i]);
    }

    puts("\n");

    for (i = 0; i < wordLength; i++) {
        printf("%c ", displayWord[i]);
    }
}
