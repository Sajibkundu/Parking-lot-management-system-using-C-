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

// Pricing per hour
#define BIKE_PRICE 10.0
#define CAR_PRICE 20.0
#define TRUCK_PRICE 50.0

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

// File Handling Functions
void load_data() {
    FILE *file;

    // Load users
    file = fopen("users.txt", "r");
    if (file != NULL) {
        while (fscanf(file, "%d %s %s %s %s %s %d",
                &users[user_count].user_id, users[user_count].name,
                users[user_count].phone, users[user_count].email,
                users[user_count].password, users[user_count].reg_date,
                &users[user_count].is_active) == 7) {
            user_count++;
        }
        fclose(file);
    }

    // Load vehicles
    file = fopen("vehicles.txt", "r");
    if (file != NULL) {
        while (fscanf(file, "%d %d %s %s %s %s",
                &vehicles[vehicle_count].vehicle_id, &vehicles[vehicle_count].user_id,
                vehicles[vehicle_count].type, vehicles[vehicle_count].license_plate,
                vehicles[vehicle_count].color, vehicles[vehicle_count].reg_date) == 6) {
            vehicle_count++;
        }
        fclose(file);
    }

    // Load slots only if not initialized
    if (slot_count == 0) {
        file = fopen("slots.txt", "r");
        if (file != NULL) {
            while (fscanf(file, "%d %s %s %d %s %d",
                    &slots[slot_count].slot_id, slots[slot_count].type,
                    slots[slot_count].status, &slots[slot_count].vehicle_id,
                    slots[slot_count].reserved_time, &slots[slot_count].user_id) == 6) {
                slot_count++;
            }
            fclose(file);
        }
    }

    // Load reservations
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
        }
        fclose(file);
    }

    // Load payments
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
        }
        fclose(file);
    }
}

void save_data() {
    FILE *file;

    // Save users
    file = fopen("users.txt", "w");
    if (file != NULL) {
        for (int i = 0; i < user_count; i++) {
            fprintf(file, "%d %s %s %s %s %s %d\n",
                    users[i].user_id, users[i].name, users[i].phone,
                    users[i].email, users[i].password, users[i].reg_date,
                    users[i].is_active);
        }
        fclose(file);
    }

    // Save vehicles
    file = fopen("vehicles.txt", "w");
    if (file != NULL) {
        for (int i = 0; i < vehicle_count; i++) {
            fprintf(file, "%d %d %s %s %s %s\n",
                    vehicles[i].vehicle_id, vehicles[i].user_id,
                    vehicles[i].type, vehicles[i].license_plate,
                    vehicles[i].color, vehicles[i].reg_date);
        }
        fclose(file);
    }

    // Save slots
    file = fopen("slots.txt", "w");
    if (file != NULL) {
        for (int i = 0; i < slot_count; i++) {
            fprintf(file, "%d %s %s %d %s %d\n",
                    slots[i].slot_id, slots[i].type, slots[i].status,
                    slots[i].vehicle_id, slots[i].reserved_time, slots[i].user_id);
        }
        fclose(file);
    }

    // Save reservations
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
    }

    // Save payments
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
    }
}

// Utility Functions
void clear_screen() {
    system(CLEAR_SCREEN);
}

void print_header() {
    printf("\n");
    printf("========================================================\n");
    printf("        WELCOME TO PARKING LOT MANAGEMENT SYSTEM       \n");
    printf("========================================================\n");
}

void pause_screen() {
    printf("\nPress any key to continue...");
    getch();
}

int validate_phone(char *phone) {
    if (strlen(phone) != 11) {
        printf("Phone number must be exactly 11 digits!\n");
        return 0;
    }

    for (int i = 0; i < 11; i++) {
        if (!isdigit(phone[i])) {
            printf("Phone number must contain only digits!\n");
            return 0;
        }
    }
    return 1;
}

int validate_email(char *email) {
    int at_count = 0, dot_count = 0;
    int len = strlen(email);

    if (len < 5) {
        printf("Email too short!\n");
        return 0;
    }

    for (int i = 0; i < len; i++) {
        if (email[i] == '@') at_count++;
        if (email[i] == '.') dot_count++;
    }

    if (at_count != 1 || dot_count < 1) {
        printf("Invalid email format!\n");
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
        printf("\n1. User Registration");
        printf("\n2. User Login");
        printf("\n3. Admin Login");
        printf("\n4. Exit");
        printf("\n\nEnter your choice: ");
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
                printf("\nThank you for using Parking Management System!");
                save_data();
                exit(0);
            default:
                printf("\nInvalid choice! Please try again.");
                pause_screen();
        }
    }
}

void user_menu() {
    int choice;

    while (1) {
        clear_screen();
        print_header();
        printf("\n*** USER DASHBOARD ***");
        printf("\n1. Update Profile");
        printf("\n2. Add Vehicle");
        printf("\n3. View My Vehicles");
        printf("\n4. View Available Slots");
        printf("\n5. Reserve Slot");
        printf("\n6. Cancel Reservation");
        printf("\n7. Make Payment");
        printf("\n8. View Payment History");
        printf("\n9. View Booking History");
        printf("\n10. Check Overstay Alerts");
        printf("\n11. Generate My Report");
        printf("\n12. Logout");
        printf("\n\nEnter your choice: ");
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
                return;
            default:
                printf("\nInvalid choice! Please try again.");
                pause_screen();
        }
    }
}

void admin_menu() {
    int choice;

    while (1) {
        clear_screen();
        print_header();
        printf("\n*** ADMIN DASHBOARD ***");
        printf("\n1. View All Users");
        printf("\n2. Search Users");
        printf("\n3. Delete User");
        printf("\n4. View User History");
        printf("\n5. View All Vehicles");
        printf("\n6. Slot Usage Report");
        printf("\n7. Generate Daily Report");
        printf("\n8. Check System Overstays");
        printf("\n9. Logout");
        printf("\n\nEnter your choice: ");
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
                return;
            default:
                printf("\nInvalid choice! Please try again.");
                pause_screen();
        }
    }
}

// Authentication Functions
void user_register() {
    clear_screen();
    print_header();
    printf("\n*** USER REGISTRATION ***\n");

    User new_user;
    new_user.user_id = get_next_user_id();
    new_user.is_active = 1;

    printf("\nEnter your name: ");
    scanf("%s", new_user.name);

    // Phone number validation
    do {
        printf("Enter phone number (11 digits): ");
        scanf("%s", new_user.phone);
    } while (!validate_phone(new_user.phone));

    // Email validation
    do {
        printf("Enter email address: ");
        scanf("%s", new_user.email);
    } while (!validate_email(new_user.email));

    printf("Enter password: ");
    mask_password(new_user.password);

    get_current_time(new_user.reg_date);

    // Check if phone or email already exists
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].phone, new_user.phone) == 0) {
            printf("\nPhone number already registered!");
            pause_screen();
            return;
        }
        if (strcmp(users[i].email, new_user.email) == 0) {
            printf("\nEmail already registered!");
            pause_screen();
            return;
        }
    }

    users[user_count] = new_user;
    user_count++;

    printf("\nRegistration successful! Your User ID is: %d", new_user.user_id);
    pause_screen();
}

void user_login() {
    clear_screen();
    print_header();
    printf("\n*** USER LOGIN ***\n");

    char phone[20], password[MAX_STRING];

    printf("Enter phone number: ");
    scanf("%s", phone);

    printf("Enter password: ");
    mask_password(password);

    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].phone, phone) == 0 &&
            strcmp(users[i].password, password) == 0 &&
            users[i].is_active == 1) {
            current_user_id = users[i].user_id;
            printf("\nLogin successful! Welcome, %s", users[i].name);
            pause_screen();
            user_menu();
            return;
        }
    }

    printf("\nInvalid credentials or account deactivated!");
    pause_screen();
}

void admin_login() {
    clear_screen();
    print_header();
    printf("\n*** ADMIN LOGIN ***\n");

    char username[MAX_STRING], password[MAX_STRING];

    printf("Enter admin username: ");
    scanf("%s", username);

    printf("Enter admin password: ");
    mask_password(password);

    if (strcmp(username, "sajib") == 0 && strcmp(password, "12345") == 0) {
        is_admin = 1;
        printf("\nAdmin login successful! Welcome, Administrator");
        pause_screen();
        admin_menu();
    } else {
        printf("\nInvalid admin credentials!");
        pause_screen();
    }
}

// User Functions
void update_profile() {
    clear_screen();
    print_header();
    printf("\n*** UPDATE PROFILE ***\n");

    int user_index = -1;
    for (int i = 0; i < user_count; i++) {
        if (users[i].user_id == current_user_id) {
            user_index = i;
            break;
        }
    }

    if (user_index == -1) {
        printf("User not found!");
        pause_screen();
        return;
    }

    printf("Current Profile Information:");
    printf("\nName: %s", users[user_index].name);
    printf("\nPhone: %s", users[user_index].phone);
    printf("\nEmail: %s", users[user_index].email);

    int choice;
    printf("\n\nWhat do you want to update?");
    printf("\n1. Name");
    printf("\n2. Phone");
    printf("\n3. Email");
    printf("\n4. Password");
    printf("\n5. Go Back");
    printf("\nEnter choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            printf("Enter new name: ");
            scanf("%s", users[user_index].name);
            printf("Name updated successfully!");
            break;
        case 2:
            do {
                printf("Enter new phone (11 digits): ");
                scanf("%s", users[user_index].phone);
            } while (!validate_phone(users[user_index].phone));
            printf("Phone updated successfully!");
            break;
        case 3:
            do {
                printf("Enter new email: ");
                scanf("%s", users[user_index].email);
            } while (!validate_email(users[user_index].email));
            printf("Email updated successfully!");
            break;
        case 4:
            printf("Enter new password: ");
            mask_password(users[user_index].password);
            printf("Password updated successfully!");
            break;
        case 5:
            return;
    }

    pause_screen();
}

void add_vehicle() {
    clear_screen();
    print_header();
    printf("\n*** ADD VEHICLE ***\n");

    Vehicle new_vehicle;
    new_vehicle.vehicle_id = get_next_vehicle_id();
    new_vehicle.user_id = current_user_id;

    printf("Select vehicle type:");
    printf("\n1. Bike");
    printf("\n2. Car");
    printf("\n3. Truck");
    printf("\nEnter choice: ");

    int type_choice;
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
            printf("Invalid vehicle type!");
            pause_screen();
            return;
    }

    printf("Enter license plate: ");
    scanf("%s", new_vehicle.license_plate);

    printf("Enter vehicle color: ");
    scanf("%s", new_vehicle.color);

    get_current_time(new_vehicle.reg_date);

    vehicles[vehicle_count] = new_vehicle;
    vehicle_count++;

    printf("\nVehicle added successfully!");
    printf("\nVehicle ID: %d", new_vehicle.vehicle_id);
    pause_screen();
}

void view_vehicles() {
    clear_screen();
    print_header();
    printf("\n*** MY VEHICLES ***\n");

    int found = 0;
    printf("\n%-10s %-10s %-15s %-15s %-20s", "ID", "Type", "License", "Color", "Reg Date");
    printf("\n----------------------------------------------------------------");

    for (int i = 0; i < vehicle_count; i++) {
        if (vehicles[i].user_id == current_user_id) {
            printf("\n%-10d %-10s %-15s %-15s %-20s",
                   vehicles[i].vehicle_id, vehicles[i].type,
                   vehicles[i].license_plate, vehicles[i].color,
                   vehicles[i].reg_date);
            found = 1;
        }
    }

    if (!found) {
        printf("\nNo vehicles found!");
    }

    pause_screen();
}

void view_available_slots() {
    clear_screen();
    print_header();
    printf("\n*** AVAILABLE PARKING SLOTS ***\n");

    printf("\nSlot Color Code:");
    printf("\n[A] = Available");
    printf("\n[O] = Occupied");
    printf("\n[R] = Reserved");

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

    printf("\n\nAvailability Summary:");
    printf("\nBike Slots Available: %d/%d", bike_available, BIKE_SLOTS);
    printf("\nCar Slots Available: %d/%d", car_available, CAR_SLOTS);
    printf("\nTruck Slots Available: %d/%d", truck_available, TRUCK_SLOTS);

    printf("\n\nPricing (per hour):");
    printf("\nBike: $%.2f", BIKE_PRICE);
    printf("\nCar: $%.2f", CAR_PRICE);
    printf("\nTruck: $%.2f", TRUCK_PRICE);

    pause_screen();
}

void print_colored_slots() {
    printf("\n\nBIKE SLOTS (1-40):");
    printf("\n");
    for (int i = 0; i < BIKE_SLOTS; i++) {
        if (strcmp(slots[i].status, "available") == 0) {
            printf("[A]%02d ", slots[i].slot_id);
        } else if (strcmp(slots[i].status, "occupied") == 0) {
            printf("[O]%02d ", slots[i].slot_id);
        } else {
            printf("[R]%02d ", slots[i].slot_id);
        }
        if ((i + 1) % 10 == 0) printf("\n");
    }

    printf("\n\nCAR SLOTS (41-80):");
    printf("\n");
    for (int i = BIKE_SLOTS; i < BIKE_SLOTS + CAR_SLOTS; i++) {
        if (strcmp(slots[i].status, "available") == 0) {
            printf("[A]%02d ", slots[i].slot_id);
        } else if (strcmp(slots[i].status, "occupied") == 0) {
            printf("[O]%02d ", slots[i].slot_id);
        } else {
            printf("[R]%02d ", slots[i].slot_id);
        }
        if ((i - BIKE_SLOTS + 1) % 10 == 0) printf("\n");
    }

    printf("\n\nTRUCK SLOTS (81-100):");
    printf("\n");
    for (int i = BIKE_SLOTS + CAR_SLOTS; i < slot_count; i++) {
        if (strcmp(slots[i].status, "available") == 0) {
            printf("[A]%02d ", slots[i].slot_id);
        } else if (strcmp(slots[i].status, "occupied") == 0) {
            printf("[O]%02d ", slots[i].slot_id);
        } else {
            printf("[R]%02d ", slots[i].slot_id);
        }
        if ((i - BIKE_SLOTS - CAR_SLOTS + 1) % 10 == 0) printf("\n");
    }
}

void reserve_slot() {
    clear_screen();
    print_header();
    printf("\n*** RESERVE PARKING SLOT ***\n");

    // Show user's vehicles
    printf("Your vehicles:");
    int user_vehicles[100], vehicle_found = 0;

    for (int i = 0; i < vehicle_count; i++) {
        if (vehicles[i].user_id == current_user_id) {
            printf("\n%d. Vehicle ID: %d (%s) - %s",
                   vehicle_found + 1, vehicles[i].vehicle_id,
                   vehicles[i].type, vehicles[i].license_plate);
            user_vehicles[vehicle_found] = i;
            vehicle_found++;
        }
    }

    if (vehicle_found == 0) {
        printf("\nNo vehicles found! Please add a vehicle first.");
        pause_screen();
        return;
    }

    int vehicle_choice;
    printf("\n\nSelect vehicle (1-%d): ", vehicle_found);
    scanf("%d", &vehicle_choice);

    if (vehicle_choice < 1 || vehicle_choice > vehicle_found) {
        printf("Invalid vehicle selection!");
        pause_screen();
        return;
    }

    int selected_vehicle_index = user_vehicles[vehicle_choice - 1];
    char *vehicle_type = vehicles[selected_vehicle_index].type;

    // Show available slots for this vehicle type
    printf("\nAvailable %s slots:", vehicle_type);
    int available_slots[100], available_count = 0;

    for (int i = 0; i < slot_count; i++) {
        if (strcmp(slots[i].type, vehicle_type) == 0 &&
            strcmp(slots[i].status, "available") == 0) {
            printf("\nSlot ID: %d", slots[i].slot_id);
            available_slots[available_count] = i;
            available_count++;
        }
    }

    if (available_count == 0) {
        printf("\nNo available slots for %s!", vehicle_type);
        pause_screen();
        return;
    }

    int slot_id;
    printf("\n\nEnter slot ID to reserve: ");
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
        printf("Invalid slot ID or slot not available!");
        pause_screen();
        return;
    }

    double duration;
    printf("Enter duration in hours: ");
    scanf("%lf", &duration);

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

    printf("\nReservation successful!");
    printf("\nReservation ID: %d", new_reservation.reservation_id);
    printf("\nSlot: %d", slot_id);
    printf("\nDuration: %.2f hours", duration);
    printf("\nTotal Amount: $%.2f", new_reservation.total_amount);
    printf("\nPlease proceed to payment.");

    pause_screen();
}

void cancel_reservation() {
    clear_screen();
    print_header();
    printf("\n*** CANCEL RESERVATION ***\n");

    // Show user's active reservations
    printf("Your active reservations:");
    int user_reservations[100], reservation_found = 0;

    for (int i = 0; i < reservation_count; i++) {
        if (reservations[i].user_id == current_user_id &&
            strcmp(reservations[i].status, "active") == 0) {
            printf("\n%d. Reservation ID: %d, Slot: %d, Amount: $%.2f",
                   reservation_found + 1, reservations[i].reservation_id,
                   reservations[i].slot_id, reservations[i].total_amount);
            user_reservations[reservation_found] = i;
            reservation_found++;
        }
    }

    if (reservation_found == 0) {
        printf("\nNo active reservations found!");
        pause_screen();
        return;
    }

    int choice;
    printf("\n\nSelect reservation to cancel (1-%d): ", reservation_found);
    scanf("%d", &choice);

    if (choice < 1 || choice > reservation_found) {
        printf("Invalid selection!");
        pause_screen();
        return;
    }

    int reservation_index = user_reservations[choice - 1];

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

    printf("\nReservation cancelled successfully!");
    pause_screen();
}

void make_payment() {
    clear_screen();
    print_header();
    printf("\n*** MAKE PAYMENT ***\n");

    // Show unpaid reservations
    printf("Unpaid reservations:");
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
                printf("\n%d. Reservation ID: %d, Amount: $%.2f",
                       unpaid_count + 1, reservations[i].reservation_id,
                       reservations[i].total_amount);
                unpaid_reservations[unpaid_count] = i;
                unpaid_count++;
            }
        }
    }

    if (unpaid_count == 0) {
        printf("\nNo unpaid reservations found!");
        pause_screen();
        return;
    }

    int choice;
    printf("\n\nSelect reservation to pay (1-%d): ", unpaid_count);
    scanf("%d", &choice);

    if (choice < 1 || choice > unpaid_count) {
        printf("Invalid selection!");
        pause_screen();
        return;
    }

    int reservation_index = unpaid_reservations[choice - 1];

    printf("\nPayment Methods:");
    printf("\n1. Cash");
    printf("\n2. Card");
    printf("\n3. bKash");
    printf("\n4. Nagad");
    printf("\nSelect payment method: ");

    int method_choice;
    scanf("%d", &method_choice);

    char payment_method[20];
    switch (method_choice) {
        case 1:
            strcpy(payment_method, "cash");
            break;
        case 2:
            strcpy(payment_method, "card");
            break;
        case 3:
            strcpy(payment_method, "bkash");
            break;
        case 4:
            strcpy(payment_method, "nagad");
            break;
        default:
            printf("Invalid payment method!");
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

    printf("\nPayment successful!");
    printf("\nPayment ID: %d", new_payment.payment_id);
    printf("\nAmount Paid: $%.2f", new_payment.amount);
    printf("\nPayment Method: %s", new_payment.method);

    pause_screen();
}

void view_payment_history() {
    clear_screen();
    print_header();
    printf("\n*** PAYMENT HISTORY ***\n");

    int found = 0;
    printf("\n%-10s %-15s %-10s %-15s %-20s %-15s",
           "Pay ID", "Reservation", "Amount", "Method", "Date", "Status");
    printf("\n-------------------------------------------------------------------------");

    for (int i = 0; i < payment_count; i++) {
        if (payments[i].user_id == current_user_id) {
            printf("\n%-10d %-15d $%-9.2f %-15s %-20s %-15s",
                   payments[i].payment_id, payments[i].reservation_id,
                   payments[i].amount, payments[i].method,
                   payments[i].payment_date, payments[i].status);
            found = 1;
        }
    }

    if (!found) {
        printf("\nNo payment history found!");
    }

    pause_screen();
}

void view_booking_history() {
    clear_screen();
    print_header();
    printf("\n*** BOOKING HISTORY ***\n");

    int found = 0;
    printf("\n%-10s %-8s %-10s %-20s %-20s %-12s %-10s",
           "Res ID", "Slot", "Vehicle", "Start Time", "End Time", "Status", "Amount");
    printf("\n--------------------------------------------------------------------------------");

    for (int i = 0; i < reservation_count; i++) {
        if (reservations[i].user_id == current_user_id) {
            printf("\n%-10d %-8d %-10d %-20s %-20s %-12s $%-9.2f",
                   reservations[i].reservation_id, reservations[i].slot_id,
                   reservations[i].vehicle_id, reservations[i].start_time,
                   reservations[i].end_time, reservations[i].status,
                   reservations[i].total_amount);
            found = 1;
        }
    }

    if (!found) {
        printf("\nNo booking history found!");
    }

    pause_screen();
}

void check_overstay() {
    clear_screen();
    print_header();
    printf("\n*** OVERSTAY ALERTS ***\n");

    int alerts_found = 0;

    for (int i = 0; i < reservation_count; i++) {
        if ((is_admin || reservations[i].user_id == current_user_id) &&
            strcmp(reservations[i].status, "active") == 0) {

            // Simplified overstay detection
            double hours_passed = 2.5;

            if (hours_passed > reservations[i].duration_hours) {
                double overstay_hours = hours_passed - reservations[i].duration_hours;
                printf("\n*** OVERSTAY ALERT! ***");
                printf("\nReservation ID: %d", reservations[i].reservation_id);
                printf("\nSlot ID: %d", reservations[i].slot_id);
                printf("\nBooked Duration: %.2f hours", reservations[i].duration_hours);
                printf("\nTime Passed: %.2f hours", hours_passed);
                printf("\nOverstay: %.2f hours", overstay_hours);

                if (is_admin) {
                    for (int j = 0; j < user_count; j++) {
                        if (users[j].user_id == reservations[i].user_id) {
                            printf("\nUser: %s", users[j].name);
                            printf("\nPhone: %s", users[j].phone);
                            break;
                        }
                    }
                }
                printf("\n----------------------------------------");
                alerts_found = 1;
            }
        }
    }

    if (!alerts_found) {
        printf("\nNo overstay alerts found!");
    }

    pause_screen();
}

void generate_user_report() {
    clear_screen();
    print_header();
    printf("\n*** MY DAILY REPORT ***\n");

    int total_bookings = 0, active_bookings = 0, cancelled_bookings = 0;
    double total_spent = 0.0;
    int vehicle_count_user = 0;

    for (int i = 0; i < reservation_count; i++) {
        if (reservations[i].user_id == current_user_id) {
            total_bookings++;
            if (strcmp(reservations[i].status, "active") == 0) {
                active_bookings++;
            } else if (strcmp(reservations[i].status, "cancelled") == 0) {
                cancelled_bookings++;
            }
        }
    }

    for (int i = 0; i < payment_count; i++) {
        if (payments[i].user_id == current_user_id) {
            total_spent += payments[i].amount;
        }
    }

    for (int i = 0; i < vehicle_count; i++) {
        if (vehicles[i].user_id == current_user_id) {
            vehicle_count_user++;
        }
    }

    printf("\n*** YOUR STATISTICS ***");
    printf("\n========================");
    printf("\nTotal Bookings: %d", total_bookings);
    printf("\nActive Bookings: %d", active_bookings);
    printf("\nCancelled Bookings: %d", cancelled_bookings);
    printf("\nTotal Vehicles Registered: %d", vehicle_count_user);
    printf("\nTotal Amount Spent: $%.2f", total_spent);

    pause_screen();
}

// Admin Functions
void admin_view_users() {
    clear_screen();
    print_header();
    printf("\n*** ALL USERS ***\n");

    if (user_count == 0) {
        printf("\nNo users found!");
        pause_screen();
        return;
    }

    printf("\n%-10s %-20s %-15s %-30s %-20s %-10s",
           "User ID", "Name", "Phone", "Email", "Reg Date", "Status");
    printf("\n----------------------------------------------------------------------------------------------");

    for (int i = 0; i < user_count; i++) {
        printf("\n%-10d %-20s %-15s %-30s %-20s %-10s",
               users[i].user_id, users[i].name, users[i].phone,
               users[i].email, users[i].reg_date,
               users[i].is_active ? "Active" : "Inactive");
    }

    pause_screen();
}

void admin_search_users() {
    clear_screen();
    print_header();
    printf("\n*** SEARCH USERS ***\n");

    printf("Search by:");
    printf("\n1. Name");
    printf("\n2. Phone");
    printf("\n3. Email");
    printf("\nEnter choice: ");

    int choice;
    scanf("%d", &choice);

    char search_term[MAX_STRING];
    printf("Enter search term: ");
    scanf("%s", search_term);

    int found = 0;
    printf("\n%-10s %-20s %-15s %-30s %-20s %-10s",
           "User ID", "Name", "Phone", "Email", "Reg Date", "Status");
    printf("\n----------------------------------------------------------------------------------------------");

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
        }

        if (match) {
            printf("\n%-10d %-20s %-15s %-30s %-20s %-10s",
                   users[i].user_id, users[i].name, users[i].phone,
                   users[i].email, users[i].reg_date,
                   users[i].is_active ? "Active" : "Inactive");
            found = 1;
        }
    }

    if (!found) {
        printf("\nNo users found matching the search criteria!");
    }

    pause_screen();
}

void admin_delete_user() {
    clear_screen();
    print_header();
    printf("\n*** DELETE USER ***\n");

    int user_id;
    printf("Enter User ID to delete: ");
    scanf("%d", &user_id);

    int user_index = -1;
    for (int i = 0; i < user_count; i++) {
        if (users[i].user_id == user_id) {
            user_index = i;
            break;
        }
    }

    if (user_index == -1) {
        printf("User not found!");
        pause_screen();
        return;
    }

    printf("\nUser Details:");
    printf("\nName: %s", users[user_index].name);
    printf("\nPhone: %s", users[user_index].phone);
    printf("\nEmail: %s", users[user_index].email);

    char confirm;
    printf("\n\nAre you sure you want to delete this user? (y/n): ");
    scanf(" %c", &confirm);

    if (confirm == 'y' || confirm == 'Y') {
        users[user_index].is_active = 0;
        printf("\nUser deactivated successfully!");
    } else {
        printf("\nDeletion cancelled!");
    }

    pause_screen();
}

void admin_view_user_history() {
    clear_screen();
    print_header();
    printf("\n*** USER HISTORY ***\n");

    int user_id;
    printf("Enter User ID to view history: ");
    scanf("%d", &user_id);

    int user_index = -1;
    for (int i = 0; i < user_count; i++) {
        if (users[i].user_id == user_id) {
            user_index = i;
            break;
        }
    }

    if (user_index == -1) {
        printf("User not found!");
        pause_screen();
        return;
    }

    printf("\nUser: %s (%s)", users[user_index].name, users[user_index].phone);

    // Show vehicles
    printf("\n\n*** VEHICLES ***");
    int vehicle_found = 0;
    for (int i = 0; i < vehicle_count; i++) {
        if (vehicles[i].user_id == user_id) {
            printf("\n- %s (%s) - %s", vehicles[i].type,
                   vehicles[i].license_plate, vehicles[i].color);
            vehicle_found = 1;
        }
    }
    if (!vehicle_found) printf("\nNo vehicles registered");

    // Show reservations
    printf("\n\n*** RESERVATIONS ***");
    int reservation_found = 0;
    for (int i = 0; i < reservation_count; i++) {
        if (reservations[i].user_id == user_id) {
            printf("\nReservation %d: Slot %d, Status: %s, Amount: $%.2f",
                   reservations[i].reservation_id, reservations[i].slot_id,
                   reservations[i].status, reservations[i].total_amount);
            reservation_found = 1;
        }
    }
    if (!reservation_found) printf("\nNo reservations found");

    // Show payments
    printf("\n\n*** PAYMENTS ***");
    int payment_found = 0;
    double total_paid = 0;
    for (int i = 0; i < payment_count; i++) {
        if (payments[i].user_id == user_id) {
            printf("\nPayment %d: $%.2f via %s (%s)",
                   payments[i].payment_id, payments[i].amount,
                   payments[i].method, payments[i].status);
            total_paid += payments[i].amount;
            payment_found = 1;
        }
    }
    if (!payment_found) printf("\nNo payments found");
    else printf("\nTotal Paid: $%.2f", total_paid);

    pause_screen();
}

void admin_view_all_vehicles() {
    clear_screen();
    print_header();
    printf("\n*** ALL VEHICLES WITH OWNER INFO ***\n");

    if (vehicle_count == 0) {
        printf("\nNo vehicles found!");
        pause_screen();
        return;
    }

    printf("\n%-10s %-10s %-15s %-15s %-20s %-15s %-20s",
           "Veh ID", "Type", "License", "Color", "Owner Name", "Owner Phone", "Reg Date");
    printf("\n--------------------------------------------------------------------------------------------------------");

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

        printf("\n%-10d %-10s %-15s %-15s %-20s %-15s %-20s",
               vehicles[i].vehicle_id, vehicles[i].type,
               vehicles[i].license_plate, vehicles[i].color,
               owner_name, owner_phone, vehicles[i].reg_date);
    }

    pause_screen();
}

void admin_slot_usage_report() {
    clear_screen();
    print_header();
    printf("\n*** SLOT USAGE REPORT ***\n");

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

    printf("\n*** BIKE SLOTS (Total: %d) ***", BIKE_SLOTS);
    printf("\n   Available: %d", bike_available);
    printf("\n   Occupied: %d", bike_occupied);
    printf("\n   Reserved: %d", bike_reserved);
    printf("\n   Usage: %.1f%%", ((double)(bike_occupied + bike_reserved) / BIKE_SLOTS) * 100);

    printf("\n\n*** CAR SLOTS (Total: %d) ***", CAR_SLOTS);
    printf("\n   Available: %d", car_available);
    printf("\n   Occupied: %d", car_occupied);
    printf("\n   Reserved: %d", car_reserved);
    printf("\n   Usage: %.1f%%", ((double)(car_occupied + car_reserved) / CAR_SLOTS) * 100);

    printf("\n\n*** TRUCK SLOTS (Total: %d) ***", TRUCK_SLOTS);
    printf("\n   Available: %d", truck_available);
    printf("\n   Occupied: %d", truck_occupied);
    printf("\n   Reserved: %d", truck_reserved);
    printf("\n   Usage: %.1f%%", ((double)(truck_occupied + truck_reserved) / TRUCK_SLOTS) * 100);

    printf("\n\n*** OVERALL STATISTICS ***");
    int total_slots = BIKE_SLOTS + CAR_SLOTS + TRUCK_SLOTS;
    int total_used = bike_occupied + bike_reserved + car_occupied + car_reserved + truck_occupied + truck_reserved;
    printf("\n   Total Slots: %d", total_slots);
    printf("\n   Total Used: %d", total_used);
    printf("\n   Overall Usage: %.1f%%", ((double)total_used / total_slots) * 100);

    pause_screen();
}

void admin_generate_daily_report() {
    clear_screen();
    print_header();
    printf("\n*** DAILY SYSTEM REPORT ***\n");

    char today[20];
    get_current_time(today);

    int total_users = user_count;
    int active_users = 0;
    int total_vehicles = vehicle_count;
    int active_reservations = 0;
    int completed_payments = 0;
    double daily_revenue = 0.0;

    // Count active users
    for (int i = 0; i < user_count; i++) {
        if (users[i].is_active) active_users++;
    }

    // Count active reservations
    for (int i = 0; i < reservation_count; i++) {
        if (strcmp(reservations[i].status, "active") == 0) {
            active_reservations++;
        }
    }

    // Calculate daily revenue
    for (int i = 0; i < payment_count; i++) {
        if (strcmp(payments[i].status, "completed") == 0) {
            completed_payments++;
            daily_revenue += payments[i].amount;
        }
    }

    printf("\n*** SYSTEM STATISTICS (as of %s) ***", today);
    printf("\n==================================================");

    printf("\n\n*** USER STATISTICS ***");
    printf("\n   Total Registered Users: %d", total_users);
    printf("\n   Active Users: %d", active_users);
    printf("\n   Inactive Users: %d", total_users - active_users);

    printf("\n\n*** VEHICLE STATISTICS ***");
    printf("\n   Total Registered Vehicles: %d", total_vehicles);
       int bikes = 0, cars = 0, trucks = 0;
    for (int i = 0; i < vehicle_count; i++) {
        if (strcmp(vehicles[i].type, "bike") == 0) bikes++;
        else if (strcmp(vehicles[i].type, "car") == 0) cars++;
        else trucks++;
    }
    printf("\n   Bikes: %d", bikes);
    printf("\n   Cars: %d", cars);
    printf("\n   Trucks: %d", trucks);

    printf("\n\n*** RESERVATION STATISTICS ***");
    printf("\n   Total Reservations: %d", reservation_count);
    printf("\n   Active Reservations: %d", active_reservations);
    printf("\n   Completed Reservations: %d", reservation_count - active_reservations);

    printf("\n\n*** FINANCIAL STATISTICS ***");
    printf("\n   Total Payments: %d", completed_payments);
    printf("\n   Total Revenue: $%.2f", daily_revenue);
    printf("\n   Average Payment: $%.2f", completed_payments > 0 ? daily_revenue / completed_payments : 0.0);

    // Slot usage summary
    int total_slots_used = 0;
    for (int i = 0; i < slot_count; i++) {
        if (strcmp(slots[i].status, "occupied") == 0 || strcmp(slots[i].status, "reserved") == 0) {
            total_slots_used++;
        }
    }

    printf("\n\n*** SLOT USAGE STATISTICS ***");
    printf("\n   Total Slots: %d", slot_count);
    printf("\n   Slots in Use: %d", total_slots_used);
    printf("\n   Available Slots: %d", slot_count - total_slots_used);
    printf("\n   Usage Rate: %.1f%%", ((double)total_slots_used / slot_count) * 100);

    pause_screen();
}
