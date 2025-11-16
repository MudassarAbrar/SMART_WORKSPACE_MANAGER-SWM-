#include "project.h"
#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

static int findDepartmentIndexByName(const char deptName[])
{
    bool namematch;

    for (int i = 0; i < departmentCount; i++)
    {
        namematch = true;

        int len = strlen(deptName);
        for (int j = 0; j <= len; j++)
        {
            if (departments[i].name[j] != deptName[j])
            {
                namematch = false;
                break;
            }
        }

        if (namematch == true)
        {
            return i;
        }
    }

    return -1;
}

// 7) Add a new department by name
void addDepartment(char deptName[])
{
    if (departmentCount >= MAX_DEPARTMENTS)
    {
        cout << "Department list is full\n";
        return;
    }
    if (findDepartmentIndexByName(deptName) != -1)
    {
        cout << "Department already exists\n";
        return;
    }
    Department &d = departments[departmentCount];
    strncpy(d.name, deptName, sizeof(d.name) - 1);
    d.name[sizeof(d.name) - 1] = '\0';
    d.employeeCount = 0;
    d.employeeCapacity = 50;
    d.employeeIds = new int[d.employeeCapacity];
    departmentCount++;
}

// 8) Remove a department by name
void removeDepartment(char deptName[])
{
    int idx = findDepartmentIndexByName(deptName);
    if (idx == -1)
    {
        cout << "Department not found\n";
        return;
    }
    else if(departments[idx].employeeIds){
        delete[] departments[idx].employeeIds;
        departments[idx].employeeIds = NULL;
    }
    for (int i = idx; i < departmentCount - 1; i++)
    {
        departments[i] = departments[i + 1];
    }
    departmentCount--;
}

// 9) Find the top performer ID in a given department.
int findTopPerformer(char deptName[])
{
    int di = findDepartmentIndexByName(deptName);
    if (di == -1)
        return -1;
    if (departments[di].employeeCount == 0)
        return -1;
    double best = -1.0; // double is lye use kia h koi k performance bhi double me h or usi k base or best emp decide hona
    int bestid = -1;
    for (int i = 0; i < departments[di].employeeCount; i++)
    {
        int id = departments[di].employeeIds[i];
        for (int e = 0; e < employeeCount; e++)
        {
            if (employees[e].id == id)
            {
                if (employees[e].performance > best)
                {
                    best = employees[e].performance;
                    bestid = id;
                }
                break;
            }
        }
    }
    return bestid;
}

// 10) Display all employees in a given department
void displayDepartment(char deptName[])
{
    int di = findDepartmentIndexByName(deptName);
    if (di == -1)
    {
        cout << "Department not found\n";
        return;
    }
    if (departments[di].employeeCount == 0)
    {
        cout << "No employees in this department\n";
        return;
    }
    for (int i = 0; i < departments[di].employeeCount; i++)
    {
        int id = departments[di].employeeIds[i];
        for (int e = 0; e < employeeCount; e++)
        {
            if (employees[e].id == id)
            {
                cout << "ID: " << employees[e].id << ", Name: " << employees[e].name << ", Performance: " << employees[e].performance << '\n';
                break;
            }
        }
    }
}

// 11) Get average performance of a given department
double getDeptAvgPerformance(char deptName[])
{
    int di = findDepartmentIndexByName(deptName);
    if (di == -1)
        return 0.0;
    if (departments[di].employeeCount == 0)
        return 0.0;
    double sum = 0.0;
    int cnt = 0;
    for (int i = 0; i < departments[di].employeeCount; i++)
    {
        int id = departments[di].employeeIds[i];
        for (int e = 0; e < employeeCount; e++)
        {
            if (employees[e].id == id)
            {
                sum += employees[e].performance;
                cnt++;
                break;
            }
        }
    }
    if (cnt == 0)
        return 0.0;
    return sum / cnt;
}

// FILE HANDLING FOR DEPARTMENTS 

void loadDepartmentsFromText(const char *filename)
{
    ifstream fin(filename);
    departmentCount = 0;
    while (departmentCount < MAX_DEPARTMENTS && fin >> departments[departmentCount].name >> departments[departmentCount].employeeCount)
    {
        int n = departments[departmentCount].employeeCount;
        if (n > 0)
        {
            departments[departmentCount].employeeCapacity = n;
            departments[departmentCount].employeeIds = new int[n];
            for (int i = 0; i < n; i++)
                fin >> departments[departmentCount].employeeIds[i];
        }
        else
            departments[departmentCount].employeeIds = nullptr;
        departmentCount++;
    }
}

void saveDepartmentsToText(const char *filename)
{
    ofstream fout(filename);
    for (int i = 0; i < departmentCount; i++)
    {
        Department &d = departments[i];
        fout << d.name << ' ' << d.employeeCount;
        for (int j = 0; j < d.employeeCount; j++)
            fout << ' ' << d.employeeIds[j];
        fout << '\n';
    }
}
