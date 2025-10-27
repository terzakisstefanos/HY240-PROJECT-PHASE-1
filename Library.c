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
        printf("%s\n","IGNORED");
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
        printf("%s\n","IGNORED");
        return NULL;
    }
    newgenre->books=NULL;
    newgenre->display=NULL;
    newgenre->gid=gid;
    newgenre->invalid_count=0;
    newgenre->lost_count=0;
    strcpy(newgenre->name,name);
    newgenre->next=NULL;
    newgenre->remainder=0;
    newgenre->slots=0;
    return newgenre;
}
// A function that creates a new member
member_t *createmember(int sid, char* name){
    member_t *newmember=malloc(sizeof(member_t));
    if (newmember == NULL){
        printf("%s\n","IGNORED");
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
        printf("%s\n","IGNORED");
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
    if(searchgenre(head,node->gid)!=NULL){
        printf("%s\n","IGNORED");
        return ;
    }    if(head->genres==NULL || node->gid < head->genres->gid){// if list is empty or we need it to be the first node 
        node->next=head->genres;
        head->genres = node;
        printf("%s\n","DONE");
        return;
    }
    genre_t *current=head->genres;
    while (current->next!=NULL && current->next->gid < node->gid){//else find where it needs to go 
        current=current->next;
    
    }
    node->next=current->next;
    current->next=node;
    printf("%s\n","DONE");
    return;
}

// A function that inserts a new book to the genre 
void insertbooktogenre(genre_t *head, book_t *newbook) {
    if(searchbook(head,newbook->bid)!=NULL){
        printf("%s\n","IGNORED");
        return ;
    }
    if (head->books == NULL || newbook->avg > head->books->avg) {
        newbook->next = head->books;
        newbook->prev = NULL;
        if (head->books != NULL) {
            head->books->prev = newbook;
        }
        head->books = newbook;
        printf("%s\n","DONE");
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
    printf("%s\n","DONE");
    return;
}

// A function that adds books to the generic list of the library 200
void insertbooktogeneric(library_t *head,book_t *book){
    if(searchbooktogeneric(head,book->bid)!=NULL){
        printf("%s\n","IGNORED");
        return ;
    }
    book->next_global = head->books;
    head->books = book;
    printf("%s\n","DONE");
    return;
}

// A function that inserts a new member to the list 
void insertmembertolist(library_t *head, member_t *newmember) {
    if(searchmember(head,newmember->sid)!=NULL){
        printf("%s\n","IGNORED");
        return ;
    }
    if (head->members == NULL || newmember->sid < head->members->sid) {
        newmember->next = head->members;
        head->members = newmember;
        printf("%s\n","DONE");
        return;
    }
    member_t *current = head->members;
    while (current->next != NULL && current->next->sid < newmember->sid) {
        current = current->next;
    }
    newmember->next = current->next;
    current->next = newmember;
    printf("%s\n","DONE");
    return;
}


//**********************************SEARCH FUNCTIONS**********************************



// A function that search the genrelist by gid 
genre_t *searchgenre(library_t *head,int gid){
    genre_t *current = head->genres;
    while (current != NULL && current->gid != gid) {
        current = current->next;
    }
    return current;
}
// A function that searches a book in a genre 
book_t *searchbook(genre_t *head, int bid){
    book_t *current= head->books;
    while (current != NULL && current->bid != bid){
        current = current->next;
    }
    return current;
}
// A function that searches a book in the generic list  
book_t *searchbooktogeneric(library_t *Lib, int bid){
        book_t *current= Lib->books;
    while (current != NULL && current->bid != bid){
        current = current->next;
    }
    return current;
}
// A function that searches a member 
member_t *searchmember(library_t *Lib, int sid){
    member_t *current= Lib->members;
    while (current != NULL && current->sid != sid){
        current = current->next;
    }
    return current;
}
// A function that searches the loan list of a member 
loan_t *searchloan(member_t *head, int bid){
    head->loans->bid=bid;
    loan_t *current = head->loans->next;
    while (current->bid != bid){
        current = current->next;
    }
    if (current==head->loans){// using the sentinel to check if we reached the end of the list 
        return NULL;
    }
    return current;

}
// A function that add a loaned book to a member 
void Loanbook(library_t *LIB, int sid, int bid){
    member_t *member= searchmember(LIB,sid);
    if (member==NULL){
        printf("%s\n","IGNORED");
        return;
    }
    loan_t *loaned=searchloan(member,bid);
    if (loaned!=NULL){
        printf("%s\n", "IGNORED");
        return;
    }
    book_t *bookingeneric = searchbooktogeneric(LIB,bid);// no need to search in genre we dont modify anything 
    if (bookingeneric==NULL){
        printf("%s\n","IGNORED");
        return;
    }
    loan_t *newloan=malloc(sizeof(loan_t));
        if (newloan==NULL){
        printf("%s\n","IGNORED");
        return;
    }
    newloan->sid=sid;
    newloan->bid=bid;
    newloan->next= member->loans->next;
    member->loans->next=newloan;
    printf("%s\n", "DONE");
    return;
}

// A function that prints genres 
void printgenres(library_t * Lib){//TODO make another one this is only for testing the other one should print one only one genre at a time 
    genre_t * current = Lib->genres;
    while(current!=NULL){
        printf("%s\n",current->name);
        printf("gid=%d name=%s\n", current->gid, current->name);
        current=current->next;
    }
}

void printmembers(library_t * Lib){
    member_t * current = Lib->members;
    while(current!=NULL){
        printf("%s\n",current->name);
        current=current->next;
    }
}
// A function that prints all of the loaned books of a user 
void printmemberloan(library_t *lib,int sid){
    member_t *member = searchmember(lib,sid);
    if (member==NULL){
        printf("%s\n", "IGNORED");
        return;
    }
    member->loans->sid=sid;
    loan_t *current=member->loans->next;
    while (current->sid != sid){
        printf("%d",current->bid);
        current = current->next;
    }
}

void returnbook(library_t * lib,int sid,int bid,int score,char *status){//TODO in the main change the score from NA to -1 
    member_t *member=searchmember(lib,sid);
    if (member==NULL){
        printf("%s\n", "IGNORED");
        return;
    }
    if (searchloan(member,bid)==NULL){
        printf("%s\n", "IGNORED");
        return;
    }
    book_t *bookingeneric = searchbooktogeneric(lib,bid);
    if (bookingeneric==NULL){
        printf("%s\n", "IGNORED");
        return 0;
    }
    genre_t *genre=searchgenre(lib,bookingeneric->gid); // no need to check if this is null because if the book exists in the generic it exists everywhere
    book_t *bookingenre = searchbook(genre,bid);
    if (strcmp(status,"ok")==0){
        if (score >= 0 && score <= 10){
            reviewbook(bookingenre ,bookingeneric,score);
            removebook(genre,bookingenre);
            insertbooktogenre(genre,bookingenre);
            printf("%s\n", "DONE");
        }else {
            genre->invalid_count++;
            printf("%s\n", "IGNORED");
        }
    }else{
    bookingeneric->lost_flag=1;
    bookingenre->lost_flag=1;
    genre->lost_count++;
    printf("%s\n", "DONE");
    }
    return;
}

void reviewbook(book_t *bookingenre, book_t * bookingeneric,int score){
    bookingenre->n_reviews++;
    bookingenre->sum_scores=bookingenre->sum_scores+score;
    bookingenre->avg=bookingenre->sum_scores/bookingenre->n_reviews;

    bookingeneric->n_reviews=bookingenre->n_reviews;// its the same book so it doesnt matter
    bookingeneric->sum_scores=bookingenre->sum_scores; 
    bookingeneric->avg=bookingenre->avg;
    return;
}

void removebook (genre_t* genre , book_t *book){
    if (book->next == NULL && book->prev == NULL){
        genre->books = NULL;
    }
    else if (book->prev == NULL){
        genre->books = book->next;
        book->next->prev = NULL;
    }
    else if (book->next == NULL){
        book->prev->next = NULL;
    }
    else{
        book->prev->next = book->next;
        book->next->prev = book->prev;
    }
    book->next = NULL;
    book->prev = NULL;
    return;
}
void removeloan(member_t *member,loan_t *loan){
    if (loan->next==member->loans && member->loans->next==loan){
        member->loans->next=member->loans;
    }else{
        member->loans->bid=loan->bid;
        loan_t *current=member->loans->next;
        while (current->next->bid==loan->bid){
            current=current->next;
        }
        current->next=loan->next;
    }
    free(loan);
    member->loans->sid=-1;
    return;
}

void display(library_t * lib){
    genre_t *current=lib->genres;
    if (SLOTS<=0){
        printf("%s\n", "IGNORED");
        return;
    }
    int remains =0;
        while (current!= NULL){
        current->slots=(computepoints(current)/(computepoints(current)/SLOTS));
        insertbookstodisplay(current);
        remains=remains +current->remainder;
    }
    while (remains){
        genre_t * genre = findmaxgenre(lib);
        genre->slots=1;
        insertbookstodisplay(genre);
        remains--;
    }
}

void insertbookstodisplay(genre_t* genre,int flag){// Flag = 0 for inserting books (slots) and Flag = 1 for inserting one book 
    int i = 0;
    book_t *current = genre->books;
    book_t *last = NULL;
    while (current != NULL && i < genre->slots){
        if (current->lost_flag==0){

            int exists = 0;
            book_t *check = genre->display;
            while (check != NULL){
                if (strcmp(check->title, current->title) == 0){
                    exists = 1;
                    break;
                }
                check = check->next;
            }
            if(!exists){
                if (genre->display == NULL){
                    genre->display=current;
                    last=current;
                }else {
                
                    last->next=current;
                    last=current;
                }
                i++;
            }
        }
        current=current->next;
    }
    if (last != NULL){
        last->next = NULL;
    }
    if (!flag) {
        genre->remainder = genre->slots - i; // only update remainder during first distribution
        genre->slots = i;
    }else{
        genre->remainder--;
    }
}

int computepoints(genre_t *genre){
    int points=0;
    book_t *current =genre->books;
    while (current!=NULL){
        if (current->lost_flag==0 || current->n_reviews>0){
            points+=current->avg;
        }
        current=current->next;
    }
    return points;
}

genre_t * findmaxgenre (library_t *Lib){
    genre_t *current =Lib->genres;
    genre_t *max =current;
    while(current!= NULL){
        if (current->remainder>max->remainder||(current->remainder == max->remainder && current->gid < max->gid)){
            max=current;
        }
        current = current->next;
    }
    return max;
}

int main(void){
    char filename[256];
    printf ("%s","Give me the name of the file where the commands are: ");
    if (fgets(filename,sizeof(filename),stdin)==NULL){
        fprintf(stderr,"Error reading filename\n");
        return 1 ;
    }
    size_t length=strlen(filename);
    if (length > 0 && filename[length-1] == '\n'){ 
        filename[length-1] = '\0';
    }
    FILE * file=fopen(filename,"r");
    if (file==NULL){
        fprintf(stderr,"Error opening file or file does not exist\n");
        return 1;
    }
    library_t *library=createLibrary();
    char buffer[256];
    while (fgets(buffer,sizeof(buffer),file)!=NULL){
        if (strncmp(buffer, "G", 1) == 0) {
            printf("Found Genre command: %s\n", buffer);

            int gid;
            char name[NAME_MAX] ;
            sscanf(buffer+2,"%d \"%63[^\"]\"",&gid,name);
            genre_t *newnode= createGenre(gid,name);
            if (newnode==NULL){
                return 1;
            }
            insertGenretolist(library,newnode);
        } else if (strncmp(buffer, "BK", 2) == 0) {
            printf("Found Book command: %s\n", buffer);
            int bid;
            int gid;
            char name[NAME_MAX];
            sscanf(buffer+3,"%d %d \"%127[^\"]\"",&bid,&gid,name);
            book_t *newbook= createBook(bid,gid,name);
            if (newbook==NULL){
                return 1 ;
            }
            genre_t *genre =searchgenre(library,gid);

            if (genre == NULL){
                free(newbook);
                printf("IGNORED\n");
                continue;
            }
            insertbooktogenre(genre,newbook);
            insertbooktogeneric(library,newbook);
        } else if (strncmp(buffer, "M", 1) == 0) {
            printf("Found Member command: %s\n", buffer);
            int sid;
            char name[NAME_MAX];
            sscanf(buffer + 2, "%d \"%63[^\"]\"", &sid, name);
            member_t *newmember= createmember(sid,name);
            if (newmember==NULL){
                return 1;
            }
            insertmembertolist(library,newmember);
        } else if (strncmp(buffer, "L", 1) == 0) {
            printf("Found Loan command: %s\n", buffer);
            int sid;
            int bid;
            sscanf(buffer+2,"%d %d",&sid,&bid);
            Loanbook(library,sid,bid);
        } else if (strncmp(buffer, "R", 1) == 0) {
            printf("Found Return command: %s\n", buffer);
        } else if (strncmp(buffer, "D", 1) == 0) {
            printf("Found Display command: %s\n", buffer);
        } else if (strncmp(buffer, "PG", 2) == 0) {
            printf("Found Print Genre command: %s\n", buffer);
            printgenres(library);//TODO forget to erase it 
            int gid;
            sscanf(buffer+3,"%d",&gid);
            genre_t *genre = searchgenre(library,gid);
            printf("%s",genre->name);
        } else if (strncmp(buffer, "PM", 2) == 0) {
            printf("Found Print Member command: %s\n", buffer);
            printmembers(library);
            int sid;
            sscanf(buffer+3,"%d",&sid);
            printmemberloan(library,sid);
        } else if (strncmp(buffer, "PD", 2) == 0) {
            printf("Found Print Display command: %s\n", buffer);
        } else if (strncmp(buffer, "PS", 2) == 0) {
            printf("Found Print Stats command: %s\n", buffer);
        } else if (strncmp(buffer, "S", 1) == 0) {
            printf("Found Slots command: %s\n", buffer);
        }
    }
    return 0; 
}