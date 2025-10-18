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
    newgenre->book_sentinel.gid=gid;
    newgenre->display=NULL;
    newgenre->display_count=0;
    newgenre->gid=gid;
    strcpy(newgenre->name,name);
    newgenre->next_by_gid=NULL;
    return newgenre;
}

void insertGenretolist(library_t *head ,genre_t *node){
    //TODO SEARCH IF THE GID IS ARLEADY HERE 
    if(head->genres==NULL || node->gid < head->genres->gid){// if list is empty or we need it to be the first node 
        node->next_by_gid=head;
        return ;
    }
    genre_t *current=head;
    while (current->next_by_gid!=NULL && current->next_by_gid->gid < node->gid){//else find where it needs to go 
        current=current->next_by_gid;
    }
    node->next_by_gid=current->next_by_gid;
    current->next_by_gid=node;
    return ;
}
// A function that creates a new book
book_t *createBook(int bid , int gid,char* title){
    book_t *newbook = malloc(sizeof(book_t));
        if (newbook==NULL){
        return NULL;
    }
    newbook->avg=0;
    newbook->bid=bid;
    newbook->gid=gid;
    newbook->lost_flag=0;
    newbook->next_global=NULL;
    newbook->next_in_genre=NULL;
    newbook->prev_in_genre=NULL;
    newbook->sum_scores=0;
    strcpy(newbook->title,title);
    return newbook;
}

// A function that inserts a new book to the genre 
void insertbooktogenre(genre_t * head,book_t *book){
    //TODO check if book is unique with the search function 
        if (head->book_sentinel.next_in_genre == &head->book_sentinel || book->avg > head->book_sentinel.next_in_genre->avg){
        head->book_sentinel.next_in_genre=book;
        head->book_sentinel.next_in_genre->prev_in_genre = book;
        book->next_in_genre=&head->book_sentinel;
        book->prev_in_genre=&head->book_sentinel;
        if (head->book_sentinel.prev_in_genre == &head->book_sentinel) {
            head->book_sentinel.prev_in_genre = book;
        }
        return;
    }
    book_t *current= head->book_sentinel.next_in_genre;
    head->book_sentinel.avg=book->avg;
    while (book->avg>current->avg){
        current=current->next_in_genre;
    }
    if (current==&head->book_sentinel){// using the sentinel to make my algorith more efficient 
        book->next_in_genre = &head->book_sentinel;
        book->prev_in_genre = head->book_sentinel.prev_in_genre;
        head->book_sentinel.prev_in_genre->next_in_genre = book;
        head->book_sentinel.prev_in_genre = book;
        return;
    }
    book->next_in_genre = current;
    book->prev_in_genre = current->prev_in_genre;
    current->prev_in_genre->next_in_genre = book;
    current->prev_in_genre = book;
    return;
}
// A function that adds books to the generic list of the library 200
void insertbooktogeneric(library_t *head,book_t *book){
    //TODO check if book is unique with the search function 
    head->books = book;
    if (head->books==NULL){
        book->next_global=NULL;
        return;
    }
    book->next_global=head->books;
    return;
}

member_t *createmember(int sid, char* name){
    member_t *newmember=malloc(sizeof(member_t));
    if (newmember == NULL){
        return NULL;
    }
    newmember->loans_head.sid = sid;    
    newmember->loans_head.bid = -1;     
    newmember->loans_head.next = NULL;
    strcpy(newmember->name, name);
    newmember->prev_all = NULL;
    newmember->next_all = NULL;
    newmember->sid = sid;
    return newmember;
}
// A function that inserts a new member to the list 
void insertmembertolist(library_t *head, member_t *newmember){
        //TODO check for duplicates 
    if (head->members == NULL || newmember->sid < head->members->sid) {
        newmember->next_all = head->members;
        newmember->prev_all = head;
        if (head->members != NULL) {
            head->members->prev_all= newmember;
        }
        head->members = newmember;
        return;
    }
    member_t *current = head;
    while(current->next_all!= NULL || current->sid< newmember->sid){
        current=current->next_all;
    }
    // finish it 
}