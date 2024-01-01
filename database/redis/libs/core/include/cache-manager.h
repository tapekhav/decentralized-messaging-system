#include <string>
#include <memory>
#include <cstdint>
#include <cstdlib>
#include <cstddef>
#include <optional>
#include <iostream>

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

    void createValue(const std::pair<KeyType, ValueType>& key_value);

    auto readValue(const KeyType& key) -> std::string;

    void updateValue(const std::pair<KeyType, ValueType>& key_value);

    void deleteByKey(const KeyType& key);

    ~CacheManager() = default;

private:
    void checkContext();
    auto doReply(const std::string& formatted_command) -> std::optional<reply_ptr>;    
    
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
    nlohmann::json json_key = key;

    std::string formatted_command("DEL " + std::string(json_key.dump()));
    auto reply = doReply(formatted_command);
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
    
    if (reply.has_value())
    {
        return reply->type == REDIS_REPLY_STRING ? reply->str : std::string();
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

        if (reply == nullptr)
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