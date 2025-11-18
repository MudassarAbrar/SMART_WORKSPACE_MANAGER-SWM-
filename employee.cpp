#include "project.h"
#include <iostream>
#include <fstream>

using namespace std;




// ---------------------------------------------------------------------------
// Lookups & validations
// ---------------------------------------------------------------------------

// // Employee list mein given ID ka index dhoondta hai
// static int findEmployeeIndex(int emp_id)
// {
//     for (int i = 0; i < employee_count; i++)
//     {
//         if (employees_list[i].emp_id == emp_id)
//             return i;
//     }
//     return -1;
// }


// Agar department ke andar capacity kam ho, to resize kar do
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

// Employee ko department ke emp_ids array mein add karna
static void addEmployeeToDepartment(int emp_id, const char* dept_name)
{
    int dept_index = findDepartmentIndexByName(dept_name);
    if (dept_index == -1)
    {
        cout << "Warning: Department \"" << dept_name
             << "\" not found. Employee stored without department link.\n";
        return;
    }

// duplicate employee ID mat add karo
    for (int i = 0; i < departments_list[dept_index].emp_count; i++)
    {
        if (departments_list[dept_index].emp_ids[i] == emp_id)
            return;
    }

    ensureDepartmentCapacity(dept_index, departments_list[dept_index].emp_count + 1);
    departments_list[dept_index].emp_ids[departments_list[dept_index].emp_count++] = emp_id;
}
// isme bhi extra condition lgi hui h

// Department se employee ID remove karna (agar exist karta ho)
static void removeEmployeeFromDepartment(int emp_id, const char* dept_name)
{
    int dept_index = findDepartmentIndexByName(dept_name);
    if (dept_index == -1)
        return;

    for (int i = 0; i < departments_list[dept_index].emp_count; i++)
    {
        if (departments_list[dept_index].emp_ids[i] == emp_id)
        {
            // simple left shift technique
            for (int j = i; j < departments_list[dept_index].emp_count - 1; j++)
                departments_list[dept_index].emp_ids[j] = departments_list[dept_index].emp_ids[j + 1];

            departments_list[dept_index].emp_count--;
            return;
        }
    }
}

// ---------------------------------------------------------------------------
// Employee Management
// ---------------------------------------------------------------------------
void addEmployee()
{
    // if (employee_count >= MAX_EMP_PER_DEPT)
    // {
    //     cout << "Employee list is full. Cannot add more employees.\n";
    //     return;
    // }

    Employee record;

    cout << "Enter Employee ID: ";
    cin >> record.emp_id;
    if (cin.fail())
    {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid ID entered.\n";
        return;
    }

    if (findEmployeeIndex(record.emp_id) != -1)
    {
        cout << "Employee ID already exists.\n";
        cin.ignore(1000, '\n');
        return;
    }

    cin.ignore(1000, '\n');

    cout << "Enter Name: ";
    cin.getline(record.emp_name, 50);
    if (stringEmpty(record.emp_name))
    {
        cout << "Name cannot be empty.\n";
        return;
    }

    cout << "Enter Department: ";
    cin.getline(record.dept_name, 30);
    if (stringEmpty(record.dept_name))
    {
        cout << "Department cannot be empty.\n";
        return;
    }

    cout << "Enter Salary: ";
    cin >> record.salary;
    if (cin.fail() || record.salary < 0)
    {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid salary.\n";
        return;
    }
    cin.ignore(1000, '\n');
// default values jab naya employee add ho
    record.attendance      = 0;
    record.performance     = 0;
    record.bonus_eligible  = 0;

    employees_list[employee_count++] = record;
    addEmployeeToDepartment(record.emp_id, record.dept_name);

    cout << "Employee added successfully.\n";
}
void removeEmployee(int emp_id)
{
    ifstream fin("employee.txt");
    if (!fin)
    {
        cout << "Unable to open employee file for reading.\n";
        return;
    }

    ofstream fout("employee_temp.txt");
    if (!fout)
    {
        cout << "Unable to open temporary file for writing.\n";
        fin.close();
        return;
    }

    bool employeeFound = false;
    Employee emp;

    // Read from the original file and write to the temporary file
    while (fin >> emp.emp_id >> emp.emp_name >> emp.dept_name >> emp.salary
               >> emp.attendance >> emp.performance >> emp.bonus_eligible)
    {
        if (emp.emp_id == emp_id)
        {
            // Skip the employee to be removed
            employeeFound = true;
            removeEmployeeFromDepartment(emp.emp_id, emp.dept_name);
            continue;
        }

        // Write all other employees to the temporary file
        fout << emp.emp_id << " "
             << emp.emp_name << " "
             << emp.dept_name << " "
             << emp.salary << " "
             << emp.attendance << " "
             << emp.performance << " "
             << emp.bonus_eligible << "\n";
    }

    fin.close();
    fout.close();

    // Replace the original file with the updated file
    if (employeeFound)
    {
        remove("employee.txt");
        rename("employee_temp.txt", "employee.txt");
        cout << "Employee ID " << emp_id << " removed successfully.\n";
    }
    else
    {
        // If the employee was not found, delete the temporary file
        remove("employee_temp.txt");
        cout << "Employee ID " << emp_id << " not found.\n";
    }
}
void markAttendance(int emp_id)
{
    int idx = findEmployeeIndex(emp_id);
    if (idx == -1)
    {
        cout << "Employee not found.\n";
        return;
    }

    int newAttendance;
    cout << "Enter new attendance (0 - 365): ";
    cin >> newAttendance;
    if (cin.fail() || newAttendance < 0 || newAttendance > 365)
    {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid attendance value.\n";
        return;
    }
    cin.ignore(1000, '\n');

    employees_list[idx].attendance = newAttendance;
    cout << "Attendance updated.\n";
}

void updatePerformance(int emp_id, double score)
{
    int idx = findEmployeeIndex(emp_id);
    if (idx == -1)
    {
        cout << "Employee not found.\n";
        return;
    }

    if (score < 0 || score > 100)
    {
        cout << "Score must be between 0 and 100.\n";
        return;
    }

    employees_list[idx].performance = score;
    cout << "Performance updated.\n";
}

void calculateBonus(int emp_id)
{
    int idx = findEmployeeIndex(emp_id);
    if (idx == -1)
    {
        cout << "Employee not found.\n";
        return;
    }

// Simple rule: high performance + good attendance = bonus
    if (employees_list[idx].performance > 90 && employees_list[idx].attendance > 85)
        employees_list[idx].bonus_eligible = 1;
    else
        employees_list[idx].bonus_eligible = 0;

    cout << (employees_list[idx].bonus_eligible ? "Eligible" : "Not eligible") << '\n';
}

void displayEmployee(int emp_id)
{
    int idx = findEmployeeIndex(emp_id);
    if (idx == -1)
    {
        cout << "Employee not found.\n";
        return;
    }

    Employee& record = employees_list[idx];

    cout << "ID: " << record.emp_id << '\n';
    cout << "Name: " << record.emp_name << '\n';
    cout << "Department: " << record.dept_name << '\n';
    cout << "Salary: " << record.salary << '\n';
    cout << "Attendance: " << record.attendance << '\n';
    cout << "Performance: " << record.performance << '\n';
    cout << "Bonus Eligible: " << (record.bonus_eligible ? "Yes" : "No") << '\n';
}

// ---------------------------------------------------------------------------
// File Handling – Employees (text format)
// Format per line:
// emp_id|emp_name|dept_name|salary|attendance|performance|bonus_eligible
// ---------------------------------------------------------------------------

// // Simple manual parser, taake external libraries use na karni pade
// static void parseEmployeeLine(const char* line, Employee& emp)
// {
//     int  i      = 0;
//     int  field  = 0;
//     char buffer[200];
//     int  bufIdx = 0;

//     while (line[i] != '\0' && field < 7)
//     {
//         if (line[i] == '|')
//         {
//             buffer[bufIdx] = '\0';
//             bufIdx = 0;

//             switch (field)
//             {
//             case 0: // emp_id
//             {
//                 int val = 0;
//                 int j   = 0;
//                 while (buffer[j] != '\0')
//                 {
//                     if (buffer[j] >= '0' && buffer[j] <= '9')
//                         val = val * 10 + (buffer[j] - '0');
//                     j++;
//                 }
//                 emp.emp_id = val;
//                 break;
//             }
//             case 1: // emp_name
//                 copyString(emp.emp_name, buffer, 50);
//                 break;
//             case 2: // dept_name
//                 copyString(emp.dept_name, buffer, 30);
//                 break;
//             case 3: // salary
//             {
//                 double val = 0.0;
//                 int    j   = 0;
//                 bool   neg = false;

//                 if (buffer[0] == '-')
//                 {
//                     neg = true;
//                     j   = 1;
//                 }

//                 bool   afterDot = false;
//                 double divisor  = 1.0;

//                 while (buffer[j] != '\0')
//                 {
//                     if (buffer[j] == '.')
//                     {
//                         afterDot = true;
//                     }
//                     else if (buffer[j] >= '0' && buffer[j] <= '9')
//                     {
//                         if (afterDot)
//                         {
//                             divisor *= 10.0;
//                             val     = val + (buffer[j] - '0') / divisor;
//                         }
//                         else
//                         {
//                             val = val * 10.0 + (buffer[j] - '0');
//                         }
//                     }
//                     j++;
//                 }
//                 emp.salary = neg ? -val : val;
//                 break;
//             }
//             case 4: // attendance
//             {
//                 int val = 0;
//                 int j   = 0;
//                 while (buffer[j] != '\0')
//                 {
//                     if (buffer[j] >= '0' && buffer[j] <= '9')
//                         val = val * 10 + (buffer[j] - '0');
//                     j++;
//                 }
//                 emp.attendance = val;
//                 break;
//             }
//             case 5: // performance
//             {
//                 double val = 0.0;
//                 int    j   = 0;
//                 bool   neg = false;

//                 if (buffer[0] == '-')
//                 {
//                     neg = true;
//                     j   = 1;
//                 }

//                 bool   afterDot = false;
//                 double divisor  = 1.0;

//                 while (buffer[j] != '\0')
//                 {
//                     if (buffer[j] == '.')
//                     {
//                         afterDot = true;
//                     }
//                     else if (buffer[j] >= '0' && buffer[j] <= '9')
//                     {
//                         if (afterDot)
//                         {
//                             divisor *= 10.0;
//                             val     = val + (buffer[j] - '0') / divisor;
//                         }
//                         else
//                         {
//                             val = val * 10.0 + (buffer[j] - '0');
//                         }
//                     }
//                     j++;
//                 }
//                 emp.performance = neg ? -val : val;
//                 break;
//             }
//             case 6: // bonus_eligible
//             {
//                 int val = 0;
//                 if (buffer[0] == '1')
//                     val = 1;
//                 emp.bonus_eligible = val;
//                 break;
//             }
//             }
//             field++;
//         }
//         else
//         {
//             if (bufIdx < 199)
//                 buffer[bufIdx++] = line[i];
//         }
//         i++;
//     }
// }

void loadEmployeesFromText(const char* filename)
{
    ifstream fin(filename);
    employee_count = 0;

    if (!fin)
        return;

    while (employee_count < MAX_EMPLOYEES)
    {
        Employee& emp = employees_list[employee_count];

        // order: id, name, dept, salary, attendance, performance, bonus
        fin >> emp.emp_id
            >> emp.emp_name
            >> emp.dept_name
            >> emp.salary
            >> emp.attendance
            >> emp.performance
            >> emp.bonus_eligible;

        if (fin.fail())
            break;   // agar read fail hua (EOF ya koi issue) to loop se nikal jao

       // Check if the department exists
        if (findDepartmentIndexByName(emp.dept_name) == -1)
        {
            cout << "Warning: Department \"" << emp.dept_name
                 << "\" not found. Skipping employee ID " << emp.emp_id << ".\n";
            continue; // Skip this employee
        }

        // Add the employee to the list
        employees_list[employee_count++] = emp;

        // Add the employee to the department
        addEmployeeToDepartment(emp.emp_id, emp.dept_name);
    }
}

void saveEmployeesToText(const char* filename)
{
    ofstream fout(filename);
    if (!fout)
    {
        cout << "Unable to open employee file for writing.\n";
        return;
    }

    for (int i = 0; i < employee_count; i++)
    {
        Employee& emp = employees_list[i];
        fout << emp.emp_id      << ' '
             << emp.emp_name    << ' '
             << emp.dept_name   << ' '
             << emp.salary      << ' '
             << emp.attendance  << ' '
             << emp.performance << ' '
             << emp.bonus_eligible << '\n';
    }
}
