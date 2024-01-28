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
	Nickname string `json:"nickname"`
	Password string `json:"password"`
}
