/* ============================================================
   STUDENT ATTENDANCE MANAGEMENT SYSTEM
   Language       : C
   Project Type   : Console Based
   Category       : Management System
   Description    : This program automates the process of student
                     attendance management. It allows Admin and
                     Teacher to log in, add/view/update/delete
                     student records, mark attendance, calculate
                     attendance percentage, and store everything
                     in text files for persistence.
   ============================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ---------- File Names (used for persistence) ---------- */
#define STUDENT_FILE    "students.txt"
#define ATTENDANCE_FILE "attendance.txt"

/* ---------- Fixed Login Credentials ----------
   (Simple authentication as required by the proposal) */
#define ADMIN_USERNAME   "admin"
#define ADMIN_PASSWORD   "admin123"
#define TEACHER_USERNAME "teacher"
#define TEACHER_PASSWORD "teacher123"

/* ---------- Structures ---------- */
typedef struct {
    int  id;
    char name[50];
} Student;

typedef struct {
    int  id;
    char name[50];
    int  present;   /* total classes present */
    int  absent;    /* total classes absent  */
} Attendance;

/* ---------- Function Prototypes ---------- */
void mainMenu(void);
int  adminLogin(void);
int  teacherLogin(void);
void adminMenu(void);
void teacherMenu(void);

void addStudent(void);
void viewStudents(void);
void searchStudent(void);
void updateStudent(void);
void deleteStudent(void);

void markAttendance(void);
void viewAttendanceRecords(void);
void calculateAttendancePercentage(void);
void displayAttendanceReport(void);

int  studentExists(int id);
void clearInputBuffer(void);
void pause_screen(void);

/* ============================================================
   MAIN FUNCTION - Entry point of the program
   ============================================================ */
int main(void) {
    mainMenu();
    return 0;
}

/* ============================================================
   MAIN MENU - Admin Login / Teacher Login / Exit
   ============================================================ */
void mainMenu(void) {
    int choice;

    while (1) {
        printf("\n============================================\n");
        printf("   STUDENT ATTENDANCE MANAGEMENT SYSTEM\n");
        printf("============================================\n");
        printf("1. Admin Login\n");
        printf("2. Teacher Login\n");
        printf("3. Exit\n");
        printf("--------------------------------------------\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            clearInputBuffer();
            printf("Invalid input! Please enter a number.\n");
            continue;
        }

        switch (choice) {
            case 1:
                if (adminLogin())
                    adminMenu();
                else
                    printf("Invalid Admin credentials!\n");
                break;
            case 2:
                if (teacherLogin())
                    teacherMenu();
                else
                    printf("Invalid Teacher credentials!\n");
                break;
            case 3:
                printf("Exiting system safely... Goodbye!\n");
                exit(0);
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
}

/* ============================================================
   LOGIN FUNCTIONS
   ============================================================ */
int adminLogin(void) {
    char username[30], password[30];

    printf("\n--- ADMIN LOGIN ---\n");
    printf("Username: ");
    scanf("%s", username);
    printf("Password: ");
    scanf("%s", password);

    if (strcmp(username, ADMIN_USERNAME) == 0 &&
        strcmp(password, ADMIN_PASSWORD) == 0) {
        printf("Login successful! Welcome Admin.\n");
        return 1;
    }
    return 0;
}

int teacherLogin(void) {
    char username[30], password[30];

    printf("\n--- TEACHER LOGIN ---\n");
    printf("Username: ");
    scanf("%s", username);
    printf("Password: ");
    scanf("%s", password);

    if (strcmp(username, TEACHER_USERNAME) == 0 &&
        strcmp(password, TEACHER_PASSWORD) == 0) {
        printf("Login successful! Welcome Teacher.\n");
        return 1;
    }
    return 0;
}

/* ============================================================
   ADMIN MENU - full control: add/view/search/update/delete
   ============================================================ */
void adminMenu(void) {
    int choice;

    while (1) {
        printf("\n--------- ADMIN MENU ---------\n");
        printf("1. Add Student Record\n");
        printf("2. View Student List\n");
        printf("3. Search Student by ID\n");
        printf("4. Update Student Information\n");
        printf("5. Delete Student Record\n");
        printf("6. View Attendance Records\n");
        printf("7. Display Attendance Report\n");
        printf("8. Logout\n");
        printf("-------------------------------\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            clearInputBuffer();
            printf("Invalid input!\n");
            continue;
        }

        switch (choice) {
            case 1: addStudent(); break;
            case 2: viewStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6: viewAttendanceRecords(); break;
            case 7: displayAttendanceReport(); break;
            case 8: printf("Logging out...\n"); return;
            default: printf("Invalid choice!\n");
        }
    }
}

/* ============================================================
   TEACHER MENU - add/view students, mark & view attendance
   ============================================================ */
void teacherMenu(void) {
    int choice;

    while (1) {
        printf("\n--------- TEACHER MENU ---------\n");
        printf("1. Add Student Record\n");
        printf("2. View Student List\n");
        printf("3. Mark Attendance\n");
        printf("4. View Attendance Records\n");
        printf("5. Calculate Attendance Percentage\n");
        printf("6. Display Attendance Report\n");
        printf("7. Logout\n");
        printf("---------------------------------\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            clearInputBuffer();
            printf("Invalid input!\n");
            continue;
        }

        switch (choice) {
            case 1: addStudent(); break;
            case 2: viewStudents(); break;
            case 3: markAttendance(); break;
            case 4: viewAttendanceRecords(); break;
            case 5: calculateAttendancePercentage(); break;
            case 6: displayAttendanceReport(); break;
            case 7: printf("Logging out...\n"); return;
            default: printf("Invalid choice!\n");
        }
    }
}

/* ============================================================
   ADD NEW STUDENT RECORD -> stored in students.txt
   ============================================================ */
void addStudent(void) {
    Student s;
    FILE *fp;

    printf("\n--- Add New Student ---\n");
    printf("Enter Student ID: ");
    scanf("%d", &s.id);

    if (studentExists(s.id)) {
        printf("A student with this ID already exists!\n");
        return;
    }

    printf("Enter Student Name: ");
    clearInputBuffer();
    fgets(s.name, sizeof(s.name), stdin);
    s.name[strcspn(s.name, "\n")] = '\0';   /* remove newline */

    fp = fopen(STUDENT_FILE, "a");
    if (fp == NULL) {
        printf("Error opening student file!\n");
        return;
    }
    fprintf(fp, "%d,%s\n", s.id, s.name);
    fclose(fp);

    printf("Student record added successfully!\n");
}

/* ============================================================
   VIEW STUDENT LIST -> reads from students.txt
   ============================================================ */
void viewStudents(void) {
    FILE *fp;
    int id;
    char name[50];

    fp = fopen(STUDENT_FILE, "r");
    if (fp == NULL) {
        printf("No student records found yet.\n");
        return;
    }

    printf("\n--- Student List ---\n");
    printf("%-10s %-30s\n", "ID", "Name");
    printf("-----------------------------------\n");

    while (fscanf(fp, "%d,%49[^\n]\n", &id, name) == 2) {
        printf("%-10d %-30s\n", id, name);
    }
    fclose(fp);
}

/* ============================================================
   SEARCH STUDENT BY ID
   ============================================================ */
void searchStudent(void) {
    FILE *fp;
    int id, searchId, found = 0;
    char name[50];

    printf("\nEnter Student ID to search: ");
    scanf("%d", &searchId);

    fp = fopen(STUDENT_FILE, "r");
    if (fp == NULL) {
        printf("No student records found yet.\n");
        return;
    }

    while (fscanf(fp, "%d,%49[^\n]\n", &id, name) == 2) {
        if (id == searchId) {
            printf("\nStudent Found!\n");
            printf("ID  : %d\n", id);
            printf("Name: %s\n", name);
            found = 1;
            break;
        }
    }
    fclose(fp);

    if (!found)
        printf("No student found with ID %d.\n", searchId);
}

/* ============================================================
   UPDATE STUDENT INFORMATION (name or ID)
   ============================================================ */
void updateStudent(void) {
    FILE *fp, *temp;
    int id, searchId, newId, found = 0;
    char name[50], newName[50];

    printf("\nEnter Student ID to update: ");
    scanf("%d", &searchId);

    fp = fopen(STUDENT_FILE, "r");
    if (fp == NULL) {
        printf("No student records found yet.\n");
        return;
    }

    temp = fopen("temp.txt", "w");
    if (temp == NULL) {
        printf("Error creating temporary file!\n");
        fclose(fp);
        return;
    }

    while (fscanf(fp, "%d,%49[^\n]\n", &id, name) == 2) {
        if (id == searchId) {
            found = 1;
            printf("Enter new ID (or same %d to keep it): ", id);
            scanf("%d", &newId);
            printf("Enter new Name: ");
            clearInputBuffer();
            fgets(newName, sizeof(newName), stdin);
            newName[strcspn(newName, "\n")] = '\0';
            fprintf(temp, "%d,%s\n", newId, newName);
        } else {
            fprintf(temp, "%d,%s\n", id, name);
        }
    }

    fclose(fp);
    fclose(temp);

    remove(STUDENT_FILE);
    rename("temp.txt", STUDENT_FILE);

    if (found)
        printf("Student record updated successfully!\n");
    else
        printf("No student found with ID %d.\n", searchId);
}

/* ============================================================
   DELETE STUDENT RECORD
   ============================================================ */
void deleteStudent(void) {
    FILE *fp, *temp;
    int id, deleteId, found = 0;
    char name[50];

    printf("\nEnter Student ID to delete: ");
    scanf("%d", &deleteId);

    fp = fopen(STUDENT_FILE, "r");
    if (fp == NULL) {
        printf("No student records found yet.\n");
        return;
    }

    temp = fopen("temp.txt", "w");
    if (temp == NULL) {
        printf("Error creating temporary file!\n");
        fclose(fp);
        return;
    }

    while (fscanf(fp, "%d,%49[^\n]\n", &id, name) == 2) {
        if (id == deleteId) {
            found = 1;   /* skip writing this record -> deletes it */
        } else {
            fprintf(temp, "%d,%s\n", id, name);
        }
    }

    fclose(fp);
    fclose(temp);

    remove(STUDENT_FILE);
    rename("temp.txt", STUDENT_FILE);

    if (found)
        printf("Student record deleted successfully!\n");
    else
        printf("No student found with ID %d.\n", deleteId);
}

/* ============================================================
   MARK ATTENDANCE (Present/Absent) -> appended to attendance.txt
   ============================================================ */
void markAttendance(void) {
    int id;
    char name[50];
    int status;
    FILE *fp;

    printf("\nEnter Student ID to mark attendance: ");
    scanf("%d", &id);

    if (!studentExists(id)) {
        printf("No student found with ID %d. Please add the student first.\n", id);
        return;
    }

    /* fetch the student's name for the attendance record */
    FILE *sfp = fopen(STUDENT_FILE, "r");
    int sid;
    char sname[50];
    while (fscanf(sfp, "%d,%49[^\n]\n", &sid, sname) == 2) {
        if (sid == id) {
            strcpy(name, sname);
            break;
        }
    }
    fclose(sfp);

    printf("Enter status (1 = Present, 0 = Absent): ");
    scanf("%d", &status);

    fp = fopen(ATTENDANCE_FILE, "a");
    if (fp == NULL) {
        printf("Error opening attendance file!\n");
        return;
    }

    fprintf(fp, "%d,%s,%s\n", id, name, (status == 1) ? "Present" : "Absent");
    fclose(fp);

    printf("Attendance marked successfully for %s.\n", name);
}

/* ============================================================
   VIEW ATTENDANCE RECORDS -> reads from attendance.txt
   ============================================================ */
void viewAttendanceRecords(void) {
    FILE *fp;
    int id;
    char name[50], status[10];

    fp = fopen(ATTENDANCE_FILE, "r");
    if (fp == NULL) {
        printf("No attendance records found yet.\n");
        return;
    }

    printf("\n--- Attendance Records ---\n");
    printf("%-10s %-25s %-10s\n", "ID", "Name", "Status");
    printf("--------------------------------------------\n");

    while (fscanf(fp, "%d,%49[^,],%9[^\n]\n", &id, name, status) == 3) {
        printf("%-10d %-25s %-10s\n", id, name, status);
    }
    fclose(fp);
}

/* ============================================================
   CALCULATE ATTENDANCE PERCENTAGE for a given student
   ============================================================ */
void calculateAttendancePercentage(void) {
    FILE *fp;
    int id, searchId;
    char name[50], status[10];
    int present = 0, total = 0;

    printf("\nEnter Student ID to calculate attendance percentage: ");
    scanf("%d", &searchId);

    fp = fopen(ATTENDANCE_FILE, "r");
    if (fp == NULL) {
        printf("No attendance records found yet.\n");
        return;
    }

    while (fscanf(fp, "%d,%49[^,],%9[^\n]\n", &id, name, status) == 3) {
        if (id == searchId) {
            total++;
            if (strcmp(status, "Present") == 0)
                present++;
        }
    }
    fclose(fp);

    if (total == 0) {
        printf("No attendance records found for ID %d.\n", searchId);
        return;
    }

    float percentage = ((float)present / total) * 100;
    printf("\nTotal Classes : %d\n", total);
    printf("Present       : %d\n", present);
    printf("Attendance %%  : %.2f%%\n", percentage);
}

/* ============================================================
   DISPLAY ATTENDANCE REPORT - summary of all students
   ============================================================ */
void displayAttendanceReport(void) {
    FILE *fp;
    int id;
    char name[50], status[10];

    /* simple arrays to accumulate per-student totals */
    int ids[200], present[200], total[200];
    char names[200][50];
    int count = 0, i, found;

    fp = fopen(ATTENDANCE_FILE, "r");
    if (fp == NULL) {
        printf("No attendance records found yet.\n");
        return;
    }

    while (fscanf(fp, "%d,%49[^,],%9[^\n]\n", &id, name, status) == 3) {
        found = 0;
        for (i = 0; i < count; i++) {
            if (ids[i] == id) {
                found = 1;
                total[i]++;
                if (strcmp(status, "Present") == 0)
                    present[i]++;
                break;
            }
        }
        if (!found && count < 200) {
            ids[count] = id;
            strcpy(names[count], name);
            total[count] = 1;
            present[count] = (strcmp(status, "Present") == 0) ? 1 : 0;
            count++;
        }
    }
    fclose(fp);

    if (count == 0) {
        printf("No attendance data available to generate report.\n");
        return;
    }

    printf("\n================= ATTENDANCE REPORT =================\n");
    printf("%-10s %-20s %-12s %-10s\n", "ID", "Name", "Present/Total", "Percent");
    printf("-------------------------------------------------------\n");

    for (i = 0; i < count; i++) {
        float pct = ((float)present[i] / total[i]) * 100;
        char ratio[20];
        sprintf(ratio, "%d/%d", present[i], total[i]);
        printf("%-10d %-20s %-12s %.2f%%\n", ids[i], names[i], ratio, pct);
    }
    printf("=======================================================\n");
}

/* ============================================================
   HELPER: Check if a student with given ID already exists
   ============================================================ */
int studentExists(int id) {
    FILE *fp;
    int sid;
    char name[50];

    fp = fopen(STUDENT_FILE, "r");
    if (fp == NULL)
        return 0;

    while (fscanf(fp, "%d,%49[^\n]\n", &sid, name) == 2) {
        if (sid == id) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

/* ============================================================
   HELPER: Clear leftover input buffer (for scanf safety)
   ============================================================ */
void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/* (Optional helper kept for future use / not required but harmless) */
void pause_screen(void) {
    printf("\nPress Enter to continue...");
    clearInputBuffer();
    getchar();
}
