#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

using namespace std;

// Student structure
struct Student {
    string name;
    string rollNumber;
    int semesterNumber;
    int marks[6];  // Assuming 6 subjects
    float percentage; // Percentage of total marks
    char grade; // Grade based on percentage
};

// Constants for file handling
const char FILENAME[] = "student_records.txt";

// Function prototypes
void displayMainMenu();
void addNewStudent();
void displayIndividualMarksSheet();
void deleteStudent();
void updateStudentRecord();
void displayAllStudentsRecords();
bool isRollNumberExists(const string& rollNumber);
char calculateGrade(float percentage);

// Function to display the main menu
void displayMainMenu() {
    cout << "-----------------------------------------------------" << endl;
    cout << "        HITEC University Taxila" << endl;
    cout << "Student Marks Sheet Management System" << endl;
    cout << "-----------------------------------------------------" << endl;
    cout << "Enter the choice:" << endl;
    cout << "1. Enter new student data" << endl;
    cout << "2. Display Individual student marks sheet" << endl;
    cout << "3. Delete student" << endl;
    cout << "4. Update an individual student record" << endl;
    cout << "5. Show all students records" << endl;
    cout << "6. Exit" << endl;
    cout << "-----------------------------------------------------" << endl;
}

// Function to add new student data
void addNewStudent() {
    Student s;
    cout << "\nEnter new student data:\n";
    cout << "Name: ";
    cin.ignore(); // Clear the input buffer
    getline(cin, s.name);

    cout << "Roll number: ";
    cin >> s.rollNumber;

    // Check if the roll number already exists
    if (isRollNumberExists(s.rollNumber)) {
        cout << "Roll number already exists. Please enter a unique roll number.\n";
        return;
    }

    cout << "Semester number: ";
    cin >> s.semesterNumber;

    cout << "Enter marks for 6 subjects:\n";
    for (int i = 0; i < 6; ++i) {
        cout << "Subject " << (i + 1) << " marks: ";
        cin >> s.marks[i];
    }

    // Calculate percentage and assign grade
    int totalMarks = 0;
    for (int i = 0; i < 6; ++i) {
        totalMarks += s.marks[i];
    }
    s.percentage = static_cast<float>(totalMarks) / 6.0;
    s.grade = calculateGrade(s.percentage);

    ofstream outFile(FILENAME, ios::app);
    outFile << s.name << " " << s.rollNumber << " " << s.semesterNumber;
    for (int i = 0; i < 6; ++i) {
        outFile << " " << setw(2) << s.marks[i];
    }
    outFile << " " << fixed << setprecision(2) << s.percentage << " " << s.grade;
    outFile << endl;

    cout << "Student added successfully.\n";
}

// Function to display individual student marks sheet
void displayIndividualMarksSheet() {
    string rollNumber;
    cout << "Enter the roll number of the student: ";
    cin >> rollNumber;

    ifstream inFile(FILENAME);

    if (!inFile) {
        cout << "Error opening file.\n";
        return;
    }

    Student tempStudent;
    bool found = false;

    while (inFile >> tempStudent.name >> tempStudent.rollNumber >> tempStudent.semesterNumber) {
        if (tempStudent.rollNumber == rollNumber) {
            found = true;
            cout << "\nDisplaying marks for student with Roll Number " << rollNumber << ":\n";
            cout << "Name: " << tempStudent.name << "\n";
            cout << "Roll Number: " << tempStudent.rollNumber << "\n";
            cout << "Semester Number: " << tempStudent.semesterNumber << "\n";
            cout << "Marks for 6 subjects:\n";
            for (int i = 0; i < 6; ++i) {
                inFile >> tempStudent.marks[i];
                cout << "Subject " << (i + 1) << " marks: " << tempStudent.marks[i] << "\n";
            }
            cout << "Percentage: " << fixed << setprecision(2) << tempStudent.percentage << "%\n";
            cout << "Grade: " << tempStudent.grade << "\n";
            break;
        }
    }

    inFile.close();

    if (!found) {
        cout << "\nStudent with Roll Number " << rollNumber << " not found.\n";
    }
}

// Function to delete a student
void deleteStudent() {
    string rollNumber;
    cout << "Enter the roll number of the student to delete: ";
    cin >> rollNumber;

    ifstream inFile(FILENAME);
    ofstream outFile("temp.txt");

    Student tempStudent;
    bool found = false;

    while (inFile >> tempStudent.name >> tempStudent.rollNumber >> tempStudent.semesterNumber) {
        if (tempStudent.rollNumber == rollNumber) {
            found = true;
            cout << "Student with Roll Number " << rollNumber << " deleted successfully.\n";
            continue; // Skip writing the deleted student to temp.txt
        }

        outFile << tempStudent.name << " " << tempStudent.rollNumber << " " << tempStudent.semesterNumber;
        for (int i = 0; i < 6; ++i) {
            outFile << " " << tempStudent.marks[i];
        }
        outFile << " " << fixed << setprecision(2) << tempStudent.percentage << " " << tempStudent.grade;
        outFile << endl;
    }

    inFile.close();
    outFile.close();

    if (found) {
        remove(FILENAME);
        rename("temp.txt", FILENAME);
    } else {
        remove("temp.txt");
    }
}

// Function to update an individual student record
void updateStudentRecord() {
    string rollNumber;
    cout << "Enter the roll number of the student to update: ";
    cin >> rollNumber;

    ifstream inFile(FILENAME);
    ofstream outFile("temp.txt");

    Student tempStudent;
    bool found = false;

    while (inFile >> tempStudent.name >> tempStudent.rollNumber >> tempStudent.semesterNumber) {
        if (tempStudent.rollNumber == rollNumber) {
            found = true;
            cout << "Enter updated information for student with Roll Number " << rollNumber << ":\n";
            cout << "Name: ";
            cin.ignore(); // Clear the input buffer
            getline(cin, tempStudent.name);

            cout << "Semester number: ";
            cin >> tempStudent.semesterNumber;

            cout << "Enter updated marks for 6 subjects:\n";
            for (int i = 0; i < 6; ++i) {
                cout << "Subject " << (i + 1) << " marks: ";
                cin >> tempStudent.marks[i];
            }

            // Calculate percentage and assign grade
            int totalMarks = 0;
            for (int i = 0; i < 6; ++i) {
                totalMarks += tempStudent.marks[i];
            }
            tempStudent.percentage = static_cast<float>(totalMarks) / 6.0;
            tempStudent.grade = calculateGrade(tempStudent.percentage);

            // Break out of the loop after updating the record
            break;
        }

        outFile << tempStudent.name << " " << tempStudent.rollNumber << " " << tempStudent.semesterNumber;
        for (int i = 0; i < 6; ++i) {
            outFile << " " << tempStudent.marks[i];
        }
        outFile << " " << fixed << setprecision(2) << tempStudent.percentage << " " << tempStudent.grade;
        outFile << endl;
    }

    inFile.close();
    outFile.close();

    if (found) {
        remove(FILENAME);
        rename("temp.txt", FILENAME);
        cout << "Student with Roll Number " << rollNumber << " updated successfully.\n";
    } else {
        remove("temp.txt");
        cout << "Student with Roll Number " << rollNumber << " not found.\n";
    }
}

// Function to display all students records
void displayAllStudentsRecords() {
    ifstream inFile(FILENAME);

    if (!inFile) {
        cout << "Error opening file.\n";
        return;
    }

    Student tempStudent;

    cout << "\nDisplaying all students records:\n";

    while (inFile >> tempStudent.name >> tempStudent.rollNumber >> tempStudent.semesterNumber) {
        cout << "---------------------------------------------\n";
        cout << "Name: " << tempStudent.name << "\n";
        cout << "Roll Number: " << tempStudent.rollNumber << "\n";
        cout << "Semester Number: " << tempStudent.semesterNumber << "\n";
        cout << "Marks for 6 subjects:\n";
        for (int i = 0; i < 6; ++i) {
            inFile >> tempStudent.marks[i];
            cout << "Subject " << (i + 1) << " marks: " << tempStudent.marks[i] << "\n";
        }
        cout << "Percentage: " << fixed << setprecision(2) << tempStudent.percentage << "%\n";
        cout << "Grade: " << tempStudent.grade << "\n";
    }

    inFile.close();
}

// Function to check if roll number already exists
bool isRollNumberExists(const string& rollNumber) {
    ifstream inFile(FILENAME);

    if (!inFile) {
        cout << "Error opening file.\n";
        return false; // Assume the roll number doesn't exist in case of an error
    }

    Student tempStudent;

    while (inFile >> tempStudent.name >> tempStudent.rollNumber >> tempStudent.semesterNumber) {
        if (tempStudent.rollNumber == rollNumber) {
            inFile.close();
            return true; // Roll number found
        }
    }

    inFile.close();
    return false; // Roll number not found
}

// Function to calculate grade based on percentage
char calculateGrade(float percentage) {
    if (percentage >= 90) {
        return 'A';
    } else if (percentage >= 85) {
        return 'A-';
    } else if (percentage >= 80) {
        return 'B+';
    } else if (percentage >= 75) {
        return 'B';
    } else if (percentage >= 70) {
        return 'B-';
    } else if (percentage >= 65) {
        return 'C+';
    } else if (percentage >= 60) {
        return 'C';
    } else if (percentage >= 55) {
        return 'C-';
    } else if (percentage >= 50) {
        return 'D';
    } else {
        return 'F';
    }
}

int main() {
    int choice;

    do {
        displayMainMenu();
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                addNewStudent();
                break;
            case 2:
                displayIndividualMarksSheet();
                break;
            case 3:
                deleteStudent();
                break;
            case 4:
                updateStudentRecord();
                break;
            case 5:
                displayAllStudentsRecords();
                break;
            case 6:
                cout << "Exiting the program. Thank you!" << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }

    } while (choice != 6);

    return 0;
}

