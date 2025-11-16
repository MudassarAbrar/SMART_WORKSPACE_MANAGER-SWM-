#include "project.h"
#include <iostream>
#include <fstream>
using namespace std;

// 1) Add a new employee (take input inside the function)
void addEmployee()
{
    if (employeeCount >= MAX_EMPLOYEES)
    {
        cout << "Employee list is full\n";
        return;
    }
    Employee e;
    cout << "Enter Employee ID: ";
    cin >> e.id;
    cin.ignore(1000, '\n');
    cout << "Enter Name: ";
    cin.getline(e.name, 50);
    cout << "Enter Department: ";
    cin.getline(e.department, 30);
    cout << "Enter Salary: ";
    cin >> e.salary;
    cin.ignore(1000, '\n');
    e.attendance = 0;
    e.performance = 0;
    e.bonusEligible = 0;
    employees[employeeCount] = e;
    employeeCount++;
    cout << "Employee added\n";
}

// 2) Remove an employee by ID
void removeEmployee(int id)
{
    int idx = -1;
    for (int i = 0; i < employeeCount; i++)
        if (employees[i].id == id)
        {
            idx = i;
            break;
        }
    if (idx == -1)
    {
        cout << "Employee not found\n";
        return;
    }
    for (int i = idx; i < employeeCount - 1; i++)
        employees[i] = employees[i + 1];
    employeeCount--;
    cout << "Employee removed\n";
}

// 3) Mark / update attendance for a specific employee ID
void markAttendance(int id)
{
    for (int i = 0; i < employeeCount; i++)
    {
        if (employees[i].id == id)
        {
            int a;
            cout << "Enter new attendance: ";
            cin >> a;
            cin.ignore(1000, '\n');
            employees[i].attendance = a;
            cout << "Attendance updated\n";
            return;
        }
    }
    cout << "Employee not found\n";
}

// 4) Update performance score for a specific employee ID
void updatePerformance(int id, double score)
{
    for (int i = 0; i < employeeCount; i++)
    {
        if (employees[i].id == id)
        {
            employees[i].performance = score;
            cout << "Performance updated\n";
            return;
        }
    }
    cout << "Employee not found\n";
}

// 5) Calculate bonus eligibility for a specific employee ID
//  hamara critera h    performance > 90 and attendance > 85
void calculateBonus(int id)
{
    for (int i = 0; i < employeeCount; i++)
    {
        if (employees[i].id == id)
        {
            if (employees[i].performance > 90 && employees[i].attendance > 85)
                employees[i].bonusEligible = 1;
            else
                employees[i].bonusEligible = 0;
            cout << (employees[i].bonusEligible ? "Eligible" : "Not eligible") << '\n';
            return;
        }
    }
    cout << "Employee not found\n";
}

// 6) Display full details for one employee by ID
void displayEmployee(int id)
{
    for (int i = 0; i < employeeCount; i++)
    {
        if (employees[i].id == id)
        {
            cout << "ID: " << employees[i].id << '\n';
            cout << "Name: " << employees[i].name << '\n';
            cout << "Department: " << employees[i].department << '\n';
            cout << "Salary: " << employees[i].salary << '\n';
            cout << "Attendance: " << employees[i].attendance << '\n';
            cout << "Performance: " << employees[i].performance << '\n';
            cout << "Bonus: " << (employees[i].bonusEligible ? "Yes" : "No") << '\n';
            return;
        }
    }
    cout << "Employee not found\n";
}

// FILE HANDLING – EMPLOYEES (BINARY FILE)

void loadEmployeesFromBinary(const char *filename)
{
    ifstream fin(filename, ios::binary);
    if (!fin)
        return;
    int cnt = 0;
    fin.read((char *)&cnt, sizeof(int));
    if (cnt < 0)
        cnt = 0;
    if (cnt > MAX_EMPLOYEES)
        cnt = MAX_EMPLOYEES;
    fin.read((char *)employees, cnt * sizeof(Employee));
    if (fin)
        employeeCount = cnt;
    else
        employeeCount = 0;
    fin.close();
}

void saveEmployeesToBinary(const char *filename)
{
    ofstream fout(filename, ios::binary);
    if (!fout)
        return;
    int cnt = employeeCount;
    fout.write((char *)&cnt, sizeof(int));
    if (cnt > 0)
        fout.write((char *)employees, cnt * sizeof(Employee));
    fout.close();
}
