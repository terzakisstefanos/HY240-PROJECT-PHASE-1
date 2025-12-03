#ifndef LIBRARY_H
#define LIBRARY_H

#include <stddef.h>

#define TITLE_MAX 128
#define NAME_MAX 64


extern int SLOTS;


typedef struct loan {
  int sid;           
  int bid;           
  struct loan *next; 
} loan_t;


typedef struct book {
  int bid; 
  int gid; 
  char title[TITLE_MAX];
  int sum_scores; 
  int n_reviews;
  int avg;        
  int lost_flag;
  int heap_pos;
  struct book *prev;
  struct book *next;
  struct book *next_global; 
} book_t;

typedef struct member {
  int sid;
  char name[NAME_MAX];
  Activity_t *activity;
  loan_t *loans;
  struct member *next;
} member_t;

typedef struct genre {
  int gid; 
  char name[NAME_MAX];
  book_t *books;
  int lost_count;
  int invalid_count;
  int slots;        
  book_t **display; 
  int remainder;
  struct genre *next;
} genre_t;

typedef struct library {
  genre_t *genres;   
  member_t *members; 
  book_t *books; 
  Heap_t *RecHeap;
  Activity_t *activity;
  bookn_t *Root;
} library_t;

typedef struct BookNode {
  char title[TITLE_MAX];
  book_t *book;
  struct BookNode *lc;
  struct BookNode *rc;
  int height;
} bookn_t;

typedef struct RecHeap {
  book_t *heap[64];
  int size ;
} Heap_t;

typedef struct MemberActivity {
  int sid;
  int loans_count;
  int reviews_count;
  int score_sum;
  struct MemberActivity *next;
} Activity_t;

// creation
library_t *createLibrary(void);
genre_t *createGenre(int gid, char *name);
member_t *createmember(int sid, char *name);
book_t *createBook(int bid, int gid, char *title);

// insertion
void insertGenretolist(library_t *head, genre_t *node);
void insertbooktogenre(genre_t *head, book_t *newbook);
void insertbooktogeneric(library_t *head, book_t *book);
void insertmembertolist(library_t *head, member_t *newmember);
void insertbookstodisplay(genre_t *genre);

// search
genre_t *searchgenre(library_t *head, int gid);
book_t *searchbook(genre_t *head, int bid);
book_t *searchbooktogeneric(library_t *Lib, int bid);
member_t *searchmember(library_t *Lib, int sid);
member_t *find_loan(library_t *Lib, int bid);
loan_t *searchloan(member_t *head, int bid);

// actions
void reviewbook(book_t *bookingenre, book_t *bookingeneric, int score);
void returnbook(library_t *lib, int sid, int bid, int score, char *status);
void Loanbook(library_t *LIB, int sid, int bid);
void display(library_t *lib);

// removal helpers
void removebook(genre_t *genre, book_t *book);
void removeloan(member_t *member, loan_t *loan);

// printing
void printmemberloan(library_t *lib, int sid);
void printdisplay(library_t *Lib);
void printgenrebooks(library_t *lib, int gid);

// utility/calculation helpers
int computepoints(genre_t *genre);
genre_t *findmaxgenre(library_t *Lib);

// Phase B: heap helpers
int isGreater(book_t *a, book_t *b);
void swap(Heap_t *heap, int i, int j);
void heapify_down(Heap_t *heap, int i);
void heapify_up(Heap_t *heap, int i);
#endif