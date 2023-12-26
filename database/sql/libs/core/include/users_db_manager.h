#pragma once

#include <array>
#include <cstddef>
#include <initializer_list>
#include <string>
#include <vector>
#include <memory>

#include <soci/soci.h>

#include <consts.h>

using string_list = std::array<std::string, consts::db::kNumOfInsertQueryArgs>;

class UsersDatabaseManager final
{
public:
    explicit UsersDatabaseManager();

    void executeModifyingRawQuery(const std::string& query);
    void insertQuery(string_list&& args);
    void deleteQuery(const std::string& where_condition);

    auto checkArgs(const string_list& args) -> bool;

    ~UsersDatabaseManager() { disconnectFromDatabase(); }

private:
    void insertIntoUsersQuery(const string_list& args);
    void insertIntoUserInfoQuery(const string_list& args);

    void addKey(std::string& query);
    void addString(std::string& query, const std::string& value);
    void changeEnd(std::string& query);

    void connectToDatabase();
    void disconnectFromDatabase();

    size_t _primary_key;
    soci::session _session;
    std::unique_ptr<soci::statement> _statement;
};

