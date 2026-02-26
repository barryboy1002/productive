#pragma once
#include <string>
#include <pqxx/pqxx> // Need this for the macro
#include <string>
#include <vector>


enum Priority { High, Mid, Low };

std::vector<std::string>& get_taskname_list();
// This tells pqxx how to handle your enum automatically
namespace pqxx{
PQXX_DECLARE_ENUM_CONVERSION(Priority);
}
class Task {
public:
    int id;
    std::string name;
    std::string desc;
    std::string date;
    Priority levl;

    Task(std::string tname, std::string tdesc, std::string tdate, Priority tlevl)
        : name{tname}, desc{tdesc}, date{tdate}, levl{tlevl} {}

    void add_to_db();
    void updatde_db_info();
};
