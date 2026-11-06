#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include <dirent.h>
#include "ANSI-color-codes.h"

#define MAX_LEN 1000
#define MAX_FILES 10

char *datei_liste[MAX_FILES];

char textFromFile[20][120];

typedef struct
{
    char word[50];
    char word2[50];
} card;

card deck[20];

int indexTextFromFile;

//Counts the number of characters without counting umlauts as two characters
int countchar(char vocab[])
{
    int count = 0;
    int x = 0;

    while (vocab[x] != '\0')
    {
        if ((unsigned char)vocab[x] < 128 || (unsigned char)vocab[x] > 191)
        {
            count++;
        }
        x++;
    }

    return count;
}
//Calculation of the flashcard dimensions
void BoxLinieOben(int breite)
{
    printf("┏");

    for (int i = 0; i < breite + 2; i++)
    {
        printf("━");
    }

    printf("┓\n");
}


void BoxLinieUnten(int breite)
{
    printf("┗");

    for (int i = 0; i < breite + 2; i++)
    {
        printf("━");
    }

    printf("┛\n");

}

int testMode()
{
    system("cls");
    char voc[50];
    int i = 1;

    int wrong = 0;

    do{
        system("cls");
        int breite = 20;

        // Determine the length of the first word
        int laenge = countchar(deck[i].word);
        if (laenge > breite)
        {
            breite = laenge + 4;
        }

        // Calculate the spaces on the left and right
        int links = (breite - laenge) / 2;
        int rechts = breite - laenge - links;


        BoxLinieOben(breite);
        printf("┃ " REDB "%*s" reset " ┃\n", breite, "");
        printf("┃ " REDB BWHT "%*s%s%*s" reset " ┃\n", links, "", deck[i].word, rechts, "");
        printf("┃ " REDB "%*s" reset " ┃\n", breite, "");
        BoxLinieUnten(breite); 

        printf("\n\n Was bedeutet das auf Englisch? ");
        scanf("%49s", voc);
           
       // Check the answer
        if (strcmp(voc, deck[i].word2) == 0)
        {
            printf(BGRN "✓\n\n" reset);

            Sleep(1000);
        }
        else{
            printf(BRED "✗\n" reset);

            wrong++;
            
            Sleep(1000);

        }
        
        i++;
    }while(i < indexTextFromFile);

    //Calculate and display the score and error rate
    float total = indexTextFromFile - 1;

    float fehlerQuote = (wrong / total) * 100;

    float punkte = total - wrong;

    printf("Du hast %.0f von %.0f Punkten!\nDeine Fehlerquote ist: %.2f%%", punkte, total, fehlerQuote);
}

int trainingMode()
{
    system("cls");
    char voc[50];
    int i = 1;


    do{
        system("cls");
        int breite = 20;

    // Determine the length of the first word
        int laenge = countchar(deck[i].word);
        if (laenge > breite)
        {
            breite = laenge + 4;
        }

        // Calculate the spaces on the left and right
        int links = (breite - laenge) / 2;
        int rechts = breite - laenge - links;


        BoxLinieOben(breite);
        printf("┃ " REDB "%*s" reset " ┃\n", breite, "");
        printf("┃ " REDB BWHT "%*s%s%*s" reset " ┃\n", links, "", deck[i].word, rechts, "");
        printf("┃ " REDB "%*s" reset " ┃\n", breite, "");
        BoxLinieUnten(breite); 

        printf("\n\n Was bedeutet das auf Englisch? ");
        scanf("%49s", voc);
           
            // Check the answer
        if (strcmp(voc, deck[i].word2) == 0)
        {
            printf(BGRN "✓\n\n" reset);

        Sleep(1000);
            
           
        }
        else{
            printf(BRED "✗\n" reset); 

            // Determine the length of the answer
            laenge = countchar(deck[i].word2);

            // Recalculate the spaces for the answer
            links = (breite - laenge) / 2;
            rechts = breite - laenge - links;


            // Display the correct answer centered
            BoxLinieOben(breite);

            // Empty line
            printf("┃ " WHTB "%*s" reset " ┃\n", breite, "");

            // Word centered
            printf("┃ " WHTB BRED "%*s%s%*s" reset " ┃\n", links, "", deck[i].word2, rechts, "");

            // Empty line
            printf("┃ " WHTB "%*s" reset " ┃\n", breite, "");

            BoxLinieUnten(breite); 
            
            Sleep(3500);
        }
        
        i++;
    }while(i < indexTextFromFile);
}

int createDeck()
{
    //Insert the vocabulary for the front and back of the flashcard into the struct
    for(int i = 1; i < indexTextFromFile; i++)
    {
        sscanf(textFromFile[i], "%50[^,], %s", deck[i].word, deck[i].word2);

        printf("[%s] [%s]\n", deck[i].word, deck[i].word2);
    }

    return 0;
}

int GetTextFromCsvFile(int ch11)
{   

    FILE *file = fopen(datei_liste[ch11], "r");

    size_t len = strlen(strcpy(textFromFile[0], datei_liste[ch11]));

    //Save the file name without .csv
    textFromFile[0][len-4] = '\0';
    
    indexTextFromFile = 1;

    printf("%s\n", textFromFile[0]);
    
    //Save the file name without .csv
    do {
        fgets(textFromFile[indexTextFromFile], 120, file);

        indexTextFromFile++;

        if (ferror(file))
        {
            printf("Error reading file.\n");
            return 1;
        }
        
    } while (!feof(file));

    printf("\n");
   

    fclose(file);

    return 0;
}

int getAllCsvFiles()
{   
    system("cls");

    DIR *directory;

    struct dirent *entry;

    directory = opendir(".");

    int ch9 = 1;

    if (directory == NULL)
    {
        printf("Error opening directory.\n");

        return 1;
    }

    //Display all .csv file names in the current directory and store them in an array
    while ((entry = readdir(directory)) != NULL)
    {
        const size_t len = strlen(entry->d_name);
        
        if (len > 4                       &&
            entry->d_name[len - 4] == '.' &&
            entry->d_name[len - 3] == 'c' &&
            entry->d_name[len - 2] == 's' &&
            entry->d_name[len - 1] == 'v')
        {   
            datei_liste[ch9] = strdup(entry->d_name);

            printf("%d. %s\n", ch9, entry->d_name);

            ch9 ++;
        }
    }
    if (closedir(directory) == -1)
        {
            printf("Error closing directory.\n");
            return 1;
        }
    return 0;
}

void decksanzeigen()
{   
    int ch2;

    do {
        system("cls");

        printf("Hier könnten Decks stehen\n\n0. zurück\n\nGib eine Nummer ein: ");

        int args3 = scanf("%d", &ch2);

        if (args3 < 1)
        {
            char dummy;
            scanf("%c", &dummy); 
        }
    } while(ch2 != 0);
}

void deckserstellen()
{
    int ch3;

    do {
        system("cls");

        getAllCsvFiles();



        printf("\n\n0. zurück\n\nGib eine Nummer ein: ");
    
        int args4 = scanf("%d", &ch3);

        if (args4 < 1)
        {
            char dummy;
            scanf("%c", &dummy);  
        }

        if (ch3 > 0)
        {
            system("cls");
            
            GetTextFromCsvFile(ch3);
            
            createDeck();

            printf("\n\n0. zurück\n\nGib eine Nummer ein: ");
    
            args4 = scanf("%d", &ch3);
        }

    } while(ch3 != 0);
}

void decksloeschen()
{
    int ch4;

    do {
        system("cls");

        printf("Hier könnte man Decks löschen\n\n0. zurück\n\nGib eine Nummer ein: ");

        int args5 = scanf("%d", &ch4);

        if (args5 < 1)
        {
            char dummy;
            scanf("%c", &dummy);  
        }
    } while(ch4 != 0);
}

void starttraining()
{   
    int ch6;

    do {
        system("cls");

        trainingMode();

        printf("\n\n0. zurück\n\nGib eine Nummer ein: ");

        int args7 = scanf("%d", &ch6);

        if (args7 < 1)
        {
            char dummy;
            scanf("%c", &dummy); 
        }
    } while(ch6 != 0);
}

void starttest()
{
    int ch8;

    do {
        system("cls");

        testMode();

        printf("\n\n0. zurück\n\nGib eine Nummer ein: ");

        int args9 = scanf("%d", &ch8);

        if (args9 < 1)
        {
            char dummy;
            scanf("%c", &dummy);  
        }                              
    } while(ch8 != 0);
}

void deckmenu()
{
    int ch1;

    do {
        system("cls");

        printf("Deck Menü\n\n1. Decks anzeigen\n2. Deck erstellen\n3. Deck löschen\n0. zurück\n\nGib eine Nummer ein: ");

        int args2 = scanf("%d", &ch1);

        if (args2 < 1)
        {
            char dummy;
            scanf("%c", &dummy);
        } else {    
            switch(ch1)
            {
                case 1:
                    decksanzeigen();
                    break;

                case 2:
                    deckserstellen();
                    break;

                case 3:
                    decksloeschen();
                    break;

                default:
                    break;
                }
        }
    } while(ch1 != 0);
}

void trainingmenu()
{
    int ch5;

    do {
        system("cls");

        printf("Trainingsmenü\n\n1.Training starten\n\n0. zurück\n\nGib eine Nummer ein: ");

        int args6 = scanf("%d", &ch5);

        if (args6 < 1)
        {
            char dummy;
            scanf("%c", &dummy);
        }
        switch(ch5)
        {
            case 1:
                starttraining();
                break;

            default:
                break;
        }
    } while(ch5 != 0);
}

void testmenu()
{
    int ch7;

    do{
        system("cls");

        printf("Prüfungsmenü\n\n1.Prüfung starten\n\n0. zurück\n\nGib eine Nummer ein: ");

        int args8 = scanf("%d", &ch7);

        if (args8 < 1)
        {
            char dummy;
            scanf("%c", &dummy);
        }
        switch(ch7)
            {
                case 1:
                    starttest();
                    break;

                default:
                    break;
            }
    } while(ch7 != 0);
}

void mainmenu()
{
    int ch;

    do {
        system("cls");

        printf("Karteikarten-Lernsoftware by Michi und Aylin\n\nMain Menü\n\n1. Decks\n2. Training\n3. Prüfung\n0. beenden\n\nGib eine Nummer ein: ");

        int args = scanf("%d", &ch);

        if (args < 1)
        {
            char dummy;
            scanf("%c", &dummy); 
        } else {
            switch(ch)
            {
                case 1:
                    deckmenu();
                    break;

                case 2:
                    trainingmenu();
                    break;
                
                case 3:
                    testmenu();
                    break;

                default:
                    break;
            } 
        }
    } while(ch != 0);
}

int main()
{
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001); 

    mainmenu();
    return 0;
}