#include <stdio.h>
#include <stdlib.h>

// Node structure: each element in the linked list
typedef struct Node {
    int value;           // Value stored in the node
    struct Node* next;   // Pointer to next node in the list
} Node;

// Linked list ADT (Abstract Data Type) with array for fast access
typedef struct {
    Node* head;      // First node in the list
    Node* tail;      // Last node in the list
    Node** nodes;    // Array storing pointers to all nodes
    int size;        // Current number of nodes
    int capacity;    // Max capacity of nodes array (doubles when full)
} LinkedListADT;

// Create a new node with given value
Node* createNode(int value) {
    Node* n = (Node*)malloc(sizeof(Node)); // Allocate memory for node
    n->value = value;                       // Store the value
    n->next = NULL;                         // Next is NULL initially
    return n;                               // Return new node pointer
}

// Create an empty linked list
LinkedListADT* createList() {
    LinkedListADT* list = (LinkedListADT*)malloc(sizeof(LinkedListADT));
    list->head = NULL;                       // No nodes yet
    list->tail = NULL;                       // No tail yet
    list->size = 0;                          // Size starts at 0
    list->capacity = 1024;                   // Initial array capacity
    list->nodes = (Node**)malloc(list->capacity * sizeof(Node*)); // Array for fast index access
    return list;
}

// Double the capacity of the nodes array if needed
void ensureCapacity(LinkedListADT* list) {
    if (list->size >= list->capacity) {          // Array is full
        list->capacity *= 2;                     // Double capacity
        list->nodes = (Node**)realloc(list->nodes, list->capacity * sizeof(Node*));
    }
}

// Insert a new value at a specific index
void insert(LinkedListADT* list, int index, int value) {
    if (index < 0 || index > list->size) {      // Check for invalid index
        printf("Invalid index\n");
        return;
    }

    ensureCapacity(list);                        // Make sure nodes array can hold new node
    Node* newNode = createNode(value);          // Create the new node

    if (index == 0) {                            // Insert at head
        newNode->next = list->head;             // Point new node to current head
        list->head = newNode;                   // Update head
        if (list->size == 0) list->tail = newNode; // First node is also tail
    } else if (index == list->size) {           // Insert at tail
        list->tail->next = newNode;             // Append at the end
        list->tail = newNode;                   // Update tail pointer
    } else {                                     // Insert in the middle
        Node* prev = list->nodes[index - 1];    // Get previous node
        newNode->next = prev->next;             // Point new node to next node
        prev->next = newNode;                   // Link previous node to new node
    }

    // Shift nodes in array to insert new node at correct index
    for (int i = list->size; i > index; i--) {
        list->nodes[i] = list->nodes[i - 1];
    }
    list->nodes[index] = newNode;               // Store new node in array
    list->size++;                               // Increase size of list
}

// Remove node at a specific index
void removeAt(LinkedListADT* list, int index) {
    if (index < 0 || index >= list->size) {    // Check for invalid index
        printf("Invalid index\n");
        return;
    }

    Node* del = list->nodes[index];            // Node to delete

    if (index == 0) {                          // Remove head
        list->head = list->head->next;         // Update head pointer
    } else {                                   // Remove middle or tail
        list->nodes[index - 1]->next = del->next; // Bypass the node to delete
    }

    if (index == list->size - 1) {             // If removing tail
        list->tail = (index > 0) ? list->nodes[index - 1] : NULL;
    }

    // Shift nodes in array to fill the gap
    for (int i = index; i < list->size - 1; i++) {
        list->nodes[i] = list->nodes[i + 1];
    }

    free(del);                                 // Free memory of deleted node
    list->size--;                              // Decrease list size
}

// Get value at a specific index
int get(LinkedListADT* list, int index) {
    if (index < 0 || index >= list->size) {   // Check index validity
        printf("Invalid index\n");
        return -1;
    }
    return list->nodes[index]->value;         // Return value from node
}

// Print the linked list in a readable format
void printList(LinkedListADT* list) {
    Node* curr = list->head;                  // Start from head
    while (curr) {
        printf("%d -> ", curr->value);        // Print current node value
        curr = curr->next;                     // Move to next node
    }
    printf("NULL\n");                          // End of list
}

// Main menu-driven program
int main() {
    LinkedListADT* list = createList();        // Create empty linked list
    int choice, index, value;

    while (1) {
        // Display menu
        printf("\n1.Insert  2.Remove  3.Get  4.Print  5.Size  0.Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter index and value: ");
                scanf("%d %d", &index, &value);
                insert(list, index, value);  // Insert node
                break;

            case 2:
                printf("Enter index to remove: ");
                scanf("%d", &index);
                removeAt(list, index);       // Remove node
                break;

            case 3:
                printf("Enter index: ");
                scanf("%d", &index);
                value = get(list, index);    // Get value
                if (value != -1)
                    printf("Value at index %d = %d\n", index, value);
                break;

            case 4:
                printList(list);             // Print list
                break;

            case 5:
                printf("Current size: %d\n", list->size); // Show size
                break;

            case 0:
                return 0;                    // Exit program

            default:
                printf("Invalid choice\n");
        }
    }
}
