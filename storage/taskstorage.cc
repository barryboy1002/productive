// taskstorage.cc
#include "taskstorage.h"
#include <iostream>

pqxx::connection conn("host=localhost port=5433 user=postgres dbname=taskbase connect_timeout=10");

// Helper to ensure the connection is alive, reconnecting if needed
void ensure_connected() {
    if (!conn.is_open()) {
        conn = pqxx::connection("host=localhost port=5433 user=postgres dbname=taskbase connect_timeout=10");
    }
}

int Task::add_to_db() {
    try {
        pqxx::work tx(conn);
        auto result = tx.exec(
            "INSERT INTO taskinfo (taskname, taskdesc, priority, duedate) "
            "VALUES ($1, $2, $3, $4) RETURNING tasknum",
            pqxx::params(name, desc, levl, duedate));
        tx.commit();
        return result[0][0].as<int>();
    } catch (const std::exception &e) {
        std::cerr << "Database error in add_to_db: " << e.what() << std::endl;
        return -1;
    }
}

std::vector<task_state> get_taskname_list() {
    std::vector<task_state> list;
    try {
        ensure_connected();
        // nontransaction is correct for read-only queries — no commit needed
        pqxx::nontransaction tx(conn);
        for (auto [id, name] : tx.query<int, std::string>("SELECT tasknum, taskname FROM taskinfo")) {
            list.push_back(task_state{id, name});
        }
    } catch (const pqxx::broken_connection &e) {
        std::cerr << "Connection lost in get_taskname_list, retrying: " << e.what() << std::endl;
        try {
            conn = pqxx::connection("host=localhost port=5433 user=postgres dbname=taskbase connect_timeout=10");
            pqxx::nontransaction tx(conn);
            for (auto [id, name] : tx.query<int, std::string>("SELECT tasknum, taskname FROM taskinfo")) {
                list.push_back(task_state{id, name});
            }
        } catch (const std::exception &e2) {
            std::cerr << "Retry failed: " << e2.what() << std::endl;
        }
    } catch (const std::exception &e) {
        std::cerr << "Database error in get_taskname_list: " << e.what() << std::endl;
    }
    return list;
}

  std::unique_ptr<Task> get_task_info(int taskid) {
    try {
        ensure_connected();
        pqxx::nontransaction tx(conn);  // Read-only, no transaction needed
        auto result = tx.exec(
            "SELECT taskname, taskdesc, priority, duedate FROM taskinfo WHERE tasknum = $1",
            pqxx::params(taskid));

        if (result.empty()) {
            std::cerr << "No task found with id: " << taskid << std::endl;
            return nullptr;
        }

        auto row = result[0];
        return std::make_unique<Task>(
            row[0].as<std::string>(),
            row[1].as<std::string>(),
            row[2].as<Priority>(),
            row[3].as<std::string>()
        );
    } catch (const std::exception &e) {
        std::cerr << "Database error in get_task_info: " << e.what() << std::endl;
}
return nullptr;
}
