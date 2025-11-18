#pragma once

#include <iostream>

// Thora sa simple structs bana diye hain employees, departments, meetings ke liye.
// Inko hum pure project mein use kar rahe hain.

struct Employee
{
    int   emp_id;
    char  emp_name[50];
    char  dept_name[30];
    double salary;
    int   attendance;
    double performance;
    int   bonus_eligible;   // 1 matlab haan, 0 matlab nahin
};

struct Department
{
    char  dept_name[30];
    int   emp_count;        // kitne employees iss department mein hain
    int   emp_capacity;     // max kitne rakh sakte hain
    int*  emp_ids;          // yahan sirf unke IDs store ho rahe hain
};

struct Meeting
{
    int   meet_id;
    char  meet_topic[50];
    char  meet_date[11];   // "YYYY-MM-DD"
    char  meet_time[6];    // "HH:MM"
    int   participant_count;
    int   participant_capacity;
    int*  participant_ids; // yahan sirf employee IDs rakh rahe hain
};

// GLOBALS (DECLARATIONS ONLY) – asli definitions global.cpp mein hain

extern const int MAX_EMPLOYEES;
extern const int MAX_DEPARTMENTS;
extern const int MAX_MEETINGS;

extern Employee*   employees_list;
extern int         employee_count;

extern Department* departments_list;
extern int         department_count;

extern Meeting*    meetings_list;
extern int         meeting_count;


// 1) Add a new employee (input function ke andar liya jaata hai)
void addEmployee();

// 2) Remove an employee by ID
void removeEmployee(int emp_id);

// 3) Mark / update attendance for a specific employee ID
void markAttendance(int emp_id);

// 4) Update performance score for a specific employee ID
void updatePerformance(int emp_id, double score);

// 5) Calculate bonus eligibility for a specific employee ID
//    Example logic: performance > 90 and attendance > 85
void calculateBonus(int emp_id);

// 6) Display full details for one employee by ID
void displayEmployee(int emp_id);

// 7) Add a new department by name and capacity
void addDepartment(char deptName[], int capacity);

// 8) Remove a department by name
void removeDepartment(char deptName[]);

// 9) Find the top performer ID in a given department.
int findTopPerformer(char deptName[]);

// 10) Display all employees in a given department
void displayDepartment(char deptName[]);

// 11) Get average performance of a given department
double getDeptAvgPerformance(char deptName[]);

// 12) Display department capacity information
void displayDeptCapacity(char deptName[]);

// 13) Add a new meeting (topic/date/time input yahi lete hain)
void addMeeting();

// 14) Schedule a meeting (ensure no time clash). Returns 1 on success.
int scheduleMeeting();

// 15) Display details of a meeting at a given index
void displayMeeting(int index);

// 16) Add a participant (employee ID) to a meeting by index
void addParticipant(int meetIdx, int empId);

// 17) Check if two meetings clash in date/time.
//     Return 1 if there is a clash, 0 otherwise.
int hasTimeClash(int indexA, int indexB);

// Allocate dynamic arrays for employees, departments, meetings
// Aur unke inner pointers ko bhi initialize kar dete hain
void initializeMemory();

// Free all dynamic memory (employees, departments, meetings and their inner arrays)
void freeMemory();


// FILE HANDLING PROTOTYPES

// Employees -> TEXT file
void loadEmployeesFromText(const char* filename);
void saveEmployeesToText(const char* filename);

// Departments -> TEXT file
void loadDepartmentsFromText(const char* filename);
void saveDepartmentsToText(const char* filename);

// Meetings -> TEXT file
void loadMeetingsFromText(const char* filename);
void saveMeetingsToText(const char* filename);
