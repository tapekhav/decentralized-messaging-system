-- migrations/001_create_users_table.sql
CREATE TABLE IF NOT EXISTS test_users (
    user_id SERIAL PRIMARY KEY,
    nickname VARCHAR(60),
    ip_v4 VARCHAR(15)
);
