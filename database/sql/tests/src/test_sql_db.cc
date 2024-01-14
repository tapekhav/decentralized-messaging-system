#include <gtest/gtest.h>

#include <users_db_manager.h>
#include <users_database_test_fixture.h>

TEST_F(UsersDatabaseManagerTest, SelectAllQuery)
{
    UsersDatabaseManager& manager = UsersDatabaseManager::getInstance();
    auto selected_users = manager.selectAllQuery();

    ASSERT_FALSE(selected_users.empty());
    ASSERT_EQ(selected_users.size(), 3);
}

TEST_F(UsersDatabaseManagerTest, SelectUser)
{
    UsersDatabaseManager& manager = UsersDatabaseManager::getInstance();

    auto selected_user = manager.selectUser(1);
    ASSERT_EQ(selected_user[1], "test4");
}

TEST_F(UsersDatabaseManagerTest, SelectWhere)
{
    UsersDatabaseManager& manager = UsersDatabaseManager::getInstance();

    auto selected_users = manager.selectWhere("user_id < 2");
    ASSERT_FALSE(selected_users.empty());
    ASSERT_EQ(selected_users.size(), 1);
    ASSERT_EQ(selected_users[0][1], "test4");
}

TEST_F(UsersDatabaseManagerTest, ExecuteReturnRawQuery)
{
    UsersDatabaseManager& manager = UsersDatabaseManager::getInstance();

    auto selected_users = manager.executeReturnRawQuery("SELECT * FROM test_users", 2);
    ASSERT_FALSE(selected_users.empty());
    ASSERT_EQ(selected_users.size(), 1);
    ASSERT_EQ(selected_users[0][1], "TestUser");
}

TEST_F(UsersDatabaseManagerTest, ExecuteModifyingRawQuery)
{
    UsersDatabaseManager& manager = UsersDatabaseManager::getInstance();

    manager.executeModifyingRawQuery("UPDATE test_users SET nickname = 'UpdatedUser' WHERE user_id = 1");
    
    auto selected_users = manager.executeReturnRawQuery("SELECT * FROM test_users", 2);
    ASSERT_FALSE(selected_users.empty());
    ASSERT_EQ(selected_users.size(), 1);
    ASSERT_EQ(selected_users[0][1], "UpdatedUser");
}