#pragma once

#include <iostream>

// STRUCTS
struct Employee
{
    int   emp_id;
    char  emp_name[50];
    char  dept_name[30];
    double salary;
    int   attendance;
    double performance;
    int   bonus_eligible;   
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
    char  meet_date[11];   // "YYYY-MM-DD" FORMAT SIRF YHI RHEY GA
    char  meet_time[6];    // "HH:MM" FORMAT SIRF YHI RHEY GA
    int   participant_count;
    int   participant_capacity;
    int*  participant_ids; 
};

// GLOBALS (DECLARATIONS ONLY) definitions global.cpp mein hain

extern const int MAX_EMPLOYEES;
extern const int MAX_DEPARTMENTS;
extern const int MAX_MEETINGS;
extern const int MAX_EMP_PER_DEPT;
extern const int MAX_DEPT_NAME_LENGTH;

extern Employee*   employees_list;
extern int         employee_count;

extern Department* departments_list;
extern int         department_count;

extern Meeting*    meetings_list;
extern int         meeting_count;

// global function prototypes
extern int findDepartmentIndexByName(const char dept_name[]);
extern void copyString(char* dest, const char* src, int destSize);
extern bool stringEmpty(const char* text);
extern int findEmployeeIndex(int empId);
extern bool stringsEqual(const char* a, const char* b);

// 1) Add a new employee 
void addEmployee();

// 2) Remove an employee by ID
void removeEmployee(int emp_id);

// 3) Mark / update attendance for a specific employee ID
void markAttendance(int emp_id);

// 4) Update performance score for a specific employee ID
void updatePerformance(int emp_id, double score);

// 5) Calculate bonus eligibility for a specific employee ID
void calculateBonus(int emp_id);

// 6) Display full details for one employee by ID
void displayEmployee(int emp_id);

// 7) Add a new department by name and capacity
void addDepartment(char deptName[], int capacity);

// 8) Remove a department by name
void removeDepartment(char deptName[]);

// 9) Find the top performer ID in a given department.
int getBestEmployeeInDepartment(int idx);

// 10) Display all employees in a given department
void displayDepartment(char deptName[]);

// 11) Get average performance of a given department
double getDeptAvgPerformance(const char* deptName);

// 12) Display department capacity information
void displayDeptCapacity(char deptName[]);

// 13) Add a new meeting 
void addMeeting();

// 14) Display details of a meeting 
void displayMeeting(int index);

// 16) Add a participant (employee ID) to a meeting 
void addParticipant(int meetIdx, int empId);



// Allocate dynamic arrays for employees, departments, meetings for whole program
void initializeMemory();

// Free all dynamic memory 
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
