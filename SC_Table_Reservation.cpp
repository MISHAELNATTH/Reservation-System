#include <stdio.h>
#include <string.h>
#define TABLE_FILE "Table.txt"
#define MAX_TABLE 500

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

int main(){
    struct Table table[MAX_TABLE];
    int TableCount = loadTable(table);
    
    int choice;
    do {
        printf("\nTable Management Menu:\n");
        printf("1. Add Table\n");
        printf("2. Display Table\n");
        printf("3. Modify Table details\n");
        printf("4. Remove Table\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        if(choice==1){
            addTable(table, &TableCount);
        }  
        if(choice == 2){
            displayTables(table, TableCount);
        }
        if(choice==3){
            modifyTable(table, TableCount);
        }
        if(choice == 4){
            deleteTable(table, &TableCount);
        }
        if(choice==5){
            printf("Exiting......\n");
        }
    }while(choice!=5);  

    return 0;
}
