#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define INITIAL_MAX_POPULATIONS 1000  // Initial maximum number of populations
#define NUM_SYMBOLS 24

// Function to generate a random color
char *generateRandomColor() {
    char *color = (char *)malloc(7 * sizeof(char));
    if (color == NULL) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }
    sprintf(color, "#%02X%02X%02X", rand() % 256, rand() % 256, rand() % 256);
    return color;
}

// Function to get a unique combination of symbol and hex code
void getSymbolAndHex(int index, char *symbol, char *hex) {
    // Use the provided set for symbolNr
    int symbols[NUM_SYMBOLS] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 21, 22, 23, 24, 25, 15, 17, 18, 19};

    // Ensure different combinations for each index
    *symbol = 'A' + symbols[index % NUM_SYMBOLS];
    sprintf(hex, "#%02X%02X%02X", rand() % 256, rand() % 256, rand() % 256);
}

int main() {
    int numPCs;
    char datasetName[256];
    char *command;

    // Get the number of principal components and dataset name
    printf("Enter the number of principal components to be calculated: ");
    scanf("%d", &numPCs);

    printf("Enter the name of the dataset (without extension): ");
    scanf("%255s", datasetName); // Limit input to 255 characters to avoid buffer overflow

    // Dynamically allocate memory for command based on the size of datasetName
    command = (char *)malloc((strlen(datasetName) + 256) * sizeof(char));
    
    if (command == NULL) {
        perror("Memory allocation error");
        return EXIT_FAILURE;
    }

    // Construct the command to run PLINK
    sprintf(command, "plink.exe --allow-no-sex --bfile %s --pca %d", datasetName, numPCs);

    // Run PLINK
    system(command);

    // Open the PLINK eigenvec file for reading
    char eigenvecFileName[256];
    sprintf(eigenvecFileName, "plink.eigenvec");
    FILE *eigenvecFile = fopen(eigenvecFileName, "r");
    if (eigenvecFile == NULL) {
        perror("Error opening eigenvec file");
        free(command);
        return EXIT_FAILURE;
    }

    // Open the PCA_Data.csv file for writing
    FILE *pcaDataFile = fopen("PCA_Data.csv", "w");
    if (pcaDataFile == NULL) {
        perror("Error opening PCA_Data.csv file");
        fclose(eigenvecFile);
        return EXIT_FAILURE;
    }

    // Write header to PCA_Data.csv
    fprintf(pcaDataFile, "Population,Individual");
    for (int i = 1; i <= numPCs; i++) {
        fprintf(pcaDataFile, ",PC%d", i);
    }
    fprintf(pcaDataFile, "\n");

    // Arrays to keep track of unique populations and their corresponding colors and symbols
    char (*populations)[50] = malloc(INITIAL_MAX_POPULATIONS * sizeof(char[50]));
    char (*colors)[8] = malloc(INITIAL_MAX_POPULATIONS * sizeof(char[8]));
    int *symbols = malloc(INITIAL_MAX_POPULATIONS * sizeof(int));
    if (populations == NULL || colors == NULL || symbols == NULL) {
        perror("Memory allocation error");
        return EXIT_FAILURE;
    }
    int maxPopulations = INITIAL_MAX_POPULATIONS;
    int numUniquePopulations = 0;

    // Read PLINK eigenvec file and write to PCA_Data.csv
    while (1) {
        char population[50];
        char individual[50];

        int result = fscanf(eigenvecFile, "%s %s", population, individual);
        if (result == EOF) {
            break;
        } else if (result != 2) {
            perror("Error reading from eigenvec file");
            fclose(eigenvecFile);
            fclose(pcaDataFile);
            free(populations);
            free(colors);
            free(symbols);
            return EXIT_FAILURE;
        }

        fprintf(pcaDataFile, "%s,%s", population, individual);
        for (int i = 0; i < numPCs; i++) {
            double pcValue;
            result = fscanf(eigenvecFile, "%lf", &pcValue);
            if (result != 1) {
                perror("Error reading from eigenvec file");
                fclose(eigenvecFile);
                fclose(pcaDataFile);
                free(populations);
                free(colors);
                free(symbols);
                return EXIT_FAILURE;
            }

            fprintf(pcaDataFile, ",%.15g", pcValue);
        }
        fprintf(pcaDataFile, "\n");

        // Check if the population is already in the array
        int populationIndex = -1;
        for (int i = 0; i < numUniquePopulations; i++) {
            if (strcmp(populations[i], population) == 0) {
                populationIndex = i;
                break;
            }
        }

        // If the population is not in the array, add it
        if (populationIndex == -1) {
            populationIndex = numUniquePopulations;

            // Check if we need to reallocate memory for arrays
            if (numUniquePopulations >= maxPopulations) {
                maxPopulations *= 2;  // Double the size
                populations = realloc(populations, maxPopulations * sizeof(char[50]));
                colors = realloc(colors, maxPopulations * sizeof(char[8]));
                symbols = realloc(symbols, maxPopulations * sizeof(int));
                if (populations == NULL || colors == NULL || symbols == NULL) {
                    perror("Memory reallocation error");
                    fclose(eigenvecFile);
                    fclose(pcaDataFile);
                    free(populations);
                    free(colors);
                    free(symbols);
                    return EXIT_FAILURE;
                }
            }

            strcpy(populations[numUniquePopulations], population);
            strcpy(colors[numUniquePopulations], generateRandomColor());

            // Assign a symbolNr from the specified set
            getSymbolAndHex(numUniquePopulations, &(colors[numUniquePopulations][1]), colors[numUniquePopulations]);
            symbols[numUniquePopulations] = numUniquePopulations % NUM_SYMBOLS;
            numUniquePopulations++;
        }
    }

    fclose(eigenvecFile);
    fclose(pcaDataFile);

    // Open the PCA_Aesthetics.csv file for writing
    FILE *pcaAestheticsFile = fopen("PCA_Aesthetics.csv", "w");
    if (pcaAestheticsFile == NULL) {
        perror("Error opening PCA_Aesthetics.csv file");
        free(populations);
        free(colors);
        free(symbols);
        return EXIT_FAILURE;
    }

    // Write header to PCA_Aesthetics.csv
    fprintf(pcaAestheticsFile, "Population,colorNr,symbolNr\n");

    // Write population information to PCA_Aesthetics.csv
    for (int i = 0; i < numUniquePopulations; i++) {
        fprintf(pcaAestheticsFile, "%s,%s,%d\n", populations[i], colors[i], symbols[i]);
    }

    fclose(pcaAestheticsFile);

    free(populations);
    free(colors);
    free(symbols);


    printf("PCA_Data.csv and PCA_Aesthetics.csv generated successfully.\n");

    return 0;
}