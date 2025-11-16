#pragma once

#include <iostream>

struct Employee
{
    int   id;
    char  name[50];
    char  department[30];
    double salary;
    int   attendance;    
    double performance;   
    int   bonusEligible; 
};

struct Department
{
    char name[30];
    int  employeeCount;    
    int  employeeCapacity; 
    int* employeeIds;      
};

struct Meeting
{
    int   id;
    char  topic[50];
    char  date[11];   // "YYYY-MM-DD"
    char  time[6];    // "HH:MM"
    int   participantCount;    
    int   participantCapacity; 
    int*  participantIds;     
};

// GLOBALS (DECLARATIONS ONLY)

 extern const int MAX_EMPLOYEES;
 extern const int MAX_DEPARTMENTS;
extern const int MAX_MEETINGS;

 extern Employee*   employees;
 extern int         employeeCount;

 extern Department* departments;
 extern int         departmentCount;

 extern Meeting*   meetings;
 extern int     meetingCount;


// 1) Add a new employee (take input inside the function)
void addEmployee();

// 2) Remove an employee by ID
void removeEmployee(int id);

// 3) Mark / update attendance for a specific employee ID
void markAttendance(int id);

// 4) Update performance score for a specific employee ID
void updatePerformance(int id, double score);

// 5) Calculate bonus eligibility for a specific employee ID
//    (e.g. performance > 90 and attendance > 85)
void calculateBonus(int id);

// 6) Display full details for one employee by ID
void displayEmployee(int id);

// 7) Add a new department by name
void addDepartment(char deptName[]);

// 8) Remove a department by name
void removeDepartment(char deptName[]);

// 9) Find the top performer ID in a given department.
int findTopPerformer(char deptName[]);

// 10) Display all employees in a given department
void displayDepartment(char deptName[]);

// 11) Get average performance of a given department
double getDeptAvgPerformance(char deptName[]);

// 12) Add a new meeting (take topic/date/time as input inside)
void addMeeting();

// 13) Schedule a meeting (check clashes before confirming).
//     meetingIndex is the index in meetings[] you want to schedule.
//     Return 1 if scheduled successfully, 0 if there is a time clash.
int scheduleMeeting(int meetingIndex);

// 14) Display details of a meeting at a given index
void displayMeeting(int index);

// 15) Add a participant (employee ID) to a meeting by index
void addParticipant(int meetIdx, int empId);

// 16) Check if two meetings clash in date/time.
//     Return 1 if there is a clash, 0 otherwise.
int hasTimeClash(int indexA, int indexB);

// Allocate dynamic arrays for employees, departments, meetings
// and also initialize inner dynamic arrays for departments/meetings when needed.
void initializeMemory();

// Free all dynamic memory (employees, departments, meetings and their inner arrays)
void freeMemory();


// FILE HANDLING PROTOTYPES

// Employees -> BINARY file
void loadEmployeesFromBinary(const char* filename);
void saveEmployeesToBinary(const char* filename);

// Departments -> TEXT file
void loadDepartmentsFromText(const char* filename);
void saveDepartmentsToText(const char* filename);

// Meetings -> TEXT file
void loadMeetingsFromText(const char* filename);
void saveMeetingsToText(const char* filename);


