// Ashley Grafner
// 11/12/2024

//Assignment 10, Question 2:

//Program will contain:
// Multi-dimensional array of structures, with rows as employees and columns as departments
// Create or open a file for employee data
// If the file is not empty read the data into array
//Add Employee, Modify the employee, Delete employee data

//Program will now contain:
//More departments with more than or less than 10 employees in each department.
//Instead of static allocation in array, move it to pointers

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure definition for Employee with id, employee name (50 chars max), and role name (50 chars max)
typedef struct {
    int id;
    char employee_name[50]; //50 chars max
    char role_name[50]; //50 chars max
} Employee; //alias name Employee

typedef struct {
    int num_employees; //number of employees
    Employee *employees; //pointer to employee
} Department;//alias name Department

// Enumeration for the department types:
typedef enum {
    TECHNOLOGY,
    HR,
    ADMIN,
    LEGAL,
    TRANSPORTATION,
    PMO,
    SECURITY,
    RETAIL,
    BUSINESS_DEVELOPMENT,
    SALES,
    MAX_DEPARTMENTS
} DepartmentEnum; //alias name DepartmentEnum

// array of departments
Department departments[MAX_DEPARTMENTS];

// Function to initialize all departments
// This sets the number of employees in each department to 0 and the employee pointer to NULL
void initialize_departments() {
    for (int i = 0; i < MAX_DEPARTMENTS; i++) {
        departments[i].num_employees = 0; // Initialize the employee count to 0
        departments[i].employees = NULL;  // Set the employee pointer to NULL, no employees initially
    }
}

// Function to save the employee data of all departments to a binary file
// This function writes the number of employees and their details for each department to a file
void save_data(const char *filename) {
    FILE *file = fopen(filename, "wb"); // Open the file for writing in binary mode
    if (file) {
        for (int i = 0; i < MAX_DEPARTMENTS; i++) {
            fwrite(&departments[i].num_employees, sizeof(int), 1, file); // Write number of employees in the department
            if (departments[i].num_employees > 0) {
                // If the department has employees, write their details
                fwrite(departments[i].employees, sizeof(Employee), departments[i].num_employees, file);
            }
        }
        fclose(file); // Close the file after saving data
    }
}

// Function to load employee data from a binary file
// This reads the data from the file and populates the department information
void load_data(const char *filename) {
    FILE *file = fopen(filename, "rb"); // Open the file for reading in binary mode
    if (file) {
        for (int i = 0; i < MAX_DEPARTMENTS; i++) {
            fread(&departments[i].num_employees, sizeof(int), 1, file); // Read number of employees in the department
            if (departments[i].num_employees > 0) {
                // If the department has employees, allocate memory for them and read the data
                departments[i].employees = malloc(departments[i].num_employees * sizeof(Employee));
                fread(departments[i].employees, sizeof(Employee), departments[i].num_employees, file);
            }
        }
        fclose(file); // Close the file after loading data
    }
}

// Helper function to clear employee information
// This function resets an employee's ID and clears their name and role
void clear_employee(Employee *employee) {
    employee->id = 0;  // Reset the employee ID
    employee->employee_name[0] = '\0';  // Clear the employee's name (set to empty string)
    employee->role_name[0] = '\0';  // Clear the employee's role (set to empty string)
}

// Helper function to resize the employee array for a department
// This function reallocates memory for the employee array to accommodate new employees
void resize_employees(Department *dept, int new_size) {
    dept->employees = realloc(dept->employees, new_size * sizeof(Employee)); // Reallocate memory for employees
}

// Adds a new employee to a specified department
// This function either updates an existing empty slot or resizes the array and adds the new employee
void add_employee(DepartmentEnum dept, int id, const char *name, const char *role) {
    // First, try to find an empty slot (employee with ID 0)
    for (int i = 0; i < departments[dept].num_employees; i++) {
        if (departments[dept].employees[i].id == 0) {
            // If found, update the employee data
            departments[dept].employees[i].id = id;
            strcpy(departments[dept].employees[i].employee_name, name);
            strcpy(departments[dept].employees[i].role_name, role);
            return;
        }
    }

    // If no empty slot is found, resize the employee array and add the new employee
    resize_employees(&departments[dept], departments[dept].num_employees + 1);
    departments[dept].employees[departments[dept].num_employees].id = id; // Set new employee ID
    strcpy(departments[dept].employees[departments[dept].num_employees].employee_name, name); // Set name
    strcpy(departments[dept].employees[departments[dept].num_employees].role_name, role); // Set role
    departments[dept].num_employees++; // Increment the employee count
}

// Modifies an existing employee's information in a department
// This function searches for the employee by ID and updates their name and role
void modify_employee(DepartmentEnum dept, int id, const char *name, const char *role) {
    // Loop through the department's employees to find the employee by ID
    for (int i = 0; i < departments[dept].num_employees; i++) {
        if (departments[dept].employees[i].id == id) {
            // If found, modify the employee's name and role
            strcpy(departments[dept].employees[i].employee_name, name);
            strcpy(departments[dept].employees[i].role_name, role);
            return;
        }
    }
    printf("Employee not found in department.\n"); // If employee not found, print an error message
}

// Deletes an employee's information by ID in the specified department
// This function searches for the employee and clears their data if found
void delete_employee(DepartmentEnum dept, int id) {
    for (int i = 0; i < departments[dept].num_employees; i++) {
        if (departments[dept].employees[i].id == id) {
            // If found, clear the employee's data
            clear_employee(&departments[dept].employees[i]);
            return;
        }
    }
    printf("Employee not found.\n"); // If employee not found, print an error message
}

// Prints all employees in each department
// This function prints the details of all employees for each department that has employees
void print_employees() {
    // Department names array to print department names
    const char *department_names[] = {
        "Technology", "HR", "Admin", "Legal", "Transportation", "PMO", "Security", "Retail",
        "Business Development", "Sales"
    };

    for (int d = 0; d < MAX_DEPARTMENTS; d++) {
        int department_has_employees = 0;

        // Check if the department has any employees
        for (int i = 0; i < departments[d].num_employees; i++) {
            if (departments[d].employees[i].id != 0) {
                department_has_employees = 1; // If at least one employee exists, set flag
                break;
            }
        }

        // If department has employees, print the department name and employee details
        if (department_has_employees) {
            printf("Department: %s\n", department_names[d]);
            for (int i = 0; i < departments[d].num_employees; i++) {
                if (departments[d].employees[i].id != 0) {
                    // Print employee information: ID, name, and role
                    printf("  ID: %d, Name: %s, Role: %s\n",
                           departments[d].employees[i].id,
                           departments[d].employees[i].employee_name,
                           departments[d].employees[i].role_name);
                }
            }
            printf("\n"); // New line after each department
        }
    }
}


int main() {
    const char *filename = "employees_ex2.txt";
    initialize_departments();
    load_data(filename);

    add_employee(TECHNOLOGY, 1, "Ashley", "Engineer");
    add_employee(HR, 2, "Angela", "HR Coordinator");
    add_employee(ADMIN, 3, "Cooper", "Administrator");
    add_employee(LEGAL, 4, "Lisa", "Lawyer");
    add_employee(TRANSPORTATION, 5, "Tom", "Driver");
    add_employee(PMO, 6, "Sammy", "Project Manager");
    add_employee(SECURITY, 7, "Wyatt", "Security Guard");
    add_employee(RETAIL, 8, "Nick", "Cashier");
    add_employee(BUSINESS_DEVELOPMENT, 9, "Gio", "BD Manager");
    add_employee(SALES, 10, "Kian", "Sales Rep");

    print_employees();

    modify_employee(HR, 2, "Angela", "Employee Relations");
    delete_employee(TECHNOLOGY, 1);

    printf("Employees after modification and deletion:\n");
    print_employees();

    save_data(filename);
    return 0;
}

