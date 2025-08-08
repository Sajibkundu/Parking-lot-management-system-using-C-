
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "admin.h"
#include "user.h"
#include "parking.h"
#include "payment.h"

void adminDashboard();
void viewAllUsers();
void viewAllParkingData();
void viewAllPayments();
void manageDataFiles();
void viewDailyReport();
// Admin Login Function
void adminPanel() {
    char username[20];
    char password[20];
    system("cls || clear");
    printf("=============================================\n");
    printf("           Admin Panel Login           \n");
    printf("=============================================\n");
    printf("Enter Username: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = 0;
    printf("Enter Password: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = 0;
    if (strcmp(username, ADMIN_USERNAME) == 0 && strcmp(password, ADMIN_PASSWORD) == 0) {
        printf("\n Login successful! Press Enter to continue...");
        getchar();
        adminDashboard();
    } else {
        printf("\n Invalid username or password. Press Enter to try again...");
        getchar();
    }
}
//  Dashboard
void adminDashboard() {
    int choice;
    while (1) {
        system("cls || clear");
        printf("=============================================\n");
        printf("              Admin Dashboard                \n");
        printf("=============================================\n");
        printf("1. View All Users\n");
        printf("2. View Parking Data\n");
        printf("3. View All Payment Records\n");
        printf("4. View Daily Report\n");
        printf("5. Manage Data Files (Delete)\n");
        printf("6. Back to Main Menu\n");
        printf("---------------------------------------------\n");
        printf("Enter your choice (1-6): ");
        if (scanf("%d", &choice) != 1) {
            printf("\n❌ Invalid input! Please enter a number.\n");
            clearInputBuffer();
            system("pause");
            continue;
        }
        clearInputBuffer();
        switch (choice) {
            case 1: viewAllUsers(); break;
            case 2: viewAllParkingData(); break;
            case 3: viewAllPayments(); break;
            case 4: viewDailyReport(); break;
            case 5: manageDataFiles(); break;
            case 6: return; // Go back to main menu
            default:
                printf("\n❌ Invalid choice. Please try again.\n");
                system("pause");
        }
    }
}
// View All  Users
void viewAllUsers() {
    system("cls || clear");
    printf("=============================================\n");
    printf("               Registered Users                \n");
    printf("=============================================\n");
    FILE *fp = fopen("users.txt", "r");
    if (!fp) {
        printf("❌ No users registered yet.\n");
        printf("Press Enter to continue...");
        getchar();
        return;
    }
    User u;
    while (fscanf(fp, "%49[^|]|%49[^|]|%14[^|]|%19[^|]|%19[^\n]\n",
                  u.name, u.email, u.phone, u.nid, u.password) == 5) {
        printf("Name: %s, Email: %s, Phone: %s, NID: %s\n", u.name, u.email, u.phone, u.nid);
    }
    fclose(fp);
    printf("\nPress Enter to continue...");
    getchar();
}
// View All Parking Slot Data
void viewAllParkingData() {
    system("cls || clear");
    printf("=============================================\n");
    printf("             Parking Slot Data               \n");
    printf("=============================================\n");
    FILE *fp = fopen("parking_data.txt", "r");
    if (!fp) {
        printf("❌ No parking data available.\n");
        printf("Press Enter to continue...");
        getchar();
        return;
    }
    ParkingSlot slot;
    printf("Slot | Status   | Vehicle No | Type\n");
    printf("-----------------------------------\n");
    while (fscanf(fp, "%d|%d|%[^|]|%[^|]|%[^\n]\n",
                  &slot.slotNumber, &slot.isOccupied, slot.vehicleNumber,
                  slot.vehicleType, slot.entryTime) == 5) {
        printf("  %2d  |  %s  |  %10s  | %s\n",
               slot.slotNumber,
               slot.isOccupied ? "Occupied" : "Available",
               slot.isOccupied ? slot.vehicleNumber : "-",
               slot.isOccupied ? slot.vehicleType : "-");
    }
    fclose(fp);
    printf("\nPress Enter to continue...");
    getchar();
}
// View Payment Records
void viewAllPayments() {
    system("cls || clear");
    printf("=============================================\n");
    printf("              Payment Records                \n");
    printf("=============================================\n");
    FILE *fp = fopen("payments.txt", "r");
    if (!fp) {
        printf("❌ No payment records available.\n");
        printf("Press Enter to continue...");
        getchar();
        return;
    }
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        printf("%s", buffer);
    }
    fclose(fp);
    printf("\nPress Enter to continue...");
    getchar();
}
// View Daily Report
void viewDailyReport() {
    system("cls || clear");
    printf("=============================================\n");
    printf("                Daily Report                 \n");
    printf("=============================================\n");
    FILE *fp = fopen("payments.txt", "r");
    if (!fp) {
        printf(" No payment records available to generate report.\n");
        printf("Press Enter to continue...");
        getchar();
        return;
    }
    //  current date
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    char today[11]; 
    strftime(today, sizeof(today), "%Y-%m-%d", tm_info);
    int totalRevenue = 0;
    int transactionCount = 0;
    char buffer[256];
    char paymentDate[11];
    int fee;
  
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        char *entryTime = strstr(buffer, "Entry: ");
        if (entryTime) {
            //  date part
            strncpy(paymentDate, entryTime + strlen("Entry: "), 10);
            paymentDate[10] = '\0';
            // Check the record 
            if (strcmp(paymentDate, today) == 0) {
                char *feeStr = strstr(buffer, "Fee: ");
                if (feeStr) {
                    sscanf(feeStr + strlen("Fee: "), "%d", &fee);
                    totalRevenue += fee;
                    transactionCount++;
                }
            }
        }
    }
    fclose(fp);
    printf("Report for today (%s):\n", today);
    printf("Total Transactions: %d\n", transactionCount);
    printf("Total Revenue: %d BDT\n", totalRevenue);
    printf("\nPress Enter to continue...");
    getchar();
}
// Delete functionality
void manageDataFiles() {
    int choice;
    while (1) {
        system("cls || clear");
        printf("=============================================\n");
        printf("           Manage Data Files                 \n");
        printf("=============================================\n");
        printf("1. Delete User Data (users.txt)\n");
        printf("2. Delete Parking Data (parking_data.txt)\n");
        printf("3. Delete Payment Records (payments.txt)\n");
        printf("4. Back to Admin Dashboard\n");
        printf("---------------------------------------------\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("\n❌ Invalid input! Please enter a number.\n");
            clearInputBuffer();
            system("pause");
            continue;
        }
        clearInputBuffer();
        switch (choice) {
            case 1: remove("users.txt"); printf("\n✅ users.txt deleted.\n"); break;
            case 2: remove("parking_data.txt"); printf("\n✅ parking_data.txt deleted.\n"); break;
            case 3: remove("payments.txt"); printf("\n✅ payments.txt deleted.\n"); break;
            case 4: return;
            default: printf("\n❌ Invalid choice.\n");
        }
        printf("Press Enter to continue...");
        getchar();
    }
}
