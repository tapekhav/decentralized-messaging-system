package models

import (
	"time"
)

type User struct {
	Nickname              string    `json:"nickname"`
	Password              string    `json:"password"`
	Ipv4                  string    `json:"ip_v4"`
	BirthDate             time.Time `json:"birth_date"`
	Name                  string    `json:"name"`
	AdditionalInformation string    `json:"additional_information"`
}

type UserAuthRequest struct {
	Nickname string `json:"nickname" bson:"_id"`
	Password string `json:"password" bson:"password"`
}

func NewUser(nickname string,
	password string,
	ipv4 string,
	birthDate time.Time,
	name string,
	additionalInfo string) User {
	return User{
		Nickname:              nickname,
		Password:              password,
		Ipv4:                  ipv4,
		BirthDate:             birthDate,
		Name:                  name,
		AdditionalInformation: additionalInfo,
	}
}

func NewUserAuthRequest(nickname, password string) UserAuthRequest {
	return UserAuthRequest{
		Nickname: nickname,
		Password: password,
	}
}
