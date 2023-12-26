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

static const std::string kUri = "postgresql://postgres:1234@localhost:5433/users";

UsersDatabaseManager::UsersDatabaseManager()
{ 
    connectToDatabase(); 
}

void UsersDatabaseManager::connectToDatabase()
{
    try 
    {
        _session.open(kUri);
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

void UsersDatabaseManager::insertQuery(string_list&& args) 
{
    if (!checkArgs(args)) 
    {
        return;
    }
    ++_primary_key;

    insertIntoUsersQuery(args);
    insertIntoUserInfoQuery(args);
}

void UsersDatabaseManager::insertIntoUsersQuery(const string_list& args)
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

void UsersDatabaseManager::insertIntoUserInfoQuery(const string_list& args)
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

bool checkArgs(const string_list& args)
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