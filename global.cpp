// global.cpp

#include "project.h"

// CONSTANT LIMITS (REAL DEFINITIONS)

const int MAX_EMPLOYEES   = 100;
const int MAX_DEPARTMENTS = 20;
const int MAX_MEETINGS    = 50;
// GLOBAL POINTERS AND COUNTERS (REAL STORAGE)

// Employee data for the whole program 
//sir ne dynamic must kaha tha is lye kia h yeh to dono is wja se confuse na hona
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
    // Allocate the main dynamic arrays
    employees_list   = new Employee[MAX_EMPLOYEES];
    departments_list = new Department[MAX_DEPARTMENTS];
    meetings_list    = new Meeting[MAX_MEETINGS];
    employee_count   = 0;
    department_count = 0;
    meeting_count    = 0;

    // Initialize department pointers
    for (int i = 0; i < MAX_DEPARTMENTS; i++)
    {
        departments_list[i].emp_ids      = nullptr;
        departments_list[i].emp_count    = 0;
        departments_list[i].emp_capacity = 0;
        departments_list[i].dept_name[0] = '\0';
    }

    // Initialize meeting inner pointers
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
    // Free inner arrays for departments
    for (int i = 0; i < department_count; i++)
    {
        if (departments_list[i].emp_ids != nullptr)
        {
            delete[] departments_list[i].emp_ids;
            departments_list[i].emp_ids = nullptr;
            departments_list[i].emp_count = 0;
            departments_list[i].emp_capacity = 0;
        }
    }

    // Free inner arrays for meetings
    for (int i = 0; i < meeting_count; i++)
    {
        if (meetings_list[i].participant_ids != nullptr)
        {
            delete[] meetings_list[i].participant_ids;
            meetings_list[i].participant_ids = nullptr;
            meetings_list[i].participant_count = 0;
            meetings_list[i].participant_capacity = 0;
        }
    }

    // Free main arrays
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

    // Reset counts
    employee_count   = 0;
    department_count = 0;
    meeting_count    = 0;
}

