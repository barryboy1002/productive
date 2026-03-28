// Improved documentation for the TaskStorage class
// This class is responsible for managing task storage in a productive environment.

#ifndef TASKSTORAGE_H
#define TASKSTORAGE_H

#include <string>
#include <vector>

class TaskStorage {
public:
    // Default constructor
    TaskStorage() = default;

    // Method to add a task
    void addTask(const std::string& taskName);

    // Method to remove a task by name
    bool removeTask(const std::string& taskName);

    // Method to retrieve all tasks
    const std::vector<std::string>& getTasks() const;

private:
    std::vector<std::string> tasks; // Vector to store the tasks
};

#endif // TASKSTORAGE_H
