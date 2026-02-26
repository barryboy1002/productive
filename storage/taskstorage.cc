#include "taskstorage.h"
#include <iostream>
#include <memory>

// Keep the connection as a member of a class or a long-lived object
// But create transactions locally within functions
pqxx::connection conn("host=localhost port=5433 user=postgres dbname=taskbase connect_timeout=10");

void Task::add_to_db() {
    try {
        pqxx::work tx(conn); // Local transaction
        tx.exec(
            "INSERT INTO taskinfo (taskname, taskdesc, priority, duedate) VALUES ($1, $2, $3, $4);",
            pqxx::params(name, desc, levl, date)
        );
        tx.commit();
    } catch (const std::exception &e) {
        std::cerr << "Database error in add_to_db: " << e.what() << std::endl;
    }
}

std::vector<task_state> get_taskname_list() {
    std::vector<task_state> list;
    try {
        pqxx::nontransaction nt(conn); // Use nontransaction for simple reads
        auto rows = nt.exec("SELECT tasknum, taskname FROM taskinfo");
        
        for (auto const &row : rows) {
            list.push_back({ row[0].as<int>(), row[1].as<std::string>() });
        }
    } catch (const std::exception &e) {
        std::cerr << "Database error in get_taskname_list: " << e.what() << std::endl;
    }
    return list;
}

// Return a unique_ptr or an object to avoid managing raw global pointers
std::unique_ptr<Task> get_task_info(int taskid) {
    try {
        pqxx::nontransaction nt(conn);
        auto result = nt.exec("SELECT taskname, taskdesc, priority, duedate FROM taskinfo WHERE tasknum = $1", pqxx::params(taskid));

        if (!result.empty()) {
            auto row = result[0];
            return std::make_unique<Task>(
                row[0].as<std::string>(),
                row[1].as<std::string>(),
                row[2].as<Priority>(),
                row[3].as<std::string>()
            );
        }
    } catch (const std::exception &e) {
        std::cerr << "Database error in get_task_info: " << e.what() << std::endl;
    }
    return nullptr;
}

