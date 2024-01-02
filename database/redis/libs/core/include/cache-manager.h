#include <mutex>
#include <ratio>
#include <string>
#include <memory>
#include <cstdint>
#include <cstdlib>
#include <cstddef>
#include <optional>
#include <iostream>
#include <algorithm>

#include <nlohmann/json.hpp>
#include <hiredis/hiredis.h>

#include <consts.h>
#include <context_error.h>
#include <nullptr_context_error.h>


template<class KeyType = int32_t, class ValueType = std::string>
class CacheManager
{
public:
    using reply_ptr = std::unique_ptr<redisReply, decltype(&freeReplyObject)>;

    CacheManager(const std::string& ip_v4, int32_t port);
    CacheManager(const CacheManager<KeyType, ValueType>& other) = delete;
    CacheManager(CacheManager<KeyType, ValueType>&& other) noexcept;

    auto operator=(const CacheManager<KeyType, ValueType>& other) -> CacheManager& = delete;
    auto operator=(CacheManager<KeyType, ValueType>&& other) noexcept -> CacheManager&; 

    void swap(CacheManager<KeyType, ValueType>&& other);

    void createValue(const std::pair<KeyType, ValueType>& key_value);

    auto keyExists(const KeyType& key) -> bool;
    auto readValue(const KeyType& key) -> std::string;
    auto readKeyByValue(const ValueType& value) -> std::optional<KeyType>;
    auto getAllKeys() -> std::vector<KeyType>;
    auto getAllValues() -> std::vector<ValueType>;
    auto getAllKeyValuePairs() -> std::vector<std::pair<KeyType, ValueType>>;

    auto getServerInfo() -> std::string;

    auto getCacheSize() -> std::size_t;

    void updateValue(const std::pair<KeyType, ValueType>& key_value);

    void deleteByKey(const KeyType& key);
    void clearCache();

    void hsetValue(const KeyType& hash_key, const std::string& field, const ValueType& value);
    auto hgetValue(const KeyType& hash_key, const std::string& field) -> std::string;
    auto hkeyExists(const KeyType& hash_key, const std::string& field) -> bool;
    void hdeleteKey(const KeyType& hash_key, const std::string& field);

    ~CacheManager() = default;

private:
    void checkContext();
    auto doReply(const std::string& formatted_command) -> std::optional<reply_ptr>;    
    
    mutable std::mutex _mutex;
    std::unique_ptr<redisContext, decltype(&redisFree)> _context;
};

template<class KeyType, class ValueType>
CacheManager<KeyType, ValueType>::CacheManager(const std::string& ip_v4, int32_t port)
                                               : _context(
                                                 nullptr,
                                                 &redisFree
                                                )
{
    std::unique_lock<std::mutex> lock(_mutex);
    
    auto *tempContext = redisConnect(ip_v4.c_str(), port);
    if (tempContext != nullptr)
    {
        _context.reset(tempContext);
    }
        
    checkContext();
}

template<class KeyType, class ValueType>
CacheManager<KeyType, ValueType>::CacheManager(CacheManager<KeyType, ValueType>&& other) noexcept
{
    std::scoped_lock<std::mutex> lock(_mutex, other._mutex);

    _context = other._context;
    other._context.reset();
}

template<class KeyType, class ValueType>
auto CacheManager<KeyType, ValueType>::operator=(CacheManager<KeyType, ValueType>&& other) 
                                                 noexcept -> CacheManager&
{    
    std::scoped_lock lock_this(_mutex, other._mutex);

    if (this == &other)
    {
        this->swap(other);
    }

    return *this;
} 

template<class KeyType, class ValueType>
void CacheManager<KeyType, ValueType>::swap(CacheManager<KeyType, ValueType>&& other)
{
    std::swap(_context, other._context);
}

template<class KeyType, class ValueType>
auto CacheManager<KeyType, ValueType>::readKeyByValue(const ValueType& value) -> std::optional<KeyType>
{
    nlohmann::json json_value = value;
    std::string scan_command("SCAN 0 MATCH * COUNT 1000");

    auto scan_reply = doReply(scan_command);

    if (!scan_reply.has_value() || scan_reply->get() == nullptr)
    {
        return std::nullopt;
    }

    if (scan_reply->get()->type != REDIS_REPLY_ARRAY)
    {
        return std::nullopt;
    }

    for (size_t i = 0; i < scan_reply->get()->element[1]->elements; i++)
    {
        redisReply* key_reply = scan_reply->get()->element[1]->element[i];

        if (key_reply->type == REDIS_REPLY_STRING && key_reply->str != nullptr)
        {
            std::string key_command("GET " + std::string(key_reply->str));
            auto key_value_reply = doReply(key_command);

            if (key_value_reply.has_value() && 
                key_value_reply->get() != nullptr &&
                key_value_reply->get()->type == REDIS_REPLY_STRING)
            {
                if (std::string(key_value_reply->get()->str) == json_value.dump())
                {
                    return nlohmann::json::parse(key_reply->str).get<KeyType>();
                }
            }
        }
    }

    return std::nullopt;
}

template<class KeyType, class ValueType>
void CacheManager<KeyType, ValueType>::checkContext() 
{
    try
    {
        if (_context == nullptr)
        {
            throw ContextNullptrError();
        }

        if (_context->err != 0)
        {
            throw ContextError(_context->errstr);
        }
    }
    catch (const RedisBaseError& error)
    {
        std::cerr << error.name() << ": " 
                  << error.what() << '\n';
        this->~CacheManager();
    }
}

template<class KeyType, class ValueType>
void CacheManager<KeyType, ValueType>::createValue(const std::pair<KeyType, ValueType>& key_value) 
{
    std::unique_lock<std::mutex> lock(_mutex);

    nlohmann::json json_key = key_value.first;
    nlohmann::json json_value = key_value.second;

    std::string formatted_command(
                            "SET " + 
                            std::string(json_key.dump()) + 
                            " " + 
                            std::string(json_value.dump())
    );
    auto reply = doReply(formatted_command);
}

template<class KeyType, class ValueType>
void CacheManager<KeyType, ValueType>::deleteByKey(const KeyType& key)
{
    std::unique_lock<std::mutex> lock(_mutex);

    nlohmann::json json_key = key;

    std::string formatted_command("DEL " + std::string(json_key.dump()));
    auto reply = doReply(formatted_command);
}

template<class KeyType, class ValueType>
void CacheManager<KeyType, ValueType>::clearCache()
{
    std::unique_lock<std::mutex> lock(_mutex);

    std::string flushall_command("FLUSHALL");
    doReply(flushall_command);
}

template<class KeyType, class ValueType>
auto CacheManager<KeyType, ValueType>::keyExists(const KeyType& key) -> bool 
{
    std::string exists_command("EXISTS " + std::to_string(key));
    auto reply = doReply(exists_command);

    return reply.has_value() && 
           reply->get() != nullptr && 
           reply->get()->integer == 1;
}

template<class KeyType, class ValueType>
auto CacheManager<KeyType, ValueType>::getCacheSize() -> std::size_t
{
    std::string dbsize_command("DBSIZE");
    auto reply = doReply(dbsize_command);

    return reply.has_value() ? reply->get()->integer : 0;
}

template<class KeyType, class ValueType>
auto CacheManager<KeyType, ValueType>::getAllValues() -> std::vector<ValueType>
{
    std::vector<KeyType> keys = getAllKeys();
    std::vector<ValueType> values;

    for (const auto& key : keys)
    {
        values.push_back(readValue(key));
    }

    return values;
}

template<class KeyType, class ValueType>
auto CacheManager<KeyType, ValueType>::getAllKeyValuePairs() 
                                    -> std::vector<std::pair<KeyType, ValueType>>
{
    std::vector<KeyType> keys = getAllKeys();
    std::vector<std::pair<KeyType, ValueType>> keyValuePairs;

    for (const auto& key : keys)
    {
        keyValuePairs.push_back({key, readValue(key)});
    }

    return keyValuePairs;
}

template<class KeyType, class ValueType>
auto CacheManager<KeyType, ValueType>::getServerInfo() -> std::string
{
    std::string info_command("INFO");
    auto reply = doReply(info_command);

    return reply.has_value() ? std::string(reply->get()->str) : "";
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
    auto reply = doReply(formatted_command);
    
    if (reply.has_value() && reply->get() != nullptr)
    {
        return reply->get()->type == REDIS_REPLY_STRING ? reply->get()->str : std::string();
    }
    return {};
}

template<class KeyType, class ValueType>
auto CacheManager<KeyType, ValueType>::doReply(const std::string& formatted_command) 
                                                            -> std::optional<reply_ptr>
{
    try 
    {
        reply_ptr reply(
            static_cast<redisReply*>(
                redisCommand(
                    _context.get(), 
                    formatted_command.c_str()
                )),
            &freeReplyObject
        );

        if (reply.get() == nullptr)
        {
            throw ContextNullptrError();
        }

        if (reply->type == REDIS_REPLY_ERROR)
        {
            throw ContextError("error in command -- " + std::string(reply->str));
        }

        return reply;
    }
    catch (const RedisBaseError& error)
    {
        std::cerr << error.what() << ": " << error.what() << '\n';
    }

    return std::nullopt;
}

template<class KeyType, class ValueType>
auto CacheManager<KeyType, ValueType>::getAllKeys() -> std::vector<KeyType>
{
    std::string keys_command("KEYS *");
    auto reply = doReply(keys_command);

    std::vector<KeyType> keys;
    if (reply.has_value() && reply->get() != nullptr && reply->get()->type == REDIS_REPLY_ARRAY)
    {
        for (size_t i = 0; i < reply->get()->elements; i++)
        {
            redisReply* key_reply = reply->get()->element[i];
            if (key_reply->type == REDIS_REPLY_STRING)
            {
                keys.push_back(nlohmann::json::parse(key_reply->str).get<KeyType>());
            }
        }
    }

    return keys;
}

template<class KeyType, class ValueType>
void CacheManager<KeyType, ValueType>::hsetValue(const KeyType& hash_key, 
                                                 const std::string& field, 
                                                 const ValueType& value)
{
    std::unique_lock<std::mutex> lock(_mutex);

    nlohmann::json json_key = hash_key;
    nlohmann::json json_value = value;

    std::string hset_command(
                             "HSET " + 
                             std::string(json_key.dump()) + 
                             " " + 
                             field + 
                             " " + 
                             std::string(json_value.dump())
    );
    doReply(hset_command);
}

template<class KeyType, class ValueType>
auto CacheManager<KeyType, ValueType>::hgetValue(const KeyType& hash_key, 
                                                 const std::string& field) -> std::string
{
    nlohmann::json json_key = hash_key;

    std::string hget_command("HGET " + std::string(json_key.dump()) + " " + field);
    auto reply = doReply(hget_command);

    if (reply.has_value() && reply->get() != nullptr && reply->get()->type == REDIS_REPLY_STRING)
    {
        return std::string(reply->get()->str);
    }

    return "";
}

template<class KeyType, class ValueType>
auto CacheManager<KeyType, ValueType>::hkeyExists(const KeyType& hash_key, 
                                                  const std::string& field) -> bool
{
    nlohmann::json json_key = hash_key;

    std::string hexists_command("HEXISTS " + std::string(json_key.dump()) + " " + field);
    auto reply = doReply(hexists_command);

    return reply.has_value() && reply->get() != nullptr && reply->get()->integer == 1;
}

template<class KeyType, class ValueType>
void CacheManager<KeyType, ValueType>::hdeleteKey(const KeyType& hash_key, const std::string& field)
{
    std::unique_lock<std::mutex> lock(_mutex);

    nlohmann::json json_key = hash_key;

    std::string hdel_command("HDEL " + std::string(json_key.dump()) + " " + field);
    doReply(hdel_command);
}
