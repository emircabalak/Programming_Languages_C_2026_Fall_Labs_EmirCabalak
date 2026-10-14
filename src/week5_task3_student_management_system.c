// week5_task3_student_management_system.c
// Name: Emir  Surname: Cabalak  Student ID: 260AEB076
//
// Task 3: Mini-project – Student management system with file persistence
// Week 5 – Files & Modular Programming
//
// Build: make lab5   Run: ./bin/week5_task3_student_management_system

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_STUDENTS 100
#define NAME_LEN 50
#define LINE_LEN 256
#define DATA_FILE "students.txt"

typedef struct Student {
    char name[NAME_LEN];
    int id;
    float gpa;
} Student;

// Function prototypes (do not change these signatures – the autograder calls them directly)
int  load_students(Student arr[], int max);          // returns number of records loaded
int  save_students(const Student arr[], int count);  // 0 on success, -1 on error
void add_student(Student arr[], int *count);
void list_students(const Student arr[], int count);
int  read_line(const char *prompt, char *buf, size_t size);

int main(void) {
    Student students[MAX_STUDENTS];
    int count = 0;
    char input[LINE_LEN];
    int running = 1;

    count = load_students(students, MAX_STUDENTS);
    printf("Loaded %d student(s) from %s\n", count, DATA_FILE);

    while (running) {
        printf("\n=== Student Management System ===\n");
        printf("1. List students\n");
        printf("2. Add student\n");
        printf("3. Save and Exit\n");

        int choice;
        if (read_line("Select an option: ", input, sizeof input) != 0) {
            choice = 3;                         // end of input: save and exit instead of looping forever
        } else {
            char *end;
            long value = strtol(input, &end, 10);
            choice = (end != input && *end == '\0') ? (int)value : 0;   // non-numbers become invalid
        }

        switch (choice) {
            case 1:
                list_students(students, count);
                break;
            case 2:
                add_student(students, &count);
                break;
            case 3:
                if (save_students(students, count) != 0) {
                    perror(DATA_FILE);
                    printf("Error: could not save data.\n");
                } else {
                    printf("Data saved. Goodbye!\n");
                }
                running = 0;
                break;
            default:
                printf("Invalid option. Try again.\n");
        }
    }

    return EXIT_SUCCESS;
}

// Provided helper – prints a prompt and reads one line from stdin into buf.
// The trailing newline is removed. Returns 0 on success, -1 on end of input (EOF).
int read_line(const char *prompt, char *buf, size_t size) {
    printf("%s", prompt);
    fflush(stdout);
    if (fgets(buf, (int)size, stdin) == NULL) {
        return -1;
    }
    size_t len = strcspn(buf, "\r\n");
    if (buf[len] == '\0' && len == size - 1) {
        // Line was longer than the buffer: discard the rest of it.
        int c;
        while ((c = getchar()) != '\n' && c != EOF) { }
    }
    buf[len] = '\0';
    return 0;
}

// Reads students.txt into arr and returns how many records were loaded.
// A missing file is normal on the first run, so it counts as 0 students and not as an error.
int load_students(Student arr[], int max) {
    FILE *fp = fopen(DATA_FILE, "r");
    if (fp == NULL) {
        return 0;
    }

    char line[LINE_LEN];
    int count = 0;
    while (count < max && fgets(line, sizeof line, fp) != NULL) {
        Student s;
        // Blank or broken lines give fewer than 3 fields, and we skip them.
        if (sscanf(line, "%49[^,],%d,%f", s.name, &s.id, &s.gpa) == 3) {
            arr[count] = s;
            count++;
        }
    }

    fclose(fp);
    return count;
}

// Overwrites students.txt with every record currently in memory.
int save_students(const Student arr[], int count) {
    FILE *fp = fopen(DATA_FILE, "w");
    if (fp == NULL) {
        return -1;
    }
    for (int i = 0; i < count; i++) {
        fprintf(fp, "%s,%d,%.2f\n", arr[i].name, arr[i].id, arr[i].gpa);
    }
    fclose(fp);
    return 0;
}

// Asks for one student's data, checks it, and appends it to arr if it is valid.
void add_student(Student arr[], int *count) {
    if (*count >= MAX_STUDENTS) {
        printf("Cannot add more students (limit reached).\n");
        return;
    }

    char name[LINE_LEN];
    char id_text[LINE_LEN];
    char gpa_text[LINE_LEN];

    if (read_line("Enter name: ", name, sizeof name) != 0 ||
        read_line("Enter ID: ", id_text, sizeof id_text) != 0 ||
        read_line("Enter GPA: ", gpa_text, sizeof gpa_text) != 0) {
        return;
    }

    // strtol and strtof report where they stopped, so leftover text such as "12abc" gets caught.
    char *end;
    long id = strtol(id_text, &end, 10);
    int id_ok = end != id_text && *end == '\0' && id >= INT_MIN && id <= INT_MAX;

    float gpa = strtof(gpa_text, &end);
    int gpa_ok = end != gpa_text && *end == '\0' && gpa >= 0.0f && gpa <= 4.0f;

    // A comma in the name would break the comma-separated file format.
    int name_ok = name[0] != '\0' && strchr(name, ',') == NULL && strlen(name) < NAME_LEN;

    if (!name_ok || !id_ok || !gpa_ok) {
        printf("Invalid input. Student not added.\n");
        return;
    }

    strcpy(arr[*count].name, name);
    arr[*count].id = (int)id;
    arr[*count].gpa = gpa;
    (*count)++;
    printf("Student added successfully!\n");
}

// Prints all students as a table, or a short message when the list is empty.
void list_students(const Student arr[], int count) {
    if (count == 0) {
        printf("No students found.\n");
        return;
    }
    printf("%-5s %-20s %s\n", "ID", "Name", "GPA");
    for (int i = 0; i < count; i++) {
        printf("%-5d %-20s %.2f\n", arr[i].id, arr[i].name, arr[i].gpa);
    }
}
