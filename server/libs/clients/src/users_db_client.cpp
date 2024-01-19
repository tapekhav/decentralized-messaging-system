#include <users_db_client.h>


auto UsersClient::getUserIp(const std::string& nickname) -> std::string
{
    db_sql::UserRequest request;
    request.set_nickname(nickname);

    db_sql::UserResponse response;

    ClientContext context;
    Status status = _stub->getUser(&context, request, &response);

    return status.ok() ? response.ip() : status.error_message();
}

auto UsersClient::setUser(
        const std::string& nickname,
        const std::string& ip,
        const std::string& birth_date,
        const std::string& name,
        const std::string& additional_information
    ) -> Status
{
    db_sql::NewUserRequest request;
    request.set_nickname(nickname);
    request.set_ip(ip);
    request.set_birth_date(birth_date);
    request.set_name(name);
    request.set_additional_information(additional_information);

    Empty response;

    ClientContext context;

    return _stub->setUser(&context, request, &response);
}