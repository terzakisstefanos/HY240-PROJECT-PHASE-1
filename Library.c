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
//-----------create member activity-----------
    newmember->activity = malloc(sizeof(Activity_t));
    if (newmember->activity == NULL) {
        free(newmember->loans);
        free(newmember);
        printf("IGNORED\n");
        return NULL;
    }
    newmember->activity->sid = sid;
    newmember->activity->loans_count = 0;
    newmember->activity->reviews_count = 0;
    newmember->activity->score_sum = 0;
    newmember->activity->next = NULL;
    strcpy(newmember->name, name);
    newmember->next = NULL;
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
    // add the member activity to the list 
    Activity_t *act = newmember->activity;
    if (act != NULL) {
        act->next = head->activity;
        head->activity = act;
    }
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
// A function that searches for a specific loan
member_t *find_loan(library_t *Lib, int bid) {
    if (Lib == NULL) return NULL;
    member_t *member = Lib->members;
    while (member != NULL) {
        if (member->loans != NULL) {
            if (searchloan(member, bid) != NULL){return member;}
        }
        member = member->next;
    }
    return NULL;
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
    member_t *member= searchmember(LIB,sid); // check if everything exists 
    if (member==NULL){
        printf("%s\n","IGNORED");
        return;
    }
    loan_t *loaned=find_loan(member,bid);
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
    if (member->activity != NULL) {
        member->activity->loans_count++;
    }
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
        if (book->lost_flag==0){
            printf("%d, %d\n", book->bid, book->avg);
        }
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
    if (strcmp(status,"ok")==0){
        if (score >= 0 && score <= 10){// If score is within appropriate values 
            removeloan(member,loan);
            reviewbook(bookingenre ,bookingeneric,score);
            removebook(genre,bookingenre);
            insertbooktogenre(genre,bookingenre);
            if (member->activity != NULL) {
                member->activity->reviews_count++;
                member->activity->score_sum += score;
            }
            printf("%s\n", "DONE");
        }else if (score==-1) {
            removeloan(member,loan);
            printf("%s\n", "DONE");
        }else{
            genre->invalid_count++;
            printf("%s\n", "IGNORED");
        }
    }else{
        removeloan(member,loan);
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
        best->remainder--;
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

//**********************************PHASE B**********************************

// Helper function that returns the height of a node 
int height (bookn_t* node){
    if (node==NULL){
        return -1;
    }
    return node->height;
}
// Helper function to calculate the bigger of 2 ints 
int max(int a, int b) {
    return (a > b)? a:b; // If a > b, return a. Otherwise, return b.
}
// A function that creates a new bookindex node 
bookn_t *createBookn(char * title , book_t * book){
    bookn_t * newbook = malloc (sizeof(bookn_t));
    if (newbook == NULL) {
      printf("%s\n", "IGNORED");
      return NULL;
    }
    newbook->book=book;
    newbook->lc=NULL;
    newbook->rc=NULL;
    newbook->height=0;
    strcpy(newbook->title,title);
    return newbook;
}
// A function to make a right rotation
bookn_t *RightRotate(bookn_t *parent) {
    bookn_t *child = parent->lc;      
    bookn_t *temp  = child->rc;    
    child->rc = parent;               // The Child takes the Parent as its new right child
    parent->lc = temp;                // The Parent takes the Temp node as its new left child
    parent->height = 1 + max(height(parent->lc), height(parent->rc));
    child->height  = 1 + max(height(child->lc), height(child->rc));
    return child;
}
// Function to make a left rotation
bookn_t *LeftRotate(bookn_t *parent) {
    bookn_t *child = parent->rc;      
    bookn_t *temp  = child->lc;       
    child->lc = parent;               // The Child takes the Parent as its new left child
    parent->rc = temp;                // The Parent takes the Temp node as its new right child
    parent->height = 1 + max(height(parent->lc), height(parent->rc));
    child->height  = 1 + max(height(child->lc), height(child->rc));
    return child;
}
// A helper function to get balance factor of a node 
int getBalance(bookn_t *node) {
    if (node == NULL)
        return 0;
    return height(node->lc)-height(node->rc);
}

bookn_t* insertBookIndex(bookn_t* node, char* title, book_t* book){
    if (node==NULL){
        return createBookn(title,book);
    }
    int cmp= strcmp(title,node->title);
    if (cmp>0){
        node->rc=insertBookIndex(node->rc,title,book);
    } else if (cmp < 0) {
        node->lc = insertBookIndex(node->lc, title, book);
    } else {
        printf("%s\n", "IGNORED");
        return NULL;
    }
    node->height = 1 + max(height(node->lc), height(node->rc));
    int balance = getBalance(node);
    // Case 1: Left Left (Single Rotation)
    if (balance > 1 && strcmp(title, node->lc->title) < 0) {
        return RightRotate(node);
    }

    // Case 2: Right Right (Single Rotation)
    if (balance < -1 && strcmp(title, node->rc->title) > 0) {
        return LeftRotate(node);
    }
    // Case 3: Left Right (Double Rotation)
    if (balance > 1 && strcmp(title, node->lc->title) > 0) {
        node->lc = LeftRotate(node->lc);
        return RightRotate(node);
    }

    // Case 4: Right Left (Double Rotation)
    if (balance < -1 && strcmp(title, node->rc->title) < 0) {
        node->rc = RightRotate(node->rc);
        return LeftRotate(node);
    }

    // if no returns are needed then return the node 
    return node;
}
// A helper function that searches for a title in the AVL tree
bookn_t *searchBookIndex(bookn_t *root, char *title) {
    if (root == NULL) {
        return NULL;
    }
    int cmp = strcmp(title, root->title);
    if (cmp == 0) {
        return root; 
    } else if (cmp < 0) {
        return searchBookIndex(root->lc, title);
    } else {
        return searchBookIndex(root->rc, title);
    }
}

// A helper function that checks if a > b
int isGreater(book_t *a, book_t *b) {
    if (a == NULL || b == NULL) return 0;
    
    if (a->avg > b->avg) {
        return 1; 
    }
    if (a->avg == b->avg && a->bid < b->bid) {
        return 1; 
    }
    return 0;
}
// A helper function that swaps heap[i],heap[j]
void swap(Heap_t *heap, int i, int j) {
    book_t *temp = heap->heap[i];
    heap->heap[i] = heap->heap[j];
    heap->heap[j] = temp;

    heap->heap[i]->heap_pos = i; // we also swap the heap_pos
    heap->heap[j]->heap_pos = j;
}
// A function that restores max-heap property starting from i going downwards
void heapify_down(Heap_t *heap, int i) {
    while (1) {
        int left = 2 * i;
        int right = 2 * i + 1;
        int largest = i;

        // If one of the childs are larger than the parent then replace him
        if (left <= (int)heap->size && isGreater(heap->heap[left], heap->heap[largest])) {
            largest = left;
        }
        if (right <= (int)heap->size && isGreater(heap->heap[right], heap->heap[largest])) {
            largest = right;
        }
        // if not do nothing 
        if (largest == i) {
            break;
        }
        swap(heap, i, largest);
        i = largest;
    }
}

// A function that restores max-heap property starting from i going upwards
void heapify_up(Heap_t *heap, int i) {
    while (i > 1) {
        int parent = i / 2;

        // If current node is not greater than parent
        if (!isGreater(heap->heap[i], heap->heap[parent])) {
            break;
        }
        // Otherwise swap with parent and move up
        swap(heap, i, parent);
        i = parent;
    }
}
int heap_insert(Heap_t *heap, book_t *book) {
    if (heap->size + 1 >= 64) {
        printf("%s\n", "IGNORED");
        return 1; 
    }
    heap->size++;
    int i = heap->size;
    heap->heap[i] = book;     
    book->heap_pos = i;        
    heapify_up(heap, i);
    return 0;
}
void heap_update_key(Heap_t *heap, book_t *book) {
    int pos = book->heap_pos;
    if (pos <= 0 || pos > heap->size){
        printf("%s\n", "IGNORED");
        return;
    }
    heapify_up(heap, pos);// if up didnt work try down 
    if (pos == book->heap_pos) {
        heapify_down(heap, pos);
    }
}
void heap_remove(Heap_t *heap, book_t *book) {
    if (heap == NULL){
        printf("%s\n", "IGNORED");
        return;
    }
    int pos = book->heap_pos;
    if (pos <= 0 || pos >heap->size) {\
        printf("%s\n", "IGNORED");
        return; 
    }
    int last = heap->size;
    if (pos != last) {
        heap->heap[pos] = heap->heap[last];
        heap->heap[pos]->heap_pos = pos;
    }
    heap->heap[last] = NULL;
    heap->size--;
    book->heap_pos = 0; // mark as not in heap anymore
    if (pos > (int)heap->size) {
        return;
    }
    heapify_up(heap, pos);// fix the heap structure 
    heapify_down(heap, pos);
}
// A helper that returns the score of a node 
int activity_score( Activity_t *a) {
    return a->loans_count + a->reviews_count;
}
// A function that prints the activity list
void print_activity_list(library_t *lib) {
    if (lib == NULL || lib->activity == NULL) {
        printf("%s\n", "IGNORED");
        return;
    }
    int count = 0;
    Activity_t *cur = lib->activity;
    while (cur != NULL) {
        count++;
        cur = cur->next;
    }
    if (count == 0){ 
        printf("%s\n", "IGNORED");
        return;
    }
    Activity_t **arr = malloc(count * sizeof(Activity_t *));
    if (arr == NULL) {
        printf("%s\n", "IGNORED");
        return;
    }
    cur = lib->activity;
    for (int i = 0; i < count; i++) {
        arr[i] = cur;
        cur = cur->next;
    }
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            int score_i = activity_score(arr[i]);
            int score_j = activity_score(arr[j]);
            if (score_j > score_i ||
                (score_j == score_i && arr[j]->sid < arr[i]->sid)) {
                Activity_t *tmp = arr[i];
                arr[i] = arr[j];
                arr[j] = tmp;
            }
        }
    }
    printf("Active Members:\n");
    for (int i = 0; i < count; i++) {
        Activity_t *a = arr[i];
        member_t *m = searchmember(lib, a->sid);
        if (m != NULL) {
            printf("%d %s loans=%d reviews=%d\n",
                   a->sid,
                   m->name,
                   a->loans_count,
                   a->reviews_count);
        }
    }

    free(arr);
}
void free_library(library_t *lib) {
    if (lib == NULL) return;

    genre_t *gcurrent = lib->genres;
    while (gcurrent != NULL) {
        genre_t *gtemp = gcurrent;
        gcurrent = gcurrent->next;
        book_t *bcurrent = gtemp->books;
        while (bcurrent != NULL) {
            book_t *btemp = bcurrent;
            bcurrent = bcurrent->next;
            free(btemp);
        }

        free(gtemp);
    }
    lib->genres = NULL;

    book_t *gbcur = lib->books;
    while (gbcur != NULL) {
        book_t *gbtemp = gbcur;
        gbcur = gbcur->next;
        free(gbtemp);
    }
    lib->books = NULL;

    member_t *mcurrent = lib->members;
    while (mcurrent != NULL) {
        member_t *temp = mcurrent;
        mcurrent = mcurrent->next;
        loan_t *sentinel = temp->loans;
        if (sentinel != NULL) {
            loan_t *loancurrent = sentinel->next;
            while (loancurrent != NULL && loancurrent != sentinel) {
                loan_t *loantemp = loancurrent;
                loancurrent = loancurrent->next;
                free(loantemp);
            }
            free(sentinel);
        }
        free(temp);// I free activity further down 
    }
    lib->members = NULL;
    Activity_t *acur = lib->activity;
    while (acur != NULL) {
        Activity_t *atemp = acur;
        acur = acur->next;
        free(atemp);
    }
    lib->activity = NULL;
    if (lib->RecHeap != NULL) {
        free(lib->RecHeap);
        lib->RecHeap = NULL;
    }
    if (lib->Root != NULL) {
        free_bookindex(lib->Root);
        lib->Root = NULL;
    }
    free(lib);
}
void free_bookindex(bookn_t *root) {
    if (root == NULL) return;
    free_bookindex(root->lc);
    free_bookindex(root->rc);
    free(root);
}
void print_stats(library_t *lib) {
    if (lib == NULL) {
        printf("Stats:\n");
        return;
    }

    int book_count = 0;
    int member_count = 0;
    int loan_count = 0;
    long long total_scores = 0;
    long long total_reviews = 0;

    book_t *book_current = lib->books;
    while (book_current != NULL) {
        if (book_current->lost_flag == 0) {
            book_count++;
            total_scores += book_current->sum_scores;
            total_reviews += book_current->n_reviews;
        }
        book_current = book_current->next_global;
    }

    member_t *member_current = lib->members;
    while (member_current != NULL) {
        member_count++;

        loan_t *sentinel = member_current->loans;
        if (sentinel != NULL) {
            loan_t *loan_current = sentinel->next;
            while (loan_current != NULL && loan_current != sentinel) {
                loan_count++;
                loan_current = loan_current->next;
            }
        }

        member_current = member_current->next;
    }

    int global_avg = 0;
    if (total_reviews > 0) {
        global_avg = (int)(total_scores / total_reviews);
    }

    printf("Stats: books=%d members=%d loans=%d avg=%d\n",
           book_count,
           member_count,
           loan_count,
           global_avg);
}
int main(int argc, char *argv[]){
    if (argc != 2) {
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
        if (buffer[0] == '#' || buffer[0] == '\n' || buffer[0] == '\r'){
            continue;
        }
        if (strncmp(buffer, "G", 1) == 0) {
            int gid;
            char name[NAME_MAX] ;
            sscanf(buffer+2,"%d \"%63[^\"]\"",&gid,name);
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
            library->Root = insertBookIndex(library->Root, newbook->title, newbook);
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
            sscanf(buffer+2,"%d %d %d %s",&sid,&bid,&score,status);
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
        } else if (strncmp(buffer, "F", 1) == 0) {
            char title[NAME_MAX];
            sscanf(buffer + 2, "\"%127[^\"]\"", title);
            bookn_t *node = searchBookIndex(library->Root, title);
            if (node == NULL || node->book == NULL || node->book->lost_flag == 1) {
                printf("IGNORED\n");
            } else {
                printf("Book %d \"%s\" avg=%d\n",
                       node->book->bid,
                       node->book->title,
                       node->book->avg);
            }
        } else if (strncmp(buffer, "TOP", 3) == 0) {
            int k;
            sscanf(buffer + 4, "%d", &k);
            print_top_k(library, k);
        } else if (strncmp(buffer, "AM", 2) == 0) {
            print_activity_list(library);
        } else if (strncmp(buffer, "U", 1) == 0) {
            int bid;
            char new_title[NAME_MAX];
            sscanf(buffer + 2, "%d \"%127[^\"]\"", &bid, new_title);
            update_book_title(library, bid, new_title);
        } else if (strncmp(buffer, "X", 1) == 0) {
            print_stats(library);
        } else if (strncmp(buffer, "BF", 2) == 0) {
            free_library(library);
            printf("DONE\n");
            fclose(file);
            return 0;
        }
    }
    fclose(file);
    free_library(library);
    return 0; 
}