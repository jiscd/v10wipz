#define _CRT_SECURE_NO_WARNINGS
#define n 8
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct library
{
    char author[20];
    char name[20];
    int year;
    int pages;
    int cost;
    struct library* next;
}book;


void print_books(book*);
void fprint_books(FILE* file, book* head);
int scan_books(FILE* file, book** head, int k);
int fscan_books(FILE* file, book** head);
void insert_book(book** head, book* new_book);
void remove_book(book** head, book* node);
void remove_small_books(book** head);
void free_library(book** head);
void print_newest_five(FILE* file,  book* head);



int main()
{
    FILE* file;
    file = fopen("file.txt", "r");
    book* head = NULL;
    int k = 0;

    if (file == NULL)
        return 0;
    if (fscan_books(file, &head))
        return 0;
    fclose(file);

    printf("How manny book you want to add?\n");
    if(!scanf("%i",&k))
        return 0;
    if(k > 0)
        printf("Enter \"Author Title Year Pages Price\" of %i books:\n", k);
    if(scan_books(file, &head, k))
        return 0;

    printf("All books:\n");
    print_books(head);

    file = fopen("output.txt", "w");
    if (file == NULL)
        return 0;
    fprintf(file, "All books:\n");
    fprint_books(file, head);

    printf("Newest 5 books:\n");
    fprintf(file, "Newest 5 books:\n");
    print_newest_five(file, head);

    remove_small_books(&head);

    printf("Books with >50 pages:\n");
    fprintf(file, "Books with >50 pages:\n");
    fprint_books(file, head);
    print_books(head);



    fclose(file);

    free_library(&head);

    fclose(file);
}

void print_books(book* head) {
    book* current = head;
    while (current)
    {
        printf("%s %s %d %d %d\n", current->author, current->name,
            current->year, current->pages, current->cost);
        current = current->next;
    }
    printf("\n");
}

void fprint_books(FILE* file, book* head) {
    book* current = head;
    while (current)
    {
        fprintf(file, "%s %s %d %d %d\n", current->author, current->name,
            current->year, current->pages, current->cost);
        current = current->next;
    }
    fprintf(file, "\n");
}

int fscan_books(FILE* file, book** head) {
    book* node;
    for (int i = 0; i < n; i++) {
        node = (book*)malloc(sizeof(book));
        if (!node)
        {
            return 1;
        }
        node->next = NULL;
        if(!fscanf(file, "%s %s %d %d %d", node->author, node->name,
            &node->year, &node->pages, &node->cost))
            return 1;
        insert_book(head, node);
    }
    return 0;
}

int scan_books(FILE* file, book** head, int k) {
    book* node;
    for (int i = 0; i < k; i++) {
        node = (book*)malloc(sizeof(book));
        if (!node)
        {
            return 1;
        }
        node->next = NULL;
        if (!scanf("%s %s %d %d %d", node->author, node->name,
            &node->year, &node->pages, &node->cost))
            return 1;
        insert_book(head, node);
    }
    return 0;
}

void insert_book(book** head, book* new_book)
{
    if (!*head || strcmp((*head)->author, new_book->author) < 0)
    {
        new_book->next = *head;
        *head = new_book;
        return;
    }

    book* current = *head;
    while (current->next && strcmp(current->next->author, new_book->author) > 0)
    {
        current = current->next;
    }
    new_book->next = current->next;
    current->next = new_book;
}
void remove_book(book** head, book* node)
{
    if (!*head || !node)
        return;
    book* temp = NULL;

    if (*head == node)
    {
        temp = *head;
        *head = (*head)->next;
        free(temp);
        return;
    }

    book* current = *head;
    while (current)
    {
        if (current->next == node)
        {
            temp = current->next;
            current->next = current->next->next;
            free(temp);
            return;
        }
        current = current->next;
    }
}

void remove_small_books(book** head)
{
    book* current = *head;
    
    while (current)
    {
        if (current->pages < 50)
        {
            book* temp = current->next;
            remove_book(head, current);
            current = temp;
            continue;
        }
        current = current->next;
    }
}

void free_library(book** head)
{
    book* current = *head;
    while (current)
    {
        book* temp = current;
        current = current->next;
        free(temp);
    }
}

void print_newest_five(FILE* file, book* head)
{
    book* current = head;
    int max = current->year, prev_max;
    while (current)
    {  

        if (current->year > max)
            max = current->year;
        current = current->next;
    }
    for (int i = 0; i < 5;)
    {
        while (max > 0)
        {
            current = head;

            while (current)
            {
                if (current->year == max)
                {
                    printf("%s %s %d %d %d\n", current->author, current->name,
                        current->year, current->pages, current->cost);
                    fprintf(file, "%s %s %d %d %d\n", current->author, current->name,
                        current->year, current->pages, current->cost);
                    i++;
                    max--;
                    break;
                }
                current = current->next;
            }
            max--;
            break;
        }
    }
    printf("\n");
    fprintf(file, "\n");
}