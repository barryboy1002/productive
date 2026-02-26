#include "taskstorage.h"
#include <iostream>

pqxx::connection conn("host=localhost port=5433 user=postgres dbname=taskbase connect_timeout=10");
pqxx::work trans(conn);

std::vector<std::string> taskname_list ;
void Task::add_to_db() {
    try {
        trans.exec(
            "INSERT INTO taskinfo (taskname, taskdesc, priority, duedate) VALUES ($1, $2, $3, $4);",
            pqxx::params(name, desc, levl, date)
        );
        trans.commit();
    } catch (const std::exception &e) {
        std::cerr << "Database error: " << e.what() << std::endl;
    }
}

std::vector<std::string>&  get_taskname_list(){
		for(auto [tasknum,taskname]: trans.query<int,std::string>("SELECT tasknum ,taskname FROM taskinfo")){
			taskname_list.push_back(taskname);
		}
		return taskname_list;


	}
