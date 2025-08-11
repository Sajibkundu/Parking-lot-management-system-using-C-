#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <ctype.h>

#ifdef _WIN32
    #include <windows.h>
    #define CLEAR_SCREEN "cls"
#else
    #include <unistd.h>
    #define CLEAR_SCREEN "clear"
#endif

// Constants
#define MAX_USERS 1000
#define MAX_VEHICLES 2000
#define MAX_SLOTS 100
#define MAX_RESERVATIONS 5000
#define MAX_PAYMENTS 5000
#define MAX_STRING 100
#define BIKE_SLOTS 40
#define CAR_SLOTS 40
#define TRUCK_SLOTS 20
#define CONSOLE_WIDTH 80

// Pricing per hour
#define BIKE_PRICE 10.0
#define CAR_PRICE 20.0
#define TRUCK_PRICE 50.0

// Color codes for Windows
#define BLACK 0
#define BLUE 1
#define GREEN 2
#define CYAN 3
#define RED 4
#define MAGENTA 5
#define BROWN 6
#define LIGHTGRAY 7
#define DARKGRAY 8
#define LIGHTBLUE 9
#define LIGHTGREEN 10
#define LIGHTCYAN 11
#define LIGHTRED 12
#define LIGHTMAGENTA 13
#define YELLOW 14
#define WHITE 15

// Color functions
void setColor(int color) {
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
#endif
}

void resetColor() {
#ifdef _WIN32
    setColor(WHITE);
#endif
}

// Center alignment function
void printCentered(const char* text, int color) {
    int len = strlen(text);
    int padding = (CONSOLE_WIDTH - len) / 2;
    setColor(color);
    for (int i = 0; i < padding; i++) {
        printf(" ");
    }
    printf("%s\n", text);
    resetColor();
}

void printCenteredLine(char ch, int color) {
    setColor(color);
    for (int i = 0; i < CONSOLE_WIDTH; i++) {
        printf("%c", ch);
    }
    printf("\n");
    resetColor();
}

// ASCII Header Function
void print_ascii_header() {
    setColor(LIGHTCYAN);
    printf("\n");
    printCentered("  _____  __  __      ___      _____  _______     _____    ___      _____   _  __", LIGHTCYAN);
    printCentered(" / ____||  \\/  |    /   \\    |  __ \\|__   __|   |  __ \\  /   \\    |  __ \\ | |/ /", LIGHTCYAN);
    printCentered("| (___  | \\  / |   /  ^  \\   | |__) |  | |      | |__) |/  ^  \\   | |__) || ' / ", LIGHTCYAN);
    printCentered(" \\___ \\ | |\\/| |  /  /_\\  \\  |  _  /   | |      |  ___//  /_\\  \\  |  _  / |  <  ", LIGHTCYAN);
    printCentered(" ____) || |  | | /  _____  \\ | | \\ \\   | |      | |   /  _____  \\ | | \\ \\ | . \\ ", LIGHTCYAN);
    printCentered("|_____/ |_|  |_|/__/     \\__\\|_|  \\_\\  |_|      |_|  /__/     \\__\\|_|  \\_\\|_|\\_\\", LIGHTCYAN);
    printCentered("                |__|     |__|                        |__|     |__|              ", LIGHTCYAN);
    printf("\n");
    resetColor();
}







// Data Structures
typedef struct {
    int user_id;
    char name[MAX_STRING];
    char phone[20];
    char email[MAX_STRING];
    char password[MAX_STRING];
    char reg_date[20];
    int is_active;
} User;

typedef struct {
    int vehicle_id;
    int user_id;
    char type[20]; // bike, car, truck
    char license_plate[20];
    char color[20];
    char reg_date[20];
} Vehicle;

typedef struct {
    int slot_id;
    char type[20]; // bike, car, truck
    char status[20]; // available, occupied, reserved
    int vehicle_id;
    char reserved_time[20];
    int user_id;
} Slot;

typedef struct {
    int reservation_id;
    int user_id;
    int slot_id;
    int vehicle_id;
    char start_time[20];
    char end_time[20];
    char status[20]; // active, completed, cancelled
    double duration_hours;
    double total_amount;
} Reservation;

typedef struct {
    int payment_id;
    int user_id;
    int reservation_id;
    double amount;
    char method[20]; // cash, card, bkash, nagad
    char payment_date[20];
    char status[20]; // completed, pending, failed
} Payment;

// Global Variables
User users[MAX_USERS];
Vehicle vehicles[MAX_VEHICLES];
Slot slots[MAX_SLOTS];
Reservation reservations[MAX_RESERVATIONS];
Payment payments[MAX_PAYMENTS];

int user_count = 0, vehicle_count = 0, slot_count = 0;
int reservation_count = 0, payment_count = 0;
int current_user_id = -1;
int is_admin = 0;

// Function Prototypes
void initialize_system();
void load_data();
void save_data();
void clear_screen();
void main_menu();
void user_menu();
void admin_menu();
void user_register();
void user_login();
void admin_login();
void update_profile();
void add_vehicle();
void view_vehicles();
void view_available_slots();
void reserve_slot();
void cancel_reservation();
void make_payment();
void view_payment_history();
void view_booking_history();
void check_overstay();
void generate_user_report();
void admin_view_users();
void admin_search_users();
void admin_delete_user();
void admin_view_user_history();
void admin_view_all_vehicles();
void admin_slot_usage_report();
void admin_generate_daily_report();
void initialize_slots();
int validate_phone(char *phone);
int validate_email(char *email);
void get_current_time(char *time_str);
double calculate_duration(char *start_time, char *end_time);
void mask_password(char *password);
int get_next_user_id();
int get_next_vehicle_id();
int get_next_reservation_id();
int get_next_payment_id();
void pause_screen();
void print_header();
void print_colored_slots();

// Main Function
int main() {
    initialize_system();
    load_data();
    main_menu();
    save_data();
    return 0;
}

// System Initialization
void initialize_system() {
    if (slot_count == 0) {
        initialize_slots();
    }
}

void initialize_slots() {
    int slot_id = 1;

    // Initialize bike slots (1-40)
    for (int i = 0; i < BIKE_SLOTS; i++) {
        slots[slot_count].slot_id = slot_id++;
        strcpy(slots[slot_count].type, "bike");
        strcpy(slots[slot_count].status, "available");
        slots[slot_count].vehicle_id = 0;
        slots[slot_count].user_id = 0;
        strcpy(slots[slot_count].reserved_time, "");
        slot_count++;
    }

    // Initialize car slots (41-80)
    for (int i = 0; i < CAR_SLOTS; i++) {
        slots[slot_count].slot_id = slot_id++;
        strcpy(slots[slot_count].type, "car");
        strcpy(slots[slot_count].status, "available");
        slots[slot_count].vehicle_id = 0;
        slots[slot_count].user_id = 0;
        strcpy(slots[slot_count].reserved_time, "");
        slot_count++;
    }

    // Initialize truck slots (81-100)
    for (int i = 0; i < TRUCK_SLOTS; i++) {
        slots[slot_count].slot_id = slot_id++;
        strcpy(slots[slot_count].type, "truck");
        strcpy(slots[slot_count].status, "available");
        slots[slot_count].vehicle_id = 0;
        slots[slot_count].user_id = 0;
        strcpy(slots[slot_count].reserved_time, "");
        slot_count++;
    }
}

// File Handling Functions - Load Data from Text Files
void load_data() {
    FILE *file;

    printCentered("Loading system data...", LIGHTCYAN);
    printf("\n");

    // Load users from users.txt
    file = fopen("users.txt", "r");
    if (file != NULL) {
        while (fscanf(file, "%d %s %s %s %s %s %d",
                &users[user_count].user_id, users[user_count].name,
                users[user_count].phone, users[user_count].email,
                users[user_count].password, users[user_count].reg_date,
                &users[user_count].is_active) == 7) {
            user_count++;
            if (user_count >= MAX_USERS) break;
        }
        fclose(file);
        char msg[100];
        sprintf(msg, "Loaded %d users from users.txt", user_count);
        printCentered(msg, LIGHTGREEN);
    } else {
        printCentered("users.txt not found - starting with empty user database", YELLOW);
    }

    // Load vehicles from vehicles.txt
    file = fopen("vehicles.txt", "r");
    if (file != NULL) {
        while (fscanf(file, "%d %d %s %s %s %s",
                &vehicles[vehicle_count].vehicle_id, &vehicles[vehicle_count].user_id,
                vehicles[vehicle_count].type, vehicles[vehicle_count].license_plate,
                vehicles[vehicle_count].color, vehicles[vehicle_count].reg_date) == 6) {
            vehicle_count++;
            if (vehicle_count >= MAX_VEHICLES) break;
        }
        fclose(file);
        char msg[100];
        sprintf(msg, "Loaded %d vehicles from vehicles.txt", vehicle_count);
        printCentered(msg, LIGHTGREEN);
    } else {
        printCentered("vehicles.txt not found - starting with empty vehicle database", YELLOW);
    }

    // Load slots from slots.txt (only if not already initialized)
    if (slot_count == 0) {
        file = fopen("slots.txt", "r");
        if (file != NULL) {
            while (fscanf(file, "%d %s %s %d %s %d",
                    &slots[slot_count].slot_id, slots[slot_count].type,
                    slots[slot_count].status, &slots[slot_count].vehicle_id,
                    slots[slot_count].reserved_time, &slots[slot_count].user_id) == 6) {
                slot_count++;
                if (slot_count >= MAX_SLOTS) break;
            }
            fclose(file);
            char msg[100];
            sprintf(msg, "Loaded %d slots from slots.txt", slot_count);
            printCentered(msg, LIGHTGREEN);
        } else {
            printCentered("slots.txt not found - initializing default slots", YELLOW);
            initialize_slots();
        }
    }

    // Load reservations from reservations.txt
    file = fopen("reservations.txt", "r");
    if (file != NULL) {
        while (fscanf(file, "%d %d %d %d %s %s %s %lf %lf",
                &reservations[reservation_count].reservation_id,
                &reservations[reservation_count].user_id,
                &reservations[reservation_count].slot_id,
                &reservations[reservation_count].vehicle_id,
                reservations[reservation_count].start_time,
                reservations[reservation_count].end_time,
                reservations[reservation_count].status,
                &reservations[reservation_count].duration_hours,
                &reservations[reservation_count].total_amount) == 9) {
            reservation_count++;
            if (reservation_count >= MAX_RESERVATIONS) break;
        }
        fclose(file);
        char msg[100];
        sprintf(msg, "Loaded %d reservations from reservations.txt", reservation_count);
        printCentered(msg, LIGHTGREEN);
    } else {
        printCentered("reservations.txt not found - starting with empty reservation database", YELLOW);
    }

    // Load payments from payments.txt
    file = fopen("payments.txt", "r");
    if (file != NULL) {
        while (fscanf(file, "%d %d %d %lf %s %s %s",
                &payments[payment_count].payment_id,
                &payments[payment_count].user_id,
                &payments[payment_count].reservation_id,
                &payments[payment_count].amount,
                payments[payment_count].method,
                payments[payment_count].payment_date,
                payments[payment_count].status) == 7) {
            payment_count++;
            if (payment_count >= MAX_PAYMENTS) break;
        }
        fclose(file);
        char msg[100];
        sprintf(msg, "Loaded %d payments from payments.txt", payment_count);
        printCentered(msg, LIGHTGREEN);
    } else {
        printCentered("payments.txt not found - starting with empty payment database", YELLOW);
    }

    printf("\n");
    printCentered("Data loading complete. Press any key to continue...", LIGHTCYAN);
    getch();
}

// File Handling Functions - Save Data to Text Files
void save_data() {
    FILE *file;

    printCentered("Saving system data...", LIGHTCYAN);
    printf("\n");

    // Save users to users.txt
    file = fopen("users.txt", "w");
    if (file != NULL) {
        for (int i = 0; i < user_count; i++) {
            fprintf(file, "%d %s %s %s %s %s %d\n",
                    users[i].user_id, users[i].name, users[i].phone,
                    users[i].email, users[i].password, users[i].reg_date,
                    users[i].is_active);
        }
        fclose(file);
        char msg[100];
        sprintf(msg, "Saved %d users to users.txt", user_count);
        printCentered(msg, LIGHTGREEN);
    } else {
        printCentered("Error: Could not save users.txt", LIGHTRED);
    }

    // Save vehicles to vehicles.txt
    file = fopen("vehicles.txt", "w");
    if (file != NULL) {
        for (int i = 0; i < vehicle_count; i++) {
            fprintf(file, "%d %d %s %s %s %s\n",
                    vehicles[i].vehicle_id, vehicles[i].user_id,
                    vehicles[i].type, vehicles[i].license_plate,
                    vehicles[i].color, vehicles[i].reg_date);
        }
        fclose(file);
        char msg[100];
        sprintf(msg, "Saved %d vehicles to vehicles.txt", vehicle_count);
        printCentered(msg, LIGHTGREEN);
    } else {
        printCentered("Error: Could not save vehicles.txt", LIGHTRED);
    }

    // Save slots to slots.txt
    file = fopen("slots.txt", "w");
    if (file != NULL) {
        for (int i = 0; i < slot_count; i++) {
            fprintf(file, "%d %s %s %d %s %d\n",
                    slots[i].slot_id, slots[i].type, slots[i].status,
                    slots[i].vehicle_id, slots[i].reserved_time, slots[i].user_id);
        }
        fclose(file);
        char msg[100];
        sprintf(msg, "Saved %d slots to slots.txt", slot_count);
        printCentered(msg, LIGHTGREEN);
    } else {
        printCentered("Error: Could not save slots.txt", LIGHTRED);
    }

    // Save reservations to reservations.txt
    file = fopen("reservations.txt", "w");
    if (file != NULL) {
        for (int i = 0; i < reservation_count; i++) {
            fprintf(file, "%d %d %d %d %s %s %s %.2f %.2f\n",
                    reservations[i].reservation_id, reservations[i].user_id,
                    reservations[i].slot_id, reservations[i].vehicle_id,
                    reservations[i].start_time, reservations[i].end_time,
                    reservations[i].status, reservations[i].duration_hours,
                    reservations[i].total_amount);
        }
        fclose(file);
        char msg[100];
        sprintf(msg, "Saved %d reservations to reservations.txt", reservation_count);
        printCentered(msg, LIGHTGREEN);
    } else {
        printCentered("Error: Could not save reservations.txt", LIGHTRED);
    }

    // Save payments to payments.txt
    file = fopen("payments.txt", "w");
    if (file != NULL) {
        for (int i = 0; i < payment_count; i++) {
            fprintf(file, "%d %d %d %.2f %s %s %s\n",
                    payments[i].payment_id, payments[i].user_id,
                    payments[i].reservation_id, payments[i].amount,
                    payments[i].method, payments[i].payment_date,
                    payments[i].status);
        }
        fclose(file);
        char msg[100];
        sprintf(msg, "Saved %d payments to payments.txt", payment_count);
        printCentered(msg, LIGHTGREEN);
    } else {
        printCentered("Error: Could not save payments.txt", LIGHTRED);
    }

    printf("\n");
    printCentered("All data saved successfully!", LIGHTCYAN);
}

// Utility Functions
void clear_screen() {
    system(CLEAR_SCREEN);
}

void print_header() {
    print_ascii_header();
    printCentered("*** SMART PARKING LOT MANAGEMENT SYSTEM ***", YELLOW);
    printCenteredLine('=', LIGHTCYAN);
    printCentered("WELCOME OUR SYSTEM", LIGHTGREEN);
    printCenteredLine('=', LIGHTCYAN);
}

void pause_screen() {
    printf("\n");
    printCentered("Press any key to continue...", LIGHTGRAY);
    getch();
}

int validate_phone(char *phone) {
    if (strlen(phone) != 11) {
        printCentered("Phone number must be exactly 11 digits!", LIGHTRED);
        return 0;
    }

    for (int i = 0; i < 11; i++) {
        if (!isdigit(phone[i])) {
            printCentered("Phone number must contain only digits!", LIGHTRED);
            return 0;
        }
    }
    return 1;
}

int validate_email(char *email) {
    int at_count = 0, dot_count = 0;
    int len = strlen(email);

    if (len < 5) {
        printCentered("Email too short!", LIGHTRED);
        return 0;
    }

    for (int i = 0; i < len; i++) {
        if (email[i] == '@') at_count++;
        if (email[i] == '.') dot_count++;
    }

    if (at_count != 1 || dot_count < 1) {
        printCentered("Invalid email format!", LIGHTRED);
        return 0;
    }
    return 1;
}

void get_current_time(char *time_str) {
    time_t now;
    struct tm *local;

    time(&now);
    local = localtime(&now);

    sprintf(time_str, "%04d-%02d-%02d_%02d:%02d:%02d",
            local->tm_year + 1900,
            local->tm_mon + 1,
            local->tm_mday,
            local->tm_hour,
            local->tm_min,
            local->tm_sec);
}

double calculate_duration(char *start_time, char *end_time) {
    return 1.0;
}

void mask_password(char *password) {
    int i = 0;
    char ch;

    while ((ch = getch()) != '\r') { // Enter key
        if (ch == '\b' && i > 0) { // Backspace
            printf("\b \b");
            i--;
        } else if (ch != '\b' && i < MAX_STRING - 1) {
            password[i++] = ch;
            printf("*");
        }
    }
    password[i] = '\0';
    printf("\n");
}

int get_next_user_id() {
    int max_id = 0;
    for (int i = 0; i < user_count; i++) {
        if (users[i].user_id > max_id) {
            max_id = users[i].user_id;
        }
    }
    return max_id + 1;
}

int get_next_vehicle_id() {
    int max_id = 0;
    for (int i = 0; i < vehicle_count; i++) {
        if (vehicles[i].vehicle_id > max_id) {
            max_id = vehicles[i].vehicle_id;
        }
    }
    return max_id + 1;
}

int get_next_reservation_id() {
    int max_id = 0;
    for (int i = 0; i < reservation_count; i++) {
        if (reservations[i].reservation_id > max_id) {
            max_id = reservations[i].reservation_id;
        }
    }
    return max_id + 1;
}

int get_next_payment_id() {
    int max_id = 0;
    for (int i = 0; i < payment_count; i++) {
        if (payments[i].payment_id > max_id) {
            max_id = payments[i].payment_id;
        }
    }
    return max_id + 1;
}












// Menu Functions
void main_menu() {
    int choice;

    while (1) {
        clear_screen();
        print_header();
        printf("\n");

        printCentered("MAIN MENU", LIGHTCYAN);
        printCentered("Please select an option:", LIGHTGREEN);
        printf("\n");

        printCentered("1. Register Customer", LIGHTGREEN);
        printCentered("2. User Login", LIGHTBLUE);
        printCentered("3. Admin Login", LIGHTMAGENTA);
        printCentered("4. Exit System", LIGHTRED);

        printf("\n");
        setColor(YELLOW);
        printf("%*s", (CONSOLE_WIDTH - 15) / 2, "");
        printf("Enter choice: ");
        resetColor();
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                user_register();
                break;
            case 2:
                user_login();
                break;
            case 3:
                admin_login();
                break;
            case 4:
                printf("\n");
                printCentered("Thank you for using SMART PARK System!", LIGHTCYAN);
                printCentered("Have a great day!", YELLOW);
                save_data();
                exit(0);
            default:
                printf("\n");
                printCentered("Invalid choice! Please try again.", LIGHTRED);
                pause_screen();
        }
    }
}

void user_menu() {
    int choice;

    while (1) {
        clear_screen();
        print_header();
        printf("\n");

        printCentered("USER DASHBOARD", LIGHTCYAN);
        printCentered("Welcome to your personal parking hub!", LIGHTGREEN);
        printf("\n");

        printCentered("1. Update Profile", YELLOW);
        printCentered("2. Add Vehicle", LIGHTBLUE);
        printCentered("3. View My Vehicles", LIGHTGREEN);
        printCentered("4. View Available Slots", CYAN);
        printCentered("5. Reserve Slot", MAGENTA);
        printCentered("6. Cancel Reservation", LIGHTRED);
        printCentered("7. Make Payment", YELLOW);
        printCentered("8. View Payment History", LIGHTCYAN);
        printCentered("9. View Booking History", WHITE);
        printCentered("10. Check Overstay Alerts", LIGHTRED);
        printCentered("11. Generate My Report", LIGHTGREEN);
        printCentered("12. Logout", DARKGRAY);

        printf("\n");
        setColor(YELLOW);
        printf("%*s", (CONSOLE_WIDTH - 15) / 2, "");
        printf("Enter choice: ");
        resetColor();
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                update_profile();
                break;
            case 2:
                add_vehicle();
                break;
            case 3:
                view_vehicles();
                break;
            case 4:
                view_available_slots();
                break;
            case 5:
                reserve_slot();
                break;
            case 6:
                cancel_reservation();
                break;
            case 7:
                make_payment();
                break;
            case 8:
                view_payment_history();
                break;
            case 9:
                view_booking_history();
                break;
            case 10:
                check_overstay();
                break;
            case 11:
                generate_user_report();
                break;
            case 12:
                current_user_id = -1;
                printCentered("Logged out successfully!", LIGHTGREEN);
                pause_screen();
                return;
            default:
                printf("\n");
                printCentered("Invalid choice! Please try again.", LIGHTRED);
                pause_screen();
        }
    }
}

void admin_menu() {
    int choice;

    while (1) {
        clear_screen();
        print_header();
        printf("\n");

        printCentered("ADMIN DASHBOARD", LIGHTRED);
        printCentered("System Administration Portal", YELLOW);
        printf("\n");

        printCentered("1. View All Users", LIGHTBLUE);
        printCentered("2. Search Users", LIGHTGREEN);
        printCentered("3. Delete User", LIGHTRED);
        printCentered("4. View User History", YELLOW);
        printCentered("5. View All Vehicles", LIGHTCYAN);
        printCentered("6. Slot Usage Report", MAGENTA);
        printCentered("7. Generate Daily Report", WHITE);
        printCentered("8. Check System Overstays", LIGHTMAGENTA);
        printCentered("9. Logout", DARKGRAY);

        printf("\n");
        setColor(YELLOW);
        printf("%*s", (CONSOLE_WIDTH - 15) / 2, "");
        printf("Enter choice: ");
        resetColor();
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                admin_view_users();
                break;
            case 2:
                admin_search_users();
                break;
            case 3:
                admin_delete_user();
                break;
            case 4:
                admin_view_user_history();
                break;
            case 5:
                admin_view_all_vehicles();
                break;
            case 6:
                admin_slot_usage_report();
                break;
            case 7:
                admin_generate_daily_report();
                break;
            case 8:
                check_overstay();
                break;
            case 9:
                is_admin = 0;
                printCentered("Admin logged out successfully!", LIGHTGREEN);
                pause_screen();
                return;
            default:
                printf("\n");
                printCentered("Invalid choice! Please try again.", LIGHTRED);
                pause_screen();
        }
    }
}

// Authentication Functions
void user_register() {
    clear_screen();
    print_header();
    printf("\n");
    printCentered("CUSTOMER REGISTRATION", LIGHTGREEN);
    printCentered("Create your SMART PARK account", LIGHTCYAN);
    printf("\n");

    User new_user;
    new_user.user_id = get_next_user_id();
    new_user.is_active = 1;

    setColor(WHITE);
    printf("%*s", (CONSOLE_WIDTH - 25) / 2, "");
    printf("Enter your full name: ");
    resetColor();
    scanf("%s", new_user.name);

    // Phone number validation
    do {
        setColor(WHITE);
        printf("%*s", (CONSOLE_WIDTH - 35) / 2, "");
        printf("Enter phone number (11 digits): ");
        resetColor();
        scanf("%s", new_user.phone);
    } while (!validate_phone(new_user.phone));

    // Email validation
    do {
        setColor(WHITE);
        printf("%*s", (CONSOLE_WIDTH - 25) / 2, "");
        printf("Enter email address: ");
        resetColor();
        scanf("%s", new_user.email);
    } while (!validate_email(new_user.email));

    setColor(WHITE);
    printf("%*s", (CONSOLE_WIDTH - 30) / 2, "");
    printf("Create a secure password: ");
    resetColor();
    mask_password(new_user.password);

    get_current_time(new_user.reg_date);

    // Check if phone or email already exists
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].phone, new_user.phone) == 0) {
            printf("\n");
            printCentered("Phone number already registered!", LIGHTRED);
            printCentered("Please use a different phone number.", YELLOW);
            pause_screen();
            return;
        }
        if (strcmp(users[i].email, new_user.email) == 0) {
            printf("\n");
            printCentered("Email already registered!", LIGHTRED);
            printCentered("Please use a different email address.", YELLOW);
            pause_screen();
            return;
        }
    }

    users[user_count] = new_user;
    user_count++;

    printf("\n");
    printCenteredLine('=', LIGHTGREEN);
    printCentered("REGISTRATION SUCCESSFUL!", LIGHTGREEN);
    printCenteredLine('=', LIGHTGREEN);
    char success_msg[100];
    sprintf(success_msg, "Your Customer ID: %d", new_user.user_id);
    printCentered(success_msg, LIGHTCYAN);
    sprintf(success_msg, "Welcome to SMART PARK, %s!", new_user.name);
    printCentered(success_msg, YELLOW);
    printCentered("You can now login and start parking!", LIGHTGREEN);

    // Save data immediately after registration
    save_data();
    pause_screen();
}

void user_login() {
    clear_screen();
    print_header();
    printf("\n");
    printCentered("CUSTOMER LOGIN", LIGHTBLUE);
    printCentered("Access your SMART PARK account", LIGHTCYAN);
    printf("\n");

    char phone[20], password[MAX_STRING];

    setColor(WHITE);
    printf("%*s", (CONSOLE_WIDTH - 25) / 2, "");
    printf("Enter phone number: ");
    resetColor();
    scanf("%s", phone);

    setColor(WHITE);
    printf("%*s", (CONSOLE_WIDTH - 20) / 2, "");
    printf("Enter password: ");
    resetColor();
    mask_password(password);

    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].phone, phone) == 0 &&
            strcmp(users[i].password, password) == 0 &&
            users[i].is_active == 1) {
            current_user_id = users[i].user_id;
            printf("\n");
            printCenteredLine('=', LIGHTGREEN);
            printCentered("LOGIN SUCCESSFUL!", LIGHTGREEN);
            printCenteredLine('=', LIGHTGREEN);
            char welcome_msg[100];
            sprintf(welcome_msg, "Welcome back, %s!", users[i].name);
            printCentered(welcome_msg, LIGHTCYAN);
            printCentered("Redirecting to your dashboard...", YELLOW);
            pause_screen();
            user_menu();
            return;
        }
    }

    printf("\n");
    printCentered("LOGIN FAILED!", LIGHTRED);
    printCentered("Invalid credentials or account deactivated!", YELLOW);
    printCentered("Please check your phone number and password.", LIGHTCYAN);
    pause_screen();
}

void admin_login() {
    clear_screen();
    print_header();
    printf("\n");
    printCentered("ADMINISTRATOR LOGIN", LIGHTRED);
    printCentered("SMART PARK System Administration", YELLOW);
    printf("\n");

    char username[MAX_STRING], password[MAX_STRING];

    setColor(WHITE);
    printf("%*s", (CONSOLE_WIDTH - 25) / 2, "");
    printf("Enter admin username: ");
    resetColor();
    scanf("%s", username);

    setColor(WHITE);
    printf("%*s", (CONSOLE_WIDTH - 25) / 2, "");
    printf("Enter admin password: ");
    resetColor();
    mask_password(password);

    if (strcmp(username, "sajib") == 0 && strcmp(password, "12345") == 0) {
        is_admin = 1;
        printf("\n");
        printCenteredLine('=', LIGHTGREEN);
        printCentered("ADMIN LOGIN SUCCESSFUL!", LIGHTGREEN);
        printCenteredLine('=', LIGHTGREEN);
        printCentered("Welcome, System Administrator!", LIGHTCYAN);
        printCentered("Full system access granted.", YELLOW);
        pause_screen();
        admin_menu();
    } else {
        printf("\n");
        printCentered("ACCESS DENIED!", LIGHTRED);
        printCentered("Invalid administrator credentials!", YELLOW);
        printCentered("Contact system admin for assistance.", LIGHTCYAN);
        pause_screen();
    }
}

// User Functions
void update_profile() {
    clear_screen();
    print_header();
    printf("\n");
    printCentered("UPDATE PROFILE", YELLOW);
    printCentered("Modify your account information", LIGHTCYAN);
    printf("\n");

    int user_index = -1;
    for (int i = 0; i < user_count; i++) {
        if (users[i].user_id == current_user_id) {
            user_index = i;
            break;
        }
    }

    if (user_index == -1) {
        printCentered("User account not found!", LIGHTRED);
        pause_screen();
        return;
    }

    printCenteredLine('-', LIGHTCYAN);
    printCentered("CURRENT PROFILE INFORMATION", LIGHTCYAN);
    printCenteredLine('-', LIGHTCYAN);
    char info[200];
    sprintf(info, "Name: %s", users[user_index].name);
    printCentered(info, WHITE);
    sprintf(info, "Phone: %s", users[user_index].phone);
    printCentered(info, WHITE);
    sprintf(info, "Email: %s", users[user_index].email);
    printCentered(info, WHITE);
    sprintf(info, "Member since: %s", users[user_index].reg_date);
    printCentered(info, LIGHTGRAY);

    printf("\n");
    printCentered("What would you like to update?", WHITE);
    printf("\n");
    printCentered("1. Update Name", LIGHTGREEN);
    printCentered("2. Update Phone", LIGHTGREEN);
    printCentered("3. Update Email", LIGHTGREEN);
    printCentered("4. Change Password", LIGHTGREEN);
    printCentered("5. Go Back", LIGHTRED);

    int choice;
    setColor(WHITE);
    printf("%*s", (CONSOLE_WIDTH - 15) / 2, "");
    printf("Enter choice: ");
    resetColor();
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            printf("\n");
            setColor(WHITE);
            printf("%*s", (CONSOLE_WIDTH - 20) / 2, "");
            printf("Enter new name: ");
            resetColor();
            scanf("%s", users[user_index].name);
            printf("\n");
            printCentered("Name updated successfully!", LIGHTGREEN);
            save_data();
            break;
        case 2:
            printf("\n");
            do {
                setColor(WHITE);
                printf("%*s", (CONSOLE_WIDTH - 30) / 2, "");
                printf("Enter new phone (11 digits): ");
                resetColor();
                scanf("%s", users[user_index].phone);
            } while (!validate_phone(users[user_index].phone));
            printf("\n");
            printCentered("Phone number updated successfully!", LIGHTGREEN);
            save_data();
            break;
        case 3:
            printf("\n");
            do {
                setColor(WHITE);
                printf("%*s", (CONSOLE_WIDTH - 20) / 2, "");
                printf("Enter new email: ");
                resetColor();
                scanf("%s", users[user_index].email);
            } while (!validate_email(users[user_index].email));
            printf("\n");
            printCentered("Email address updated successfully!", LIGHTGREEN);
            save_data();
            break;
        case 4:
            printf("\n");
            setColor(WHITE);
            printf("%*s", (CONSOLE_WIDTH - 25) / 2, "");
            printf("Enter new password: ");
            resetColor();
            mask_password(users[user_index].password);
            printf("\n");
            printCentered("Password changed successfully!", LIGHTGREEN);
            save_data();
            break;
        case 5:
            return;
        default:
            printf("\n");
            printCentered("Invalid selection!", LIGHTRED);
    }

    pause_screen();
}

void add_vehicle() {
    clear_screen();
    print_header();
    printf("\n");
    printCentered("ADD VEHICLE", LIGHTBLUE);
    printCentered("Register a new vehicle to your account", LIGHTCYAN);
    printf("\n");

    Vehicle new_vehicle;
    new_vehicle.vehicle_id = get_next_vehicle_id();
    new_vehicle.user_id = current_user_id;

    printCentered("Select your vehicle type:", WHITE);
    printf("\n");
    printCentered("1. Bike/Motorcycle", LIGHTGREEN);
    printCentered("2. Car/SUV", LIGHTBLUE);
    printCentered("3. Truck/Van", YELLOW);

    int type_choice;
    printf("\n");
    setColor(WHITE);
    printf("%*s", (CONSOLE_WIDTH - 15) / 2, "");
    printf("Enter choice: ");
    resetColor();
    scanf("%d", &type_choice);

    switch (type_choice) {
        case 1:
            strcpy(new_vehicle.type, "bike");
            break;
        case 2:
            strcpy(new_vehicle.type, "car");
            break;
        case 3:
            strcpy(new_vehicle.type, "truck");
            break;
        default:
            printf("\n");
            printCentered("Invalid vehicle type selection!", LIGHTRED);
            pause_screen();
            return;
    }

    printf("\n");
    setColor(WHITE);
    printf("%*s", (CONSOLE_WIDTH - 25) / 2, "");
    printf("Enter license plate: ");
    resetColor();
    scanf("%s", new_vehicle.license_plate);

    setColor(WHITE);
    printf("%*s", (CONSOLE_WIDTH - 25) / 2, "");
    printf("Enter vehicle color: ");
    resetColor();
    scanf("%s", new_vehicle.color);

    get_current_time(new_vehicle.reg_date);

    vehicles[vehicle_count] = new_vehicle;
    vehicle_count++;

    printf("\n");
    printCenteredLine('=', LIGHTGREEN);
    printCentered("VEHICLE REGISTERED SUCCESSFULLY!", LIGHTGREEN);
    printCenteredLine('=', LIGHTGREEN);
    char vehicle_info[200];
    sprintf(vehicle_info, "Vehicle ID: %d", new_vehicle.vehicle_id);
    printCentered(vehicle_info, LIGHTCYAN);
    sprintf(vehicle_info, "Type: %s", new_vehicle.type);
    printCentered(vehicle_info, WHITE);
    sprintf(vehicle_info, "License: %s", new_vehicle.license_plate);
    printCentered(vehicle_info, WHITE);
    sprintf(vehicle_info, "Color: %s", new_vehicle.color);
    printCentered(vehicle_info, WHITE);
    printCentered("Vehicle ready for parking reservations!", YELLOW);

    // Save data immediately after adding vehicle
    save_data();
    pause_screen();
}

void view_vehicles() {
    clear_screen();
    print_header();
    printf("\n");
    printCentered("MY REGISTERED VEHICLES", LIGHTGREEN);
    printCentered("Your vehicle fleet overview", LIGHTCYAN);
    printf("\n");

    int found = 0;
    setColor(LIGHTCYAN);
    printf("%*s%-10s %-10s %-15s %-15s %-20s\n",
           (CONSOLE_WIDTH - 70) / 2, "", "ID", "Type", "License", "Color", "Registered");
    printf("%*s", (CONSOLE_WIDTH - 70) / 2, "");
    for (int i = 0; i < 70; i++) printf("=");
    printf("\n");
    resetColor();

    for (int i = 0; i < vehicle_count; i++) {
        if (vehicles[i].user_id == current_user_id) {
            // Color code by vehicle type
            if (strcmp(vehicles[i].type, "bike") == 0) {
                setColor(LIGHTGREEN);
            } else if (strcmp(vehicles[i].type, "car") == 0) {
                setColor(LIGHTBLUE);
            } else {
                setColor(YELLOW);
            }
            printf("%*s%-10d %-10s %-15s %-15s %-20s\n",
                   (CONSOLE_WIDTH - 70) / 2, "",
                   vehicles[i].vehicle_id, vehicles[i].type,
                   vehicles[i].license_plate, vehicles[i].color,
                   vehicles[i].reg_date);
            found = 1;
        }
    }
    resetColor();

    if (!found) {
        printf("\n");
        printCentered("No vehicles registered yet!", YELLOW);
        printCentered("Add your first vehicle to start parking!", LIGHTCYAN);
    } else {
        printf("\n");
        char total_msg[50];
        int vehicle_count_user = 0;
        for (int i = 0; i < vehicle_count; i++) {
            if (vehicles[i].user_id == current_user_id) vehicle_count_user++;
        }
        sprintf(total_msg, "Total Vehicles: %d", vehicle_count_user);
        printCentered(total_msg, LIGHTCYAN);
    }

    pause_screen();
}

void view_available_slots() {
    clear_screen();
    print_header();
    printf("\n");
    printCentered("AVAILABLE PARKING SLOTS", LIGHTCYAN);
    printCentered("Real-time slot availability", LIGHTGREEN);
    printf("\n");

    printCentered("SLOT STATUS LEGEND:", WHITE);
    printCentered("A = Available  O = Occupied  R = Reserved", LIGHTGRAY);
    printf("\n");

    print_colored_slots();

    // Show available count by type
    int bike_available = 0, car_available = 0, truck_available = 0;

    for (int i = 0; i < slot_count; i++) {
        if (strcmp(slots[i].status, "available") == 0) {
            if (strcmp(slots[i].type, "bike") == 0) bike_available++;
            else if (strcmp(slots[i].type, "car") == 0) car_available++;
            else if (strcmp(slots[i].type, "truck") == 0) truck_available++;
        }
    }

    printf("\n");
    printCenteredLine('-', YELLOW);
    printCentered("AVAILABILITY SUMMARY", YELLOW);
    printCenteredLine('-', YELLOW);
    char summary[100];
    sprintf(summary, "Bike Slots: %d/%d Available", bike_available, BIKE_SLOTS);
    printCentered(summary, LIGHTGREEN);
    sprintf(summary, "Car Slots: %d/%d Available", car_available, CAR_SLOTS);
    printCentered(summary, LIGHTGREEN);
    sprintf(summary, "Truck Slots: %d/%d Available", truck_available, TRUCK_SLOTS);
    printCentered(summary, LIGHTGREEN);

    printf("\n");
    printCenteredLine('-', LIGHTCYAN);
    printCentered("HOURLY RATES", LIGHTCYAN);
    printCenteredLine('-', LIGHTCYAN);
    sprintf(summary, "Bike: $%.2f/hour", BIKE_PRICE);
    printCentered(summary, WHITE);
    sprintf(summary, "Car: $%.2f/hour", CAR_PRICE);
    printCentered(summary, WHITE);
    sprintf(summary, "Truck: $%.2f/hour", TRUCK_PRICE);
    printCentered(summary, WHITE);

    pause_screen();
}

void print_colored_slots() {
    printf("\n");
    printCentered("BIKE/MOTORCYCLE SLOTS (1-40)", YELLOW);
    printf("\n");

    for (int i = 0; i < BIKE_SLOTS; i++) {
        if (i % 10 == 0) {
            printf("%*s", (CONSOLE_WIDTH - 40) / 2, "");
        }

        if (strcmp(slots[i].status, "available") == 0) {
            setColor(LIGHTGREEN);
            printf("A%02d ", slots[i].slot_id);
        } else if (strcmp(slots[i].status, "occupied") == 0) {
            setColor(LIGHTRED);
            printf("O%02d ", slots[i].slot_id);
        } else {
            setColor(LIGHTBLUE);
            printf("R%02d ", slots[i].slot_id);
        }

        if ((i + 1) % 10 == 0) {
            resetColor();
            printf("\n");
        }
    }

    printf("\n");
    printCentered("CAR/SUV SLOTS (41-80)", YELLOW);
    printf("\n");

    for (int i = BIKE_SLOTS; i < BIKE_SLOTS + CAR_SLOTS; i++) {
        if ((i - BIKE_SLOTS) % 10 == 0) {
            printf("%*s", (CONSOLE_WIDTH - 40) / 2, "");
        }

        if (strcmp(slots[i].status, "available") == 0) {
            setColor(LIGHTGREEN);
            printf("A%02d ", slots[i].slot_id);
        } else if (strcmp(slots[i].status, "occupied") == 0) {
            setColor(LIGHTRED);
            printf("O%02d ", slots[i].slot_id);
        } else {
            setColor(LIGHTBLUE);
            printf("R%02d ", slots[i].slot_id);
        }

        if ((i - BIKE_SLOTS + 1) % 10 == 0) {
            resetColor();
            printf("\n");
        }
    }

    printf("\n");
    printCentered("TRUCK/VAN SLOTS (81-100)", YELLOW);
    printf("\n");

    for (int i = BIKE_SLOTS + CAR_SLOTS; i < slot_count; i++) {
        if ((i - BIKE_SLOTS - CAR_SLOTS) % 10 == 0) {
            printf("%*s", (CONSOLE_WIDTH - 40) / 2, "");
        }

        if (strcmp(slots[i].status, "available") == 0) {
            setColor(LIGHTGREEN);
            printf("A%02d ", slots[i].slot_id);
        } else if (strcmp(slots[i].status, "occupied") == 0) {
            setColor(LIGHTRED);
            printf("O%02d ", slots[i].slot_id);
        } else {
            setColor(LIGHTBLUE);
            printf("R%02d ", slots[i].slot_id);
        }

        if ((i - BIKE_SLOTS - CAR_SLOTS + 1) % 10 == 0) {
            resetColor();
            printf("\n");
        }
    }
    resetColor();
}










// Reservation Functions
void reserve_slot() {
    clear_screen();
    print_header();
    printf("\n");
    printCentered("RESERVE PARKING SLOT", MAGENTA);
    printCentered("Secure your parking space instantly", LIGHTCYAN);
    printf("\n");

    // Show user's vehicles
    printCenteredLine('-', LIGHTCYAN);
    printCentered("YOUR REGISTERED VEHICLES", LIGHTCYAN);
    printCenteredLine('-', LIGHTCYAN);

    int user_vehicles[100], vehicle_found = 0;

    for (int i = 0; i < vehicle_count; i++) {
        if (vehicles[i].user_id == current_user_id) {
            char vehicle_info[200];
            sprintf(vehicle_info, "%d. %s (%s) - Vehicle ID: %d - %s",
                   vehicle_found + 1, vehicles[i].license_plate, vehicles[i].color,
                   vehicles[i].vehicle_id, vehicles[i].type);
            printCentered(vehicle_info, LIGHTCYAN);
            user_vehicles[vehicle_found] = i;
            vehicle_found++;
        }
    }

    if (vehicle_found == 0) {
        printf("\n");
        printCentered("NO VEHICLES REGISTERED!", YELLOW);
        printCentered("Please add a vehicle first to make reservations.", LIGHTCYAN);
        pause_screen();
        return;
    }

    int vehicle_choice;
    char prompt[100];
    sprintf(prompt, "Select vehicle (1-%d): ", vehicle_found);
    printf("\n");
    setColor(WHITE);
    printf("%*s%s", (CONSOLE_WIDTH - strlen(prompt)) / 2, "", prompt);
    resetColor();
    scanf("%d", &vehicle_choice);

    if (vehicle_choice < 1 || vehicle_choice > vehicle_found) {
        printf("\n");
        printCentered("Invalid vehicle selection!", LIGHTRED);
        pause_screen();
        return;
    }

    int selected_vehicle_index = user_vehicles[vehicle_choice - 1];
    char *vehicle_type = vehicles[selected_vehicle_index].type;

    // Show available slots for this vehicle type
    printf("\n");
    printCenteredLine('-', LIGHTGREEN);
    char available_msg[100];
    sprintf(available_msg, "AVAILABLE %s SLOTS",
            strcmp(vehicle_type, "bike") == 0 ? "BIKE/MOTORCYCLE" :
            strcmp(vehicle_type, "car") == 0 ? "CAR/SUV" : "TRUCK/VAN");
    printCentered(available_msg, LIGHTGREEN);
    printCenteredLine('-', LIGHTGREEN);

    int available_slots[100], available_count = 0;
    printf("\n");

    for (int i = 0; i < slot_count; i++) {
        if (strcmp(slots[i].type, vehicle_type) == 0 &&
            strcmp(slots[i].status, "available") == 0) {
            if (available_count % 10 == 0) {
                printf("%*s", (CONSOLE_WIDTH - 40) / 2, "");
            }
            setColor(LIGHTGREEN);
            printf("%02d ", slots[i].slot_id);
            resetColor();
            if ((available_count + 1) % 10 == 0) {
                printf("\n");
            }
            available_slots[available_count] = i;
            available_count++;
        }
    }
    if (available_count % 10 != 0) printf("\n");

    if (available_count == 0) {
        printf("\n");
        char no_slots_msg[100];
        sprintf(no_slots_msg, "NO AVAILABLE %s SLOTS!",
                strcmp(vehicle_type, "bike") == 0 ? "BIKE" :
                strcmp(vehicle_type, "car") == 0 ? "CAR" : "TRUCK");
        printCentered(no_slots_msg, LIGHTRED);
        printCentered("Please try again later or choose a different vehicle.", YELLOW);
        pause_screen();
        return;
    }

    int slot_id;
    printf("\n");
    setColor(WHITE);
    printf("%*s", (CONSOLE_WIDTH - 35) / 2, "");
    printf("Enter slot number to reserve: ");
    resetColor();
    scanf("%d", &slot_id);

    // Find and reserve the slot
    int slot_index = -1;
    for (int i = 0; i < available_count; i++) {
        if (slots[available_slots[i]].slot_id == slot_id) {
            slot_index = available_slots[i];
            break;
        }
    }

    if (slot_index == -1) {
        printf("\n");
        printCentered("Invalid slot number or slot not available!", LIGHTRED);
        printCentered("Please select from the available slots shown above.", YELLOW);
        pause_screen();
        return;
    }

    double duration;
    setColor(WHITE);
    printf("%*s", (CONSOLE_WIDTH - 35) / 2, "");
    printf("Enter parking duration (hours): ");
    resetColor();
    scanf("%lf", &duration);

    if (duration <= 0 || duration > 24) {
        printf("\n");
        printCentered("Invalid duration! Please enter 1-24 hours.", LIGHTRED);
        pause_screen();
        return;
    }

    // Create reservation
    Reservation new_reservation;
    new_reservation.reservation_id = get_next_reservation_id();
    new_reservation.user_id = current_user_id;
    new_reservation.slot_id = slot_id;
    new_reservation.vehicle_id = vehicles[selected_vehicle_index].vehicle_id;
    new_reservation.duration_hours = duration;

    get_current_time(new_reservation.start_time);
    strcpy(new_reservation.end_time, "");
    strcpy(new_reservation.status, "active");

    // Calculate amount
    double rate;
    if (strcmp(vehicle_type, "bike") == 0) rate = BIKE_PRICE;
    else if (strcmp(vehicle_type, "car") == 0) rate = CAR_PRICE;
    else rate = TRUCK_PRICE;

    new_reservation.total_amount = duration * rate;

    // Update slot status
    strcpy(slots[slot_index].status, "reserved");
    slots[slot_index].vehicle_id = vehicles[selected_vehicle_index].vehicle_id;
    slots[slot_index].user_id = current_user_id;
    strcpy(slots[slot_index].reserved_time, new_reservation.start_time);

    // Add reservation
    reservations[reservation_count] = new_reservation;
    reservation_count++;

    printf("\n");
    printCenteredLine('=', LIGHTGREEN);
    printCentered("RESERVATION CONFIRMED!", LIGHTGREEN);
    printCenteredLine('=', LIGHTGREEN);

    char res_details[200];
    sprintf(res_details, "Reservation ID: %d", new_reservation.reservation_id);
    printCentered(res_details, LIGHTCYAN);
    sprintf(res_details, "Slot Number: %d (%s)", slot_id, vehicle_type);
    printCentered(res_details, WHITE);
    sprintf(res_details, "Vehicle: %s (%s)",
            vehicles[selected_vehicle_index].license_plate,
            vehicles[selected_vehicle_index].color);
    printCentered(res_details, WHITE);
    sprintf(res_details, "Duration: %.1f hours", duration);
    printCentered(res_details, WHITE);
    sprintf(res_details, "Rate: $%.2f/hour", rate);
    printCentered(res_details, WHITE);
    sprintf(res_details, "Total Amount: $%.2f", new_reservation.total_amount);
    printCentered(res_details, YELLOW);
    sprintf(res_details, "Reserved Time: %s", new_reservation.start_time);
    printCentered(res_details, LIGHTGRAY);

    printf("\n");
    printCentered("NEXT STEP: Please proceed to payment to secure your slot!", LIGHTCYAN);

    // Save data immediately after reservation
    save_data();
    pause_screen();
}

void cancel_reservation() {
    clear_screen();
    print_header();
    printf("\n");
    printCentered("CANCEL RESERVATION", LIGHTRED);
    printCentered("Cancel your active parking reservations", LIGHTCYAN);
    printf("\n");

    // Show user's active reservations
    printCenteredLine('-', LIGHTCYAN);
    printCentered("YOUR ACTIVE RESERVATIONS", LIGHTCYAN);
    printCenteredLine('-', LIGHTCYAN);

    int user_reservations[100], reservation_found = 0;

    for (int i = 0; i < reservation_count; i++) {
        if (reservations[i].user_id == current_user_id &&
            strcmp(reservations[i].status, "active") == 0) {
            char res_info[200];
            sprintf(res_info, "%d. Reservation ID: %d - Slot %d - $%.2f (%.1fh) - %s",
                   reservation_found + 1, reservations[i].reservation_id,
                   reservations[i].slot_id, reservations[i].total_amount,
                   reservations[i].duration_hours, reservations[i].start_time);
            printCentered(res_info, LIGHTCYAN);
            user_reservations[reservation_found] = i;
            reservation_found++;
        }
    }

    if (reservation_found == 0) {
        printf("\n");
        printCentered("NO ACTIVE RESERVATIONS FOUND!", YELLOW);
        printCentered("You don't have any reservations to cancel.", LIGHTCYAN);
        pause_screen();
        return;
    }

    int choice;
    char cancel_prompt[100];
    sprintf(cancel_prompt, "Select reservation to cancel (1-%d): ", reservation_found);
    printf("\n");
    setColor(WHITE);
    printf("%*s%s", (CONSOLE_WIDTH - strlen(cancel_prompt)) / 2, "", cancel_prompt);
    resetColor();
    scanf("%d", &choice);

    if (choice < 1 || choice > reservation_found) {
        printf("\n");
        printCentered("Invalid selection!", LIGHTRED);
        pause_screen();
        return;
    }

    int reservation_index = user_reservations[choice - 1];

    // Confirmation
    printf("\n");
    printCenteredLine('-', YELLOW);
    printCentered("CANCELLATION CONFIRMATION", YELLOW);
    printCenteredLine('-', YELLOW);
    char confirm_info[200];
    sprintf(confirm_info, "Reservation ID: %d", reservations[reservation_index].reservation_id);
    printCentered(confirm_info, WHITE);
    sprintf(confirm_info, "Slot: %d | Amount: $%.2f",
            reservations[reservation_index].slot_id,
            reservations[reservation_index].total_amount);
    printCentered(confirm_info, WHITE);

    char confirm;
    printf("\n");
    setColor(YELLOW);
    printf("%*s", (CONSOLE_WIDTH - 35) / 2, "");
    printf("Confirm cancellation? (y/n): ");
    resetColor();
    scanf(" %c", &confirm);

    if (confirm == 'y' || confirm == 'Y') {
        // Update reservation status
        strcpy(reservations[reservation_index].status, "cancelled");
        get_current_time(reservations[reservation_index].end_time);

        // Free up the slot
        for (int i = 0; i < slot_count; i++) {
            if (slots[i].slot_id == reservations[reservation_index].slot_id) {
                strcpy(slots[i].status, "available");
                slots[i].vehicle_id = 0;
                slots[i].user_id = 0;
                strcpy(slots[i].reserved_time, "");
                break;
            }
        }

        printf("\n");
        printCenteredLine('=', LIGHTGREEN);
        printCentered("RESERVATION CANCELLED SUCCESSFULLY!", LIGHTGREEN);
        printCenteredLine('=', LIGHTGREEN);
        printCentered("Your slot has been released and is now available for others.", LIGHTCYAN);

        // Save data immediately after cancellation
        save_data();
    } else {
        printf("\n");
        printCentered("Cancellation aborted. Your reservation remains active.", LIGHTCYAN);
    }

    pause_screen();
}

// Payment Functions
void make_payment() {
    clear_screen();
    print_header();
    printf("\n");
    printCentered("MAKE PAYMENT", YELLOW);
    printCentered("Complete your parking payment", LIGHTCYAN);
    printf("\n");

    // Show unpaid reservations
    printCenteredLine('-', LIGHTCYAN);
    printCentered("PENDING PAYMENTS", LIGHTCYAN);
    printCenteredLine('-', LIGHTCYAN);

    int unpaid_reservations[100], unpaid_count = 0;

    for (int i = 0; i < reservation_count; i++) {
        if (reservations[i].user_id == current_user_id &&
            strcmp(reservations[i].status, "active") == 0) {
            // Check if payment already made
            int payment_made = 0;
            for (int j = 0; j < payment_count; j++) {
                if (payments[j].reservation_id == reservations[i].reservation_id &&
                    strcmp(payments[j].status, "completed") == 0) {
                    payment_made = 1;
                    break;
                }
            }

            if (!payment_made) {
                char unpaid_info[200];
                sprintf(unpaid_info, "%d. Reservation ID: %d - Slot %d - $%.2f (%.1fh)",
                       unpaid_count + 1, reservations[i].reservation_id,
                       reservations[i].slot_id, reservations[i].total_amount,
                       reservations[i].duration_hours);
                printCentered(unpaid_info, LIGHTCYAN);
                unpaid_reservations[unpaid_count] = i;
                unpaid_count++;
            }
        }
    }

    if (unpaid_count == 0) {
        printf("\n");
        printCentered("NO PENDING PAYMENTS!", LIGHTGREEN);
        printCentered("All your reservations are fully paid.", LIGHTCYAN);
        pause_screen();
        return;
    }

    int choice;
    char payment_prompt[100];
    sprintf(payment_prompt, "Select reservation to pay (1-%d): ", unpaid_count);
    printf("\n");
    setColor(WHITE);
    printf("%*s%s", (CONSOLE_WIDTH - strlen(payment_prompt)) / 2, "", payment_prompt);
    resetColor();
    scanf("%d", &choice);

    if (choice < 1 || choice > unpaid_count) {
        printf("\n");
        printCentered("Invalid selection!", LIGHTRED);
        pause_screen();
        return;
    }

    int reservation_index = unpaid_reservations[choice - 1];

    printf("\n");
    printCenteredLine('-', YELLOW);
    printCentered("PAYMENT METHODS", YELLOW);
    printCenteredLine('-', YELLOW);
    printCentered("1. Cash Payment", LIGHTGREEN);
    printCentered("2. Credit/Debit Card", LIGHTBLUE);
    printCentered("3. bKash Mobile Payment", MAGENTA);
    printCentered("4. Nagad Mobile Payment", CYAN);

    int method_choice;
    printf("\n");
    setColor(WHITE);
    printf("%*s", (CONSOLE_WIDTH - 25) / 2, "");
    printf("Select payment method: ");
    resetColor();
    scanf("%d", &method_choice);

    char payment_method[20];
    char method_display[30];
    switch (method_choice) {
        case 1:
            strcpy(payment_method, "cash");
            strcpy(method_display, "Cash Payment");
            break;
        case 2:
            strcpy(payment_method, "card");
            strcpy(method_display, "Credit/Debit Card");
            break;
        case 3:
            strcpy(payment_method, "bkash");
            strcpy(method_display, "bKash Mobile Payment");
            break;
        case 4:
            strcpy(payment_method, "nagad");
            strcpy(method_display, "Nagad Mobile Payment");
            break;
        default:
            printf("\n");
            printCentered("Invalid payment method!", LIGHTRED);
            pause_screen();
            return;
    }

    // Payment confirmation
    printf("\n");
    printCenteredLine('-', YELLOW);
    printCentered("PAYMENT CONFIRMATION", YELLOW);
    printCenteredLine('-', YELLOW);
    char payment_info[200];
    sprintf(payment_info, "Reservation: %d", reservations[reservation_index].reservation_id);
    printCentered(payment_info, WHITE);
    sprintf(payment_info, "Amount: $%.2f", reservations[reservation_index].total_amount);
    printCentered(payment_info, WHITE);
    sprintf(payment_info, "Method: %s", method_display);
    printCentered(payment_info, WHITE);

    char confirm;
    printf("\n");
    setColor(YELLOW);
    printf("%*s", (CONSOLE_WIDTH - 35) / 2, "");
    printf("Confirm payment? (y/n): ");
    resetColor();
    scanf(" %c", &confirm);

    if (confirm != 'y' && confirm != 'Y') {
        printf("\n");
        printCentered("Payment cancelled.", LIGHTCYAN);
        pause_screen();
        return;
    }

    // Create payment record
    Payment new_payment;
    new_payment.payment_id = get_next_payment_id();
    new_payment.user_id = current_user_id;
    new_payment.reservation_id = reservations[reservation_index].reservation_id;
    new_payment.amount = reservations[reservation_index].total_amount;
    strcpy(new_payment.method, payment_method);
    strcpy(new_payment.status, "completed");
    get_current_time(new_payment.payment_date);

    payments[payment_count] = new_payment;
    payment_count++;

    // Update slot to occupied
    for (int i = 0; i < slot_count; i++) {
        if (slots[i].slot_id == reservations[reservation_index].slot_id) {
            strcpy(slots[i].status, "occupied");
            break;
        }
    }

    printf("\n");
    printCenteredLine('=', LIGHTGREEN);
    printCentered("PAYMENT SUCCESSFUL!", LIGHTGREEN);
    printCenteredLine('=', LIGHTGREEN);

    char success_details[200];
    sprintf(success_details, "Payment ID: %d", new_payment.payment_id);
    printCentered(success_details, LIGHTCYAN);
    sprintf(success_details, "Amount Paid: $%.2f", new_payment.amount);
    printCentered(success_details, WHITE);
    sprintf(success_details, "Payment Method: %s", method_display);
    printCentered(success_details, WHITE);
    sprintf(success_details, "Transaction Date: %s", new_payment.payment_date);
    printCentered(success_details, LIGHTGRAY);

    printf("\n");
    printCentered("Your parking slot is now ACTIVE and ready to use!", LIGHTCYAN);
    printCentered("Thank you for choosing SMART PARK!", YELLOW);

    // Save data immediately after payment
    save_data();
    pause_screen();
}

// History and Report Functions
void view_payment_history() {
    clear_screen();
    print_header();
    printf("\n");
    printCentered("PAYMENT HISTORY", LIGHTCYAN);
    printCentered("Your complete transaction history", LIGHTGREEN);
    printf("\n");

    int found = 0;
    double total_paid = 0;

    setColor(LIGHTCYAN);
    printf("%*s%-8s %-12s %-10s %-15s %-20s %-12s\n",
           (CONSOLE_WIDTH - 77) / 2, "",
           "Pay ID", "Reservation", "Amount", "Method", "Date", "Status");
    printf("%*s", (CONSOLE_WIDTH - 77) / 2, "");
    for (int i = 0; i < 77; i++) printf("=");
    printf("\n");
    resetColor();

    for (int i = 0; i < payment_count; i++) {
        if (payments[i].user_id == current_user_id) {
            if (strcmp(payments[i].status, "completed") == 0) {
                setColor(LIGHTGREEN);
                total_paid += payments[i].amount;
            } else {
                setColor(YELLOW);
            }
            printf("%*s%-8d %-12d $%-9.2f %-15s %-20s %-12s\n",
                   (CONSOLE_WIDTH - 77) / 2, "",
                   payments[i].payment_id, payments[i].reservation_id,
                   payments[i].amount, payments[i].method,
                   payments[i].payment_date, payments[i].status);
            found = 1;
        }
    }
    resetColor();

    if (!found) {
        printf("\n");
        printCentered("NO PAYMENT HISTORY FOUND!", YELLOW);
        printCentered("Make your first reservation to start using SMART PARK!", LIGHTCYAN);
    } else {
        printf("\n");
        printCenteredLine('-', LIGHTCYAN);
        char total_info[100];
        sprintf(total_info, "Total Amount Paid: $%.2f", total_paid);
        printCentered(total_info, LIGHTGREEN);
        int payment_count_user = 0;
        for (int i = 0; i < payment_count; i++) {
            if (payments[i].user_id == current_user_id) payment_count_user++;
        }
        sprintf(total_info, "Total Transactions: %d", payment_count_user);
        printCentered(total_info, WHITE);
        sprintf(total_info, "Average per Transaction: $%.2f",
                payment_count_user > 0 ? total_paid / payment_count_user : 0);
        printCentered(total_info, LIGHTCYAN);
        printCenteredLine('-', LIGHTCYAN);
    }

    pause_screen();
}

void view_booking_history() {
    clear_screen();
    print_header();
    printf("\n");
    printCentered("BOOKING HISTORY", WHITE);
    printCentered("Complete record of all your reservations", LIGHTGREEN);
    printf("\n");

    int found = 0;
    int active_count = 0, completed_count = 0, cancelled_count = 0;

    setColor(LIGHTCYAN);
    printf("%*s%-8s %-6s %-8s %-18s %-18s %-10s %-8s\n",
           (CONSOLE_WIDTH - 80) / 2, "",
           "Res ID", "Slot", "Vehicle", "Start Time", "End Time", "Status", "Amount");
    printf("%*s", (CONSOLE_WIDTH - 80) / 2, "");
    for (int i = 0; i < 80; i++) printf("=");
    printf("\n");
    resetColor();

    for (int i = 0; i < reservation_count; i++) {
        if (reservations[i].user_id == current_user_id) {
            if (strcmp(reservations[i].status, "active") == 0) {
                setColor(LIGHTGREEN);
                active_count++;
            } else if (strcmp(reservations[i].status, "completed") == 0) {
                setColor(LIGHTBLUE);
                completed_count++;
            } else {
                setColor(LIGHTRED);
                cancelled_count++;
            }

            char end_time_display[20];
            if (strlen(reservations[i].end_time) == 0) {
                strcpy(end_time_display, "Ongoing");
            } else {
                strcpy(end_time_display, reservations[i].end_time);
            }

            printf("%*s%-8d %-6d %-8d %-18s %-18s %-10s $%-7.2f\n",
                   (CONSOLE_WIDTH - 80) / 2, "",
                   reservations[i].reservation_id, reservations[i].slot_id,
                   reservations[i].vehicle_id, reservations[i].start_time,
                   end_time_display, reservations[i].status,
                   reservations[i].total_amount);
            found = 1;
        }
    }
    resetColor();

    if (!found) {
        printf("\n");
        printCentered("NO BOOKING HISTORY FOUND!", YELLOW);
        printCentered("Start your parking journey by making your first reservation!", LIGHTCYAN);
    } else {
        printf("\n");
        printCenteredLine('-', LIGHTCYAN);
        printCentered("BOOKING SUMMARY", LIGHTCYAN);
        printCenteredLine('-', LIGHTCYAN);
        char summary[200];
        sprintf(summary, "Total Bookings: %d", active_count + completed_count + cancelled_count);
        printCentered(summary, WHITE);
        sprintf(summary, "Active: %d | Completed: %d | Cancelled: %d",
                active_count, completed_count, cancelled_count);
        printCentered(summary, LIGHTCYAN);
        printCenteredLine('-', LIGHTCYAN);
    }

    pause_screen();
}

void check_overstay() {
    clear_screen();
    print_header();
    printf("\n");
    printCentered("OVERSTAY ALERTS", LIGHTRED);
    if (is_admin) {
        printCentered("System-wide overstay monitoring", LIGHTCYAN);
    } else {
        printCentered("Your parking duration alerts", LIGHTCYAN);
    }
    printf("\n");

    int alerts_found = 0;

    for (int i = 0; i < reservation_count; i++) {
        if ((is_admin || reservations[i].user_id == current_user_id) &&
            strcmp(reservations[i].status, "active") == 0) {

            // Simplified overstay detection - assume 2.5 hours passed
            double hours_passed = 2.5;

            if (hours_passed > reservations[i].duration_hours) {
                double overstay_hours = hours_passed - reservations[i].duration_hours;

                printCenteredLine('!', LIGHTRED);
                printCentered("OVERSTAY ALERT!", LIGHTRED);
                printCenteredLine('!', LIGHTRED);

                char alert_info[200];
                sprintf(alert_info, "Reservation ID: %d", reservations[i].reservation_id);
                printCentered(alert_info, WHITE);
                sprintf(alert_info, "Slot Number: %d", reservations[i].slot_id);
                printCentered(alert_info, WHITE);
                sprintf(alert_info, "Booked Duration: %.1f hours", reservations[i].duration_hours);
                printCentered(alert_info, WHITE);
                sprintf(alert_info, "Time Elapsed: %.1f hours", hours_passed);
                printCentered(alert_info, WHITE);
                sprintf(alert_info, "Overstay Time: %.1f hours", overstay_hours);
                printCentered(alert_info, LIGHTRED);

                // Calculate additional charges
                double base_rate;
                for (int j = 0; j < slot_count; j++) {
                    if (slots[j].slot_id == reservations[i].slot_id) {
                        if (strcmp(slots[j].type, "bike") == 0) base_rate = BIKE_PRICE;
                        else if (strcmp(slots[j].type, "car") == 0) base_rate = CAR_PRICE;
                        else base_rate = TRUCK_PRICE;
                        break;
                    }
                }
                double additional_charge = overstay_hours * base_rate * 1.5; // 1.5x penalty
                sprintf(alert_info, "Additional Penalty: $%.2f (1.5x rate)", additional_charge);
                printCentered(alert_info, YELLOW);

                if (is_admin) {
                    for (int j = 0; j < user_count; j++) {
                        if (users[j].user_id == reservations[i].user_id) {
                            sprintf(alert_info, "Customer: %s", users[j].name);
                            printCentered(alert_info, LIGHTCYAN);
                            sprintf(alert_info, "Contact: %s", users[j].phone);
                            printCentered(alert_info, LIGHTCYAN);
                            break;
                        }
                    }
                }

                printf("\n");
                printCenteredLine('-', LIGHTCYAN);
                printf("\n");
                alerts_found = 1;
            }
        }
    }

    if (!alerts_found) {
        printCenteredLine('=', LIGHTGREEN);
        printCentered("NO OVERSTAY ALERTS!", LIGHTGREEN);
        printCenteredLine('=', LIGHTGREEN);
        if (is_admin) {
            printCentered("All customers are within their booked time slots.", LIGHTCYAN);
        } else {
            printCentered("You are within your booked parking duration.", LIGHTCYAN);
        }
    }

    pause_screen();
}

void generate_user_report() {
    clear_screen();
    print_header();
    printf("\n");
    printCentered("MY SMART PARK REPORT", LIGHTGREEN);
    printCentered("Your complete parking statistics", LIGHTCYAN);
    printf("\n");

    int total_bookings = 0, active_bookings = 0, cancelled_bookings = 0, completed_bookings = 0;
    double total_spent = 0.0;
    int vehicle_count_user = 0;

    // Count user's bookings
    for (int i = 0; i < reservation_count; i++) {
        if (reservations[i].user_id == current_user_id) {
            total_bookings++;
            if (strcmp(reservations[i].status, "active") == 0) {
                active_bookings++;
            } else if (strcmp(reservations[i].status, "cancelled") == 0) {
                cancelled_bookings++;
            } else if (strcmp(reservations[i].status, "completed") == 0) {
                completed_bookings++;
            }
        }
    }

    // Count user's spending
    for (int i = 0; i < payment_count; i++) {
        if (payments[i].user_id == current_user_id &&
            strcmp(payments[i].status, "completed") == 0) {
            total_spent += payments[i].amount;
        }
    }

    // Count user's vehicles
    for (int i = 0; i < vehicle_count; i++) {
        if (vehicles[i].user_id == current_user_id) {
            vehicle_count_user++;
        }
    }

    printCenteredLine('=', YELLOW);
    printCentered("YOUR PARKING STATISTICS", YELLOW);
    printCenteredLine('=', YELLOW);

    char stats[200];
    sprintf(stats, "Total Reservations Made: %d", total_bookings);
    printCentered(stats, WHITE);

    sprintf(stats, "Currently Active: %d", active_bookings);
    printCentered(stats, LIGHTGREEN);

    sprintf(stats, "Successfully Completed: %d", completed_bookings);
    printCentered(stats, LIGHTBLUE);

    sprintf(stats, "Cancelled: %d", cancelled_bookings);
    printCentered(stats, LIGHTRED);

    sprintf(stats, "Registered Vehicles: %d", vehicle_count_user);
    printCentered(stats, LIGHTCYAN);

    printf("\n");
    printCenteredLine('-', LIGHTCYAN);
    printCentered("FINANCIAL SUMMARY", LIGHTCYAN);
    printCenteredLine('-', LIGHTCYAN);

    sprintf(stats, "Total Amount Spent: $%.2f", total_spent);
    printCentered(stats, LIGHTGREEN);

    sprintf(stats, "Average Per Booking: $%.2f",
            total_bookings > 0 ? total_spent / total_bookings : 0.0);
    printCentered(stats, YELLOW);

    // Calculate loyalty status
    printf("\n");
    printCenteredLine('-', MAGENTA);
    if (total_spent >= 500) {
        printCentered("LOYALTY STATUS: PLATINUM MEMBER", YELLOW);
        printCentered("You're our most valued customer!", LIGHTGREEN);
    } else if (total_spent >= 200) {
        printCentered("LOYALTY STATUS: GOLD MEMBER", YELLOW);
        printCentered("You're a frequent parker!", LIGHTGREEN);
    } else if (total_spent >= 100) {
        printCentered("LOYALTY STATUS: SILVER MEMBER", LIGHTCYAN);
        printCentered("You're building your parking history!", LIGHTGREEN);
    } else {
        printCentered("LOYALTY STATUS: BRONZE MEMBER", LIGHTGRAY);
        printCentered("Welcome to SMART PARK family!", LIGHTGREEN);
    }
    printCenteredLine('-', MAGENTA);

    pause_screen();
}







// Admin Functions
void admin_view_users() {
    clear_screen();
    print_header();
    printf("\n");
    printCentered("USER MANAGEMENT DASHBOARD", LIGHTBLUE);
    printCentered("Complete overview of all registered customers", LIGHTCYAN);
    printf("\n");

    if (user_count == 0) {
        printCenteredLine('!', YELLOW);
        printCentered("NO USERS REGISTERED YET!", YELLOW);
        printCenteredLine('!', YELLOW);
        printCentered("The system is ready to accept new registrations.", LIGHTCYAN);
        pause_screen();
        return;
    }

    setColor(LIGHTCYAN);
    printf("%*s%-8s %-18s %-13s %-25s %-18s %-8s\n",
           (CONSOLE_WIDTH - 90) / 2, "",
           "ID", "Name", "Phone", "Email", "Reg Date", "Status");
    printf("%*s", (CONSOLE_WIDTH - 90) / 2, "");
    for (int i = 0; i < 90; i++) printf("=");
    printf("\n");
    resetColor();

    for (int i = 0; i < user_count; i++) {
        if (users[i].is_active) {
            setColor(LIGHTGREEN);
        } else {
            setColor(LIGHTRED);
        }
        printf("%*s%-8d %-18s %-13s %-25s %-18s %-8s\n",
               (CONSOLE_WIDTH - 90) / 2, "",
               users[i].user_id, users[i].name, users[i].phone,
               users[i].email, users[i].reg_date,
               users[i].is_active ? "Active" : "Inactive");
    }
    resetColor();

    int active_count = 0;
    for (int i = 0; i < user_count; i++) {
        if (users[i].is_active) active_count++;
    }

    printf("\n");
    printCenteredLine('-', LIGHTCYAN);
    printCentered("USER STATISTICS", LIGHTCYAN);
    printCenteredLine('-', LIGHTCYAN);
    char summary[200];
    sprintf(summary, "Total Registered: %d | Active: %d | Inactive: %d",
            user_count, active_count, user_count - active_count);
    printCentered(summary, WHITE);
    sprintf(summary, "Active Rate: %.1f%% | Growth Potential: %d users",
            user_count > 0 ? ((double)active_count/user_count)*100 : 0,
            user_count - active_count);
    printCentered(summary, LIGHTCYAN);
    printCenteredLine('-', LIGHTCYAN);

    pause_screen();
}

void admin_search_users() {
    clear_screen();
    print_header();
    printf("\n");
    printCentered("ADVANCED USER SEARCH", LIGHTGREEN);
    printCentered("Find customers by various criteria", LIGHTCYAN);
    printf("\n");

    printCenteredLine('-', WHITE);
    printCentered("SEARCH OPTIONS", WHITE);
    printCenteredLine('-', WHITE);
    printCentered("1. Search by Name", LIGHTGREEN);
    printCentered("2. Search by Phone", LIGHTGREEN);
    printCentered("3. Search by Email", LIGHTGREEN);
    printCentered("4. Search by User ID", LIGHTGREEN);

    int choice;
    printf("\n");
    setColor(WHITE);
    printf("%*s", (CONSOLE_WIDTH - 15) / 2, "");
    printf("Enter choice: ");
    resetColor();
    scanf("%d", &choice);

    char search_term[MAX_STRING];
    int search_id = 0;

    printf("\n");
    if (choice == 4) {
        setColor(WHITE);
        printf("%*s", (CONSOLE_WIDTH - 18) / 2, "");
        printf("Enter User ID: ");
        resetColor();
        scanf("%d", &search_id);
    } else {
        setColor(WHITE);
        printf("%*s", (CONSOLE_WIDTH - 20) / 2, "");
        printf("Enter search term: ");
        resetColor();
        scanf("%s", search_term);
    }

    int found = 0;
    printf("\n");
    printCenteredLine('-', LIGHTCYAN);
    printCentered("SEARCH RESULTS", LIGHTCYAN);
    printCenteredLine('-', LIGHTCYAN);

    setColor(LIGHTCYAN);
    printf("%*s%-8s %-18s %-13s %-25s %-18s %-8s\n",
           (CONSOLE_WIDTH - 90) / 2, "",
           "ID", "Name", "Phone", "Email", "Reg Date", "Status");
    printf("%*s", (CONSOLE_WIDTH - 90) / 2, "");
    for (int i = 0; i < 90; i++) printf("=");
    printf("\n");
    resetColor();

    for (int i = 0; i < user_count; i++) {
        int match = 0;

        switch (choice) {
            case 1:
                if (strstr(users[i].name, search_term) != NULL) match = 1;
                break;
            case 2:
                if (strstr(users[i].phone, search_term) != NULL) match = 1;
                break;
            case 3:
                if (strstr(users[i].email, search_term) != NULL) match = 1;
                break;
            case 4:
                if (users[i].user_id == search_id) match = 1;
                break;
        }

        if (match) {
            if (users[i].is_active) {
                setColor(LIGHTGREEN);
            } else {
                setColor(LIGHTRED);
            }
            printf("%*s%-8d %-18s %-13s %-25s %-18s %-8s\n",
                   (CONSOLE_WIDTH - 90) / 2, "",
                   users[i].user_id, users[i].name, users[i].phone,
                   users[i].email, users[i].reg_date,
                   users[i].is_active ? "Active" : "Inactive");
            found = 1;
        }
    }
    resetColor();

    if (!found) {
        printf("\n");
        printCentered("NO MATCHING USERS FOUND!", YELLOW);
        printCentered("Try different search criteria or check spelling.", LIGHTCYAN);
    } else {
        printf("\n");
        printCentered("Search completed successfully.", LIGHTGREEN);
    }

    pause_screen();
}

void admin_delete_user() {
    clear_screen();
    print_header();
    printf("\n");
    printCentered("USER DEACTIVATION PORTAL", LIGHTRED);
    printCentered("Safely deactivate customer accounts", YELLOW);
    printf("\n");

    int user_id;
    setColor(WHITE);
    printf("%*s", (CONSOLE_WIDTH - 35) / 2, "");
    printf("Enter User ID to deactivate: ");
    resetColor();
    scanf("%d", &user_id);

    int user_index = -1;
    for (int i = 0; i < user_count; i++) {
        if (users[i].user_id == user_id) {
            user_index = i;
            break;
        }
    }

    if (user_index == -1) {
        printf("\n");
        printCentered("USER NOT FOUND!", LIGHTRED);
        printCentered("Please verify the User ID and try again.", YELLOW);
        pause_screen();
        return;
    }

    printf("\n");
    printCenteredLine('-', LIGHTCYAN);
    printCentered("USER ACCOUNT DETAILS", LIGHTCYAN);
    printCenteredLine('-', LIGHTCYAN);
    char details[200];
    sprintf(details, "User ID: %d", users[user_index].user_id);
    printCentered(details, WHITE);
    sprintf(details, "Full Name: %s", users[user_index].name);
    printCentered(details, WHITE);
    sprintf(details, "Phone Number: %s", users[user_index].phone);
    printCentered(details, WHITE);
    sprintf(details, "Email Address: %s", users[user_index].email);
    printCentered(details, WHITE);
    sprintf(details, "Registration Date: %s", users[user_index].reg_date);
    printCentered(details, LIGHTGRAY);
    sprintf(details, "Current Status: %s", users[user_index].is_active ? "ACTIVE" : "INACTIVE");
    if (users[user_index].is_active) {
        printCentered(details, LIGHTGREEN);
    } else {
        printCentered(details, LIGHTRED);
    }

    if (!users[user_index].is_active) {
        printf("\n");
        printCentered("This user is already deactivated!", YELLOW);
        pause_screen();
        return;
    }

    // Check for active reservations
    int active_reservations = 0;
    for (int i = 0; i < reservation_count; i++) {
        if (reservations[i].user_id == user_id &&
            strcmp(reservations[i].status, "active") == 0) {
            active_reservations++;
        }
    }

    if (active_reservations > 0) {
        printf("\n");
        printCenteredLine('!', YELLOW);
        char warning[100];
        sprintf(warning, "WARNING: User has %d active reservation(s)!", active_reservations);
        printCentered(warning, YELLOW);
        printCentered("Deactivating will cancel all active bookings.", LIGHTRED);
        printCenteredLine('!', YELLOW);
    }

    char confirm;
    printf("\n");
    setColor(YELLOW);
    printf("%*s", (CONSOLE_WIDTH - 50) / 2, "");
    printf("Confirm user deactivation? (y/n): ");
    resetColor();
    scanf(" %c", &confirm);

    if (confirm == 'y' || confirm == 'Y') {
        users[user_index].is_active = 0;

        // Cancel active reservations
        for (int i = 0; i < reservation_count; i++) {
            if (reservations[i].user_id == user_id &&
                strcmp(reservations[i].status, "active") == 0) {
                strcpy(reservations[i].status, "cancelled");
                get_current_time(reservations[i].end_time);

                // Free up slots
                for (int j = 0; j < slot_count; j++) {
                    if (slots[j].slot_id == reservations[i].slot_id) {
                        strcpy(slots[j].status, "available");
                        slots[j].vehicle_id = 0;
                        slots[j].user_id = 0;
                        strcpy(slots[j].reserved_time, "");
                        break;
                    }
                }
            }
        }

        printf("\n");
        printCenteredLine('=', LIGHTGREEN);
        printCentered("USER DEACTIVATED SUCCESSFULLY!", LIGHTGREEN);
        printCenteredLine('=', LIGHTGREEN);
        if (active_reservations > 0) {
            sprintf(details, "%d active reservation(s) have been cancelled.", active_reservations);
            printCentered(details, LIGHTCYAN);
        }
        printCentered("User account is now inactive but data is preserved.", LIGHTCYAN);
        save_data();
    } else {
        printf("\n");
        printCentered("Deactivation cancelled. User remains active.", LIGHTCYAN);
    }

    pause_screen();
}

void admin_view_user_history() {
    clear_screen();
    print_header();
    printf("\n");
    printCentered("CUSTOMER PROFILE ANALYZER", YELLOW);
    printCentered("Comprehensive customer activity report", LIGHTCYAN);
    printf("\n");

    int user_id;
    setColor(WHITE);
    printf("%*s", (CONSOLE_WIDTH - 35) / 2, "");
    printf("Enter User ID for analysis: ");
    resetColor();
    scanf("%d", &user_id);

    int user_index = -1;
    for (int i = 0; i < user_count; i++) {
        if (users[i].user_id == user_id) {
            user_index = i;
            break;
        }
    }

    if (user_index == -1) {
        printf("\n");
        printCentered("CUSTOMER NOT FOUND!", LIGHTRED);
        printCentered("Please verify the User ID.", YELLOW);
        pause_screen();
        return;
    }

    printf("\n");
    printCenteredLine('=', LIGHTCYAN);
    printCentered("CUSTOMER PROFILE", LIGHTCYAN);
    printCenteredLine('=', LIGHTCYAN);
    char user_info[200];
    sprintf(user_info, "%s (ID: %d)", users[user_index].name, users[user_index].user_id);
    printCentered(user_info, LIGHTGREEN);
    sprintf(user_info, "Contact: %s | Email: %s", users[user_index].phone, users[user_index].email);
    printCentered(user_info, WHITE);
    sprintf(user_info, "Member Since: %s | Status: %s",
           users[user_index].reg_date,
           users[user_index].is_active ? "ACTIVE" : "INACTIVE");
    printCentered(user_info, LIGHTGRAY);

    // Vehicle Analysis
    printf("\n");
    printCenteredLine('-', LIGHTGREEN);
    printCentered("REGISTERED VEHICLES", LIGHTGREEN);
    printCenteredLine('-', LIGHTGREEN);
    int vehicle_found = 0;
    int bike_count = 0, car_count = 0, truck_count = 0;

    for (int i = 0; i < vehicle_count; i++) {
        if (vehicles[i].user_id == user_id) {
            char vehicle_info[200];
            sprintf(vehicle_info, "%s %s (%s) - ID: %d - Reg: %s",
                   vehicles[i].type, vehicles[i].license_plate, vehicles[i].color,
                   vehicles[i].vehicle_id, vehicles[i].reg_date);

            if (strcmp(vehicles[i].type, "bike") == 0) {
                printCentered(vehicle_info, LIGHTGREEN);
                bike_count++;
            } else if (strcmp(vehicles[i].type, "car") == 0) {
                printCentered(vehicle_info, LIGHTBLUE);
                car_count++;
            } else {
                printCentered(vehicle_info, YELLOW);
                truck_count++;
            }
            vehicle_found = 1;
        }
    }
    if (!vehicle_found) {
        printCentered("No vehicles registered", YELLOW);
    } else {
        char vehicle_summary[200];
        sprintf(vehicle_summary, "Fleet: %d Bikes | %d Cars | %d Trucks = %d Total",
                bike_count, car_count, truck_count, bike_count + car_count + truck_count);
        printCentered(vehicle_summary, LIGHTCYAN);
    }

    // Reservation Analysis
    printf("\n");
    printCenteredLine('-', LIGHTBLUE);
    printCentered("RESERVATION HISTORY", LIGHTBLUE);
    printCenteredLine('-', LIGHTBLUE);
    int reservation_found = 0;
    int active_res = 0, completed_res = 0, cancelled_res = 0;
    double total_booking_value = 0;

    for (int i = 0; i < reservation_count; i++) {
        if (reservations[i].user_id == user_id) {
            char res_info[200];
            sprintf(res_info, "ID: %d | Slot %d | $%.2f | %.1fh | %s | %s",
                   reservations[i].reservation_id, reservations[i].slot_id,
                   reservations[i].total_amount, reservations[i].duration_hours,
                   reservations[i].status, reservations[i].start_time);

            if (strcmp(reservations[i].status, "active") == 0) {
                printCentered(res_info, LIGHTGREEN);
                active_res++;
            } else if (strcmp(reservations[i].status, "completed") == 0) {
                printCentered(res_info, LIGHTBLUE);
                completed_res++;
            } else {
                printCentered(res_info, LIGHTRED);
                cancelled_res++;
            }
            total_booking_value += reservations[i].total_amount;
            reservation_found = 1;
        }
    }
    if (!reservation_found) {
        printCentered("No reservations found", YELLOW);
    } else {
        char res_summary[200];
        sprintf(res_summary, "Bookings: %d Active | %d Completed | %d Cancelled",
                active_res, completed_res, cancelled_res);
        printCentered(res_summary, LIGHTCYAN);
        sprintf(res_summary, "Total Booking Value: $%.2f", total_booking_value);
        printCentered(res_summary, LIGHTGREEN);
    }

    // Payment Analysis
    printf("\n");
    printCenteredLine('-', MAGENTA);
    printCentered("PAYMENT HISTORY", MAGENTA);
    printCenteredLine('-', MAGENTA);
    int payment_found = 0;
    double total_paid = 0;
    int cash_count = 0, card_count = 0, mobile_count = 0;

    for (int i = 0; i < payment_count; i++) {
        if (payments[i].user_id == user_id) {
            char pay_info[200];
            sprintf(pay_info, "ID: %d | $%.2f via %s | %s | %s",
                   payments[i].payment_id, payments[i].amount,
                   payments[i].method, payments[i].status, payments[i].payment_date);

            if (strcmp(payments[i].status, "completed") == 0) {
                printCentered(pay_info, LIGHTGREEN);
                total_paid += payments[i].amount;

                if (strcmp(payments[i].method, "cash") == 0) cash_count++;
                else if (strcmp(payments[i].method, "card") == 0) card_count++;
                else mobile_count++;
            } else {
                printCentered(pay_info, YELLOW);
            }
            payment_found = 1;
        }
    }
    if (!payment_found) {
        printCentered("No payments found", YELLOW);
    } else {
        char pay_summary[200];
        sprintf(pay_summary, "Total Paid: $%.2f | Methods: %d Cash, %d Card, %d Mobile",
                total_paid, cash_count, card_count, mobile_count);
        printCentered(pay_summary, LIGHTGREEN);

        // Customer value analysis
        printf("\n");
        printCenteredLine('-', YELLOW);
        if (total_paid >= 500) {
            printCentered("CUSTOMER TIER: PLATINUM (Premium Customer)", YELLOW);
        } else if (total_paid >= 200) {
            printCentered("CUSTOMER TIER: GOLD (Valuable Customer)", YELLOW);
        } else if (total_paid >= 100) {
            printCentered("CUSTOMER TIER: SILVER (Regular Customer)", LIGHTCYAN);
        } else {
            printCentered("CUSTOMER TIER: BRONZE (New Customer)", LIGHTGRAY);
        }
        printCenteredLine('-', YELLOW);
    }

    pause_screen();
}

void admin_view_all_vehicles() {
    clear_screen();
    print_header();
    printf("\n");
    printCentered("VEHICLE REGISTRY SYSTEM", LIGHTCYAN);
    printCentered("Complete database of all registered vehicles", LIGHTGREEN);
    printf("\n");

    if (vehicle_count == 0) {
        printCenteredLine('!', YELLOW);
        printCentered("NO VEHICLES REGISTERED!", YELLOW);
        printCenteredLine('!', YELLOW);
        printCentered("System is ready to accept vehicle registrations.", LIGHTCYAN);
        pause_screen();
        return;
    }

    setColor(LIGHTCYAN);
    printf("%*s%-6s %-8s %-12s %-10s %-15s %-12s %-15s\n",
           (CONSOLE_WIDTH - 78) / 2, "",
           "V-ID", "Type", "License", "Color", "Owner", "Phone", "Registered");
    printf("%*s", (CONSOLE_WIDTH - 78) / 2, "");
    for (int i = 0; i < 78; i++) printf("=");
    printf("\n");
    resetColor();

    int bike_count = 0, car_count = 0, truck_count = 0;

    for (int i = 0; i < vehicle_count; i++) {
        char owner_name[MAX_STRING] = "Unknown";
        char owner_phone[20] = "Unknown";

        for (int j = 0; j < user_count; j++) {
            if (users[j].user_id == vehicles[i].user_id) {
                strcpy(owner_name, users[j].name);
                strcpy(owner_phone, users[j].phone);
                break;
            }
        }

        // Color code by vehicle type and count
        if (strcmp(vehicles[i].type, "bike") == 0) {
            setColor(LIGHTGREEN);
            bike_count++;
        } else if (strcmp(vehicles[i].type, "car") == 0) {
            setColor(LIGHTBLUE);
            car_count++;
        } else {
            setColor(YELLOW);
            truck_count++;
        }

        printf("%*s%-6d %-8s %-12s %-10s %-15s %-12s %-15s\n",
               (CONSOLE_WIDTH - 78) / 2, "",
               vehicles[i].vehicle_id, vehicles[i].type,
               vehicles[i].license_plate, vehicles[i].color,
               owner_name, owner_phone, vehicles[i].reg_date);
    }
    resetColor();

    printf("\n");
    printCenteredLine('-', LIGHTCYAN);
    printCentered("VEHICLE FLEET ANALYSIS", LIGHTCYAN);
    printCenteredLine('-', LIGHTCYAN);
    char fleet_stats[200];
    sprintf(fleet_stats, "Total Vehicles: %d", vehicle_count);
    printCentered(fleet_stats, WHITE);
    sprintf(fleet_stats, "Bikes: %d (%.1f%%) | Cars: %d (%.1f%%) | Trucks: %d (%.1f%%)",
            bike_count, ((double)bike_count/vehicle_count)*100,
            car_count, ((double)car_count/vehicle_count)*100,
            truck_count, ((double)truck_count/vehicle_count)*100);
    printCentered(fleet_stats, LIGHTCYAN);

    // Most popular vehicle type
    if (bike_count > car_count && bike_count > truck_count) {
        printCentered("Most Popular: Bikes/Motorcycles", LIGHTGREEN);
    } else if (car_count > truck_count) {
        printCentered("Most Popular: Cars/SUVs", LIGHTBLUE);
    } else {
        printCentered("Most Popular: Trucks/Vans", YELLOW);
    }
    printCenteredLine('-', LIGHTCYAN);

    pause_screen();
}

void admin_slot_usage_report() {
    clear_screen();
    print_header();
    printf("\n");
    printCentered("PARKING INFRASTRUCTURE ANALYTICS", MAGENTA);
    printCentered("Real-time slot utilization and efficiency metrics", LIGHTCYAN);
    printf("\n");

    int bike_available = 0, bike_occupied = 0, bike_reserved = 0;
    int car_available = 0, car_occupied = 0, car_reserved = 0;
    int truck_available = 0, truck_occupied = 0, truck_reserved = 0;

    for (int i = 0; i < slot_count; i++) {
        if (strcmp(slots[i].type, "bike") == 0) {
            if (strcmp(slots[i].status, "available") == 0) bike_available++;
            else if (strcmp(slots[i].status, "occupied") == 0) bike_occupied++;
            else bike_reserved++;
        } else if (strcmp(slots[i].type, "car") == 0) {
            if (strcmp(slots[i].status, "available") == 0) car_available++;
            else if (strcmp(slots[i].status, "occupied") == 0) car_occupied++;
            else car_reserved++;
        } else if (strcmp(slots[i].type, "truck") == 0) {
            if (strcmp(slots[i].status, "available") == 0) truck_available++;
            else if (strcmp(slots[i].status, "occupied") == 0) truck_occupied++;
            else truck_reserved++;
        }
    }

    printCenteredLine('=', LIGHTGREEN);
    printCentered("BIKE/MOTORCYCLE PARKING ZONE", LIGHTGREEN);
    printCenteredLine('=', LIGHTGREEN);
    char stats[200];
    sprintf(stats, "Capacity: %d slots | Available: %d | Occupied: %d | Reserved: %d",
            BIKE_SLOTS, bike_available, bike_occupied, bike_reserved);
    printCentered(stats, WHITE);
    sprintf(stats, "Utilization: %.1f%% | Efficiency: %.1f%% | Revenue/Hour: $%.2f",
            ((double)(bike_occupied + bike_reserved) / BIKE_SLOTS) * 100,
            ((double)bike_occupied / BIKE_SLOTS) * 100,
            bike_occupied * BIKE_PRICE);
    printCentered(stats, LIGHTCYAN);

    printf("\n");
    printCenteredLine('=', LIGHTBLUE);
    printCentered("CAR/SUV PARKING ZONE", LIGHTBLUE);
    printCenteredLine('=', LIGHTBLUE);
    sprintf(stats, "Capacity: %d slots | Available: %d | Occupied: %d | Reserved: %d",
            CAR_SLOTS, car_available, car_occupied, car_reserved);
    printCentered(stats, WHITE);
    sprintf(stats, "Utilization: %.1f%% | Efficiency: %.1f%% | Revenue/Hour: $%.2f",
            ((double)(car_occupied + car_reserved) / CAR_SLOTS) * 100,
            ((double)car_occupied / CAR_SLOTS) * 100,
            car_occupied * CAR_PRICE);
    printCentered(stats, LIGHTCYAN);

    printf("\n");
    printCenteredLine('=', YELLOW);
    printCentered("TRUCK/VAN PARKING ZONE", YELLOW);
    printCenteredLine('=', YELLOW);
    sprintf(stats, "Capacity: %d slots | Available: %d | Occupied: %d | Reserved: %d",
            TRUCK_SLOTS, truck_available, truck_occupied, truck_reserved);
    printCentered(stats, WHITE);
    sprintf(stats, "Utilization: %.1f%% | Efficiency: %.1f%% | Revenue/Hour: $%.2f",
            ((double)(truck_occupied + truck_reserved) / TRUCK_SLOTS) * 100,
            ((double)truck_occupied / TRUCK_SLOTS) * 100,
            truck_occupied * TRUCK_PRICE);
    printCentered(stats, LIGHTCYAN);

    printf("\n");
    printCenteredLine('-', MAGENTA);
    printCentered("OVERALL FACILITY PERFORMANCE", MAGENTA);
    printCenteredLine('-', MAGENTA);

    int total_slots = BIKE_SLOTS + CAR_SLOTS + TRUCK_SLOTS;
    int total_used = bike_occupied + bike_reserved + car_occupied + car_reserved + truck_occupied + truck_reserved;
    int total_available = bike_available + car_available + truck_available;
    int total_occupied = bike_occupied + car_occupied + truck_occupied;
    double total_revenue_per_hour = (bike_occupied * BIKE_PRICE) + (car_occupied * CAR_PRICE) + (truck_occupied * TRUCK_PRICE);

    sprintf(stats, "Total Capacity: %d slots | In Use: %d (%.1f%%) | Available: %d (%.1f%%)",
            total_slots, total_used, ((double)total_used/total_slots)*100,
            total_available, ((double)total_available/total_slots)*100);
    printCentered(stats, WHITE);

    sprintf(stats, "Occupancy Rate: %.1f%% | Current Revenue/Hour: $%.2f",
            ((double)total_occupied / total_slots) * 100, total_revenue_per_hour);
    printCentered(stats, LIGHTGREEN);

    sprintf(stats, "Peak Capacity: $%.2f/hour | Daily Potential: $%.2f",
            (BIKE_SLOTS * BIKE_PRICE) + (CAR_SLOTS * CAR_PRICE) + (TRUCK_SLOTS * TRUCK_PRICE),
            ((BIKE_SLOTS * BIKE_PRICE) + (CAR_SLOTS * CAR_PRICE) + (TRUCK_SLOTS * TRUCK_PRICE)) * 24);
    printCentered(stats, YELLOW);

    // Performance indicators
    printf("\n");
    double occupancy_rate = ((double)total_occupied / total_slots) * 100;
    if (occupancy_rate >= 80) {
        printCentered("STATUS: EXCELLENT UTILIZATION", LIGHTGREEN);
        printCentered("Facility is performing at optimal capacity", LIGHTCYAN);
    } else if (occupancy_rate >= 60) {
        printCentered("STATUS: GOOD UTILIZATION", LIGHTBLUE);
        printCentered("Solid performance with room for growth", LIGHTCYAN);
    } else if (occupancy_rate >= 40) {
        printCentered("STATUS: MODERATE UTILIZATION", YELLOW);
        printCentered("Consider marketing strategies to increase usage", LIGHTCYAN);
    } else {
        printCentered("STATUS: LOW UTILIZATION", LIGHTRED);
        printCentered("Significant opportunity for improvement", LIGHTCYAN);
    }

    pause_screen();
}

void admin_generate_daily_report() {
    clear_screen();
    print_header();
    printf("\n");
    printCentered("SMART PARK EXECUTIVE DASHBOARD", WHITE);
    printCentered("Comprehensive Business Intelligence Report", LIGHTGREEN);
    printf("\n");

    char today[20];
    get_current_time(today);
    char report_title[100];
    sprintf(report_title, "Generated: %s | By: Administrator", today);
    printCentered(report_title, LIGHTGRAY);
    printCenteredLine('=', LIGHTCYAN);

    // Calculate all metrics
    int total_users = user_count;
    int active_users = 0;
    int total_vehicles = vehicle_count;
    int active_reservations = 0;
    int completed_reservations = 0;
    int cancelled_reservations = 0;
    int completed_payments = 0;
    double total_revenue = 0.0;

    for (int i = 0; i < user_count; i++) {
        if (users[i].is_active) active_users++;
    }

    for (int i = 0; i < reservation_count; i++) {
        if (strcmp(reservations[i].status, "active") == 0) {
            active_reservations++;
        } else if (strcmp(reservations[i].status, "completed") == 0) {
            completed_reservations++;
        } else if (strcmp(reservations[i].status, "cancelled") == 0) {
            cancelled_reservations++;
        }
    }

    for (int i = 0; i < payment_count; i++) {
        if (strcmp(payments[i].status, "completed") == 0) {
            completed_payments++;
            total_revenue += payments[i].amount;
        }
    }

    printf("\n");
    printCenteredLine('-', LIGHTBLUE);
    printCentered("CUSTOMER BASE ANALYTICS", LIGHTBLUE);
    printCenteredLine('-', LIGHTBLUE);
    char metrics[200];
    sprintf(metrics, "Total Customers: %d | Active: %d (%.1f%%) | Inactive: %d (%.1f%%)",
            total_users, active_users,
            total_users > 0 ? ((double)active_users/total_users)*100 : 0,
            total_users - active_users,
            total_users > 0 ? ((double)(total_users - active_users)/total_users)*100 : 0);
    printCentered(metrics, WHITE);

    sprintf(metrics, "Customer Retention Rate: %.1f%% | Growth Potential: High",
            total_users > 0 ? ((double)active_users/total_users)*100 : 0);
    printCentered(metrics, LIGHTCYAN);

    printf("\n");
    printCenteredLine('-', LIGHTGREEN);
    printCentered("VEHICLE FLEET OVERVIEW", LIGHTGREEN);
    printCenteredLine('-', LIGHTGREEN);
    int bikes = 0, cars = 0, trucks = 0;
    for (int i = 0; i < vehicle_count; i++) {
        if (strcmp(vehicles[i].type, "bike") == 0) bikes++;
        else if (strcmp(vehicles[i].type, "car") == 0) cars++;
        else trucks++;
    }

    sprintf(metrics, "Total Registered Vehicles: %d", total_vehicles);
    printCentered(metrics, WHITE);
    sprintf(metrics, "Distribution: %d Bikes (%.1f%%) | %d Cars (%.1f%%) | %d Trucks (%.1f%%)",
            bikes, total_vehicles > 0 ? ((double)bikes/total_vehicles)*100 : 0,
            cars, total_vehicles > 0 ? ((double)cars/total_vehicles)*100 : 0,
            trucks, total_vehicles > 0 ? ((double)trucks/total_vehicles)*100 : 0);
    printCentered(metrics, LIGHTCYAN);
    sprintf(metrics, "Vehicles per Customer: %.1f | Fleet Diversity: %s",
            total_users > 0 ? (double)total_vehicles/total_users : 0,
            (bikes > 0 && cars > 0 && trucks > 0) ? "Excellent" : "Good");
    printCentered(metrics, LIGHTGREEN);

    printf("\n");
    printCenteredLine('-', MAGENTA);
    printCentered("BOOKING & RESERVATION METRICS", MAGENTA);
    printCenteredLine('-', MAGENTA);
    int total_reservations = reservation_count;
    sprintf(metrics, "Total Bookings: %d | Success Rate: %.1f%%",
            total_reservations,
            total_reservations > 0 ? ((double)(active_reservations + completed_reservations)/total_reservations)*100 : 0);
    printCentered(metrics, WHITE);
    sprintf(metrics, "Active: %d | Completed: %d | Cancelled: %d",
            active_reservations, completed_reservations, cancelled_reservations);
    printCentered(metrics, LIGHTCYAN);
    sprintf(metrics, "Average Bookings per Customer: %.1f",
            total_users > 0 ? (double)total_reservations/total_users : 0);
    printCentered(metrics, LIGHTGREEN);

    printf("\n");
    printCenteredLine('-', YELLOW);
    printCentered("FINANCIAL PERFORMANCE", YELLOW);
    printCenteredLine('-', YELLOW);
    sprintf(metrics, "Total Revenue Generated: $%.2f", total_revenue);
    printCentered(metrics, LIGHTGREEN);
    sprintf(metrics, "Completed Transactions: %d | Payment Success Rate: %.1f%%",
            completed_payments,
            payment_count > 0 ? ((double)completed_payments/payment_count)*100 : 0);
    printCentered(metrics, WHITE);
    sprintf(metrics, "Average Transaction Value: $%.2f | Revenue per Customer: $%.2f",
            completed_payments > 0 ? total_revenue / completed_payments : 0.0,
            total_users > 0 ? total_revenue / total_users : 0.0);
    printCentered(metrics, LIGHTCYAN);

    // Payment method analysis
    int cash_payments = 0, card_payments = 0, mobile_payments = 0;
    double cash_amount = 0, card_amount = 0, mobile_amount = 0;

    for (int i = 0; i < payment_count; i++) {
        if (strcmp(payments[i].status, "completed") == 0) {
            if (strcmp(payments[i].method, "cash") == 0) {
                cash_payments++;
                cash_amount += payments[i].amount;
            } else if (strcmp(payments[i].method, "card") == 0) {
                card_payments++;
                card_amount += payments[i].amount;
            } else {
                mobile_payments++;
                mobile_amount += payments[i].amount;
            }
        }
    }

    sprintf(metrics, "Payment Methods: Cash: %d ($%.2f) | Card: %d ($%.2f) | Mobile: %d ($%.2f)",
            cash_payments, cash_amount, card_payments, card_amount, mobile_payments, mobile_amount);
    printCentered(metrics, LIGHTBLUE);

    // Calculate current facility utilization
    int total_occupied = 0, total_reserved = 0;
    for (int i = 0; i < slot_count; i++) {
        if (strcmp(slots[i].status, "occupied") == 0) total_occupied++;
        else if (strcmp(slots[i].status, "reserved") == 0) total_reserved++;
    }

    double current_utilization = ((double)(total_occupied + total_reserved) / slot_count) * 100;
    double revenue_efficiency = ((double)total_occupied / slot_count) * 100;

    printf("\n");
    printCenteredLine('-', LIGHTCYAN);
    printCentered("OPERATIONAL EFFICIENCY", LIGHTCYAN);
    printCenteredLine('-', LIGHTCYAN);
    sprintf(metrics, "Current Facility Utilization: %.1f%% | Revenue Efficiency: %.1f%%",
            current_utilization, revenue_efficiency);
    printCentered(metrics, WHITE);
    sprintf(metrics, "Slots in Use: %d | Available Capacity: %d | Peak Potential: $%.2f/day",
            total_occupied + total_reserved, slot_count - total_occupied - total_reserved,
            ((BIKE_SLOTS * BIKE_PRICE) + (CAR_SLOTS * CAR_PRICE) + (TRUCK_SLOTS * TRUCK_PRICE)) * 24);
    printCentered(metrics, LIGHTCYAN);

    // Business performance indicators
    printf("\n");
    printCenteredLine('*', YELLOW);
    if (current_utilization >= 70 && total_revenue >= 1000) {
        printCentered("BUSINESS STATUS: EXCELLENT PERFORMANCE", LIGHTGREEN);
        printCentered("Facility is operating at optimal levels", LIGHTCYAN);
    } else if (current_utilization >= 50 && total_revenue >= 500) {
        printCentered("BUSINESS STATUS: STRONG PERFORMANCE", LIGHTBLUE);
        printCentered("Good operational metrics with growth opportunities", LIGHTCYAN);
    } else if (current_utilization >= 30 && total_revenue >= 200) {
        printCentered("BUSINESS STATUS: MODERATE PERFORMANCE", YELLOW);
        printCentered("Steady progress, focus on customer acquisition", LIGHTCYAN);
    } else {
        printCentered("BUSINESS STATUS: BUILDING PHASE", LIGHTRED);
        printCentered("Early stage, implement growth strategies", LIGHTCYAN);
    }
    printCenteredLine('*', YELLOW);

    printf("\n");
    printCenteredLine('=', LIGHTCYAN);
    printCentered("END OF EXECUTIVE REPORT", LIGHTCYAN);
    sprintf(metrics, "SMART PARK System v2.0 | Report ID: %s", today);
    printCentered(metrics, LIGHTGRAY);
    printCenteredLine('=', LIGHTCYAN);

    pause_screen();
}
