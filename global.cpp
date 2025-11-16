// global.cpp

#include "project.h"

// CONSTANT LIMITS (REAL DEFINITIONS)

const int MAX_EMPLOYEES   = 100;
const int MAX_DEPARTMENTS = 20;
const int MAX_MEETINGS    = 50;
// GLOBAL POINTERS AND COUNTERS (REAL STORAGE)

// Employee data for the whole program 
//sir ne dynamic must kaha tha is lye kia h yeh to dono is wja se confuse na hona
Employee* employees      = nullptr;
int       employeeCount  = 0;    // yeh bata rha h k how many employees are currently stored

// Department data for the whole program 
Department* departments   = nullptr;
int         departmentCount = 0;  // yeh bata rha h k how many depart are currently stored


// Meeting data for the whole program 
Meeting* meetings        = nullptr;
int      meetingCount    = 0;     // yeh bata rha h k how many meetings are currently stored

    void initializeMemory()
{
    // Allocate the main dynamic arrays
    employees   = new Employee[MAX_EMPLOYEES];
    departments = new Department[MAX_DEPARTMENTS];
    meetings    = new Meeting[MAX_MEETINGS];
    employeeCount   = 0;
    departmentCount = 0;
    meetingCount    = 0;

    // Initialize department pointers
    for (int i = 0; i < MAX_DEPARTMENTS; i++)
    {
        departments[i].employeeIds      = nullptr;
        departments[i].employeeCount    = 0;
        departments[i].employeeCapacity = 0;
        departments[i].name[0] = '\0';
    }

    // Initialize meeting inner pointers
    for (int i = 0; i < MAX_MEETINGS; i++)
    {
        meetings[i].participantIds      = nullptr;
        meetings[i].participantCount    = 0;
        meetings[i].participantCapacity = 0;
        meetings[i].topic[0] = '\0';
        meetings[i].date[0]  = '\0';
        meetings[i].time[0]  = '\0';
    }
}



void freeMemory()
{
    // Free inner arrays for departments
    for (int i = 0; i < departmentCount; i++)
    {
        if (departments[i].employeeIds != nullptr)
        {
            delete[] departments[i].employeeIds;
            departments[i].employeeIds = nullptr;
            departments[i].employeeCount = 0;
            departments[i].employeeCapacity = 0;
        }
    }

    // Free inner arrays for meetings
    for (int i = 0; i < meetingCount; i++)
    {
        if (meetings[i].participantIds != nullptr)
        {
            delete[] meetings[i].participantIds;
            meetings[i].participantIds = nullptr;
            meetings[i].participantCount = 0;
            meetings[i].participantCapacity = 0;
        }
    }

    // Free main arrays
    if (employees != nullptr)
    {
        delete[] employees;
        employees = nullptr;
    }

    if (departments != nullptr)
    {
        delete[] departments;
        departments = nullptr;
    }

    if (meetings != nullptr)
    {
        delete[] meetings;
        meetings = nullptr;
    }

    // Reset counts
    employeeCount   = 0;
    departmentCount = 0;
    meetingCount    = 0;
}

