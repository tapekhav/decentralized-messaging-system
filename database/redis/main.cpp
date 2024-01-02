#include <iostream>

#include <cache-manager.h>

int main()
{
    std::cout << "Redis database\n";
    CacheManager manager(redis_db::consts::ip_v4, redis_db::consts::port);

    manager.createValue({1, "127.0.0.1"});
    std::cout << manager.readValue(1) << '\n'; 

    auto result = manager.readKeyByValue("127.0.0.1");
    
    std::cout << result.value() << '\n';
    
    auto all_keys = manager.getAllKeys();
    for (auto z : all_keys) 
    {
        std::cout << z << '\n';
    }

    std::cout << "\n\n\n";

    auto all_values = manager.getAllValues();
    for (const auto& value : all_values)
    {
        std::cout << value << '\n';
    }
    std::cout << "\n\n\n";

    auto all_pairs = manager.getAllKeyValuePairs();
    for (auto& pair : all_pairs)
    {
        std::cout << pair.first << '\t' << pair.second << '\n';
    }

    return 0;
}