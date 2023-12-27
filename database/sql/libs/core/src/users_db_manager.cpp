#include <array>
#include <functional>
#include <string>
#include <users_db_manager.h>

#include <exception>
#include <iostream>
#include <cstddef>
#include <memory>
#include <vector>

#include <insert_number_arguments_exception.h>
#include <date_structure_exception.h>
#include <ip_structure_exception.h>
#include <size_of_word_exception.h>
#include <validation_funcs.h>
#include <consts.h>

using str_type = const std::string&;

static const std::map<std::string, std::function<void(str_type value)>> kChecksMap = 
{
    {"ip_v4", [](str_type value) { if (!ip_v4::isIpValid(value)) { throw IpStructureException(); }} },
    {"birth_date", [](str_type value) { if (!date::isDateValid(value)) { throw DateStructureException(); }} },
    {"nickname", [](str_type value) { if (value.size() > consts::db::kMaxNameSize) { throw SizeException();}}},
    {"name", [](str_type value) { if (value.size() > consts::db::kMaxNameSize) { throw SizeException();}}},
    {"additional_information", [](str_type value) { if (value.size() > 255) { throw SizeException();}}}
};

static const std::map<std::size_t, std::string> kIndexMap = 
{
    {0, "ip_v4"},
    {1, "birth_date"},
    {2, "nickname"},
    {3, "name"},
    {4, "additional_information"}
};

UsersDatabaseManager::UsersDatabaseManager(const std::string& uri)
{ 
    connectToDatabase(uri); 
    setPK();
}

void UsersDatabaseManager::connectToDatabase(const std::string& uri)
{
    try 
    {
        _session.open(uri);
    } 
    catch (const soci::soci_error& error)
    {
        std::cerr << "Error while connecting: " << error.what() << '\n';
    }
}

void UsersDatabaseManager::disconnectFromDatabase()
{
    try 
    {
        _session.close();
    } 
    catch (const soci::soci_error& error)
    {
        std::cerr << "Error while disconnecting: " << error.what() << '\n';
    }
}

void UsersDatabaseManager::executeModifyingRawQuery(const std::string& query)
{
    try 
    {
        _session.begin();
        _session << query;
        _session.commit();
    }
    catch (const soci::soci_error& error)
    {
        std::cerr << "Error while disconnecting: " << error.what() << '\n';
        _session.rollback();
    }
}


void UsersDatabaseManager::addKey(std::string& query)
{
    query += std::to_string(_primary_key) + ",";
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
    ++_primary_key;

    insertIntoUsersQuery(args);
    insertIntoUserInfoQuery(args);
}

void UsersDatabaseManager::finishInsertQuery(std::string& query,
                                             const mod_query_list& args,
                                             limits_type limits)
{
    addKey(query);
    for (std::size_t i = limits.first; i < limits.second; ++i)
    {
        addString(query, args[i]);
    }
    changeEnd(query);
}

void UsersDatabaseManager::insertIntoUsersQuery(const mod_query_list& args)
{
    std::string insert_into_users("INSERT INTO public.\"Users\" \
                                        (user_id, nickname, ip_v4) \
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
                                            (user_id, name, birth_date, additional_information) \
                                          VALUES(");
    finishInsertQuery(
        insert_into_user_info, 
        args, 
        {consts::db::kNumOfUsersColumns, consts::db::kNumOfDataArgs}
    );
    
    executeModifyingRawQuery(insert_into_user_info);
}

auto checkArgs(const mod_query_list& args) -> bool
{
    try
    {
        for (std::size_t i = 0; i < consts::db::kNumOfDataArgs; ++i)
        {
            kChecksMap.at(kIndexMap.at(i))(args[i]);
        }
    }
    catch (const SociBaseException& error)
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

    return executeReturnRawQuery(select_user_query, consts::db::kNumOfDataArgs)[0];
}

auto UsersDatabaseManager::executeReturnRawQuery(const std::string& query, 
                                                std::size_t num_of_columns) ->
                                                std::vector<return_query_list>
{
    try
    {
        soci::rowset<soci::row> rows = (_session.prepare << query);

        std::vector<return_query_list> table; 
        for (const soci::row& row : rows)
        {
            return_query_list row_values;
            for (std::size_t i = 0; i < num_of_columns; i++)
            {
                row.get<std::string>(i, row_values.at(i));
            }

            table.push_back(row_values);
        }

        return table;
    } 
    catch (const soci::soci_error& e)
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

    return executeReturnRawQuery(select_all_query, consts::db::kNumOfAllColumns);
}

void UsersDatabaseManager::deleteUser(std::size_t user_id)
{    
    std::string delete_user_query = "DELETE FROM public.\"Users\" \
                                     WHERE user_id = " + std::to_string(user_id) + ";";
    executeModifyingRawQuery(delete_user_query);
}

void UsersDatabaseManager::setPK()
{
    std::string pk_query = "SELECT MAX(user_id) FROM public.\"Users\";";

    auto result = executeReturnRawQuery(pk_query, 1);

    _primary_key = std::stoi(result[0][0]);
}

void UsersDatabaseManager::updateQuery(std::size_t user_id, std::string&& column, std::string&& value)
{
    try 
    {
        kChecksMap.at(column)(value);
    }
    catch(const SociBaseException& error)
    {
        std::cerr << error.name() << ": " << error.what() << "\n";
    }

    std::string update_query = "UPDATE public.\"Users\" \
                                SET " + column + " = \'" + value + "\' \
                                FROM public.\"UserInfo\" \
                                WHERE public.\"Users\".user_id = public.\"UserInfo\".user_id \
                                AND Users.user_id = " + std::to_string(user_id) + ";";

    executeModifyingRawQuery(update_query);
}