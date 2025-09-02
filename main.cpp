//DSS PROJECT
//MOVIE DATABASE SEARCH ENGINE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_MOVIES 100
#define NAME_LENGTH 50

// Movie structure definition
typedef struct {
    int id;
    char name[NAME_LENGTH];
    int year;
    float rating;
} Movie;

Movie database[MAX_MOVIES];
int movieCount = 0;
int sortedByYear = 0;  // Flag to track if sorted by year

// Function prototypes
void addMovie();
void displayMovies();
void linearSearchByName();
void binarySearchByYear();
void sortByRating();
void sortByYear();
void swapMovies(Movie *a, Movie *b);
int partition(int low, int high);
void quickSort(int low, int high);

int main() {
    int choice;
    
    printf("Movie Database Search Engine\n");
    printf("----------------------------\n");
    
    do {
        printf("\nMain Menu\n");
        printf("1. Add Movie\n");
        printf("2. Display All Movies\n");
        printf("3. Search by Name\n");
        printf("4. Search by Year\n");
        printf("5. Sort by Rating\n");
        printf("6. Sort by Year\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        // Clear input buffer
        while (getchar() != '\n');
        
        switch(choice) {
            case 1: addMovie(); break;
            case 2: displayMovies(); break;
            case 3: linearSearchByName(); break;
            case 4: binarySearchByYear(); break;
            case 5: sortByRating(); break;
            case 6: sortByYear(); break;
            case 7: printf("Exiting program...\n"); break;
            default: printf("Invalid choice! Please try again.\n");
        }
    } while(choice != 7);
    
    return 0;
}

void addMovie() {
    if (movieCount >= MAX_MOVIES) {
        printf("Database is full! Cannot add more movies.\n");
        return;
    }
    
    Movie newMovie;
    newMovie.id = movieCount + 1;
    
    printf("\nEnter movie name: ");
    fgets(newMovie.name, NAME_LENGTH, stdin);
    newMovie.name[strcspn(newMovie.name, "\n")] = '\0';  // Remove newline
    
    printf("Enter release year: ");
    scanf("%d", &newMovie.year);
    
    printf("Enter rating (0.0-10.0): ");
    scanf("%f", &newMovie.rating);
    
    // Clear input buffer
    while (getchar() != '\n');
    
    database[movieCount++] = newMovie;
    sortedByYear = 0;  // Reset sort flag
    printf("Movie added successfully! (ID: %d)\n", newMovie.id);
}

void displayMovies() {
    if (movieCount == 0) {
        printf("\nNo movies in the database!\n");
        return;
    }
    
    printf("\n%-5s %-30s %-10s %-10s\n", "ID", "Movie Name", "Year", "Rating");
    printf("----------------------------------------------------\n");
    for (int i = 0; i < movieCount; i++) {
        printf("%-5d %-30s %-10d %-10.1f\n", 
               database[i].id, 
               database[i].name, 
               database[i].year, 
               database[i].rating);
    }
}

void linearSearchByName() {
    if (movieCount == 0) {
        printf("\nNo movies in the database!\n");
        return;
    }
    
    char searchName[NAME_LENGTH];
    printf("\nEnter movie name to search: ");
    fgets(searchName, NAME_LENGTH, stdin);
    searchName[strcspn(searchName, "\n")] = '\0';  // Remove newline
    
    int found = 0;
    for (int i = 0; i < movieCount; i++) {
        if (strstr(database[i].name, searchName) != NULL) {
            if (!found) {
                printf("\nSearch Results:\n");
                printf("%-5s %-30s %-10s %-10s\n", "ID", "Movie Name", "Year", "Rating");
                printf("----------------------------------------------------\n");
                found = 1;
            }
            printf("%-5d %-30s %-10d %-10.1f\n", 
                   database[i].id, 
                   database[i].name, 
                   database[i].year, 
                   database[i].rating);
        }
    }
    
    if (!found) {
        printf("\nNo movies found with name: '%s'\n", searchName);
    }
}

void binarySearchByYear() {
    if (movieCount == 0) {
        printf("\nNo movies in the database!\n");
        return;
    }
    
    // Sort by year if not already sorted
    if (!sortedByYear) {
        sortByYear();
    }
    
    int searchYear;
    printf("\nEnter release year to search: ");
    scanf("%d", &searchYear);
    
    // Clear input buffer
    while (getchar() != '\n');
    
    int low = 0;
    int high = movieCount - 1;
    int found = -1;  // Index of first found movie
    
    while (low <= high) {
        int mid = low + (high - low) / 2;
        
        if (database[mid].year == searchYear) {
            found = mid;
            break;
        }
        else if (database[mid].year < searchYear) {
            low = mid + 1;
        }
        else {
            high = mid - 1;
        }
    }
    
    if (found == -1) {
        printf("\nNo movies found from year: %d\n", searchYear);
        return;
    }
    
    // Find first occurrence of the year
    int first = found;
    while (first > 0 && database[first - 1].year == searchYear) {
        first--;
    }
    
    // Find last occurrence of the year
    int last = found;
    while (last < movieCount - 1 && database[last + 1].year == searchYear) {
        last++;
    }
    
    printf("\nMovies from %d:\n", searchYear);
    printf("%-5s %-30s %-10s %-10s\n", "ID", "Movie Name", "Year", "Rating");
    printf("----------------------------------------------------\n");
    
    for (int i = first; i <= last; i++) {
        printf("%-5d %-30s %-10d %-10.1f\n", 
               database[i].id, 
               database[i].name, 
               database[i].year, 
               database[i].rating);
    }
}

void sortByRating() {
    if (movieCount == 0) {
        printf("\nNo movies in the database!\n");
        return;
    }
    
    // Selection Sort (Descending Order)
    for (int i = 0; i < movieCount - 1; i++) {
        int maxIndex = i;
        for (int j = i + 1; j < movieCount; j++) {
            if (database[j].rating > database[maxIndex].rating) {
                maxIndex = j;
            }
        }
        swapMovies(&database[i], &database[maxIndex]);
    }
    
    printf("\nMovies sorted by rating (highest first)!\n");
    displayMovies();
}

void sortByYear() {
    if (movieCount == 0) {
        printf("\nNo movies in the database!\n");
        return;
    }
    
    // Insertion Sort (Ascending Order)
    for (int i = 1; i < movieCount; i++) {
        Movie key = database[i];
        int j = i - 1;
        
        while (j >= 0 && database[j].year > key.year) {
            database[j + 1] = database[j];
            j--;
        }
        database[j + 1] = key;
    }
    
    sortedByYear = 1;
    printf("\nMovies sorted by year (oldest first)!\n");
    displayMovies();
}

// Helper function to swap two movies
void swapMovies(Movie *a, Movie *b) {
    Movie temp = *a;
    *a = *b;
    *b = temp;
}