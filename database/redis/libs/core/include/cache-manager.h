#include <string>
#include <memory>
#include <cstdint>
#include <cstdlib>
#include <iostream>

#include <nlohmann/json.hpp>
#include <hiredis/hiredis.h>

template<class KeyType = int32_t, class ValueType = std::string>
class CacheManager
{
public:
    CacheManager(const std::string& ip_v4, int32_t port);
    void checkContext();

    void createValue(const std::pair<KeyType, ValueType>& key_value);

    auto readValue(const KeyType& key) -> std::string;

    void updateValue(const std::pair<KeyType, ValueType>& key_value);

    void deleteByKey(const KeyType& key);

    ~CacheManager() = default;

private:
    std::unique_ptr<redisContext, decltype(&redisFree)> _context;
};

template<class KeyType, class ValueType>
CacheManager<KeyType, ValueType>::CacheManager(const std::string& ip_v4, int32_t port)
                                               : _context(redisConnect(
                                                    ip_v4.c_str(),
                                                    port
                                                ),
                                                 &redisFree
                                                )
{
    checkContext();
}

template<class KeyType, class ValueType>
void CacheManager<KeyType, ValueType>::checkContext() {
    if (_context == nullptr || _context->err != 0)
    {
        std::cerr << "Connection error: " << _context->errstr << '\n';
        this->~CacheManager();
    }
}

template<class KeyType, class ValueType>
void CacheManager<KeyType, ValueType>::createValue(const std::pair<KeyType, ValueType>& key_value)
{
    nlohmann::json json_key = key_value.first;
    nlohmann::json json_value = key_value.second;

    std::string formatted_command(
                            "SET " +
                            std::string(json_key.dump()) +
                            " " + 
                            std::string(json_value.dump())
    );
    std::unique_ptr<redisReply, decltype(&freeReplyObject)> reply(
        static_cast<redisReply*>(
            redisCommand(
                _context.get(), 
                formatted_command.c_str()
            )),
        &freeReplyObject
    );
}

template<class KeyType, class ValueType>
void CacheManager<KeyType, ValueType>::deleteByKey(const KeyType& key)
{
    nlohmann::json json_key = key;

    std::string formatted_command("DEL " + std::string(json_key.dump()));
    std::unique_ptr<redisReply, decltype(&freeReplyObject)> reply(
        static_cast<redisReply*>(
            redisCommand(
                _context.get(), 
                formatted_command.c_str()
            )),
        &freeReplyObject
    );
}

template<class KeyType, class ValueType>
void CacheManager<KeyType, ValueType>::updateValue(const std::pair<KeyType, ValueType>& key_value)
{
    createValue(key_value);
}

template<class KeyType, class ValueType>
auto CacheManager<KeyType, ValueType>::readValue(const KeyType& key) -> std::string
{
    nlohmann::json json_key = key;

    std::string formatted_command("GET " + std::string(json_key.dump()));
    std::unique_ptr<redisReply, decltype(&freeReplyObject)> reply(
        static_cast<redisReply*>(
            redisCommand(
                _context.get(), 
                formatted_command.c_str()
            )),
        &freeReplyObject
    );
    
    return reply->type == REDIS_REPLY_STRING ? reply->str : std::string();
}