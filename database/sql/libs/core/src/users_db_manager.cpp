#include <users_db_manager.h>

#include <exception>
#include <iostream>
#include <memory>

#include <insert_number_arguments_exception.h>
#include <date_structure_exception.h>
#include <ip_structure_exception.h>
#include <size_of_word_exception.h>
#include <validation_funcs.h>

using str_type = const std::string&;

static const std::map<std::string, std::function<void(str_type value)>> kChecksMap = 
{
    {"nickname", [](str_type value) { if (value.size() > consts::db::kMaxNameSize) { throw SizeException(); }}},
    {"ip_v4", [](str_type value) { if (!ip_v4::isIpValid(value)) { throw IpStructureException(); }} },
    {"name", [](str_type value) { if (value.size() > consts::db::kMaxNameSize) { throw SizeException(); }}},
    {"birth_date", [](str_type value) { if (!date::isDateValid(value)) { throw DateStructureException(); }} },
    {"additional_information", [](str_type value) { if (value.size() > 255) { throw SizeException(); }}}
};

static const std::array<std::string, 5> kIndexArray = 
{
    "nickname",
    "ip_v4",
    "name",
    "birth_date",
    "additional_information"
};

UsersDatabaseManager::UsersDatabaseManager(const std::string& uri)
{ 
    connectToDatabase(uri); 
}

void UsersDatabaseManager::connectToDatabase(const std::string& uri)
{
    try 
    {
        _connection = std::make_unique<pqxx::connection>(uri);
    } 
    catch (const pqxx::broken_connection& error)
    {
        std::cerr << "Error while connecting: " << error.what() << '\n';
    }
}

void UsersDatabaseManager::disconnectFromDatabase()
{
    try 
    {
        _connection->close();
    } 
    catch (const pqxx::broken_connection& error)
    {
        std::cerr << "Error while disconnecting: " << error.what() << '\n';
    }
}

void UsersDatabaseManager::executeModifyingRawQuery(const std::string& query)
{
    try 
    {
        pqxx::work txn(*_connection);
        txn.exec(query);
        txn.commit();
    }
    catch (const pqxx::sql_error& error)
    {
        std::cerr << "Error while executing modifying query: " << error.what() << '\n';
    }
}

void UsersDatabaseManager::addString(std::string& query, const std::string& value)
{
    query += "\'" + value + "\'" + ",";
}

void UsersDatabaseManager::changeEnd(std::string& query)
{
    query.pop_back();
    query += ");";
}

void UsersDatabaseManager::insertQuery(mod_query_list&& args) 
{
    if (!checkArgs(args)) 
    {
        return;
    }

    insertIntoUsersQuery(args);
    insertIntoUserInfoQuery(args);
}

void UsersDatabaseManager::finishInsertQuery(std::string& query,
                                             const mod_query_list& args,
                                             limits_type limits)
{
    for (std::size_t i = limits.first; i < limits.second; ++i)
    {
        addString(query, args[i]);
    }
    changeEnd(query);
}

void UsersDatabaseManager::insertIntoUsersQuery(const mod_query_list& args)
{
    std::string insert_into_users("INSERT INTO public.\"Users\" \
                                        (nickname, ip_v4) \
                                      VALUES(");
    finishInsertQuery(
        insert_into_users, 
        args, 
        {0, consts::db::kNumOfUsersColumns}
    );

    executeModifyingRawQuery(insert_into_users);
}

void UsersDatabaseManager::insertIntoUserInfoQuery(const mod_query_list& args)
{
    std::string insert_into_user_info("INSERT INTO public.\"UserInfo\" \
                                            (name, birth_date, additional_information) \
                                          VALUES(");
    finishInsertQuery(
        insert_into_user_info, 
        args, 
        {consts::db::kNumOfUsersColumns, consts::db::kNumOfDataArgs}
    );
    
    executeModifyingRawQuery(insert_into_user_info);
}

bool UsersDatabaseManager::checkArgs(const mod_query_list& args)
{
    try
    {
        for (std::size_t i = 0; i < consts::db::kNumOfDataArgs; ++i)
        {
            kChecksMap.at(kIndexArray[i])(args[i]);
        }
    }
    catch (const BaseException& error)
    {
        std::cerr << error.name() << ": " << error.what() << "\n"; 
        return false;
    }

    return true;
}

auto UsersDatabaseManager::selectUser(std::size_t user_id) -> return_query_list
{
    std::string select_user_query = "SELECT * \
                                     FROM public.\"Users\" \
                                     INNER JOIN public.\"UserInfo\" USING(user_id) \
                                     WHERE user_id = " + std::to_string(user_id) + ";";

    return executeReturnRawQuery(select_user_query, consts::db::kNumOfDataArgs + 1)[0];
}

auto UsersDatabaseManager::executeReturnRawQuery(const std::string& query, std::size_t num_of_columns) ->
    std::vector<return_query_list>
{
    try
    {
        pqxx::nontransaction txn(*_connection);

        pqxx::result result = txn.exec(query);

        std::vector<return_query_list> table;
        for (const auto& row : result)
        {
            return_query_list row_values;
           for (std::size_t i = 0; i < num_of_columns; i++)
            {
                row.at(static_cast<int>(i)).to(row_values[i]);
            }

            table.push_back(row_values);
        }

        return table;
    }
    catch (const pqxx::sql_error& e)
    {
        std::cerr << "Return raw query error: " << e.what() << std::endl;
    }

    return {};
}

auto UsersDatabaseManager::selectAllQuery() -> std::vector<return_query_list>
{
    std::string select_all_query = "SELECT * \
                                    FROM public.\"Users\" \
                                    INNER JOIN public.\"UserInfo\" USING(user_id);";

    return executeReturnRawQuery(select_all_query, consts::db::kNumOfDataArgs + 1);
}

void UsersDatabaseManager::deleteUser(std::size_t user_id)
{    
    std::string delete_user_query = "DELETE FROM public.\"Users\" \
                                     WHERE user_id = " + std::to_string(user_id) + ";";
    executeModifyingRawQuery(delete_user_query);
}

void UsersDatabaseManager::updateQuery(std::size_t user_id, 
                                       std::string&& table, 
                                       std::string&& column, 
                                       std::string&& value)
{
    try 
    {
        kChecksMap.at(column)(value);
    }
    catch(const BaseException& error)
    {
        std::cerr << error.name() << ": " << error.what() << "\n";
    }

    std::string update_query = "UPDATE public.\""+ table + "\" \
                                SET " + column + " = \'" + value + "\' \
                                WHERE public.\"" + table + "\".user_id = " + std::to_string(user_id) + ";";

    executeModifyingRawQuery(update_query);
}

auto UsersDatabaseManager::selectWhere(std::string&& column,
                                       std::string&& value) -> std::vector<return_query_list>
{
    std::string select_query = "SELECT * \
                                FROM public.\"Users\" \
                                INNER JOIN public.\"UserInfo\" USING(user_id) \
                                WHERE " + column + " = \'" + value + "\';";

    return executeReturnRawQuery(select_query, consts::db::kNumOfDataArgs + 1);
}