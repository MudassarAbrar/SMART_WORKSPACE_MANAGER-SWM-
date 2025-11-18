// global.cpp
// Yahan pe saare global arrays / counts ka real memory allocate ho raha hai.
// Sir ne dynamic memory must bola tha, is liye new [] use kar rahe hain.

#include "project.h"

// CONSTANT LIMITS (REAL DEFINITIONS)
// Yeh limits poore program mein same rahenge.

const int MAX_EMPLOYEES   = 100;
const int MAX_DEPARTMENTS = 20;
const int MAX_MEETINGS    = 50;

// GLOBAL POINTERS AND COUNTERS (REAL STORAGE)

// Employee data for the whole program
// Dhyaan: yeh pointers ko ham initializeMemory() mein allocate karte hain.
Employee* employees_list      = nullptr;
int       employee_count      = 0;

// Department data for the whole program
Department* departments_list  = nullptr;
int         department_count  = 0;

// Meeting data for the whole program
Meeting* meetings_list        = nullptr;
int      meeting_count        = 0;

void initializeMemory()
{
    // Yahan main dynamic arrays allocate kar rahe hain
    employees_list   = new Employee[MAX_EMPLOYEES];
    departments_list = new Department[MAX_DEPARTMENTS];
    meetings_list    = new Meeting[MAX_MEETINGS];

    employee_count   = 0;
    department_count = 0;
    meeting_count    = 0;

    // Har department ke andar jo emp_ids pointer hai usko initialize kar rahe hain
    for (int i = 0; i < MAX_DEPARTMENTS; i++)
    {
        departments_list[i].emp_ids      = nullptr;
        departments_list[i].emp_count    = 0;
        departments_list[i].emp_capacity = 0;
        departments_list[i].dept_name[0] = '\0';
    }

    // Meetings ke inner pointers bhi reset kar rahe hain
    for (int i = 0; i < MAX_MEETINGS; i++)
    {
        meetings_list[i].participant_ids      = nullptr;
        meetings_list[i].participant_count    = 0;
        meetings_list[i].participant_capacity = 0;
        meetings_list[i].meet_topic[0] = '\0';
        meetings_list[i].meet_date[0]  = '\0';
        meetings_list[i].meet_time[0]  = '\0';
    }
}

void freeMemory()
{
    // Pehle departments ke inner arrays free karo
    for (int i = 0; i < department_count; i++)
    {
        if (departments_list[i].emp_ids != nullptr)
        {
            delete[] departments_list[i].emp_ids;
            departments_list[i].emp_ids      = nullptr;
            departments_list[i].emp_count    = 0;
            departments_list[i].emp_capacity = 0;
        }
    }

    // Meetings ke inner participant arrays free karo
    for (int i = 0; i < meeting_count; i++)
    {
        if (meetings_list[i].participant_ids != nullptr)
        {
            delete[] meetings_list[i].participant_ids;
            meetings_list[i].participant_ids      = nullptr;
            meetings_list[i].participant_count    = 0;
            meetings_list[i].participant_capacity = 0;
        }
    }

    // Ab main arrays free karte hain
    if (employees_list != nullptr)
    {
        delete[] employees_list;
        employees_list = nullptr;
    }

    if (departments_list != nullptr)
    {
        delete[] departments_list;
        departments_list = nullptr;
    }

    if (meetings_list != nullptr)
    {
        delete[] meetings_list;
        meetings_list = nullptr;
    }

    // Counts reset kar do, taake koi garbage na rahe
    employee_count   = 0;
    department_count = 0;
    meeting_count    = 0;
}
