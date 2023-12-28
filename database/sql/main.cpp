#include <iostream>
#include <users_db_manager.h>

int main()
{
    //soci::backend_factory const &backEnd;
    std::cout << "SQL database\n";
    UsersDatabaseManager db_manager(consts::db::kUri);

    return 0;
}