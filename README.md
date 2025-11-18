----------------------Smart Workspace Manager----------------------

A modular C++ console application designed to manage employees, departments, and meetings within an organization.
The system demonstrates dynamic memory allocation, structured project architecture, file persistence, and multi-module program design.

- Table of Contents

1. Overview

2. Key Features

3. Project Structure

4. Data Flow & Architecture

5. Build Instructions

6. Running the Program

7. Data File Formats

8. Module Responsibilities

- Overview

Smart Workspace Manager is a C++ application developed to simulate a real-world workplace management system.
It relies on dynamic memory and modular code architecture to efficiently handle employees, departments, and meetings.
All system data is persisted to simple text files for reusability across sessions.

- Key Features

> Employee Management

Add and remove employees

Update attendance and performance

Determine bonus eligibility

View complete employee details

Auto-link employee to respective department

> Department Management

Create and delete departments

Store and track employee IDs

Display full department information

Identify top performer

Compute department average performance

Check capacity and utilization

> Meeting Management

Create meetings with topic, date, and time

Ensure no scheduling clashes

Add participants by employee ID

Display meeting details and participant list

- Data Persistence

Data is saved automatically into:

employee.txt

departments.txt

meetings.txt

Data reloads whenever the program starts.

- Project Structure

.
├── main.cpp // Menu system and program flow
├── project.h // Structs, constants, global declarations
├── global.cpp // Memory allocation + global variable definitions
├── employee.cpp // Employee management logic
├── department.cpp // Department operations logic
├── meeting.cpp // Meeting scheduling and participant handling
│
├── employee.txt // Employee data storage
├── departments.txt // Department data storage
├── meetings.txt // Meeting data storage
│
└── README.md // Project documentation

- Data Flow & Architecture

1. Global Resources (global.cpp)

Allocates dynamic arrays for employees, departments, meetings

Stores counters:

employee_count

department_count

meeting_count

Implements:

initializeMemory()

freeMemory()

2. Core Modules

File Responsibility

employee.cpp Employee creation, deletion, updates, and file I/O
department.cpp Department creation, capacity checks, metric calculations
meeting.cpp Meeting creation, clash detection, participant management
main.cpp User-facing menu and program navigation

3. Struct & Header Definitions (project.h)

Describes:

Employee, Department, Meeting structures

Global pointer declarations

Function prototypes shared across modules

- Build Instructions

> Linux / macOS

g++ main.cpp global.cpp employee.cpp department.cpp meeting.cpp -o workspace_manager

> Windows (TDM-GCC / MinGW)

g++ main.cpp global.cpp employee.cpp department.cpp meeting.cpp -o manager.exe

Running the Program

> Linux / macOS

./workspace_manager

Windows

manager.exe

Data File Formats

employee.txt

emp_id emp_name dept_name salary attendance performance bonus_eligible

departments.txt

dept_name employee_count emp_id_1 emp_id_2 ...

meetings.txt

meet_id topic date time participant_count emp_id_1 emp_id_2 ...

- Module Responsibilities

> employee.cpp

Create and delete employees

Manage attendance, performance, and bonus

Link employees to departments

Load and save employee data to text file

> department.cpp

Add and remove departments

Maintain employee ID list for each department

Resize department capacity dynamically

Identify top performer

Compute average department performance

Load and save department data

> meeting.cpp

Create meetings with schedule validation

Prevent time/date clashes

Add participants

Display meeting details

Manage meeting data persistence

> main.cpp

Master flow of the program

Menu-driven interface

Input handling and delegation to modules

- Team Members & Work Division

Group Members:

Name: Mudassir Abrar Baig
1- Roll No.: BCF23M035
Responsibilities :
Developed the Department Module (department.cpp), capacity handling, top performer, average performance logic, and file I/O for departments.

2 - Name: Abdul Rahman Bin Abid
Roll No.: BCSF24M021
Responsibilities : Implemented the Meeting Module (meeting.cpp), including meeting creation, validation, participant handling, and clash detection. Responsible for formatting and organizing project documentation.

3- Name: Zainab Irfan
Roll No.: BCSF24M013
Responsibilities :
Built the Employee Module (employee.cpp), including employee CRUD operations, bonus logic, and linking employees to departments. Also designed menu navigation in main.cpp.

Work Strategy
Each member took ownership of one core module.
All modules were integrated through shared header files and global dynamic memory.
Team collaborated on error handling, naming conventions, and file storage formats.
Final testing, debugging, and documentation were completed jointly.
