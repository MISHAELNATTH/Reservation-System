/*
   Reservation System Management Program
   ---------------------------------------
   This program implements a table reservation booking system with three modules:
   1. Customer Management:
      - Add, modify, delete, display customers (sorted alphabetically)
      - Save customer data to "customers.txt"
   2. Table Management:
      - Add, modify, delete, display tables (available/reserved)
      - Save table data to "Table.txt"
   3. Reservation Management:
      - Book a table (creates a reservation)
      - Cancel a reservation
      - Display all reservations
      - Display customers with reservations
      - Prevent overbooking (checks table availability)
      - Save reservation data to "reservations.txt"

   Developed for the C course project.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Define constants
#define MAX_CUSTOMERS 10000
#define MAX_TABLE 500
#define MAX_RESERVATIONS 1000
#define CUSTOMER_FILE "customers.txt"
#define TABLE_FILE "Table.txt"
#define RESERVATION_FILE "reservations.txt"

// Structure definitions

struct Customer {
    int ID;             // Unique customer ID
    char name[50];      // Customer name
    char phone[15];     // Contact number
    int VIP;            // VIP status (1 = Yes, 0 = No)
};

struct Table {
    int tableID;        // Unique table ID
    int capacity;       // Number of seats
    int Availablity;    // Availability status (1 = Available, 0 = Reserved)
    char name[30];      // Table name or theme
};

struct Reservation {
    int reservationID;  // Unique reservation ID
    int customerID;     // Link to a Customer
    int tableID;        // Link to a Table
    char date[15];      // Reservation date (YYYY-MM-DD)
    char time[10];      // Reservation time (HH:MM)
};

//Procedure to Extrate Date
void extrate_date(char *date, int *year, int *month, int *day) {
  sscanf(date, "%d-%d-%d", year, month, day);
}


// Function to validate date input
int isValidDate(char *date) {
    int inputYear, inputMonth, inputDay;
    int currentYear, currentMonth, currentDay;
    
    extrate_date(date, &inputYear, &inputMonth, &inputDay);
    
    if (inputYear < currentYear) return 0;
    if (inputYear == currentYear && inputMonth<currentMonth) return 0;
    if (inputYear == currentYear && inputMonth==currentMonth && inputDay<currentDay) return 0;
    
    return 1; 
  }
  
// Customer Management Functions

// To Save customers to file
void saveCustomersToFile(struct Customer *customers, int count){
    FILE *file = fopen(CUSTOMER_FILE, "w");
    if (!file){
        printf("Error opening customer file for writing!\n");
        return;
    }
    for(int i=0; i < count; i++){
        fprintf(file, "%d %s %s %d\n", customers[i].ID,customers[i].name,customers[i].phone,customers[i].VIP);
    }
    fclose(file);
}

// Load customers from file
int loadCustomersFromFile(struct Customer *customers){
    FILE *file=fopen(CUSTOMER_FILE, "r");
    if(!file)return 0;

    int count = 0;
    while(fscanf(file,"%d %49s %14s %d", &customers[count].ID, customers[count].name, customers[count].phone, &customers[count].VIP)==4){
        count++;
    }
    fclose(file);
    return count;
}

// Add a new customer
void addCustomer(struct Customer *customers, int *count){
    if(*count>=MAX_CUSTOMERS){
        printf("Customer list is full!\n");
        return;
    }

    struct Customer newCustomer;
    newCustomer.ID = (*count > 0) ? customers[*count - 1].ID + 1 : 1;

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

// Modify customer details by ID
void modifyCustomer(struct Customer *customers, int count) {
    int ID;
    printf("Enter Customer ID to modify: ");
    scanf("%d", &ID);

    for (int i=0; i < count; i++){
        if (customers[i].ID == ID) {
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

// Delete a customer by ID
void deleteCustomer(struct Customer *customers, int *count){
    int ID;
    printf("Enter Customer ID to delete: ");
    scanf("%d", &ID);

    for(int i=0; i < *count; i++){
        if(customers[i].ID == ID){
            for (int j = i; j < *count - 1; j++){
                customers[j] = customers[j + 1];
            }
            (*count)--;
            saveCustomersToFile(customers, *count);
            printf("Customer deleted successfully!\n");
            return;
        }
    }
    printf("Customer ID not found!\n");
}

// Display all customers sorted alphabetically by name
void displayCustomers(struct Customer *customers, int count){
    if(count == 0){
        printf("No customers found!\n");
        return;
    }
    // Bubble sort for alphabetical order
    for(int i=0; i < count - 1; i++){
        for(int j = i + 1; j < count; j++){
            if (strcmp(customers[i].name, customers[j].name) > 0){
                struct Customer temp = customers[i];
                customers[i] = customers[j];
                customers[j] = temp;
            }
        }
    }

    printf("Customer List:\n");
    for(int i=0; i < count; i++){
        printf("ID: %d, Name: %s, Phone: %s, VIP: %s\n", 
               customers[i].ID, customers[i].name, customers[i].phone, 
               customers[i].VIP ? "Yes" : "No");
    }
}


// Table Management Functions


// Save table data to file
void saveTableFile(struct Table *table, int count){
    FILE *file = fopen(TABLE_FILE, "w");
    if (!file){
        printf("Error opening table file for writing!\n");
        return;
    }
    for(int i=0; i < count; i++){
        fprintf(file, "%d %d %s %d\n", table[i].tableID, table[i].capacity, table[i].name, table[i].Availablity);
    }
    fclose(file);
}

// Load table data from file; returns number of tables loaded
int loadTable(struct Table *table){
    FILE *file = fopen(TABLE_FILE, "r");
    if(!file) return 0;

    int count = 0;
    while(fscanf(file, "%d %d %29s %d", &table[count].tableID, &table[count].capacity, table[count].name, &table[count].Availablity) == 4){
        count++;
    }
    fclose(file);
    return count;
}

// Add a new table
void addTable(struct Table *table, int *count){
    if(*count >= MAX_TABLE){
        printf("No more space for new tables!\n");
        return;
    }

    struct Table newTable;
    newTable.tableID = (*count > 0) ? table[*count - 1].tableID + 1 : 1;
    printf("Enter Table Capacity: ");
    scanf("%d", &newTable.capacity);
    newTable.Availablity = 1; // Table is available by default
    printf("Enter Table Name: ");
    scanf("%29s", newTable.name);

    table[*count] = newTable;
    (*count)++;
    saveTableFile(table, *count);
    printf("Table added successfully!\n");
}

// Modify table details by table ID
void modifyTable(struct Table *table, int count){
    int T_ID;
    printf("Enter Table ID to modify: ");
    scanf("%d", &T_ID);

    for(int i=0; i < count; i++){
        if(table[i].tableID == T_ID) {
            printf("Enter new Capacity: ");
            scanf("%d", &table[i].capacity);
            printf("Enter new Table Name: ");
            scanf("%29s", table[i].name);
            saveTableFile(table, count);
            printf("Table details updated successfully!\n");
            return;
        }
    }
    printf("Table ID not found!\n");
}

// Delete a table by ID
void deleteTable(struct Table *table, int *count){
    int T_ID;
    printf("Enter Table ID to delete: ");
    scanf("%d", &T_ID);

    for(int i=0; i < *count; i++){
        if(table[i].tableID == T_ID) {
            for(int j = i; j < *count - 1; j++){
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

// Display all tables with their availability status
void displayTables(struct Table *table, int count){
    if(count==0){
        printf("No tables found!\n");
        return;
    }

    printf("Table List:\n");
    for(int i=0; i < count; i++){
        printf("ID: %d, Capacity: %d, Name: %s, Status: %s\n", 
               table[i].tableID, table[i].capacity, table[i].name, 
               table[i].Availablity ? "Available" : "Reserved");
    }
}

// Reservation Management Functions

// Save reservations to file
void saveReservationsToFile(struct Reservation *reservations, int count){
    FILE *file = fopen(RESERVATION_FILE,"w");
    if(!file){
        printf("Error opening reservation file for writing!\n");
        return;
    }
    for(int i=0; i < count; i++){
        fprintf(file, "%d %d %d %s %s\n", 
                reservations[i].reservationID, reservations[i].customerID, reservations[i].tableID, reservations[i].date, reservations[i].time);
    }
    fclose(file);
}

// Load reservations from file; returns number of reservations loaded
int loadReservationsFromFile(struct Reservation *reservations){
    FILE *file = fopen(RESERVATION_FILE, "r");
    if(!file) return 0;

    int count = 0;
    while (fscanf(file, "%d %d %d %14s %9s", 
                  &reservations[count].reservationID, &reservations[count].customerID, &reservations[count].tableID, reservations[count].date, reservations[count].time) == 5) {
        count++;
    }
    fclose(file);
    return count;
}

// Book a table by creating a new reservation
void addReservation(struct Reservation *reservations, int *resCount, struct Customer *customers, int custCount, struct Table *tables, int tableCount){
    if (*resCount >= MAX_RESERVATIONS){
        printf("Reservation list is full!\n");
        return;
    }

    int custID, tableID;
    char date[15], time[10];

    printf("Enter Customer ID for reservation: ");
    scanf("%d", &custID);
    // Verify customer exists
    int custFound = 0;
    for (int i=0; i < custCount; i++){
        if(customers[i].ID == custID){
            custFound = 1;
            break;
        }
    }
    if(!custFound){
        printf("Customer ID not found!\n");
        return;
    }

    printf("Enter Table ID to reserve: ");
    scanf("%d", &tableID);
    // Check table existence and availability
    int tableIndex = -1;
    for(int i=0; i < tableCount; i++){
        if(tables[i].tableID == tableID){
            tableIndex = i;
            break;
        }
    }
    if(tableIndex==-1){
        printf("Table ID not found!\n");
        return;
    }
    if(tables[tableIndex].Availablity==0){
        printf("Selected table is already reserved. Overbooking prevented!\n");
        return;
    }
    
    do{
      printf("Enter Reservation Date (YYYY-MM-DD): ");
      scanf("%14s", date);
      
      if(!isValidDate(date)) printf("Invalid date! Please enter a future date.\n");
      
    } while(!isValidDate(date));

  printf("Enter Reservation Time (HH:MM): ");
  scanf("%9s", time);
    struct Reservation newRes;
    newRes.reservationID = (*resCount > 0) ? reservations[*resCount - 1].reservationID + 1 : 1;
    newRes.customerID=custID;
    newRes.tableID=tableID;
    strcpy(newRes.date, date);
    strcpy(newRes.time, time);

    reservations[*resCount] = newRes;
    (*resCount)++;

    // Mark table as reserved
    tables[tableIndex].Availablity = 0;
    saveTableFile(tables, tableCount);
    saveReservationsToFile(reservations, *resCount);
    printf("Reservation booked successfully! Reservation ID: %d\n", newRes.reservationID);
}

// Cancel a reservation by reservation ID and free the table
void cancelReservation(struct Reservation *reservations, int *resCount, struct Table *tables, int tableCount) {
    int resID;
    printf("Enter Reservation ID to cancel: ");
    scanf("%d", &resID);

    int found = 0;
    for(int i=0; i<*resCount; i++){
        if(reservations[i].reservationID == resID){
            found = 1;
            int tableID = reservations[i].tableID;
            for(int j = 0; j < tableCount; j++){
                if(tables[j].tableID == tableID){
                    tables[j].Availablity = 1;
                    break;
                }
            }
            for(int j = i; j < *resCount - 1; j++){
                reservations[j] = reservations[j + 1];
            }
            (*resCount)--;
            saveTableFile(tables, tableCount);
            saveReservationsToFile(reservations, *resCount);
            printf("Reservation cancelled successfully!\n");
            break;
        }
    }
    if(!found){
        printf("Reservation ID not found!\n");
    }
}

// Display all reservations
void displayReservations(struct Reservation *reservations, int resCount){
    if (resCount == 0){
        printf("No reservations found!\n");
        return;
    }

    printf("Reservation List:\n");
    for (int i=0; i < resCount; i++){
        printf("Reservation ID: %d, Customer ID: %d, Table ID: %d, Date: %s, Time: %s\n", 
               reservations[i].reservationID, reservations[i].customerID, 
               reservations[i].tableID, reservations[i].date, reservations[i].time);
    }
}

// Display customers who have reservations
void displayCustomersWithReservations(struct Reservation *reservations, int resCount, 
                                      struct Customer *customers, int custCount) {
    if(resCount == 0){
        printf("No reservations to display customer information!\n");
        return;
    }

    printf("Customers with Reservations:\n");
    for(int i=0; i < resCount; i++){
        for(int j = 0; j < custCount; j++){
            if(customers[j].ID == reservations[i].customerID){
                printf("Customer ID: %d, Name: %s, Phone: %s, VIP: %s\n", 
                       customers[j].ID, customers[j].name, customers[j].phone, 
                       customers[j].VIP ? "Yes" : "No");
                break;
            }
        }
    }
}

// Main Function with Menus

int main(){
    struct Customer customers[MAX_CUSTOMERS];
    int customerCount=loadCustomersFromFile(customers);

    struct Table table[MAX_TABLE];
    int tableCount=loadTable(table);

    struct Reservation reservations[MAX_RESERVATIONS];
    int resCount=loadReservationsFromFile(reservations);

    int menu_choice;

    do{
        printf("\n==== Reservation System Main Menu ====\n");
        printf("0. Exit\n");
        printf("1. Customer Management\n");
        printf("2. Table Management\n");
        printf("3. Reservation Management\n");
        printf("Enter your menu choice: ");
        scanf("%d", &menu_choice);

        if(menu_choice == 1){
            int choice;
            do{
                printf("\n--- Customer Management Menu ---\n");
                printf("1. Add Customer\n");
                printf("2. Display Customers\n");
                printf("3. Modify Customer Details\n");
                printf("4. Delete Customer\n");
                printf("5. Return to Main Menu\n");
                printf("Enter your choice: ");
                scanf("%d", &choice);

                if(choice == 1)
                    addCustomer(customers, &customerCount);
                else if(choice == 2)
                    displayCustomers(customers, customerCount);
                else if(choice == 3)
                    modifyCustomer(customers, customerCount);
                else if(choice == 4)
                    deleteCustomer(customers, &customerCount);
                else if(choice != 5)
                    printf("Invalid choice! Please try again.\n");
            } while(choice != 5);
        }else if(menu_choice == 2){
            int choice_T;
            do{
                printf("\n--- Table Management Menu ---\n");
                printf("1. Add Table\n");
                printf("2. Display Tables\n");
                printf("3. Modify Table Details\n");
                printf("4. Delete Table\n");
                printf("5. Return to Main Menu\n");
                printf("Enter your choice: ");
                scanf("%d", &choice_T);

                if(choice_T == 1)
                    addTable(table, &tableCount);
                else if(choice_T == 2)
                    displayTables(table, tableCount);
                else if(choice_T == 3)
                    modifyTable(table, tableCount);
                else if(choice_T == 4)
                    deleteTable(table, &tableCount);
                else if(choice_T != 5)
                    printf("Invalid choice! Please try again.\n");
            }while(choice_T != 5);
        }else if(menu_choice == 3){
            int choice_R;
            do{
                printf("\n--- Reservation Management Menu ---\n");
                printf("1. Book a Table (Add Reservation)\n");
                printf("2. Cancel a Reservation\n");
                printf("3. Display All Reservations\n");
                printf("4. Display Customers with Reservations\n");
                printf("5. Return to Main Menu\n");
                printf("Enter your choice: ");
                scanf("%d", &choice_R);

                if(choice_R == 1)
                    addReservation(reservations, &resCount, customers, customerCount, table, tableCount);
                else if(choice_R == 2)
                    cancelReservation(reservations, &resCount, table, tableCount);
                else if(choice_R == 3)
                    displayReservations(reservations, resCount);
                else if(choice_R == 4)
                    displayCustomersWithReservations(reservations, resCount, customers, customerCount);
                else if(choice_R != 5)
                    printf("Invalid choice! Please try again.\n");
            }while(choice_R != 5);
        }
        else if(menu_choice == 0){
            printf("Exiting the program. Goodbye!\n");
        }
        else{
            printf("Invalid menu choice! Please try again.\n");
        }
    }while(menu_choice != 0);
    return 0;
}
