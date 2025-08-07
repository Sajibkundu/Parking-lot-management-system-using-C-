#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// --- Global Constants and Definitions ---
#define USERS_FILE "users.txt"
#define MAX_NAME_LEN 50
#define MAX_EMAIL_LEN 50
#define MAX_PHONE_LEN 15
#define MAX_NID_LEN 20
#define MAX_PASSWORD_LEN 20
#define MAX_LINE_LEN 256 // Increased line buffer for safety

// --- Data Structure for User ---
typedef struct {
    char name[MAX_NAME_LEN];
    char email[MAX_EMAIL_LEN];
    char phone[MAX_PHONE_LEN];
    char nid[MAX_NID_LEN];
    char password[MAX_PASSWORD_LEN];
} User;

// --- Function Prototypes ---
void clearScreen();
void clearInputBuffer();
void mainMenu();
void adminPanel();
void userPanel();
void userRegistration();
void userLogin();
void userDashboard(char username[]);
void aboutSystem();
int isValidEmail(char email[]);
int isValidPhone(char phone[]);
int isValidNID(char nid[]);
int isEmailRegistered(char email[]);
int isFieldExist(const char* filename, int fieldIndex, const char* value);


// --- Main Function ---
int main() {
    mainMenu();
    return 0;
}

// --- Utility Functions ---
void clearScreen() {
    // Check for operating system to use the correct command
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// --- Main Menu ---
void mainMenu() {
    int choice;
    while (1) {
        clearScreen();
        printf("=============================================\n");
        printf("   Smart Parking Lot Management System (C)   \n");
        printf("=============================================\n");
        printf("1. Admin Panel\n");
        printf("2. User Panel\n");
        printf("3. About the System\n");
        printf("4. Exit\n");
        printf("---------------------------------------------\n");
        printf("Enter your choice (1-4): ");
        if (scanf("%d", &choice) != 1) {
            printf("\n❌ Invalid input! Please enter a number.\n");
            clearInputBuffer();
            system("pause");
            continue;
        }
        clearInputBuffer();

        switch (choice) {
            case 1: adminPanel(); break;
            case 2: userPanel(); break;
            case 3: aboutSystem(); break;
            case 4:
                printf("\nThank you for using the system. Goodbye!\n");
                exit(0);
            default:
                printf("\n❌ Invalid choice. Please try again.\n");
                system("pause");
        }
    }
}

// --- Admin Panel (Simplified Conditional Logic) ---
void adminPanel() {
    char username[30], password[MAX_PASSWORD_LEN];
    clearScreen();
    printf("=== Admin Login ===\n");
    printf("Enter Admin Username: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = 0;
    printf("Enter Password: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = 0;

    if (strcmp(username, "admin") == 0) {
        if (strcmp(password, "123") == 0) {
            printf("\n✅ Login successful! Welcome, Admin.\n");
            // Placeholder for admin dashboard functionality
        } else {
            printf("\n❌ Invalid password!\n");
        }
    } else {
        printf("\n❌ Invalid username!\n");
    }
    system("pause");
}

// --- User Panel ---
void userPanel() {
    int option;
    while (1) {
        clearScreen();
        printf("=== User Panel ===\n");
        printf("1. Register\n");
        printf("2. Login\n");
        printf("3. Back to Main Menu\n");
        printf("---------------------------------------------\n");
        printf("Enter your choice: ");
        if (scanf("%d", &option) != 1) {
            printf("\n❌ Invalid input! Please enter a number.\n");
            clearInputBuffer();
            system("pause");
            continue;
        }
        clearInputBuffer();

        switch (option) {
            case 1: userRegistration(); break;
            case 2: userLogin(); break;
            case 3: return;
            default:
                printf("\n❌ Invalid choice.\n");
                system("pause");
        }
    }
}

// --- User Registration ---
void userRegistration() {
    User new_user;
    char password_confirm[MAX_PASSWORD_LEN];
    FILE *fp;

    clearScreen();
    printf("=== User Registration ===\n");

    printf("Enter Full Name: ");
    fgets(new_user.name, sizeof(new_user.name), stdin);
    new_user.name[strcspn(new_user.name, "\n")] = 0;

    while (1) {
        printf("Enter Email: ");
        fgets(new_user.email, sizeof(new_user.email), stdin);
        new_user.email[strcspn(new_user.email, "\n")] = 0;
        if (isValidEmail(new_user.email) && !isEmailRegistered(new_user.email)) break;
        else printf("❌ Invalid or already registered email! Please try again.\n");
    }

    while (1) {
        printf("Enter Phone Number (BD format 01XXXXXXXXX): ");
        fgets(new_user.phone, sizeof(new_user.phone), stdin);
        new_user.phone[strcspn(new_user.phone, "\n")] = 0;
        if (isValidPhone(new_user.phone)) break;
        else printf("❌ Invalid phone number! Must be 11 digits and start with 01.\n");
    }

    while (1) {
        printf("Enter NID Number (10 or 17 digits): ");
        fgets(new_user.nid, sizeof(new_user.nid), stdin);
        new_user.nid[strcspn(new_user.nid, "\n")] = 0;
        if (isValidNID(new_user.nid)) break;
        else printf("❌ Invalid NID! Must be 10 or 17 digits.\n");
    }
    
    while(1) {
        printf("Enter Password: ");
        fgets(new_user.password, sizeof(new_user.password), stdin);
        new_user.password[strcspn(new_user.password, "\n")] = 0;

        printf("Confirm Password: ");
        fgets(password_confirm, sizeof(password_confirm), stdin);
        password_confirm[strcspn(password_confirm, "\n")] = 0;
        
        if (strcmp(new_user.password, password_confirm) == 0 && strlen(new_user.password) > 0) {
            break;
        } else {
            printf("❌ Passwords do not match or are empty. Please try again.\n");
        }
    }

    fp = fopen(USERS_FILE, "a");
    if (fp == NULL) {
        printf("\n❌ Error: Could not open file to save user data!\n");
        system("pause");
        return;
    }

    fprintf(fp, "%s|%s|%s|%s|%s\n", new_user.name, new_user.email, new_user.phone, new_user.nid, new_user.password);
    fclose(fp);

    printf("\n✅ Registration successful! Please log in to continue.\n");
    system("pause");
}

// --- User Login ---
void userLogin() {
    char email[MAX_EMAIL_LEN], password[MAX_PASSWORD_LEN], line[MAX_LINE_LEN];
    FILE *fp;
    User current_user;
    int login_success = 0;

    clearScreen();
    printf("=== User Login ===\n");

    printf("Enter Email: ");
    fgets(email, sizeof(email), stdin);
    email[strcspn(email, "\n")] = 0;

    printf("Enter Password: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = 0;

    fp = fopen(USERS_FILE, "r");
    if (fp == NULL) {
        printf("\n❌ No registered users found. Please register first.\n");
        system("pause");
        return;
    }

    while (fgets(line, sizeof(line), fp)) {
        // Use safer sscanf format string
        sscanf(line, "%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]",
               current_user.name, current_user.email, current_user.phone, current_user.nid, current_user.password);

        if (strcmp(email, current_user.email) == 0 && strcmp(password, current_user.password) == 0) {
            login_success = 1;
            break;
        }
    }
    
    fclose(fp);

    if (login_success) {
        printf("\n✅ Login successful! Welcome, %s.\n", current_user.name);
        system("pause");
        userDashboard(current_user.name);
    } else {
        printf("\n❌ Invalid email or password.\n");
        system("pause");
    }
}

// --- User Dashboard (Placeholder) ---
void userDashboard(char username[]) {
    int choice;
    while(1) {
        clearScreen();
        printf("=============================================\n");
        printf("         User Dashboard                      \n");
        printf("=============================================\n");
        printf("Welcome, %s!\n", username);
        printf("1. View Parking Slots\n");
        printf("2. Reserve a Slot\n");
        printf("3. Logout\n");
        printf("---------------------------------------------\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("\n❌ Invalid input! Please enter a number.\n");
            clearInputBuffer();
            system("pause");
            continue;
        }
        clearInputBuffer();

        switch(choice) {
            case 1:
                printf("\nViewing parking slots... (Coming Soon)\n");
                system("pause");
                break;
            case 2:
                printf("\nReserving a slot... (Coming Soon)\n");
                system("pause");
                break;
            case 3:
                printf("\nLogging out...\n");
                return;
            default:
                printf("\n❌ Invalid choice.\n");
                system("pause");
        }
    }
}

// --- About the System ---
void aboutSystem() {
    clearScreen();
    printf("=== About Smart Parking System ===\n");
    printf("Developed in C Language\n");
    printf("Features will include:\n");
    printf("- Slot Reservation\n");
    printf("- User/Admin Panels\n");
    printf("- Payment System\n");
    printf("- Overstay Alert\n");
    printf("- Daily Report\n");
    printf("\nThis is a console-based application for demonstration purposes.\n");
    system("pause");
}

// --- Validation Functions ---
int isValidEmail(char email[]) {
    char* at_sign = strchr(email, '@');
    char* dot_sign = strchr(email, '.');
    return at_sign && dot_sign && (dot_sign > at_sign);
}

int isValidPhone(char phone[]) {
    if (strlen(phone) != 11) return 0;
    if (phone[0] != '0' || phone[1] != '1') return 0;
    for (int i = 0; i < 11; i++) {
        if (!isdigit(phone[i])) return 0;
    }
    return 1;
}

int isValidNID(char nid[]) {
    int len = strlen(nid);
    if (len != 10 && len != 17) return 0;
    for (int i = 0; i < len; i++) {
        if (!isdigit(nid[i])) return 0;
    }
    return 1;
}

int isEmailRegistered(char email[]) {
    FILE *fp = fopen(USERS_FILE, "r");
    if (fp == NULL) return 0;
    
    char line[MAX_LINE_LEN];
    while (fgets(line, sizeof(line), fp)) {
        char stored_name[MAX_NAME_LEN], stored_email[MAX_EMAIL_LEN];
        sscanf(line, "%[^|]|%[^|]", stored_name, stored_email);
        if (strcmp(email, stored_email) == 0) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}
