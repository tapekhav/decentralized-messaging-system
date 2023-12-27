#pragma once

#include <array>
#include <cstddef>
#include <initializer_list>
#include <string>
#include <vector>
#include <memory>

#include <soci/soci.h>

#include <consts.h>

using mod_query_list = std::array<std::string, consts::db::kNumOfInsertQueryArgs>;
using return_query_list = std::array<std::string, consts::db::kNumOfAllColumns>;

class UsersDatabaseManager final
{
public:    
    explicit UsersDatabaseManager(const std::string& uri);

    void executeModifyingRawQuery(const std::string& query);
    void insertQuery(mod_query_list&& args);
    void deleteQuery(const std::string& where_condition);

    auto selectAllQuery() -> std::vector<return_query_list>;

    auto executeReturnRawQuery(const std::string& query, 
                               std::size_t num_of_columns) -> std::vector<return_query_list>;

    ~UsersDatabaseManager() { disconnectFromDatabase(); }

private:
    auto checkArgs(const mod_query_list& args) -> bool;
    void insertIntoUsersQuery(const mod_query_list& args);
    void insertIntoUserInfoQuery(const mod_query_list& args);

    void addKey(std::string& query);
    void addString(std::string& query, const std::string& value);
    void changeEnd(std::string& query);

    void connectToDatabase(const std::string& uri);
    void disconnectFromDatabase();

    size_t _primary_key;
    soci::session _session;
    std::unique_ptr<soci::statement> _statement;
};

