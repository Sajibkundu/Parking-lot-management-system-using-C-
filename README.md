### 🅿️ Smart Parking Lot Management System (C)

A modular, command-line interface (CLI) based parking management system written in the C programming language. This project, developed by students at **Daffodil International University**, provides a comprehensive solution for managing a parking lot by digitizing and automating processes that are traditionally manual and inefficient. The system is designed to be a lightweight, offline solution for small to medium-sized institutions.

-----

### 🚀 Features

#### **Core Functionality**

  - ✅ **User Authentication**: A complete system for user registration and secure login for both users and administrators.
  - ✅ **Admin Panel**: A dedicated section for administrators to manage users, set pricing rules, add/remove parking slots, and view reports.
  - ✅ **Parking Slot Management**: A system to handle parking lot operations, including real-time slot views with color-coded indicators (green for available, red for occupied, blue for reserved).
  - ✅ **Reservation System**: Users can book parking slots in advance, with confirmation sent via SMS or email.
  - ✅ **Payment System**: Calculates parking fees automatically based on duration and vehicle type, and supports multiple payment methods like cash, card, and mobile wallets.
  - ✅ **Overstay Alert System**: Tracks parking duration and notifies users and admins if a vehicle exceeds its reserved time.
  - ✅ **Daily Report Generation**: Admins can generate daily reports on vehicle count, revenue, and slot usage, with options to filter by date or vehicle type.
  - ✅ **Multi-Vehicle Support**: The system supports different vehicle types (bike, car, truck) with separate slots and pricing for each.

#### **Data Validation**

  - ✅ **Email Validation**: Ensures proper email format (e.g., `user@domain.com`).
  - ✅ **Phone Validation**: Validates phone numbers for an 11-digit format (starting with `01`).
  - ✅ **NID Validation**: Verifies National ID numbers (10 or 17 digits).
  - ✅ **Input Validation**: Ensures all user inputs are handled correctly to prevent incorrect or incomplete data.

-----

### 🤔 Why My Project Is Different from Others

My project stands out from typical C-based projects for several key reasons. These differences make it a more comprehensive, practical, and well-structured solution.

1.  **Comprehensive and Realistic Feature Set**: Unlike many projects that focus on a single core function, my project integrates a variety of advanced features that simulate a real-world application, such as a **reservation system**, **overstay alerts**, and **daily report generation**. These features make it a more complete and useful application.
2.  **Advanced Data Management and File Structure**: Instead of storing all data in a single file, the project uses a modular approach with separate files for different data types (`users.txt`, `payments.txt`) and a well-organized codebase in the `src/` folder. This makes the project cleaner, easier to debug, and more scalable.
3.  **Practical Utility and Benefits**: The system is designed as a **cost-effective**, offline solution that simplifies parking management and provides tangible benefits like **improved efficiency**, **revenue generation**, and **enhanced security**, making it a practical tool rather than just a coding exercise.
4.  **Professional Design and Methodology**: The project's design and documentation, including the use of **UML diagrams** and adherence to a structured development approach, demonstrate a professional level of planning and execution.

-----

### 📁 Project Structure

The project is organized into multiple modules for better clarity and maintainability.

```
parking_system/
├── src/
│   ├── main.c              # Main program logic and entry point
│   ├── user.c              # Handles user registration, login, and dashboard
│   ├── admin.c             # Manages admin-specific functionalities
│   ├── payment.c           # Processes payment transactions
│   └── parking.c           # Manages parking lot, slots, and vehicle data
│
├── data/
│   ├── users.txt           # Stores all user account data
│   ├── payments.txt        # Stores payment history
│   ├── vehicles.txt        # Stores vehicle information
│   └── parking_slots.txt   # Stores parking slot status (occupied/vacant)
│
└── Makefile                # Script for easy compilation and project building
```

-----

### 🛠️ Installation & Compilation

#### **Prerequisites**

  * A **C compiler** such as GCC.
  * **`make`** utility for easy compilation.

#### **Compilation Instructions**

The project can be compiled by simply running the `Makefile`.

```sh
make
```

This will compile all the source files and create an executable file named `parking_system`.

#### **Running the Program**

To run the compiled program from the terminal:

```sh
./parking_system
```

-----

### 📖 Usage Guide

#### **Main Menu**

The system begins with a main menu offering access to the different panels and features.

```
===========================================
🅂🄼🄰🅁🅃 🄿🄰🅁🄺🄸🄽🄶 🄻🄾🅃 🄼🄰🄽🄰🄶🄴🄼🄴🄽🅃 🅂🅈🅂🅃🄴🄼
============================================
1. Login as User
2. Login as Admin
3. Register as New User
4. Exit
Enter your choice: _
```

#### **1. User Registration**

Follow the on-screen prompts to create a new account by providing your name, email, phone number, and NID. The system will validate all inputs before saving your data to `users.txt`.

#### **2. User Login**

From the User Panel, enter your registered email and password to access the User Dashboard and its features.

#### **3. Admin Login**

From the main menu, select the Admin Panel. Use the default credentials to gain access:

  * **Username**: `admin`
  * **Password**: `123`

-----

### ✅ Benefits of the Project

This system brings several benefits to both users and administrators.

1.  **Improved Efficiency**: By automating the process of finding and reserving a parking spot, the system reduces confusion and saves time for drivers.
2.  **Revenue Generation**: The system automatically calculates parking fees and supports various payment methods, helping in efficient revenue management.
3.  **Enhanced Security**: Features like overstay alerts enhance overall security by monitoring vehicles and preventing unauthorized parking.
4.  **Accessibility and Affordability**: The system is designed as a simple, offline console application that can run on any basic computer, making it an affordable solution for small organizations that can't afford expensive technologies.

-----

### 🔧 Technical Details

  - **Programming Language**: C was chosen because it's fast, reliable, and works on any basic machine.
  - **IDEs**: The project was developed using **VS Code** and **CodeBlocks**.
  - **Data Structures**: The project uses `structs` to organize data for users and other entities.
  - **File Handling**: All data is stored and retrieved from plain text files (`.txt`) in a pipe-separated format.
  - **Design Methodology**: The project followed a structured approach including requirement gathering, system design, development, and testing.
  - **UML Diagrams**: The project includes detailed Use Case Diagrams, a Sequence Diagram, and descriptions to map out system functionalities and interactions between actors (User, Admin, and Payment Gateway).

-----

### 📝 License

This project is open-source and available under the MIT License.

-----

### 🤝 Contributing

Feel free to contribute to this project by reporting bugs, suggesting new features, or improving the code.

-----

### 📞 Support

For any questions or issues, please refer to the code comments within each source file (`.c`) for implementation details.
