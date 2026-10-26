
// ProjectTwo.cpp
// Author: Andrew Brannan
// Course: CS-300: Data Structures and Algorithms
// Date: October 18, 2026
// Description: ABCU Academic Advising Course Planner implemented with a
//              Binary Search Tree (BST) to load, sort, and display courses.

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// Structure to hold individual course information
struct Course {
    string courseNumber;
    string courseTitle;
    vector<string> prerequisites;
};

// Internal node structure for the Binary Search Tree
struct Node {
    Course course;
    Node* left;
    Node* right;

    Node() {
        left = nullptr;
        right = nullptr;
    }

    Node(Course aCourse) : Node() {
        course = aCourse;
    }
};

// Binary Search Tree class definition
class BinarySearchTree {
private:
    Node* root;

    void addNode(Node* node, Course course);
    void inOrder(Node* node);
    void destroyRecursive(Node* node);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void Insert(Course course);
    void InOrder();
    Course Search(string courseNumber);
    bool IsEmpty();
};

// Default constructor
BinarySearchTree::BinarySearchTree() {
    root = nullptr;
}

// Destructor to free dynamically allocated tree nodes
BinarySearchTree::~BinarySearchTree() {
    destroyRecursive(root);
}

// Helper method to recursively clean up memory
void BinarySearchTree::destroyRecursive(Node* node) {
    if (node != nullptr) {
        destroyRecursive(node->left);
        destroyRecursive(node->right);
        delete node;
    }
}

// Check whether the tree currently holds any nodes
bool BinarySearchTree::IsEmpty() {
    return root == nullptr;
}

// Insert a course into the tree
void BinarySearchTree::Insert(Course course) {
    if (root == nullptr) {
        root = new Node(course);
    } else {
        addNode(root, course);
    }
}

// Helper method to insert a node based on alphanumeric ordering
void BinarySearchTree::addNode(Node* node, Course course) {
    if (course.courseNumber < node->course.courseNumber) {
        if (node->left == nullptr) {
            node->left = new Node(course);
        } else {
            addNode(node->left, course);
        }
    } else {
        if (node->right == nullptr) {
            node->right = new Node(course);
        } else {
            addNode(node->right, course);
        }
    }
}

// Public method to start alphanumeric in-order traversal
void BinarySearchTree::InOrder() {
    inOrder(root);
}

// Helper method to traverse tree in-order (Left, Root, Right)
void BinarySearchTree::inOrder(Node* node) {
    if (node != nullptr) {
        inOrder(node->left);
        cout << node->course.courseNumber << ", " << node->course.courseTitle << endl;
        inOrder(node->right);
    }
}

// Search for a specific course by its course number
Course BinarySearchTree::Search(string courseNumber) {
    Node* current = root;

    while (current != nullptr) {
        if (current->course.courseNumber == courseNumber) {
            return current->course;
        }
        if (courseNumber < current->course.courseNumber) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    Course emptyCourse;
    emptyCourse.courseNumber = "";
    return emptyCourse;
}

// Helper function to trim whitespace and carriage returns
string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

// Helper function to convert a string to uppercase for case-insensitive search
string toUpperCase(string str) {
    transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

// Function to load courses from a CSV file into the BST
bool loadCourses(string csvPath, BinarySearchTree* bst) {
    ifstream file(csvPath);

    if (!file.is_open()) {
        cout << "Error: Could not open file " << csvPath << endl;
        return false;
    }

    string line;
    int lineCount = 0;

    while (getline(file, line)) {
        line = trim(line);
        if (line.empty()) {
            continue;
        }

        stringstream ss(line);
        string token;
        vector<string> tokens;

        while (getline(ss, token, ',')) {
            token = trim(token);
            if (!token.empty()) {
                tokens.push_back(token);
            }
        }

        // Validate that line contains at least courseNumber and courseTitle
        if (tokens.size() < 2) {
            cout << "File Format Error: Line " << (lineCount + 1) << " does not contain required fields." << endl;
            file.close();
            return false;
        }

        Course course;
        course.courseNumber = tokens[0];
        course.courseTitle = tokens[1];

        for (size_t i = 2; i < tokens.size(); ++i) {
            course.prerequisites.push_back(tokens[i]);
        }

        bst->Insert(course);
        lineCount++;
    }

    file.close();
    return true;
}

// Function to display detailed information for a specific course
void printCourseInformation(BinarySearchTree* bst, string courseNumber) {
    Course course = bst->Search(courseNumber);

    if (course.courseNumber.empty()) {
        cout << "Course " << courseNumber << " not found." << endl;
        return;
    }

    cout << course.courseNumber << ", " << course.courseTitle << endl;

    if (course.prerequisites.empty()) {
        cout << "Prerequisites: None" << endl;
    } else {
        cout << "Prerequisites: ";
        for (size_t i = 0; i < course.prerequisites.size(); ++i) {
            cout << course.prerequisites[i];
            if (i < course.prerequisites.size() - 1) {
                cout << ", ";
            }
        }
        cout << endl;
    }
}

int main() {
    BinarySearchTree* bst = new BinarySearchTree();
    string userInput = "";
    int choice = 0;

    cout << "Welcome to the course planner." << endl;

    while (choice != 9) {
        cout << endl;
        cout << "1. Load Data Structure." << endl;
        cout << "2. Print Course List." << endl;
        cout << "3. Print Course." << endl;
        cout << "9. Exit" << endl;
        cout << endl;
        cout << "What would you like to do? ";

        if (!getline(cin, userInput)) {
            break;
        }

        userInput = trim(userInput);

        if (userInput.empty()) {
            continue;
        }

        stringstream stream(userInput);
        if (!(stream >> choice) || !stream.eof()) {
            cout << userInput << " is not a valid option." << endl;
            choice = 0;
            continue;
        }

        switch (choice) {
            case 1: {
                string csvPath = "";
                cout << "Enter the file name containing the course data: ";
                getline(cin, csvPath);
                csvPath = trim(csvPath);

                if (csvPath.empty()) {
                    csvPath = "ABCU_Advising_Program_Input.csv";
                }

                // Reset tree in case a file was already loaded
                delete bst;
                bst = new BinarySearchTree();

                if (!loadCourses(csvPath, bst)) {
                    string altPath = "CS 300 " + csvPath;
                    if (loadCourses(altPath, bst)) {
                        cout << "Data loaded successfully." << endl;
                    }
                } else {
                    cout << "Data loaded successfully." << endl;
                }
                break;
            }

            case 2: {
                if (bst->IsEmpty()) {
                    cout << "No data loaded. Please load the data structure first (Option 1)." << endl;
                } else {
                    cout << "Here is a sample schedule:" << endl;
                    bst->InOrder();
                }
                break;
            }

            case 3: {
                if (bst->IsEmpty()) {
                    cout << "No data loaded. Please load the data structure first (Option 1)." << endl;
                } else {
                    string targetCourse;
                    cout << "What course do you want to know about? ";
                    getline(cin, targetCourse);
                    targetCourse = toUpperCase(trim(targetCourse));
                    printCourseInformation(bst, targetCourse);
                }
                break;
            }

            case 9: {
                cout << "Thank you for using the course planner!" << endl;
                break;
            }

            default: {
                cout << choice << " is not a valid option." << endl;
                break;
            }
        }
    }

    delete bst;
    return 0;
}