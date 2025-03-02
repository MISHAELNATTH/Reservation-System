#include <stdio.h>
#include <string.h>
#define MAX_CUSTOMERS 10000
#define CUSTOMER_FILE "customers.txt"

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

// Function to save customers to a file
void saveCustomersToFile(struct Customer *customers, int count) {
    FILE *file = fopen(CUSTOMER_FILE, "w");
    if(!file){
        printf("Error opening file for writing!\n");
        return;
    }
    for(int i=0; i<count; i++) {
        fprintf(file,"%d %s %s %d\n", customers[i].ID, customers[i].name, customers[i].phone, customers[i].VIP);
    }
    fclose(file);
}

// Function to load customers from a file
int loadCustomersFromFile(struct Customer *customers) {
    FILE *file = fopen(CUSTOMER_FILE, "r");
    if (!file) return 0;
    
    int count = 0;
    while (fscanf(file, "%d %49s %14s %d", &customers[count].ID, customers[count].name, customers[count].phone, &customers[count].VIP)==4){
        count++;
    }
    fclose(file);
    return count;
}

// Function to add a new customer
void addCustomer(struct Customer *customers, int *count){
    if (*count>=MAX_CUSTOMERS){
        printf("Customer list is full!\n");
        return;
    } 
    struct Customer newCustomer;
    newCustomer.ID = (*count >0) ? customers[*count - 1].ID + 1 : 1;
    
    printf("Enter customer name: ");
    scanf("%49s", newCustomer.name);
    
    printf("Enter phone number: ");
    scanf("%14s", newCustomer.phone);
    
    printf("Is the customer VIP? (1 for Yes, 0 for No): ");
    scanf("%d", &newCustomer.VIP);
    
    customers[*count] = newCustomer;
    (*count)++;
    saveCustomersToFile(customers, *count);
    printf("Customer added successfully!\n");
}

// Function to modify customer details
void modifyCustomer(struct Customer *customers, int count) {
    int ID;
    printf("Enter Customer ID to modify: ");
    scanf("%d", &ID);
    
    for (int i = 0; i < count; i++){
        if (customers[i].ID == ID){
            printf("Enter new name: ");
            scanf("%49s", customers[i].name);
            printf("Enter new phone number: ");
            scanf("%14s", customers[i].phone);
            printf("Is the customer VIP? (1 for Yes, 0 for No): ");
            scanf("%d", &customers[i].VIP);
            saveCustomersToFile(customers, count);
            printf("Customer details updated successfully!\n");
            return;
        }
    }
    printf("Customer ID not found!\n");
}

// Function to display all customers in alphabetical order
void displayCustomers(struct Customer *customers, int count){
    if (count == 0){
        printf("No customers found!\n");
        return;
    }

//alphabetical Sorting
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
        printf("ID: %d, Name: %s, Phone: %s, VIP: %s\n", customers[i].ID, customers[i].name, customers[i].phone, customers[i].VIP? "Yes" : "No");
    }
}

// Function to delete a customer
void deleteCustomer(struct Customer *customers, int *count){
    int ID;
    printf("Enter Customer ID to delete: ");
    scanf("%d", &ID);
    
    for (int i = 0; i < *count; i++) {
        if (customers[i].ID == ID) {
            for (int j = i; j < *count-1; j++) {
                customers[j] = customers[j + 1];
            }
            (*count)--;
            saveCustomersToFile(customers,*count);
            printf("Customer deleted successfully!\n");
            return;
        }
    }
    printf("Customer ID not found!\n");
}
    

int main(){
    struct Customer customers[MAX_CUSTOMERS];
    int customerCount = loadCustomersFromFile(customers);
    
    int choice;
    do {
        printf("\nCustomer Management Menu:\n");
        printf("1. Add Customer\n");
        printf("2. Display Customers\n");
        printf("3. Modify customer details.\n");
        printf("4. Delete Customer\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        if(choice==1){
            addCustomer(customers, &customerCount);
        }  
        if(choice == 2){
            displayCustomers(customers, customerCount);
        }
        if(choice==3){
            modifyCustomer(customers, customerCount);
        }
        if(choice == 4){
            deleteCustomer(customers, &customerCount);
        }
        if(choice==5){
            printf("Exiting......\n");
        }
    }while(choice!=5);  

    return 0;   
}
