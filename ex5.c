/***
 *Name: Lee Anne Rachman
 *ID: 323917286
 *Assingment: ex5
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
    //array of songs
    Song **songs;
    int songsNum;
    // Pointer to the next playlist in the list
    struct Playlist *next;
} Playlist;

//this function prints the menu of a single playlist and scans the action input from the user
void playlistMenu(Playlist *playlist);

//this function receives a pointer to a playlist and prints the menu of the playlist
void printPlaylistMenu(Playlist *playlist);

//this function prints the main menu
void printMainMenu();

//this function free's the space of a certain playlist
void freePlaylist(Playlist *playlistPointer[], int sizeOfPlaylist);

//this function free's the space of a certain song
void freeSong(Song **songs[], int numOfSongs);

//this function plays a song
void playSong(Song *song);

//this function adds a song to the playlist
void addSong(Playlist *playlist);

Song *createSong(char *title, char *artist, int yearOfRelease, char *lyrics);

//this function deletes a song
void deleteSong();

//this function shows the playlists
void scanForPlaylistToWatch(Playlist **playlists, int *currentNumOfPlaylists);

void printListOfPlaylists(Playlist **playlists, int *currentNumOfPlaylists);

//this function adds a playlist
void addPlaylist(Playlist **head, int *currentNumberOfPlaylists);

Playlist *createPlaylist(const char *name);

void removePlaylist();

void sortPlaylist();

//this function prints the songs in the playlist
void watchPlaylist(Playlist *playlist);


void deleteSong() {
    printf("Song deleted successfully.\n");
}


void freeSong(Song **songs[], int numOfSongs) {
    for (int i = 0; i < numOfSongs; i++) {
        free((*songs)[i]);
    }
}

void freePlaylist(Playlist *playlistPointer[], int sizeOfPlaylist) {
    for (int i = 0; i < sizeOfPlaylist; i++) {
        free(playlistPointer[i]);
        for (int j = 0; j < playlistPointer[i]->songsNum; j++) {
            free(playlistPointer[i]->songs[j]);
        }
    }
}

void printMainMenu() {
    printf("Please Choose:\n");
    printf("\t1. Watch playlists\n\t2. Add playlist\n\t3. Remove playlist\n\t4. exit\n");
}

void printPlaylistMenu(Playlist *playlist) {
    printf("playlist %s:\n", playlist->name);
    printf("\tShow Playlist\n");
    printf("\tAdd Song\n");
    printf("\tDelete Song\n");
    printf("\tSort\n");
    printf("\tPlay\n");
    printf("\tExit\n");
}

void scanForPlaylistToWatch(Playlist **playlists, int *currentNumOfPlaylists) {
    int playlistChoice = 0;
    printListOfPlaylists(playlists, currentNumOfPlaylists);

    scanf("%d", &playlistChoice);

    while (*currentNumOfPlaylists == 0) {
        if (playlistChoice != 1) {
            printf("Invalid option\n");
            printListOfPlaylists(playlists, currentNumOfPlaylists);
            scanf("%d", &playlistChoice);
        } else if (playlistChoice == 1) {
            return;
        }
    }

    if (playlistChoice == *currentNumOfPlaylists) {
        return;
    }

    playlistMenu(playlists[playlistChoice]);
}

void printListOfPlaylists(Playlist **playlists, int *currentNumOfPlaylists) {
    printf("Choose a playlist:\n");
    if (currentNumOfPlaylists == 0) {
        printf("\t1. Back to main menu\n");
        return;
    }

    for (int i = 1; i <= *currentNumOfPlaylists; i++) {
        printf("\t%d.%s\n", i, playlists[i]->name);
    }

    printf("\t%d. Back to main menu", *currentNumOfPlaylists + 1);
}

void playlistMenu(Playlist *playlist) {
    int choice = 0;
    printPlaylistMenu(playlist);
    scanf("%d", &choice);

    while (choice != DONE_PLAYLIST_MENU) {
        switch (choice) {
            case 1: {
                if (playlist->songsNum == 0) {
                    break;
                }
                watchPlaylist(playlist);
            }
            case 2: {
                addSong(playlist);
            }
            default:
                printf("Invalid input\n");
        }

        printPlaylistMenu(playlist);
    }
}

void addSong(Playlist *playlist) {
    char *title;
    char *artist;
    int yearOfRelease;
    char *lyrics;

    printf("Enter song's details\n");
    printf("Title:\n");
    scanf("%s", &title);
    printf("Artist:\n");
    scanf("%s", &artist);
    printf("Year or release:\n");
    scanf("%d", &yearOfRelease);
    printf("Lyrics:\n");
    scanf("%s", &lyrics);

    Song *newSong = createSong(title, artist, yearOfRelease, lyrics);
    newSong->next = playlist->songs; // Add the new song at the beginning of the song list
    playlist->songs = newSong; // Update the playlist's song list
}

Song *createSong(char *title, char *artist, int yearOfRelease, char *lyrics) {
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

    return song;
}

void watchPlaylist(Playlist *playlist) {
    int songIndex = 0;
    printf("Choose a song to play, or 0 to quit:\n");
    scanf("%d", &songIndex);
    playSong(playlist->songs[songIndex]);
}

void playSong(Song *song) {
    printf("Now playing %s\n$%s$", song->title, song->lyrics);
}

void addPlaylist(Playlist **head, int *currentNumberOfPlaylists) {
    char *playlistName;

    printf("Enter playlist's name:\n");
    scanf("%s", &playlistName);
    Playlist *newPlaylist = (Playlist *) malloc(sizeof(Playlist));
    if (newPlaylist == NULL) {
        exit(1);
    }
    //segmentation fault
    newPlaylist->name = (char*) malloc(strlen(playlistName) + 1);
    if (newPlaylist->name == NULL) {
        exit(1); // Exit if memory allocation fails for the name
    }

    strcpy(newPlaylist->name, playlistName);

    newPlaylist->next = *head;
    *head = newPlaylist;

    // Update the playlists counter
    (*currentNumberOfPlaylists)++;
}

void removePlaylist() {
}


void sortPlaylist() {
    printf("sorted\n");
}


int main() {
    int choice = 0;
    int currentNumberOfPlaylists = 0;
    int currentNumberOfSongs = 0;

    printMainMenu();
    scanf("%d", &choice);
    //dynamically allocate memory for the array of playlists
    Playlist *playlists = malloc(currentNumberOfPlaylists * sizeof(Playlist));
    //if malloc failed
    if (playlists == NULL) {
        exit(1);
    }
    while (choice != DONE_MAIN_MENU) {
        switch (choice) {
            case 1:
                scanForPlaylistToWatch(&playlists, &currentNumberOfPlaylists);
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
    freePlaylist(&playlists, sizeof(Playlist));
    printf("Goodbye!\n");
    return 0;
}
