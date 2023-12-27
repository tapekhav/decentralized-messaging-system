#include <array>
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

UsersDatabaseManager::UsersDatabaseManager(const std::string& uri)
{ 
    connectToDatabase(uri); 
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

void UsersDatabaseManager::insertIntoUsersQuery(const mod_query_list& args)
{
    std::string insert_into_users("INSERT INTO Users \
                                        (user_id, nickname, ip_v4) \
                                      VALUES(");
    
    addKey(insert_into_users);
    for (std::size_t i = 0; i < consts::db::kNumOfUsersColumns; ++i)
    {
        addString(insert_into_users, args[i]);
    }
    changeEnd(insert_into_users);

    executeModifyingRawQuery(insert_into_users);
}

void UsersDatabaseManager::insertIntoUserInfoQuery(const mod_query_list& args)
{
    std::string insert_into_user_info("INSERT INTO UserInfo \
                                            (info_id, name, birth_date, additional_information, user_id) \
                                          VALUES(");

    addKey(insert_into_user_info);
    for (std::size_t i = consts::db::kNumOfUsersColumns; i < consts::db::kNumOfUsersColumns; ++i)
    {
        addString(insert_into_user_info, args[i]);
    }
    addKey(insert_into_user_info);
    changeEnd(insert_into_user_info);
    
    executeModifyingRawQuery(insert_into_user_info);
}

auto checkArgs(const mod_query_list& args) -> bool
{
    try
    {
        if (!ip_v4::isIpValid(*(args.begin() + 1)))
        {
            throw IpStructureException();
        }

        if (!date::isDateValid(*(args.begin() + 2)))
        {
            throw DateStructureException();
        }

        std::size_t index = 0;
        for (const auto& col : args)
        {
            if (col.size() > consts::db::kMaxWordSize)
            {
                throw SizeException(index);
            }
            ++index;
        }
    }
    catch (const SociBaseException& error)
    {
        std::cerr << error.name() << ": " << error.what() << "\n"; 
        return false;
    }

    return true;
}

void UsersDatabaseManager::deleteQuery(const std::string& where_condition)
{

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
                                    FROM Users \
                                    INNER JOIN UserInfo USING(user_id);";

    return executeReturnRawQuery(select_all_query, consts::db::kNumOfAllColumns);
}