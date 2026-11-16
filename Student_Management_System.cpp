#include <iostream>
#include <fstream>
#include <iomanip>
#include <limits>
#include <cstdio>
using namespace std;

struct Student {
    int rollNo;
    char name[50];
    char course[30];
    float marks;
};

const char* FILE_NAME = "students.dat";

// Check whether a student with this roll number already exists.
bool rollNoExists(int rollNo) {
    ifstream file(FILE_NAME, ios::binary);
    Student s;

    while (file.read(reinterpret_cast<char*>(&s), sizeof(s))) {
        if (s.rollNo == rollNo) {
            return true;
        }
    }
    return false;
}

void addStudent() {
    Student s{};

    cout << "\nEnter Roll Number: ";
    cin >> s.rollNo;

    if (rollNoExists(s.rollNo)) {
        cout << "A student with this roll number already exists.\n";
        return;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter Name: ";
    cin.getline(s.name, sizeof(s.name));

    cout << "Enter Course: ";
    cin.getline(s.course, sizeof(s.course));

    cout << "Enter Marks: ";
    cin >> s.marks;

    if (s.marks < 0 || s.marks > 100) {
        cout << "Marks must be between 0 and 100.\n";
        return;
    }

    ofstream file(FILE_NAME, ios::binary | ios::app);
    if (!file) {
        cout << "Unable to open the file.\n";
        return;
    }

    file.write(reinterpret_cast<const char*>(&s), sizeof(s));
    cout << "Student record added successfully.\n";
}

void displayStudents() {
    ifstream file(FILE_NAME, ios::binary);
    Student s;
    bool found = false;

    if (!file) {
        cout << "\nNo student records found yet.\n";
        return;
    }

    cout << "\n---------------- STUDENT RECORDS ----------------\n";
    cout << left << setw(12) << "Roll No"
         << setw(25) << "Name"
         << setw(18) << "Course"
         << setw(10) << "Marks" << '\n';
    cout << string(65, '-') << '\n';

    while (file.read(reinterpret_cast<char*>(&s), sizeof(s))) {
        found = true;
        cout << left << setw(12) << s.rollNo
             << setw(25) << s.name
             << setw(18) << s.course
             << fixed << setprecision(2) << s.marks << '\n';
    }

    if (!found) {
        cout << "No student records found.\n";
    }
}

void searchStudent() {
    int rollNo;
    cout << "\nEnter Roll Number to search: ";
    cin >> rollNo;

    ifstream file(FILE_NAME, ios::binary);
    Student s;
    bool found = false;

    while (file.read(reinterpret_cast<char*>(&s), sizeof(s))) {
        if (s.rollNo == rollNo) {
            cout << "\nStudent found:\n";
            cout << "Roll Number: " << s.rollNo << '\n';
            cout << "Name       : " << s.name << '\n';
            cout << "Course     : " << s.course << '\n';
            cout << "Marks      : " << fixed << setprecision(2) << s.marks << '\n';
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "Student record not found.\n";
    }
}

void updateStudent() {
    int rollNo;
    cout << "\nEnter Roll Number to update: ";
    cin >> rollNo;

    fstream file(FILE_NAME, ios::binary | ios::in | ios::out);
    if (!file) {
        cout << "No student records found.\n";
        return;
    }

    Student s;
    bool found = false;

    while (file.read(reinterpret_cast<char*>(&s), sizeof(s))) {
        if (s.rollNo == rollNo) {
            cout << "Current Name: " << s.name << '\n';
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "Enter New Name: ";
            cin.getline(s.name, sizeof(s.name));

            cout << "Enter New Course: ";
            cin.getline(s.course, sizeof(s.course));

            cout << "Enter New Marks: ";
            cin >> s.marks;

            if (s.marks < 0 || s.marks > 100) {
                cout << "Invalid marks. Update cancelled.\n";
                return;
            }

            // Move the write pointer back to the beginning of this record.
            file.seekp(-static_cast<streamoff>(sizeof(s)), ios::cur);
            file.write(reinterpret_cast<const char*>(&s), sizeof(s));

            cout << "Student record updated successfully.\n";
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "Student record not found.\n";
    }
}

void deleteStudent() {
    int rollNo;
    cout << "\nEnter Roll Number to delete: ";
    cin >> rollNo;

    ifstream inFile(FILE_NAME, ios::binary);
    if (!inFile) {
        cout << "No student records found.\n";
        return;
    }

    ofstream outFile("temp.dat", ios::binary);
    Student s;
    bool found = false;

    while (inFile.read(reinterpret_cast<char*>(&s), sizeof(s))) {
        if (s.rollNo == rollNo) {
            found = true; // Skip this record.
        } else {
            outFile.write(reinterpret_cast<const char*>(&s), sizeof(s));
        }
    }

    inFile.close();
    outFile.close();

    if (found) {
        if (std::remove(FILE_NAME) != 0 || std::rename("temp.dat", FILE_NAME) != 0) {
            cout << "Could not replace the data file. Keep temp.dat as a backup.\n";
            return;
        }
        cout << "Student record deleted successfully.\n";
    } else {
        std::remove("temp.dat");
        cout << "Student record not found.\n";
    }
}

int main() {
    int choice;

    do {
        cout << "\n========== STUDENT MANAGEMENT SYSTEM ==========\n";
        cout << "1. Add Student\n";
        cout << "2. Display All Students\n";
        cout << "3. Search Student\n";
        cout << "4. Update Student\n";
        cout << "5. Delete Student\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";

        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 0: cout << "Thank you for using the Student Management System.\n"; break;
            default: cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 0);

    return 0;
}
