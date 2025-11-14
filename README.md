# Project phase 1 
Στέφανος Τερζακης 
# Μεταγλώττιση with make file 
make
# Εκτέλεση με test3 (default)
make run
# Εκτέλεση με άλλο αρχείο
make run TESTFILE=test1
# Καθαρισμός
make clean
# Αλγόριθμος Display (D) - Μέθοδος Μέτρου και Μεγαλύτερων Υπολοίπων
points(g) = Σ(sum_scores) για κάθε βιβλίο του g με n_reviews > 0 και lost_flag = 0
Το sum_scores (όχι avg) μετράει τη συνολική δημοτικότητα.
### Βήμα 2: Υπολογισμός Quota (Μέτρο)
VALID = Σ(points) για όλα τα genres
quota = floor(VALID / SLOTS)
Το quota είναι η τιμή κάθε θέσης προβολής.
### Βήμα 3: Αρχική Κατανομή
seats(g) = floor(points(g) / quota)
remainder(g) = points(g) - seats(g) × quota
Κάθε genre αγοράζει θέσεις με τους πόντους του.
leftover_slots = SLOTS - Σ(seats(g))
Τα leftover_slots πρώτα genres παίρνουν +1 θέση
Τα genres με μεγαλύτερα υπόλοιπα παίρνουν τις εναπομείνασες θέσεις.




## Αρχεία
- `Library.c` - Υλοποίηση
- `Library.h` - Δηλώσεις
- `Makefile` - Μεταγλώττιση