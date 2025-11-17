#include "project.h"
#include <iostream>
#include <fstream>

using namespace std;

static void copyString(char* destination, const char* source, int destinationSize)
{
    if (destinationSize <= 0)
        return;
    int i = 0;
    while (i < destinationSize - 1 && source[i] != '\0')
    {
        destination[i] = source[i];
        i++;
    }
    destination[i] = '\0';
}

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

static bool stringEmpty(const char* text)
{
    return text[0] == '\0';
}

// ---------------------------------------------------------------------------
// Lookups & validations
// ---------------------------------------------------------------------------
static int findEmployeeIndex(int emp_id)
{
    for (int i = 0; i < employee_count; i++)
    {
        if (employees_list[i].emp_id == emp_id)
            return i;
    }
    return -1;
}

static int findDepartmentIndex(const char* dept_name)
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

static void addEmployeeToDepartment(int emp_id, const char* dept_name)
{
    int dept_index = findDepartmentIndex(dept_name);
    if (dept_index == -1)
    {
        cout << "Warning: Department \"" << dept_name << "\" not found. Employee stored without department link.\n";
        return;
    }

    // prevent duplicates
    for (int i = 0; i < departments_list[dept_index].emp_count; i++)
    {
        if (departments_list[dept_index].emp_ids[i] == emp_id)
            return;
    }

    ensureDepartmentCapacity(dept_index, departments_list[dept_index].emp_count + 1);
    departments_list[dept_index].emp_ids[departments_list[dept_index].emp_count++] = emp_id;
}

static void removeEmployeeFromDepartment(int emp_id, const char* dept_name)
{
    int dept_index = findDepartmentIndex(dept_name);
    if (dept_index == -1)
        return;

    for (int i = 0; i < departments_list[dept_index].emp_count; i++)
    {
        if (departments_list[dept_index].emp_ids[i] == emp_id)
        {
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
    if (employee_count >= MAX_EMPLOYEES)
    {
        cout << "Employee list is full. Cannot add more employees.\n";
        return;
    }

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

    record.attendance = 0;
    record.performance = 0;
    record.bonus_eligible = 0;

    employees_list[employee_count++] = record;
    addEmployeeToDepartment(record.emp_id, record.dept_name);
    cout << "Employee added successfully.\n";
}

void removeEmployee(int emp_id)
{
    int idx = findEmployeeIndex(emp_id);
    if (idx == -1)
    {
        cout << "Employee not found.\n";
        return;
    }

    removeEmployeeFromDepartment(emp_id, employees_list[idx].dept_name);

    for (int i = idx; i < employee_count - 1; i++)
        employees_list[i] = employees_list[i + 1];
    employee_count--;

    cout << "Employee removed successfully.\n";
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
// File Handling – Employees (binary)
// ---------------------------------------------------------------------------
void loadEmployeesFromBinary(const char* filename)
{
    ifstream fin(filename, ios::binary);
    employee_count = 0;
    if (!fin)
        return;

    int stored_count = 0;
    fin.read(reinterpret_cast<char*>(&stored_count), sizeof(int));
    if (!fin)
        return;

    if (stored_count < 0)
        stored_count = 0;
    if (stored_count > MAX_EMPLOYEES)
        stored_count = MAX_EMPLOYEES;

    fin.read(reinterpret_cast<char*>(employees_list), stored_count * sizeof(Employee));
    if (!fin)
    {
        employee_count = 0;
        return;
    }

    employee_count = stored_count;
}

void saveEmployeesToBinary(const char* filename)
{
    ofstream fout(filename, ios::binary);
    if (!fout)
    {
        cout << "Unable to open employee file for writing.\n";
        return;
    }

    fout.write(reinterpret_cast<const char*>(&employee_count), sizeof(int));
    if (employee_count > 0)
        fout.write(reinterpret_cast<const char*>(employees_list), employee_count * sizeof(Employee));
}
