#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct Book {
    int bookID;
    char title[100];
    char author[100];
    int isAvailable;  // 1 if the book is available, 0 if not
    struct Book* next;
};
struct Queue {
    int front, rear;
    int capacity;
    int* queue;
};
struct Stack {
    int top;
    int capacity;
    int* stack;
};
void addBook(struct Book** head, int bookID, char* title, char* author);
void removeBook(struct Book** head, int bookID);
void displayBooks(struct Book* head);
struct Book* searchBook(struct Book* head, int bookID);

struct Queue* createQueue(int capacity);
int isQueueFull(struct Queue* queue);
int isQueueEmpty(struct Queue* queue);
void enqueue(struct Queue* queue, int customerID);
int dequeue(struct Queue* queue);

struct Stack* createStack(int capacity);
int isStackFull(struct Stack* stack);
int isStackEmpty(struct Stack* stack);
void push(struct Stack* stack, int bookID);
int pop(struct Stack* stack);

void borrowBook(struct Book** head, struct Queue* queue, int customerID);
void returnBook(struct Book** head, struct Stack* stack, int bookID);
int main() {
    struct Book* library = NULL;
    struct Queue* customerQueue = createQueue(5);
    struct Stack* returnStack = createStack(5);

    addBook(&library, 101, "C Programming", "Dennis Ritchie");
    addBook(&library, 102, "Data Structures", "Robert Lafore");
    addBook(&library, 103, "Algorithms", "Thomas Cormen");
    addBook(&library, 104, "Operating Systems", "Andrew Tanenbaum");
    addBook(&library, 105, "Database Management", "Ramez Elmasri");

    int choice, customerID, bookID;
    
    while (1) {
        printf("\nLibrary Management System\n");
        printf("1. Display Books\n");
        printf("2. Borrow Book\n");
        printf("3. Return Book\n");
        printf("4. Remove Book\n");
        printf("5. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                displayBooks(library);
                break;

            case 2:
                printf("Enter customer ID: ");
                scanf("%d", &customerID);
                borrowBook(&library, customerQueue, customerID);
                break;

            case 3:
                printf("Enter customer ID: ");
                scanf("%d", &customerID);
                printf("Enter book ID to return: ");
                scanf("%d", &bookID);
                returnBook(&library, returnStack, bookID);
                break;

            case 4:
                printf("Enter book ID to remove: ");
                scanf("%d", &bookID);
                removeBook(&library, bookID);
                break;

            case 5:
                printf("Exiting the system.\n");
                return 0;
        }
    }
}
void addBook(struct Book** head, int bookID, char* title, char* author) {
    struct Book* newBook = (struct Book*)malloc(sizeof(struct Book));
    newBook->bookID = bookID;
    strcpy(newBook->title, title);
    strcpy(newBook->author, author);
    newBook->isAvailable = 1;
    newBook->next = *head;
    *head = newBook;
}
void removeBook(struct Book** head, int bookID) {
    struct Book* temp = *head;
    struct Book* prev = NULL;
    if (temp != NULL && temp->bookID == bookID) {
        *head = temp->next;
        free(temp);
        printf("Book with ID %d removed from the library.\n", bookID);
        return;
    }
    while (temp != NULL && temp->bookID != bookID) {
        prev = temp;
        temp = temp->next;
    }
    if (temp == NULL) {
        printf("Book with ID %d not found.\n", bookID);
        return;
    }
    prev->next = temp->next;
    free(temp);
    printf("Book with ID %d removed from the library.\n", bookID);
}
void displayBooks(struct Book* head) {
    if (head == NULL) {
        printf("No books available.\n");
        return;
    }

    printf("\nBooks in the library:\n");
    while (head != NULL) {
        printf("Book ID: %d, Title: %s, Author: %s, Available: %s\n", 
                head->bookID, head->title, head->author, 
                (head->isAvailable ? "Yes" : "No"));
        head = head->next;
    }
}
struct Book* searchBook(struct Book* head, int bookID) {
    struct Book* temp = head;
    while (temp != NULL) {
        if (temp->bookID == bookID)
            return temp;
        temp = temp->next;
    }
    return NULL;
}
struct Queue* createQueue(int capacity) {
    struct Queue* queue = (struct Queue*)malloc(sizeof(struct Queue));
    queue->capacity = capacity;
    queue->front = queue->rear = -1;
    queue->queue = (int*)malloc(queue->capacity * sizeof(int));
    return queue;
}

int isQueueFull(struct Queue* queue) {
    return (queue->rear == queue->capacity - 1);
}

int isQueueEmpty(struct Queue* queue) {
    return (queue->front == -1);
}

void enqueue(struct Queue* queue, int customerID) {
    if (isQueueFull(queue)) {
        printf("Queue is full. Cannot process more customers.\n");
        return;
    }
    if (queue->front == -1) {
        queue->front = 0;
    }
    queue->rear++;
    queue->queue[queue->rear] = customerID;
    printf("Customer %d added to the queue.\n", customerID);
}

int dequeue(struct Queue* queue) {
    if (isQueueEmpty(queue)) {
        printf("Queue is empty.\n");
        return -1;
    }
    int customerID = queue->queue[queue->front];
    if (queue->front == queue->rear) {
        queue->front = queue->rear = -1;
    } else {
        queue->front++;
    }
    return customerID;
}
struct Stack* createStack(int capacity) {
    struct Stack* stack = (struct Stack*)malloc(sizeof(struct Stack));
    stack->capacity = capacity;
    stack->top = -1;
    stack->stack = (int*)malloc(stack->capacity * sizeof(int));
    return stack;
}

int isStackFull(struct Stack* stack) {
    return (stack->top == stack->capacity - 1);
}

int isStackEmpty(struct Stack* stack) {
    return (stack->top == -1);
}

void push(struct Stack* stack, int bookID) {
    if (isStackFull(stack)) {
        printf("Stack is full.\n");
        return;
    }
    stack->stack[++stack->top] = bookID;
}

int pop(struct Stack* stack) {
    if (isStackEmpty(stack)) {
        printf("Stack is empty.\n");
        return -1;
    }
    return stack->stack[stack->top--];
}
void borrowBook(struct Book** head, struct Queue* queue, int customerID) {
    if (isQueueFull(queue)) {
        printf("Customer queue is full.\n");
        return;
    }

    int bookID;
    printf("Enter book ID to borrow: ");
    scanf("%d", &bookID);
    
    struct Book* book = searchBook(*head, bookID);
    if (book == NULL) {
        printf("Book with ID %d not found.\n", bookID);
        return;
    }

    if (book->isAvailable == 0) {
        printf("Book with ID %d is not available.\n", bookID);
        return;
    }

    book->isAvailable = 0;
    enqueue(queue, customerID);
    printf("Customer %d has borrowed the book with ID %d.\n", customerID, bookID);
}

void returnBook(struct Book** head, struct Stack* stack, int bookID) {
    struct Book* book = searchBook(*head, bookID);
    if (book == NULL) {
        printf("Book with ID %d not found.\n", bookID);
        return;
    }

    book->isAvailable = 1;
    push(stack, bookID);
    printf("Book with ID %d has been returned.\n", bookID);
}









