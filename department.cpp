#include "project.h"
#include <iostream>
#include <fstream>

using namespace std;

static bool stringsEqual(const char* a, const char* b)
{
    int idx = 0;
    while (a[idx] != '\0' || b[idx] != '\0')
    {
        if (a[idx] != b[idx])
            return false;
        idx++;
    }
    return true;
}

static void copyString(char* destination, const char* source, int destinationSize)
{
    int i = 0;
    while (i < destinationSize - 1 && source[i] != '\0')
    {
        destination[i] = source[i];
        i++;
    }
    destination[i] = '\0';
}

static bool stringEmpty(const char* text)
{
    return text[0] == '\0';
}

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------
static int findDepartmentIndexByName(const char dept_name[])
{
    for (int i = 0; i < department_count; i++)
    {
        if (stringsEqual(departments_list[i].dept_name, dept_name))
            return i;
    }
    return -1;
}

static void ensureDepartmentCapacity(int dept_index, int minCapacity)
{
    if (dept_index < 0 || dept_index >= department_count)
        return;

    if (departments_list[dept_index].emp_capacity >= minCapacity)
        return;

    int newCapacity = departments_list[dept_index].emp_capacity * 2;
    if (newCapacity < minCapacity)
        newCapacity = minCapacity;
    if (newCapacity < 10)
        newCapacity = 10;

    int* newArray = new int[newCapacity];
    for (int i = 0; i < departments_list[dept_index].emp_count; i++)
        newArray[i] = departments_list[dept_index].emp_ids[i];

    delete[] departments_list[dept_index].emp_ids;
    departments_list[dept_index].emp_ids = newArray;
    departments_list[dept_index].emp_capacity = newCapacity;
}

// ---------------------------------------------------------------------------
// Department level operations
// ---------------------------------------------------------------------------
void addDepartment(char deptName[], int capacity)
{
    if (department_count >= MAX_DEPARTMENTS)
    {
        cout << "Department list is full.\n";
        return;
    }
    if (stringEmpty(deptName))
    {
        cout << "Department name cannot be empty.\n";
        return;
    }
    if (findDepartmentIndexByName(deptName) != -1)
    {
        cout << "Department already exists.\n";
        return;
    }
    if (capacity <= 0)
    {
        cout << "Capacity must be greater than 0.\n";
        return;
    }
    if (capacity > 1000)
    {
        cout << "Capacity too large. Maximum allowed is 1000.\n";
        return;
    }

    Department& slot = departments_list[department_count];
    copyString(slot.dept_name, deptName, 30);
    slot.emp_count = 0;
    slot.emp_capacity = capacity;
    slot.emp_ids = new int[slot.emp_capacity];
    department_count++;
    cout << "Department added with capacity: " << capacity << ".\n";
}

void removeDepartment(char deptName[])
{
    int idx = findDepartmentIndexByName(deptName);
    if (idx == -1)
    {
        cout << "Department not found.\n";
        return;
    }

    if (departments_list[idx].emp_ids != nullptr)
        delete[] departments_list[idx].emp_ids;

    for (int i = idx; i < department_count - 1; i++)
        departments_list[i] = departments_list[i + 1];

    departments_list[department_count - 1].emp_ids = nullptr;
    departments_list[department_count - 1].emp_count = 0;
    departments_list[department_count - 1].emp_capacity = 0;
    departments_list[department_count - 1].dept_name[0] = '\0';

    department_count--;
    cout << "Department removed.\n";
}

int findTopPerformer(char deptName[])
{
    int idx = findDepartmentIndexByName(deptName);
    if (idx == -1)
    {
        cout << "Department not found.\n";
        return -1;
    }
    if (departments_list[idx].emp_count == 0)
    {
        cout << "No employees in this department.\n";
        return -1;
    }

    if (employee_count == 0)
    {
        cout << "No employees loaded in system.\n";
        return -1;
    }

    double bestScore = -1.0;
    int bestId = -1;
    int foundCount = 0;
    
    for (int i = 0; i < departments_list[idx].emp_count; i++)
    {
        int empId = departments_list[idx].emp_ids[i];
        bool found = false;
        for (int e = 0; e < employee_count; e++)
        {
            if (employees_list[e].emp_id == empId)
            {
                found = true;
                foundCount++;
                if (employees_list[e].performance > bestScore)
                {
                    bestScore = employees_list[e].performance;
                    bestId = empId;
                }
                break;
            }
        }
        if (!found)
        {
            cout << "Warning: Employee ID " << empId << " not found in employee list.\n";
        }
    }
    
    if (foundCount == 0)
    {
        cout << "No valid employees found in department (employee IDs don't match).\n";
        return -1;
    }
    
    if (bestId == -1)
    {
        cout << "No employees with valid performance scores found.\n";
        return -1;
    }
    
    return bestId;
}

void displayDepartment(char deptName[])
{
    int idx = findDepartmentIndexByName(deptName);
    if (idx == -1)
    {
        cout << "Department not found.\n";
        return;
    }

    cout << "Department: " << departments_list[idx].dept_name << '\n';
    cout << "Capacity: " << departments_list[idx].emp_capacity 
         << ", Current Employees: " << departments_list[idx].emp_count << '\n';

    if (departments_list[idx].emp_count == 0)
    {
        cout << "No employees in this department.\n";
        return;
    }

    if (employee_count == 0)
    {
        cout << "Warning: No employees loaded in system. Cannot display employee details.\n";
        cout << "Employee IDs in department: ";
        for (int i = 0; i < departments_list[idx].emp_count; i++)
        {
            if (i > 0) cout << ", ";
            cout << departments_list[idx].emp_ids[i];
        }
        cout << '\n';
        return;
    }

    int foundCount = 0;
    int notFoundCount = 0;
    
    for (int i = 0; i < departments_list[idx].emp_count; i++)
    {
        int empId = departments_list[idx].emp_ids[i];
        bool found = false;
        for (int e = 0; e < employee_count; e++)
        {
            if (employees_list[e].emp_id == empId)
            {
                found = true;
                foundCount++;
                cout << "ID: " << employees_list[e].emp_id
                     << ", Name: " << employees_list[e].emp_name
                     << ", Performance: " << employees_list[e].performance << '\n';
                break;
            }
        }
        if (!found)
        {
            notFoundCount++;
            cout << "ID: " << empId << " (NOT FOUND in employee list)\n";
        }
    }
    
    if (notFoundCount > 0)
    {
        cout << "\nWarning: " << notFoundCount << " employee ID(s) from department file not found in employee list.\n";
        cout << "Make sure employee.txt contains employees with these IDs.\n";
    }
    
    if (foundCount == 0)
    {
        cout << "No valid employees found. All employee IDs in department don't match employee list.\n";
    }
}

void displayDeptCapacity(char deptName[])
{
    int idx = findDepartmentIndexByName(deptName);
    if (idx == -1)
    {
        cout << "Department not found.\n";
        return;
    }

    Department& dept = departments_list[idx];
    cout << "Department: " << dept.dept_name << '\n';
    cout << "Maximum Capacity: " << dept.emp_capacity << '\n';
    cout << "Current Employees: " << dept.emp_count << '\n';
    cout << "Available Slots: " << (dept.emp_capacity - dept.emp_count) << '\n';
    
    if (dept.emp_count >= dept.emp_capacity)
    {
        cout << "Status: FULL (at capacity)\n";
    }
    else
    {
        double percent = (dept.emp_count * 100.0) / dept.emp_capacity;
        cout << "Status: " << percent << "% full\n";
    }
}

double getDeptAvgPerformance(char deptName[])
{
    int idx = findDepartmentIndexByName(deptName);
    if (idx == -1)
    {
        cout << "Department not found.\n";
        return 0.0;
    }
    if (departments_list[idx].emp_count == 0)
    {
        cout << "No employees in this department.\n";
        return 0.0;
    }

    if (employee_count == 0)
    {
        cout << "No employees loaded in system.\n";
        return 0.0;
    }

    double sum = 0.0;
    int counted = 0;
    int notFoundCount = 0;
    
    for (int i = 0; i < departments_list[idx].emp_count; i++)
    {
        int empId = departments_list[idx].emp_ids[i];
        bool found = false;
        for (int e = 0; e < employee_count; e++)
        {
            if (employees_list[e].emp_id == empId)
            {
                found = true;
                sum += employees_list[e].performance;
                counted++;
                break;
            }
        }
        if (!found)
        {
            notFoundCount++;
            cout << "Warning: Employee ID " << empId << " not found in employee list.\n";
        }
    }
    
    if (counted == 0)
    {
        cout << "No valid employees found in department (employee IDs don't match).\n";
        return 0.0;
    }
    
    if (notFoundCount > 0)
    {
        cout << "Note: " << notFoundCount << " employee ID(s) from department file not found in employee list.\n";
    }
    
    return sum / counted;
}

// ---------------------------------------------------------------------------
// File handling for departments (text)
// Format per line:
// <department_name> <employee_count> <emp_id_1> <emp_id_2> ...
// ---------------------------------------------------------------------------
void loadDepartmentsFromText(const char* filename)
{
    ifstream fin(filename);

    // Clear existing allocations
    for (int i = 0; i < department_count; i++)
    {
        delete[] departments_list[i].emp_ids;
        departments_list[i].emp_ids = nullptr;
        departments_list[i].emp_count = 0;
        departments_list[i].emp_capacity = 0;
        departments_list[i].dept_name[0] = '\0';
    }
    department_count = 0;

    if (!fin)
        return;

    while (department_count < MAX_DEPARTMENTS)
    {
        char nameBuffer[30];
        int employeeTotal = 0;

        fin >> nameBuffer >> employeeTotal;
        if (fin.fail())
            break;

        if (employeeTotal < 0)
            employeeTotal = 0;

        Department& slot = departments_list[department_count];
        copyString(slot.dept_name, nameBuffer, 30);
        slot.emp_count = employeeTotal;
        slot.emp_capacity = (employeeTotal > 0) ? employeeTotal : 10;
        slot.emp_ids = new int[slot.emp_capacity];

        for (int i = 0; i < employeeTotal; i++)
        {
            fin >> slot.emp_ids[i];
            if (fin.fail())
            {
                slot.emp_count = i;
                break;
            }
        }

        department_count++;
    }
}

void saveDepartmentsToText(const char* filename)
{
    ofstream fout(filename);
    if (!fout)
    {
        cout << "Unable to open departments file for writing.\n";
        return;
    }

    for (int i = 0; i < department_count; i++)
    {
        Department& d = departments_list[i];
        fout << d.dept_name << ' ' << d.emp_count;
        for (int j = 0; j < d.emp_count; j++)
            fout << ' ' << d.emp_ids[j];
        fout << '\n';
    }
}
