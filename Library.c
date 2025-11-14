#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Library.h"

int SLOTS = 0;



//**********************************CREATE FUNCTIONS**********************************

// A function that creates the library
library_t* createLibrary (void){
    library_t *library = malloc(sizeof(library_t));
    if (library == NULL) {
        printf("%s\n","IGNORED");
        return NULL;
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
void insertbooktogenre(genre_t *head, book_t *newbook) {// no need to check for the search books because we check it outside of the function 
    if (head->books == NULL || newbook->avg > head->books->avg) {
        newbook->next = head->books;
        newbook->prev = NULL;
        if (head->books != NULL) {
            head->books->prev = newbook;
        }
        head->books = newbook;
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
    return;
}

// A function that adds books to the generic list of the library 200
void insertbooktogeneric(library_t *LIB, book_t *book) {
    book->next_global = LIB->books; 
    LIB->books = book;              
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

// A function that inserts books to the display list 
void insertbookstodisplay(genre_t *genre) {
    if (genre->slots <= 0 || genre->display == NULL) return;

    int i = 0;
    book_t *current = genre->books;
    while (current != NULL && i < genre->slots) {
        if (current->lost_flag == 0) {
            genre->display[i++] = current;
        }
        current = current->next;
    }
    genre->slots = i;
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
        current = current->next_global;
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
    head->loans->bid = -1;
    if (current==head->loans){// using the sentinel to check if we reached the end of the list 
        return NULL;
    }
    return current;

}
// A function that add a loaned book to a member 
void Loanbook(library_t *LIB, int sid, int bid){
    member_t *member= searchmember(LIB,sid); // check if everythink exists 
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
    if (bookingeneric->lost_flag==1){
        printf("%s\n", "IGNORED");
        return;
    }
    loan_t *newloan=malloc(sizeof(loan_t));
        if (newloan==NULL){
        printf("%s\n","IGNORED");
        return;
    }
    newloan->sid=sid;
    newloan->bid = bid;
    newloan->next = member->loans->next;
    member->loans->next = newloan;
    printf("%s\n", "DONE");
    return;
}

// A function that prints the books of the genres
void printgenrebooks(library_t *lib, int gid) {
    genre_t *genre = searchgenre(lib, gid);
    if (genre == NULL) {
        printf("IGNORED\n");
        return;
    }
    book_t *book = genre->books;
    while (book != NULL) {
        printf("%d, %d\n", book->bid, book->avg);
        book = book->next;
    }
}
// A function that prints all of the loaned books of a user 
void printmemberloan(library_t *lib,int sid){
    member_t *member = searchmember(lib,sid);
    if (member==NULL){
        printf("%s\n", "IGNORED");
        return;
    }
    printf("Loans:\n");
    loan_t *current=member->loans->next;
    while (current != member->loans){
        printf("%d\n",current->bid);
        current = current->next;
    }
}
// A function that prints the display of each genre 
void printdisplay(library_t *lib) {
    printf("Display:\n");

    genre_t *genre = lib->genres;
    int empty = 1;

    while (genre != NULL) {
        if (genre->slots > 0 && genre->display != NULL) {
            empty = 0;
            printf("%d:\n", genre->gid);

            int i = 0;
            while (i < genre->slots) {
                book_t *book = genre->display[i];
                if (book != NULL) {
                    printf("%d, %d\n", book->bid, book->avg);
                }
                i++;
            }
        }
        genre = genre->next;
    }

    if (empty) {
        printf("(empty)\n");
    }
}

// A function that returns a loaned book 
void returnbook(library_t * lib,int sid,int bid,int score,char *status){
    member_t *member=searchmember(lib,sid); // check if everything exists
    if (member==NULL){
        printf("%s\n", "IGNORED");
        return;
    }
    loan_t *loan=searchloan(member,bid);
    if (loan==NULL){
        printf("%s\n", "IGNORED");
        return;
    }
    book_t *bookingeneric = searchbooktogeneric(lib,bid);
    if (bookingeneric==NULL){
        printf("%s\n", "IGNORED");
        return ;
    }
    genre_t *genre=searchgenre(lib,bookingeneric->gid); // no need to check if this is null because if the book exists in the generic it exists everywhere
    book_t *bookingenre = searchbook(genre,bid);
    removeloan(member,loan);
    if (strcmp(status,"ok")==0){
        if (score >= 0 && score <= 10){// If score is within appropriate values 
            reviewbook(bookingenre ,bookingeneric,score);
            removebook(genre,bookingenre);
            insertbooktogenre(genre,bookingenre);
            printf("%s\n", "DONE");
        }else if (score==-1) {
            printf("%s\n", "DONE");
        }else{
            genre->invalid_count++;
            printf("%s\n", "IGNORED");
        }
    }else{
        bookingeneric->lost_flag = 1;
        bookingenre->lost_flag = 1;
        genre->lost_count++;
        printf("%s\n", "DONE");
    }
    return;
}
// A function that reviews books 
void reviewbook(book_t *bookingenre, book_t * bookingeneric,int score){
    bookingenre->n_reviews++;
    bookingenre->sum_scores=bookingenre->sum_scores+score;
    bookingenre->avg=bookingenre->sum_scores/bookingenre->n_reviews;

    bookingeneric->n_reviews=bookingenre->n_reviews;// its the same book so it doesnt matter
    bookingeneric->sum_scores=bookingenre->sum_scores; 
    bookingeneric->avg=bookingenre->avg;
    return;
}
// A function that removes books 
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
// A function that removes a loan 
void removeloan(member_t *member,loan_t *loan){
    if (loan->next==member->loans && member->loans->next==loan){
        member->loans->next=member->loans;
        free(loan);
        member->loans->bid = -1;
        return;
    }
    member->loans->bid = loan->bid;
    loan_t *current = member->loans->next;
    while (current->next->bid != loan->bid){
        current = current->next;
    }
    member->loans->bid = -1;
    if (current->next == member->loans){
        return;
    }
    current->next=loan->next;
    free(loan);
    member->loans->bid=-1;
    return;
}

// A function that puts books to the display 
void display(library_t *lib) {
    if (SLOTS <= 0) {
        printf("IGNORED\n");
        return;
    }

// Free previous display and rest the slots and remainders 
    genre_t *genre = lib->genres;
    while (genre != NULL) {
        if (genre->display != NULL) {
            free(genre->display);
            genre->display = NULL;
        }
        genre->slots = 0;
        genre->remainder = 0;
        genre = genre->next;
    }
    int total_points = 0;
    int genre_count = 0;
    genre = lib->genres;
    while (genre != NULL) {
        int pts = computepoints(genre);   
        genre->remainder = pts;           
        total_points += pts;
        genre_count++;
        genre = genre->next;
    }

    if (total_points == 0) {
        printf("DONE\n");
        return;
    }
    int quota = total_points / SLOTS;
    genre = lib->genres;
    int seats_sum = 0;
    while (genre != NULL) {
        int pts = genre->remainder;
        if (quota > 0) {
            genre->slots = pts / quota;                     
            genre->remainder = pts - (genre->slots * quota);   
        } else {
            genre->slots = 0;
            genre->remainder = pts;
        }
        seats_sum += genre->slots;
        genre = genre->next;
    }

// Distribute remaining slots by repeatedly choosing the genre with max remainder
    int remaining = SLOTS - seats_sum;
    while (remaining > 0 && genre_count > 0) {
        genre_t *best = findmaxgenre(lib);
        if (best == NULL){break;} // if no more genres with remains are left stop
        if (best->remainder <= 0){break;} 
        best->slots += 1;
        best->remainder = -1;
        remaining--;
    }
    genre = lib->genres;
    while (genre != NULL) {
        if (genre->slots > 0) {
            int available = 0; // number of non lost boojs 
            book_t *book = genre->books;
            while (book != NULL) {
                if (book->lost_flag == 0) available++;
                book = book->next;
            }

            if (available == 0) {
                genre->slots = 0; 
            } else {
                if (genre->slots > available) genre->slots = available;
                genre->display = malloc(genre->slots * sizeof(book_t *));
                if (genre->display == NULL) {
                    genre->slots = 0;
                } else {
                    insertbookstodisplay(genre);
                }
            }
        }
        genre = genre->next;
    }
    printf("DONE\n");
}
 

// A function that computes the points of each genre 
int computepoints(genre_t *genre) {
    int points = 0;
    book_t *current = genre->books;
    while (current != NULL) {
        if (current->lost_flag == 0 && current->n_reviews > 0) {
            points += current->sum_scores;
        }
        current = current->next;
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


int main(int argc, char *argv[]){
    if (argc != 2) { // get the file name  make 
        fprintf(stderr, "Usage: %s <input-file>\n", argv[0]);
        return 1;
    }
    
    FILE *file = fopen(argv[1], "r");
    if (file == NULL){
        fprintf(stderr,"Error opening file: %s\n", argv[1]);
        return 1;
    }
    library_t *library=createLibrary();
    char buffer[256];
    while (fgets(buffer,sizeof(buffer),file)!=NULL){
        if (buffer[0] == '#' || buffer[0] == '\n' || buffer[0] == '\r'){ // remove comments 
            continue;
        }
        if (strncmp(buffer, "G", 1) == 0) {
            int gid;
            char name[NAME_MAX] ;
            sscanf(buffer+2,"%d \"%63[^\"]\"",&gid,name);// The 63 and later the 127 is to limit the reading of the name to the maxname
            genre_t *newnode= createGenre(gid,name);
            if (newnode==NULL){
                return 1;
            }
            insertGenretolist(library,newnode);
        } else if (strncmp(buffer, "BK", 2) == 0) {
            int bid;
            int gid;
            char name[NAME_MAX];
            sscanf(buffer+3,"%d %d \"%127[^\"]\"",&bid,&gid,name);
            if (searchbooktogeneric(library,bid)!=NULL){
                printf("IGNORED\n");
                continue;
            }
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
            printf("DONE\n");
        } else if (strncmp(buffer, "M", 1) == 0) {
            int sid;
            char name[NAME_MAX];
            sscanf(buffer + 2, "%d \"%63[^\"]\"", &sid, name);
            member_t *newmember= createmember(sid,name);
            if (newmember==NULL){
                return 1;
            }
            insertmembertolist(library,newmember);
        } else if (strncmp(buffer, "L", 1) == 0) {
            int sid;
            int bid;
            sscanf(buffer+2,"%d %d",&sid,&bid);
            Loanbook(library,sid,bid);
        } else if (strncmp(buffer, "R", 1) == 0) {
            int sid,bid,score=-1;
            char status[10];
            sscanf(buffer+2,"%d %d %d %s",&sid,&bid,&score,status); // No need to check if score is NA becuase the sscanf wont even read it 
            returnbook(library,sid,bid,score,status);
        } else if (strncmp(buffer, "D", 1) == 0) {
            display(library);
        } else if (strncmp(buffer, "PG", 2) == 0) {
            int gid;
            sscanf(buffer+3,"%d",&gid);
            genre_t *genre = searchgenre(library,gid);
                        if (genre == NULL) {
                printf("IGNORED\n");
            } else {
            printgenrebooks(library,gid);    
            }
        } else if (strncmp(buffer, "PM", 2) == 0) {
            int sid;
            sscanf(buffer+3,"%d",&sid);
            printmemberloan(library,sid);
        } else if (strncmp(buffer, "PD", 2) == 0) {
            printdisplay(library);
        } else if (strncmp(buffer, "S", 1) == 0) {
            sscanf(buffer + 2, "%d", &SLOTS);
            printf("DONE\n"); 
        }
    }
    return 0; 
}