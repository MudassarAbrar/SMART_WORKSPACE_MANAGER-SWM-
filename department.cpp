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
void addDepartment(char deptName[])
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

    Department& slot = departments_list[department_count];
    copyString(slot.dept_name, deptName, 30);
    slot.emp_count = 0;
    slot.emp_capacity = 10;
    slot.emp_ids = new int[slot.emp_capacity];
    department_count++;
    cout << "Department added.\n";
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
        return -1;
    if (departments_list[idx].emp_count == 0)
        return -1;

    double bestScore = -1.0;
    int bestId = -1;
    for (int i = 0; i < departments_list[idx].emp_count; i++)
    {
        int empId = departments_list[idx].emp_ids[i];
        for (int e = 0; e < employee_count; e++)
        {
            if (employees_list[e].emp_id == empId)
            {
                if (employees_list[e].performance > bestScore)
                {
                    bestScore = employees_list[e].performance;
                    bestId = empId;
                }
                break;
            }
        }
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

    if (departments_list[idx].emp_count == 0)
    {
        cout << "No employees in this department.\n";
        return;
    }

    cout << "Department: " << departments_list[idx].dept_name << '\n';
    for (int i = 0; i < departments_list[idx].emp_count; i++)
    {
        int empId = departments_list[idx].emp_ids[i];
        for (int e = 0; e < employee_count; e++)
        {
            if (employees_list[e].emp_id == empId)
            {
                cout << "ID: " << employees_list[e].emp_id
                     << ", Name: " << employees_list[e].emp_name
                     << ", Performance: " << employees_list[e].performance << '\n';
                break;
            }
        }
    }
}

double getDeptAvgPerformance(char deptName[])
{
    int idx = findDepartmentIndexByName(deptName);
    if (idx == -1 || departments_list[idx].emp_count == 0)
        return 0.0;

    double sum = 0.0;
    int counted = 0;
    for (int i = 0; i < departments_list[idx].emp_count; i++)
    {
        int empId = departments_list[idx].emp_ids[i];
        for (int e = 0; e < employee_count; e++)
        {
            if (employees_list[e].emp_id == empId)
            {
                sum += employees_list[e].performance;
                counted++;
                break;
            }
        }
    }
    if (counted == 0)
        return 0.0;
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
