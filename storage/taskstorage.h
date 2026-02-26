#pragma once
#include <string>
#include <vector>
#include <memory>
#include <pqxx/pqxx>

// 1. Enum Definition
enum class Priority { High, Mid, Low };

// 2. Register Enum with pqxx 
// This allows you to pass 'Priority' directly into pqxx parameters

namespace pqxx {
    PQXX_DECLARE_ENUM_CONVERSION(Priority);
}


// 3. Data Structures
struct task_state {
    int task_id;
    std::string task_name;
};

class Task {
public:
    int id;
    std::string name;
    std::string desc;
    Priority levl;
    std::string date;

    // Fixed constructor order to match your .cpp usage
    Task(std::string tname, std::string tdesc, Priority tlevl, std::string tdate)
        : name{tname}, desc{tdesc}, levl{tlevl}, date{tdate} {}

    void add_to_db();
    void update_db_info(); // Fixed typo from 'updatde'
};

// 4. Global Function Declarations
std::vector<task_state> get_taskname_list();
std::unique_ptr<Task> get_task_info(int taskid);
