/***
 *Name: Lee Anne Rachman
 *ID: 323917286
 *Assignment: ex5
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DONE_MAIN_MENU 4
#define DONE_PLAYLIST_MENU 6

typedef struct Song {
    char *title;
    char *artist;
    int year;
    char *lyrics;
    int streams;
    // Pointer to the next song in the list
    struct Song *next;
} Song;

typedef struct Playlist {
    char *name;
    Song *headSong;
    int songsNum;
    // Pointer to the next playlist in the list
    struct Playlist *next;
} Playlist;

//this function prints the menu of a single playlist and scans the action input from the user
void managePlaylistMenu(Playlist *playlist, Playlist *playlists, const int *currentNumOfPlaylists);

//this function receives a pointer to a playlist and prints the menu of the playlist
void printPlaylistMenu();

//this function prints the main menu
void printMainMenu();

//this function free's the space of a certain playlist
void freePlaylist(Playlist *playlistPointer);

//this function free's the space of a certain song
void freeSong(Song *song);

//this function plays a song
void playSong(const Playlist *playlist, int songIndex);

//this function adds a song to the playlist
void addSong(Playlist *playlist);

Song *createSong(char *title, char *artist, int yearOfRelease, char *lyrics);

//this function deletes a song
void deleteSong(Playlist *playlist);

//this function shows the playlists
void watchPlaylists(Playlist *playlists, const int *currentNumOfPlaylists);

Playlist *findPlaylist(Playlist *head, int userChoice);

void printListOfPlaylists(Playlist *playlists, const int *currentNumOfPlaylists);

//this function adds a playlist
void addPlaylist(Playlist **head, int *currentNumberOfPlaylists);

Playlist *createPlaylist(const char *playlistName);

char *allocateMemoryForField();

void removePlaylist(Playlist **head, int *currentNumberOfPlaylists);

void sortPlaylist();

void sortByYear(Playlist *playlist);

void sortByStreams(Playlist *playlist, int ascendingOrDescendingOrder);

void sortAlphabetically(Playlist *playlist);

void swap(Song *firstSong, Song *secondSong);

void playPlaylist(Playlist *playlist);

//this function prints the songs in the playlist
void showPlaylist(Playlist *playlist);

void printSongs(const Playlist *playlist);

int main() {
    int choice = 0;
    int currentNumberOfPlaylists = 0;

    printMainMenu();
    scanf("%d", &choice);

    Playlist *playlists = NULL;

    while (choice != DONE_MAIN_MENU) {
        switch (choice) {
            case 1:
                watchPlaylists(playlists, &currentNumberOfPlaylists);
                break;
            case 2:
                addPlaylist(&playlists, &currentNumberOfPlaylists);
                break;
            case 3:
                removePlaylist(&playlists, &currentNumberOfPlaylists);
                break;
            case 4:
                printf("Goodbye!");
                break;
            default:
                printf("Invalid input\n");
        }
        printMainMenu();
        scanf("%d", &choice);
    }
    printf("Goodbye!\n");
    // Free all playlists
    Playlist *tempPlaylist;
    while (playlists != NULL) {
        tempPlaylist = playlists;
        playlists = playlists->next;
        freePlaylist(tempPlaylist);
    }
    return 0;
}

void freePlaylist(Playlist *playlistPointer) {
    Song *tempSong = playlistPointer->headSong;

    while (tempSong != NULL) {
        Song *songToDelete = tempSong;
        tempSong = tempSong->next; // Move to the next song
        freeSong(songToDelete);
    }

    free(playlistPointer->name);
    free(playlistPointer);
}

void freeSong(Song *song) {
    free(song->artist);
    free(song->title);
    free(song->lyrics);
    free(song);
}

void printMainMenu() {
    printf("Please Choose:\n");
    printf("\t1. Watch playlists\n\t2. Add playlist\n\t3. Remove playlist\n\t4. exit\n");
}

void watchPlaylists(Playlist *playlists, const int *currentNumOfPlaylists) {
    int playlistChoice = 0;
    printListOfPlaylists(playlists, currentNumOfPlaylists);

    scanf("%d", &playlistChoice);

    //if there are no playlists
    while (*currentNumOfPlaylists == 0) {
        //if the user clicked invalid option
        if (playlistChoice != 1) {
            printf("Invalid option\n");
            printListOfPlaylists(playlists, currentNumOfPlaylists);
            scanf("%d", &playlistChoice);
            //if the user wants to go back to the main menu
        } else if (playlistChoice == 1) {
            return;
        }
    }

    //if there are playlists but the user wants to go back to the main menu
    if (playlistChoice == *currentNumOfPlaylists + 1) {
        return;
    }
    //find playlist user wants
    Playlist *playlistToWatch = findPlaylist(playlists, playlistChoice - 1);
    if (playlistToWatch == NULL) {
        printf("Invalid input\n");
        return;
    }

    managePlaylistMenu(playlistToWatch, playlists, currentNumOfPlaylists);
}

void printListOfPlaylists(Playlist *playlists, const int *currentNumOfPlaylists) {
    printf("Choose a playlist:\n");
    if (currentNumOfPlaylists == 0) {
        printf("\t1. Back to main menu\n");
        return;
    }

    Playlist *head = playlists;
    int index = 1;
    while (head != NULL) {
        printf("\t%d. %s\n", index, head->name);
        head = head->next;
        index++;
    }

    printf("\t%d. Back to main menu\n", *currentNumOfPlaylists + 1);
}

Playlist *findPlaylist(Playlist *head, int userChoice) {
    int index = 0;
    Playlist *currentPlaylist = head;
    while (currentPlaylist != NULL) {
        if (index == userChoice) {
            return currentPlaylist;
        }
        currentPlaylist = currentPlaylist->next;
        index++;
    }

    return NULL;
}

void managePlaylistMenu(Playlist *playlist, Playlist *playlists, const int *currentNumOfPlaylists) {
    int choice = 0;
    printf("playlist %s:\n", playlist->name);
    printPlaylistMenu();
    scanf("%d", &choice);

    while (choice != DONE_PLAYLIST_MENU) {
        switch (choice) {
            //case: show playlist
            case 1:
                showPlaylist(playlist);
                break;
            case 2:
                addSong(playlist);
                break;
            case 3:
                deleteSong(playlist);
                break;
            case 4:
                sortPlaylist(playlist);
                break;
            case 5:
                playPlaylist(playlist);
                break;
            default:
                printf("Invalid input\n");
        }

        printPlaylistMenu(playlist);
        scanf("%d", &choice);
    }
    watchPlaylists(playlists, currentNumOfPlaylists);
}

void printPlaylistMenu() {
    printf("\t1. Show Playlist\n");
    printf("\t2. Add Song\n");
    printf("\t3. Delete Song\n");
    printf("\t4. Sort\n");
    printf("\t5. Play\n");
    printf("\t6. Exit\n");
}

void showPlaylist(Playlist *playlist) {
    printSongs(playlist);
    printf("\nChoose a song to play, or 0 to quit:\n");
    int songIndex = -1;
    scanf("%d", &songIndex);
    if (songIndex <= 0 || songIndex > playlist->songsNum) {
        return;
    }
    while (songIndex != 0) {
        playSong(playlist, songIndex - 1);
        printf("Choose a song to play, or 0 to quit:\n");
        scanf("%d", &songIndex);
    }
}

void printSongs(const Playlist *playlist) {
    Song *currentSong = playlist->headSong;
    int index = 1;
    while (currentSong != NULL) {
        printf("%d. Title: %s\n   Artist: %s\n   Releases: %d\n   streams: %d\n", index, currentSong->title,
               currentSong->artist, currentSong->year, currentSong->streams);
        index++;
        currentSong = currentSong->next;
    }
}

void playSong(const Playlist *playlist, int songIndex) {
    Song *currentSong = playlist->headSong;
    //get to the right song index
    for (int i = 0; i < songIndex; i++) {
        currentSong = currentSong->next;
    }

    printf("Now playing %s:\n$%s$\n", currentSong->title, currentSong->lyrics);
    (currentSong->streams)++;
}

void addSong(Playlist *playlist) {
    int year = 0;
    printf("Enter song's details\n");
    printf("Title:\n");
    //check if allowed
    getchar();
    char *title = allocateMemoryForField();

    printf("Artist:\n");
    char *artist = allocateMemoryForField();
    printf("Year of release:\n");
    scanf("%d", &year);
    printf("Lyrics:\n");
    getchar();
    char *lyrics = allocateMemoryForField();

    Song *newSong = createSong(title, artist, year, lyrics);

    if (playlist->headSong == NULL) {
        playlist->headSong = newSong;
    } else {
        Song *temp = playlist->headSong;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newSong;
    }
    //insertion

    newSong->next = NULL;
    (playlist->songsNum)++;

    free(title);
    free(artist);
    free(lyrics);
}

Song *createSong(char *title, char *artist, int yearOfRelease, char *lyrics) {
    Song *song = malloc(sizeof(Song));
    if (song == NULL) {
        exit(1);
    }

    song->title = (char *) malloc(strlen(title) + 1);
    song->artist = (char *) malloc(strlen(artist) + 1);
    song->lyrics = (char *) malloc(strlen(lyrics) + 1);

    if (song->artist == NULL || song->lyrics == NULL || song->title == NULL) {
        freeSong(song);
        exit(1);
    }

    strcpy(song->title, title);
    strcpy(song->artist, artist);
    strcpy(song->lyrics, lyrics);
    song->year = yearOfRelease;
    song->streams = 0;

    return song;
}

void deleteSong(Playlist *playlist) {
    int songIndex = -1;
    printSongs(playlist);
    printf("Choose a song to delete, or 0 to quit:\n");
    scanf("%d", &songIndex);
    if (playlist->headSong == NULL) {
        return;
    }
    if (songIndex != 0) {
        songIndex--;
        int index = 0;
        Song *currentSong = playlist->headSong;
        Song *previousSong = NULL;

        while (currentSong != NULL) {
            if (index == songIndex) {
                // Found the song to remove
                if (previousSong == NULL) {
                    // Removing the first song (head of the list)
                    playlist->headSong = currentSong->next;
                } else {
                    // Removing any other song
                    previousSong->next = currentSong->next;
                }

                // Free the memory for the song
                freeSong(currentSong);
                (playlist->songsNum)--;

                printf("Song deleted successfully.\n");
                return;
            }

            previousSong = currentSong;
            currentSong = currentSong->next;
            index++;
        }
    }
}

void addPlaylist(Playlist **head, int *currentNumberOfPlaylists) {
    printf("Enter playlist's name:\n");
    getchar();
    char *playlistName = allocateMemoryForField();
    Playlist *newPlaylist = createPlaylist(playlistName);
    // // If the list is empty (head is NULL), make newPlaylist the head
    if (*head == NULL) {
        *head = newPlaylist;
    } else {
        Playlist *temp = *head;
        // find the last playlist and append the new one
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newPlaylist;
        newPlaylist->next = NULL;
    }

    // Update the playlists counter
    (*currentNumberOfPlaylists)++;
    free(playlistName);
}

Playlist *createPlaylist(const char *playlistName) {
    Playlist *newPlaylist = malloc(sizeof(Playlist));
    if (newPlaylist == NULL) {
        exit(1);
    }

    // Allocate memory for the playlist name and copy it
    newPlaylist->name = (char *) malloc(strlen(playlistName) + 1); // +1 for the null terminator
    if (newPlaylist->name == NULL) {
        free(newPlaylist);
        exit(1);
    }

    strcpy(newPlaylist->name, playlistName);

    // Initialize the new playlist
    newPlaylist->headSong = NULL; // No songs initially
    newPlaylist->songsNum = 0; // No songs initially
    newPlaylist->next = NULL; // No next playlist for now

    return newPlaylist;
}

//ask claude to add a cleaning of tavim shkufim
char *allocateMemoryForField() {
    // Dynamically allocate memory for the playlist name
    char *field = NULL;
    int size = 0;
    char ch;

    // Read the playlist name one character at a time
    while ((ch = getchar()) != '\n') {
        char *temp = realloc(field, size + 2);
        if (temp == NULL) {
            free(field);
            exit(1);
        }
        field = temp;
        field[size++] = ch; // Add the character to the name
    }

    field[size] = '\0'; // Null-terminate the string

    return field;
}

void sortPlaylist(Playlist *playlist) {
    int sortingMethod = 0;
    printf("choose:\n1. sort by year\n2. sort by streams - ascending order\n3. sort by streams - descending order"
        "\n4. sort alphabetically\n");
    scanf("%d", &sortingMethod);
    switch (sortingMethod) {
        case 1:
            sortByYear(playlist);
            break;
        case 2:
            sortByStreams(playlist, 0);
            break;
        case 3:
            sortByStreams(playlist, 1);
            break;
        default:
            sortAlphabetically(playlist);
            break;
    }
    printf("sorted\n");
}

void sortAlphabetically(Playlist *playlist) {
    if (playlist->headSong == NULL) {
        return;
    }

    int swapped;
    Song *prev = NULL;
    Song *current = NULL;

    do {
        swapped = 0; // Reset swapped flag
        prev = NULL;
        current = playlist->headSong;

        // Traverse the list to compare adjacent nodes
        while (current != NULL && current->next != NULL) {
            // Compare the titles alphabetically (lexicographical order)
            if (strcmp(current->title, current->next->title) > 0) {
                if (prev == NULL) {
                    // If current is the head, we update the head pointer
                    playlist->headSong = current->next;
                } else {
                    prev->next = current->next;
                }

                // Swap the next pointers of the nodes
                Song *temp = current->next;
                current->next = temp->next;
                temp->next = current;

                swapped = 1; // A swap was made
            }
            prev = current;
            current = current->next;
        }
    } while (swapped); // If a swap occurred, repeat the loop
}

// ascending = 1, descending = 0
void sortByStreams(Playlist *playlist, int ascendingOrDescendingOrder) {
    if (playlist->headSong == NULL) {
        return;
    }

    int swapped;
    Song *prev = NULL;
    Song *current = NULL;

    do {
        swapped = 0;
        prev = NULL;
        current = playlist->headSong;

        // Traverse the list and compare adjacent nodes
        while (current != NULL && current->next != NULL) {
            // Compare streams based on the order (ascending or descending)
            if ((ascendingOrDescendingOrder == 0 && current->streams > current->next->streams) ||
                (ascendingOrDescendingOrder == 1 && current->streams < current->next->streams)) {
                // Swap the two songs if they are in the wrong order
                if (prev == NULL) {
                    // If current is the head, we update the head pointer
                    playlist->headSong = current->next;
                } else {
                    prev->next = current->next;
                }

                // Swap the next pointers of the nodes
                Song *temp = current->next;
                current->next = temp->next;
                temp->next = current;

                swapped = 1; // A swap was made
            }

            prev = current;
            current = current->next;
        }
    } while (swapped); // If a swap occurred, repeat the loop
}

void sortByYear(Playlist *playlist) {
    if (playlist->headSong == NULL) {
        return;
    }

    int swapped;
    Song *current;

    // Keep swapping until no more swaps are needed
    do {
        swapped = 0; // Reset the swapped flag on each pass
        current = playlist->headSong;

        // Traverse the list and compare adjacent nodes
        while (current != NULL && current->next != NULL) {
            if (current->year > current->next->year) {
                // Swap the songs if they are in the wrong order
                Song *temp = current->next; // Point to the next node
                current->next = temp->next;
                temp->next = current; // Now, the next of temp points to current (swap complete)

                // Update the head if the swapped node is the new head
                if (current == playlist->headSong) {
                    playlist->headSong = temp;
                }

                swapped = 1; // A swap was made
            } else {
                current = current->next;
            }
        }
    } while (swapped); // If a swap occurred, repeat the loop
}

// Function to swap data of two songs
void swap(Song *firstSong, Song *secondSong) {
    // Swap other fields like title, artist if needed
    char *tempTitle = firstSong->title;
    firstSong->title = secondSong->title;
    secondSong->title = tempTitle;

    char *tempArtist = firstSong->artist;
    firstSong->artist = secondSong->artist;
    secondSong->artist = tempArtist;

    int tempYear = firstSong->year;
    firstSong->year = secondSong->year;
    secondSong->year = tempYear;

    Song *tempNext = firstSong->next;
    firstSong->next = secondSong->next;
    secondSong->next = tempNext;

    char *tempLyrics = firstSong->lyrics;
    firstSong->lyrics = secondSong->lyrics;
    secondSong->lyrics = tempLyrics;

    int tempStreams = firstSong->streams;
    firstSong->streams = secondSong->streams;
    secondSong->streams = tempStreams;
}

void playPlaylist(Playlist *playlist) {
    int index = 0;
    Song *currentSong = playlist->headSong;
    while (currentSong != NULL) {
        playSong(playlist, index);
        index++;
        currentSong = currentSong->next;
    }
}

void removePlaylist(Playlist **head, int *currentNumberOfPlaylists) {
    printListOfPlaylists(*head, currentNumberOfPlaylists); // Note: head is now a pointer to pointer
    int choice = 0;
    scanf("%d", &choice);

    // If the list is empty
    if (*head == NULL) {
        return; // Empty list, nothing to remove
    }

    // If user wants to remove the first playlist (head)
    if (choice == 1) {
        Playlist *temp = *head;
        *head = (*head)->next; // Move the head to the next playlist
        freePlaylist(temp); // Free the old head
        (*currentNumberOfPlaylists)--; // Decrease the count
        return;
    }

    // Traversing the list to find the playlist to remove
    int index = 1;
    Playlist *prev = *head;
    Playlist *currentPlaylist = (*head)->next;

    while (currentPlaylist != NULL) {
        index++;
        if (index == choice) {
            prev->next = currentPlaylist->next; // Bypass the current playlist
            freePlaylist(currentPlaylist);
            (*currentNumberOfPlaylists)--; // Decrease the playlist count
            return;
        }
        prev = currentPlaylist; // Move to the next playlist
        currentPlaylist = currentPlaylist->next; // Move to the next playlist
    }
}
