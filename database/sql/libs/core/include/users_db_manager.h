#pragma once

#include <array>
#include <string>
#include <vector>
#include <memory>
#include <cstddef>
#include <functional>

#include <consts.h>

#include <pqxx/pqxx>


using mod_query_list = std::array<std::string, consts::db::kNumOfDataArgs>;
using return_query_list = std::array<std::string, consts::db::kNumOfAllColumns>;
using limits_type = std::pair<std::size_t, std::size_t>;

class UsersDatabaseManager final
{
public:
    explicit UsersDatabaseManager(const std::string& uri);

    void executeModifyingRawQuery(const std::string& query);
    void insertQuery(mod_query_list&& args);

    void deleteUser(std::size_t user_id);

    auto selectAllQuery() -> std::vector<return_query_list>;
    auto selectUser(std::size_t user_id) -> return_query_list;

    auto executeReturnRawQuery(const std::string& query,
                               std::size_t num_of_columns) -> std::vector<return_query_list>;

    void updateQuery(std::size_t user_id, std::string&& column, std::string&& value);

    ~UsersDatabaseManager() { disconnectFromDatabase(); }

private:
    bool checkArgs(const mod_query_list& args);
    void insertIntoUsersQuery(const mod_query_list& args);
    void insertIntoUserInfoQuery(const mod_query_list& args);

    void finishInsertQuery(std::string& query,
                           const mod_query_list& args,
                           limits_type limits);

    void addKey(std::string& query);
    void addString(std::string& query, const std::string& value);
    void changeEnd(std::string& query);

    void connectToDatabase(const std::string& uri);
    void disconnectFromDatabase();

    void setPK();

    size_t _primary_key;
    pqxx::connection _connection;
};
