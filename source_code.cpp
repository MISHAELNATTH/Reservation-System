#include <stdio.h>
#include <string.h>
#define MAX_CUSTOMERS 10000
#define CUSTOMER_FILE "customers.txt"
#define TABLE_FILE "Table.txt"
#define MAX_TABLE 500

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

// Save Table to the file
void saveTableFile(struct Table *table, int count) {
    FILE *file = fopen(TABLE_FILE, "w");
    if(!file){
        printf("Error opening file for writing!\n");
        return;
    }
    for(int i=0; i<count; i++) {
        fprintf(file,"%d %d %s %d\n", table[i].tableID, table[i].capacity, table[i].name, table[i].Availablity);
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

// Load Tables from a file
int loadTable(struct Table *table) {
    FILE *file = fopen(TABLE_FILE, "r");
    if (!file){
        return 0;
    }
    int count = 0;
    while(fscanf(file,"%d %d %29s %d\n", &table[count].tableID, &table[count].capacity, table[count].name, &table[count].Availablity)==4){
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

//ADD TABLES
void addTable(struct Table *table, int *count){
    if (*count>=MAX_TABLE){
        printf("No more Space!!");
        return;
    } 
    struct Table newTable;
    newTable.tableID = (*count>0) ? table[*count - 1].tableID + 1 : 1;
    
    printf("Enter Table Capacity: ");
    scanf("%d", &newTable.capacity);
    
    newTable.Availablity = 1;       // New tables are available by default
    
    printf("Enter Table Name: ");
    scanf("%29s", newTable.name);
    
    table[*count] = newTable;
    (*count)++;
    saveTableFile(table, *count);
    printf("Table added successfully!\n");
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

void modifyTable(struct Table *table, int count) {
    int T_ID;
    printf("Enter Table ID to modify: ");
    scanf("%d", &T_ID);
    
    for (int i = 0; i<count; i++){
        if (table[i].tableID == T_ID){
            printf("Enter new Capacity: ");
            scanf("%d", &table[i].capacity);
            printf("Enter new name: ");
            scanf("%29s", table[i].name);
            saveTableFile(table, count);
            printf("Table details updated successfully!\n");
            return;
        }
    }
    printf("Table ID not found!\n");
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

// Function to display all tables
void displayTables(struct Table *table, int count) {
    if (count == 0) {
        printf("No tables found!\n");
        return;
    }
    
    printf("Table List:\n");
    for (int i = 0; i < count; i++) {
        printf("ID: %d, Capacity: %d, Name: %s, Status: %s\n", table[i].tableID, table[i].capacity, table[i].name, table[i].Availablity ? "Available" : "Reserved");
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

// Function to delete a table
void deleteTable(struct Table *table, int *count) {
    int T_ID;
    printf("Enter Table ID to delete: ");
    scanf("%d", &T_ID);
    
    for (int i=0; i<*count; i++) {
        if (table[i].tableID == T_ID) {
            for (int j=i; j<*count-1; j++) {
                table[j] = table[j + 1];
            }
            (*count)--;
            saveTableFile(table, *count);
            printf("Table deleted successfully!\n");
            return;
        }
    }
    printf("Table ID not found!\n");
}

int main(){
    struct Customer customers[MAX_CUSTOMERS];
    int customerCount = loadCustomersFromFile(customers);

    struct Table table[MAX_TABLE];
    int TableCount = loadTable(table);

    int menu_choice;
    int choice;
    int choice_T;

    do{
        printf("Enter 0 to exit\n");
        printf("1. Customer Management Menu:\n");
        printf("2. Table Management Menu:\n");
        printf("Enter your Menu choice: ");
        scanf("%d", &menu_choice);

        if(menu_choice==1){
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
        }
    
        else if(menu_choice==2){
            do {
                printf("\nTable Management Menu:\n");
                printf("1. Add Table\n");
                printf("2. Display Table\n");
                printf("3. Modify Table details\n");
                printf("4. Remove Table\n");
                printf("5. Exit\n");
                printf("Enter your choice: ");
                scanf("%d", &choice_T);
                
                if(choice_T==1){
                    addTable(table, &TableCount);
                }  
                if(choice_T == 2){
                    displayTables(table, TableCount);
                }
                if(choice_T==3){
                    modifyTable(table, TableCount);
                }
                if(choice_T == 4){
                    deleteTable(table, &TableCount);
                }
                if(choice_T==5){
                    printf("Exiting......\n");
                }
            }while(choice_T!=5);  
        
            
        }
    }while(menu_choice!=0);
}