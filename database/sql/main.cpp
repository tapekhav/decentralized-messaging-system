#include <cstddef>
#include <iterator>
#include <string>
#include <iostream>
#include <thread>

#include "libs/core/include/server.h"

UsersDatabaseManager* UsersDatabaseManager::_instance = nullptr;
std::mutex UsersDatabaseManager::_mutex;

int main()
{
    //! auto& db_manager = UsersDatabaseManager::getInstance(consts::db::kUri);

    //! db_manager.insertQuery({"test4", "127.0.0.4", "Test4", "2003-06-01", "ahah4"});
    //! db_manager.insertQuery({"test5", "127.0.0.2", "Test2", "2003-06-28", "ahah2"});
    //! db_manager.insertQuery({"test3", "127.0.0.3", "Test3", "2003-06-29", "ahah3"});

    /*
    auto i = db_manager.selectAllQuery();

    for (const auto& j : i)
    {
        for (const auto& k : j)
        {
            std::cout << k << " ";
        }
        std::cout << '\n';
    }
    */

    RunServer run("127.0.0.1", 50051);
    run.runDatabaseService();


    return 0;
}