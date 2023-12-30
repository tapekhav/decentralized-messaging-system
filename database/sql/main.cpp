#include <cstddef>
#include <iterator>
#include <string>
#include <users_db_manager.h>
#include <iostream>
#include <thread>

int main()
{
    UsersDatabaseManager db_manager(consts::db::kUri);
    testInsertQuery(db_manager, 5);

    //! db_manager.insertQuery({"test1", "127.0.0.1", "Test1", "2003-06-27", "ahah1"});
    //! db_manager.insertQuery({"test2", "127.0.0.2", "Test2", "2003-06-28", "ahah2"});
    //! db_manager.insertQuery({"test3", "127.0.0.3", "Test3", "2003-06-29", "ahah3"});

    auto i = db_manager.selectAllQuery();

    for (const auto& j : i)
    {
        for (const auto& k : j)
        {
            std::cout << k << " ";
        }
        std::cout << '\n';
    }


    return 0;
}