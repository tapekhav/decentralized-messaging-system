#include <gtest/gtest.h>

#include <users_db_manager.h>

UsersDatabaseManager* UsersDatabaseManager::_instance = nullptr;
std::mutex UsersDatabaseManager::_mutex;

int main(int argc, char** argv) 
{
    ::testing::InitGoogleTest(&argc, argv);
    
    return RUN_ALL_TESTS();
}