#include <iostream>

#include <consts.h>
#include <cache-manager.h>

int main()
{
    std::cout << "Redis database\n";
    CacheManager manager(redis_db::consts::ip_v4, redis_db::consts::port);

    return 0;
}