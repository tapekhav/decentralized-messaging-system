#pragma once

#include <string>
#include <fstream>

class UsersDatabaseManagerTest : public ::testing::Test 
{
protected:
    void SetUp() override 
    {
        applySetUpMigrations();
    }

    void TearDown() override 
    {
        applyTearDownMigrations();
    }

private:
    void applySetUpMigrations()
    {
        const std::vector<std::string> migration_files = 
        {
            "001_create_users_table.sql",
            "002_insert_test_data.sql"
        };

        executeMigrations(migration_files);
    }

    void applyTearDownMigrations()
    {
        const std::vector<std::string> migration_files = 
        {
            "003_drop_users_table.sql"
        };

        executeMigrations(migration_files);
    }

    void executeMigrations(const std::vector<std::string>& migration_files)
    {
        UsersDatabaseManager& manager(UsersDatabaseManager::getInstance(consts::db::kUri));
        for (const auto& migration_file : migration_files)
        {
            std::ifstream file(kMigrationPath + migration_file);
            std::string query((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

            manager.executeModifyingRawQuery(query);
        }
    }
};