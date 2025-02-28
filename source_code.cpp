#include <stdio.h>
#include <string.h>
#define MAX_CUSTOMERS 10000

// Structure to store customer details
struct Customer{
    int ID;             // Unique customer ID
    char name[50];      // Customer name
    char phone[15];     // Contact number
    int VIP;            // VIP status (1 = Yes, 0 = No)
};

// Structure to store table details
struct Table{
    int tableID;        // Unique table ID
    int capacity;       // Number of seats
    int Availablity;    // Availability status (1 = Available, 0 = Reserved)
    char name[30];      // Thematic name (e.g., "Cedar Table")
};

// Structure to store reservations
struct Reservation{
    int reservationID;  // Unique reservation ID
    int customerID;     // Links to a Customer
    int tableID;        // Links to a Table
    char date[15];      // Reservation date
    char time[10];      // Reservation time
};

// Function to add a new customer
void addCustomer(struct Customer customers[], int *count){
    if (*count >= MAX_CUSTOMERS) {
        printf("Customer list is full!\n");
        return;
    }
    
    struct Customer newCustomer;
    newCustomer.ID = *count + 1;
    
    printf("Enter customer name: ");
    scanf(" %49s", newCustomer.name);
    
    printf("Enter phone number: ");
    scanf(" %14s", newCustomer.phone);
    
    printf("Is the customer VIP? (1 for Yes, 0 for No): ");
    scanf("%d", &newCustomer.VIP);
    
    customers[*count] = newCustomer;
    (*count)++;
    printf("Customer added successfully!\n");
}

// Function to display all customers in alphabetical order
void displayCustomers(struct Customer customers[], int count) {
    if (count == 0) {
        printf("No customers found!\n");
        return;
    }
    
    // Simple bubble sort for alphabetical order
    for(int i=0; i<count-1; i++){
        for(int j=i+1; j<count; j++){
            if(strcmp(customers[i].name , customers[j].name)>0){
                struct Customer temp=customers[i];
                customers[i]=customers[j];
                customers[j]=temp;
            }
        }
    }
    printf("Customer List:\n");
    for (int i = 0; i < count; i++) {
        printf("ID: %d, Name: %s, Phone: %s, VIP: %s\n", 
               customers[i].ID, customers[i].name, customers[i].phone, 
               customers[i].VIP ? "Yes" : "No");
    }
}

    

int main(){
    struct Customer customers[MAX_CUSTOMERS];
    int customerCount=0;
    
    int choice;
    do {
        printf("\nCustomer Management Menu:\n");
        printf("1. Add Customer\n");
        printf("2. Display Customers\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        if(choice==1){
            addCustomer(customers, &customerCount);
        }
        if(choice == 2){
            displayCustomers(customers, customerCount);
        }
    }while(choice != 3);  
}