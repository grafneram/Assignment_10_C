// Ashley Grafner
// 11/12/2024

//Assignment 10, Question 1:

//Program will contain:
// Multi-dimensional array of structures, with rows as employees and columns as departments
// Create or open a file for employee data
// If the file is not empty read the data into array
//Add Employee, Modify the employee, Delete employee data

#include <stdio.h>
#include <string.h>

#define MAX_EMPLOYEES 10  // Maximum employees for each department.
#define MAX_DEPARTMENTS 4  // Number of departments types.

// Structure definition for Employee with id, employee name (50 chars max), and role name (50 chars max)
typedef struct {
    int id;
    char employee_name[50]; //50 chars max
    char role_name[50]; //50 chars max
} Employee; //alias name Employee

// Structure definition for Department an array of employees using 'Employee' structure
typedef struct {
    Employee employees[MAX_EMPLOYEES]; //4 employees per department
} Department; //alias name Department

// Enumeration for the department types:
typedef enum {
    TECHNOLOGY,
    HR,
    ADMIN,
    LEGAL
} DepartmentEnum; //alias name DepartmentEnum

// array of departments
Department departments[MAX_DEPARTMENTS] = {0};

// Save employee data to a file
void save_data(const char *filename) {
    FILE *file = fopen(filename, "wb");  // Open file for writing in BINARY mode
    if (file) {
        fwrite(departments, sizeof(departments), 1, file);  // Write department data to file
        fclose(file); //close file after writing data to file
    }
}

// Load employee data from a file
void load_data(const char *filename) {
    FILE *file = fopen(filename, "rb");  // Open file for reading in binary mode
    if (file) {
        fread(departments, sizeof(departments), 1, file);  // Read department data from file
        fclose(file); //close file after reading
    }
}

// adds a new employee to a specified department:
void add_employee(DepartmentEnum dept, int id, const char *name, const char *role) {
    for (int i = 0; i < MAX_EMPLOYEES; i++) { // Loop through the array of employees within the specified department

        // Check if the current slot is empty (an id of 0 indicates an empty slot)
        if (departments[dept].employees[i].id == 0) {
            departments[dept].employees[i].id = id;  // Assign the provided id to the empty slot

            // Copy the employee's NAME into the department's employee array at the empty slot
            strcpy(departments[dept].employees[i].employee_name, name);

            // Copy the employee's ROLE into the department's employee array at the empty slot
            strcpy(departments[dept].employees[i].role_name, role);
            return;
        }
    }
    printf(" This Department is full, please choose another.\n"); //error message
}


// Modifies an existing employee's information:
void modify_employee(DepartmentEnum dept, int id, const char *name, const char *role) {
    for (int i = 0; i < MAX_EMPLOYEES; i++) {
        if (departments[dept].employees[i].id == id) {  // Find employee by id number, then...

            // Copy the employee's NAME into the department's employee array at the empty slot
            strcpy(departments[dept].employees[i].employee_name, name);

            // Copy the employee's ROLE into the department's employee array at the empty slot
            strcpy(departments[dept].employees[i].role_name, role);
            return;
        }
    }
    printf(" This department is full, please choose another.\n");  // error message
}

// Helper function to clear employee information (sets ID to 0 and clears name and role strings)
void clear_employee(Employee *employee) {

    employee->id = 0; //employee id set to 0
    employee->employee_name[0] = '\0'; //employee id set to a newline char
    employee->role_name[0] = '\0'; //employee role set to a newline char
}


// Deletes an employee's information by ID in the specified department
void delete_employee(DepartmentEnum dept, int id) {
    // Search for the employee by ID within the specified department
    for (int i = 0; i < MAX_EMPLOYEES; i++) {
        if (departments[dept].employees[i].id == id) {  // Employee found... then will:
            clear_employee(&departments[dept].employees[i]);  // Clears the employee data
            return;
        }
    }
    printf("Employee was not found.\n"); //error if the employee was not found
}


// Prints all the employees in ALL department types
// Prints all the employees in ALL department types
// Function to print all employees in each department
void print_employees() {

    // Array of department names (assumes four departments in this example)
    const char *department_names[] = {"Technology", "HR", "Admin", "Legal"};

    // Loop through each department
    for (int d = 0; d < MAX_DEPARTMENTS; d++) {
        int department_has_employees = 0;  // Flag to check if the department has any employees

        // Check if the department has any employees
        for (int i = 0; i < MAX_EMPLOYEES; i++) {
            if (departments[d].employees[i].id != 0) {
                department_has_employees = 1;
                break;
            }
        }

        // Only print the department name if there are employees
        if (department_has_employees) {
            printf("Department: %s\n", department_names[d]);

            // Print all employees in the department
            for (int i = 0; i < MAX_EMPLOYEES; i++) {
                if (departments[d].employees[i].id != 0) {
                    printf("  ID: %d, Name: %s, Role: %s\n",
                           departments[d].employees[i].id,
                           departments[d].employees[i].employee_name,
                           departments[d].employees[i].role_name);
                }
            }
            printf("\n");  // newline after each department's employees
        }
    }
}


int main() {
    const char *filename = "employees_ex1.txt"; // File to read/write employee data in binary mode
    load_data(filename);  // Load existing data from file

    // Add employees to the following departments:
    add_employee(TECHNOLOGY, 1, "Ashley", "Engineer");
    add_employee(HR, 2, "Angela", "HR Coordinator");
    add_employee(ADMIN, 3, "Cooper", "Administrator");
    add_employee(LEGAL, 4, "Lisa", "Lawyer");
    print_employees();  // Print all employees

    // Modify Angela's role in the HR department to "Employee Relations" instead of "HR Coordinator".
    modify_employee(HR, 2, "Angela", "Employee Relations");

    // Delete Ashley from the Technology department.
    delete_employee(TECHNOLOGY, 1);

    printf("Employees after modification and deletion:\n");
    print_employees();  // Print all employees

    save_data(filename);  // Save updated data back to file
    return 0;
}
