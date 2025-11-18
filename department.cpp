#include "project.h"
#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

static void ensureDepartmentCapacity(int dept_index, int minCapacity)
{
    if (dept_index < 0 || dept_index >= department_count)
        return;

    if (departments_list[dept_index].emp_capacity >= minCapacity)
        return;

    int newCapacity = departments_list[dept_index].emp_capacity * 2;
    if (newCapacity < minCapacity)
        newCapacity = minCapacity;
   

    int* newArray = new int[newCapacity];
    for (int i = 0; i < departments_list[dept_index].emp_count; i++)
        newArray[i] = departments_list[dept_index].emp_ids[i];
//delete the previous emoloyee ids array
    delete[] departments_list[dept_index].emp_ids;
    //assign the new array to the department
    departments_list[dept_index].emp_ids = newArray;
    //assign the new capacity to the department
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
    if (capacity > MAX_EMP_PER_DEPT)
    {
        cout << "Capacity too large. Maximum allowed is " << MAX_EMP_PER_DEPT << " employees.\n";
        return;
    }

    //reference object
    Department& slot = departments_list[department_count];
    strcpy(slot.dept_name, deptName); 
    slot.emp_capacity = capacity;    
    slot.emp_count = 0;

   
    slot.emp_ids = new int[slot.emp_capacity];

    department_count++;

    cout << "Department " << deptName << " added with capacity: " << capacity << ".\n";
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

    departments_list[department_count - 1].emp_ids      = nullptr;
    departments_list[department_count - 1].emp_count    = 0;
    departments_list[department_count - 1].emp_capacity = 0;
    departments_list[department_count - 1].dept_name[0] = '\0';

    department_count--;
    cout << "Department removed.\n";
}



int getBestEmployeeInDepartment(int idx)
{
    if (idx < 0 || idx >= department_count)
        return -1;

    if (departments_list[idx].emp_count == 0)
        return -1;

    double bestScore = -1e18;   
    int    bestId    = -1;
    bool   anyValid  = false;

    for (int i = 0; i < departments_list[idx].emp_count; i++)
    {
        int empId = departments_list[idx].emp_ids[i];

        int pos = findEmployeeIndex(empId);

        if (pos == -1)
        {
            cout << "Warning: Employee ID " << empId
                 << " not found in employee list.\n";
            continue;
        }

        anyValid = true;

        double perf = employees_list[pos].performance;

        if (perf > bestScore)
        {
            bestScore = perf;
            bestId    = empId;
        }
    }

    if (!anyValid)
        return -1;

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
    cout << "Capacity: "  << departments_list[idx].emp_capacity
         << ", Current Employees: " << departments_list[idx].emp_count << '\n';

    if (departments_list[idx].emp_count == 0)
    {
        cout << "No employees in this department.\n";
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
        cout << "\nWarning: " << notFoundCount
             << " employee ID(s) from department not found in employee list.\n";
    }

    if (foundCount == 0)
    {
        cout << "No valid employees found in this department.\n";
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
    //REFERENCE OBJECT
//DEPT IS A NICKNAME FOR DEPT
    Department& dept = departments_list[idx];

    cout << "Department: "       << dept.dept_name << '\n';
    cout << "Maximum Capacity: " << dept.emp_capacity << '\n';
    cout << "Current Employees: "<< dept.emp_count    << '\n';
    cout << "Available Slots: "  << (dept.emp_capacity - dept.emp_count) << '\n';

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





double getDeptAvgPerformance(const char *deptName)
{
    int deptIdx = findDepartmentIndexByName(deptName);
    if (deptIdx == -1)
    {
        cout << "Department not found.\n";
        return -1;
    }

    double totalPerformance = 0;
    int validEmployees = 0;

    for (int i = 0; i < departments_list[deptIdx].emp_count; i++)
    {
        int empId = departments_list[deptIdx].emp_ids[i];
        int empIdx = findEmployeeIndex(empId);

        if (empIdx != -1) 
        {
            totalPerformance += employees_list[empIdx].performance;
            validEmployees++;
        }
    }

    if (validEmployees == 0)
        return -1; 

    return totalPerformance / validEmployees;
}




void loadDepartmentsFromText(const char* filename)
{
    ifstream fin(filename);

    // Clear old allocations
    for (int i = 0; i < department_count; i++)
    {
        delete[] departments_list[i].emp_ids;
        departments_list[i].emp_ids      = nullptr;
        departments_list[i].emp_count    = 0;
        departments_list[i].emp_capacity = 0;
        departments_list[i].dept_name[0] = '\0';
    }
    department_count = 0;

    if (!fin)
        return;

    while (department_count < MAX_DEPARTMENTS)
    {
        
        char tempDept[30];
        int  employeeTotal = 0;

        fin >> tempDept >> employeeTotal;
        if (fin.fail())
            break;

        
        copyString(departments_list[department_count].dept_name, tempDept, MAX_DEPT_NAME_LENGTH);
        departments_list[department_count].emp_count    = employeeTotal;
        departments_list[department_count].emp_capacity = (employeeTotal > MAX_EMPLOYEES) ? MAX_EMPLOYEES : employeeTotal;
        departments_list[department_count].emp_ids      = new int[departments_list[department_count].emp_capacity];

        for (int i = 0; i < employeeTotal; i++)
        {
            fin >> departments_list[department_count].emp_ids[i];
            if (fin.fail())
            {
                departments_list[department_count].emp_count = i;
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
