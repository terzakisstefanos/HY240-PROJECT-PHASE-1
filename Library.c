#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Library.h"




//**********************************CREATE FUNCTIONS**********************************

// A function that creates the library
library_t* createLibrary (void){
    library_t *library = malloc(sizeof(library_t));
    if (library == NULL) {
        return NULL;
        printf("%s","IGNORED");
    }
    library->genres = NULL;
    library->members = NULL;
    library->books = NULL;
    return library;
}
// A function that creates a new genre 
genre_t * createGenre(int gid, char* name){
    genre_t *newgenre= malloc(sizeof(genre_t));
    if (newgenre==NULL){
        printf("%s","IGNORED");
        return NULL;
    }
    if (strlen(name)>=NAME_MAX){
        printf("%s","WRONG NAME");
        printf("%s","IGNORED");
        return NULL;
    }
    newgenre->books=NULL;
    newgenre->display=NULL;
    newgenre->gid=gid;
    newgenre->invalid_count=0;
    newgenre->lost_count=0;
    strcpy(newgenre->name,name);//TODO check for all of the creates where name is reqiured if it has the NULL terminator 
    newgenre->next=NULL;
    newgenre->slots=0;
    return newgenre;
}
// A function that creates a new member
member_t *createmember(int sid, char* name){
    member_t *newmember=malloc(sizeof(member_t));
    if (newmember == NULL){
        printf("%s","IGNORED");
        return NULL;
    }
    if (strlen(name)>=NAME_MAX){
        printf("%s","WRONG NAME");
        printf("%s","IGNORED");
        return NULL;
    }
    newmember->loans=malloc(sizeof(loan_t));
    if (newmember->loans==NULL){
        free(newmember);
        return NULL;
    }
    newmember->loans->bid=-1;
    newmember->loans->next=newmember->loans;
    newmember->loans->sid=-1;
    strcpy(newmember->name, name);
    newmember->next=NULL;
    newmember->sid = sid;
    return newmember;
}


// A function that creates a new book
book_t *createBook(int bid , int gid,char* title){
    book_t *newbook = malloc(sizeof(book_t));
    if (newbook==NULL){
        printf("%s","IGNORED");
        return NULL;
    }
    if (strlen(title)>=TITLE_MAX){
        printf("%s","WRONG NAME");
        printf("%s","IGNORED");
        return NULL;
    }
    newbook->avg=0;
    newbook->bid=bid;
    newbook->gid=gid;
    newbook->lost_flag=0;
    newbook->n_reviews=0;
    newbook->next=NULL;
    newbook->next_global=NULL;
    newbook->prev=NULL;
    newbook->sum_scores=0;
    strcpy(newbook->title,title);
    return newbook;
}
//**********************************INSERT FUNCTIONS**********************************

void insertGenretolist(library_t *head ,genre_t *node){
    //TODO SEARCH IF THE GID IS ARLEADY HERE 
    if(head->genres==NULL || node->gid < head->genres->gid){// if list is empty or we need it to be the first node 
        node->next=head->genres;
        head->genres = node;
        printf("%s","DONE");
        return;
    }
    genre_t *current=head->genres;
    while (current->next!=NULL && current->next->gid < node->gid){//else find where it needs to go 
        current=current->next;
    
    }
    node->next=current->next;
    current->next=node;
    return;
    printf("%s","DONE");
}

// A function that inserts a new book to the genre 
void insertbooktogenre(genre_t *head, book_t *newbook) {
    // TODO: check if the book is unique in this genre
    if (head->books == NULL || newbook->avg > head->books->avg) {
        newbook->next = head->books;
        newbook->prev = NULL;
        if (head->books != NULL) {
            head->books->prev = newbook;
        }
        head->books = newbook;
        printf("%s","DONE");
        return;
    }
    book_t *current = head->books;
    while (current->next != NULL && current->next->avg > newbook->avg) {
        current = current->next;
    }
    newbook->next = current->next;
    newbook->prev = current;
    if (current->next != NULL) {
        current->next->prev = newbook;
    }
    current->next = newbook;
    printf("%s","DONE");
    return;
}

// A function that adds books to the generic list of the library 200
void insertbooktogeneric(library_t *head,book_t *book){
    //TODO check if book is unique with the search function 
    head->books = book;
    if (head->books==NULL){
        book->next_global=NULL;
         printf("%s","DONE");
        return;
    }
    book->next_global=head->books;
    printf("%s","DONE");
    return;
}

// A function that inserts a new member to the list 
void insertmembertolist(library_t *head, member_t *newmember) {
    // TODO: check for duplicates by sid
    if (head->members == NULL || newmember->sid < head->members->sid) {
        newmember->next = head->members;
        head->members = newmember;
        printf("%s","DONE");
        return;
    }
    member_t *current = head->members;
    while (current->next != NULL && current->next->sid < newmember->sid) {
        current = current->next;
    }
    newmember->next = current->next;
    current->next = newmember;
    printf("%s","DONE");
    return;
}

// A function that search the genrelist by gid 
genre_t *searchgenre(library_t *head,int gid){
    genre_t *current = head->genres;
    while (current != NULL && current->gid != gid) {
        current = current->next;
    }
    return current;
}
book_t *searchbook(genre_t *head, int bid){
    book_t *current= head->books;
    while (current != NULL && current->bid != bid){
        current = current->next;
    }
    return current;
}
book_t *searchbooktogeneric(library_t *Lib, int bid){
        book_t *current= Lib->books;
    while (current != NULL && current->bid != bid){
        current = current->next;
    }
    return current;
}

member_t *searchmember(library_t *Lib, int sid){
    member_t *current= Lib->members;
    while (current != NULL && current->sid != sid){
        current = current->next;
    }
    return current;
}
loan_t *searchloan(member_t *head, int bid){
    head->loans->bid=bid;
    loan_t *current = head->loans->next;
    while (current->bid != bid){
        current = current->next;
    }
    if (current==head->loans){
        return NULL;
    }
    return current;

}
// A function that add a loaned book to a member 
void Loanbook(library_t *LIB, int sid, int bid){
    member_t *member= searchmember(LIB,sid);
    if (member==NULL){
        printf("%s","IGNORED");
        return;
    }
    loan_t *loaned=searchloan(member,bid);
    if (loaned==NULL){
        printf("%s", "IGNORED");
        return;
    }
    book_t *bookingeneric = searchbooktogeneric(LIB,bid);
    if (bookingeneric==NULL){
        printf("%s","IGNORED");
        return;
    }
    loan_t *newloan=malloc(sizeof(loan_t));
        if (newloan==NULL){
        printf("%s","IGNORED");
        return;
    }
    newloan->sid=sid;
    newloan->bid=bid;
    newloan->next= member->loans->next;
    member->loans->next=newloan;
    printf("%s", "DONE");
    return;
}

int main(void){
    char filename[256];
    printf ("%s","Give me the name of the file where the commands are: ");
    if (fgets(filename,sizeof(filename),stdin)==NULL){
        fprintf(stderr,"Error reading filename\n");
    }
    size_t length=strlen(filename);
    if (length > 0 && filename[length-1] == '\n'){ 
        filename[length-1] = '\0';
    }
    FILE * file=fopen(filename,"r");
    if (file==NULL){
        fprintf(stderr,"Error opening file\n");
    }
    
}