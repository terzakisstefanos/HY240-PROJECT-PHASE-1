#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Library.h"


int main (void){

}
// A function that creates the library
library_t* createLibrary (void){
    library_t *library = malloc(sizeof(library_t));
    if (library == NULL) {
        return NULL;
    }
    library->genres = NULL;
    library->members = NULL;
    library->books = NULL;
    return library;
}
// A function that creates a new genre 
genre_t * createGenre(int gid, char* name){//TODO you need to check the name <Maxname 
    genre_t *newgenre= malloc(sizeof(genre_t));
    if (newgenre==NULL){
        return NULL;
    }
    newgenre->book_sentinel.prev_in_genre=&newgenre->book_sentinel;
    newgenre->book_sentinel.next_in_genre=&newgenre->book_sentinel;
    newgenre->display=NULL;
    newgenre->display_count=0;
    newgenre->gid=gid;
    strcpy(newgenre->name,name);
    newgenre->next_by_gid=NULL;
    return newgenre;
}

genre_t *insertGenretolist(genre_t *head ,genre_t *node){
    //TODO SEARCH IF THE GID IS ARLEADY HERE 
    if(head->next_by_gid==NULL || node->gid < head->gid){// if list is empty or we need it to be the first node 
        node->next_by_gid=head;
        return node;
    }
    genre_t *current=head;
    while (node->next_by_gid!=NULL && current->next_by_gid->gid < node->gid){//else find where it needs to go 
        current=current->next_by_gid;
    }
    node->next_by_gid=current->next_by_gid;
    current->next_by_gid=node;
    return node;
}//test