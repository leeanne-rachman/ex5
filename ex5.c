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
void managePlaylistMenu(Playlist *playlist);

//this function receives a pointer to a playlist and prints the menu of the playlist
void printPlaylistMenu(const Playlist *playlist);

//this function prints the main menu
void printMainMenu();

//this function free's the space of a certain playlist
void freePlaylist(Playlist *playlistPointer, int sizeOfPlaylist);

//this function free's the space of a certain song
void freeSong(Song **songs[], int numOfSongs);

//this function plays a song
void playSong(const Playlist *playlist, int songIndex);

//this function adds a song to the playlist
void addSong(Playlist *playlist);

Song *createSong(const char *title, const char *artist, int yearOfRelease, const char *lyrics);

//this function deletes a song
void deleteSong(Playlist *playlist);

//this function shows the playlists
void watchPlaylists(Playlist *playlists, const int *currentNumOfPlaylists);

Playlist *findPlaylist(Playlist *head, int userChoice);

void printListOfPlaylists(Playlist *playlists, const int *currentNumOfPlaylists);

//this function adds a playlist
void addPlaylist(Playlist **head, int *currentNumberOfPlaylists);

Playlist *createPlaylist(const char *name);

char *allocateMemoryForField();

void removePlaylist();

void sortPlaylist();

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
                removePlaylist();
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
    //freePlaylist(&playlists, sizeof(Playlist));
    printf("Goodbye!\n");
    return 0;
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

    managePlaylistMenu(playlistToWatch);
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

        index++;
    }

    return NULL;
}

void managePlaylistMenu(Playlist *playlist) {
    int choice = 0;
    printf("playlist %s:\n", playlist->name);
    printPlaylistMenu(playlist);
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
}

void printPlaylistMenu(const Playlist *playlist) {
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
        printf("\nChoose a song to play, or 0 to quit:\n");
        scanf("%d", &songIndex);
    }
}

void printSongs(const Playlist *playlist) {
    Song *currentSong = playlist->headSong;
    int index = 1;
    while (currentSong != NULL) {
        printf("%d. Title: %s\n\t Artist: %s\n\t Releases: %d\n\t streams: %d\n\t", index, currentSong->title,
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
    getchar();
    const char *title = allocateMemoryForField();

    printf("Artist:\n");
    const char *artist = allocateMemoryForField();
    printf("Year of release:\n");
    scanf("%d", &year);
    printf("Lyrics:\n");
    getchar();
    const char *lyrics = allocateMemoryForField();

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
}

Song *createSong(const char *title, const char *artist, const int yearOfRelease, const char *lyrics) {
    Song *song = (Song *) malloc(sizeof(Song));
    if (song == NULL) {
        exit(1);
    }

    song->title = (char *) malloc(strlen(title) + 1);
    song->artist = (char *) malloc(strlen(artist) + 1);
    song->lyrics = (char *) malloc(strlen(lyrics) + 1);


    if (song->artist == NULL || song->lyrics == NULL || song->title == NULL) {
        free(song);
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
                free(currentSong->title);
                free(currentSong->artist);
                free(currentSong->lyrics);
                free(currentSong);
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
    Playlist *newPlaylist = (Playlist *) malloc(sizeof(Playlist));
    if (newPlaylist == NULL) {
        printf("Memory allocation failed for new playlist!\n");
        exit(1);
    }

    // If the list is empty (head is NULL), make newPlaylist the head
    if (*head == NULL) {
        *head = newPlaylist;
    }

    printf("Enter playlist's name:\n");
    getchar();
    char *playlistName = allocateMemoryForField();

    // Allocate memory for the playlist name and copy it
    newPlaylist->name = (char *) malloc(strlen(playlistName) + 1); // +1 for the null terminator
    if (newPlaylist->name == NULL) {
        exit(1);
    }
    strcpy(newPlaylist->name, playlistName);

    // Clean up the temporary name buffer
    free(playlistName);

    // Initialize the new playlist
    newPlaylist->headSong = NULL; // No songs initially
    newPlaylist->songsNum = 0; // No songs initially
    newPlaylist->next = NULL; // No next playlist for now

    // find the last playlist and append the new one
    Playlist *temp = *head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = newPlaylist;
    newPlaylist->next = NULL;

    // Update the playlists counter
    (*currentNumberOfPlaylists)++;
}

char *allocateMemoryForField() {
    // Dynamically allocate memory for the playlist name
    char *field = NULL;
    int size = 0;
    char ch;

    //getchar();
    // Read the playlist name one character at a time
    while ((ch = getchar()) != '\n' && ch != EOF) {
        // Allocate or reallocate memory for the name
        field = (char *) realloc(field, size + 2); // +1 for the new char, +1 for '\0'
        if (field == NULL) {
            exit(1);
        }
        field[size++] = ch; // Add the character to the name
    }

    field[size] = '\0'; // Null-terminate the string

    return field;
}

void freeSong(Song **songs[], int numOfSongs) {
    for (int i = 0; i < numOfSongs; i++) {
        free((*songs)[i]);
    }
}

void freePlaylist(Playlist *playlistPointer, int sizeOfPlaylist) {
}

void removePlaylist() {
}

void playPlaylist(Playlist *playlist) {
}

void sortPlaylist() {
    printf("sorted\n");
}
