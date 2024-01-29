package models

import (
	"time"
)

type RefreshToken struct {
	Nickname string
	Hash     string
	Expires  time.Time
}

func NewRefreshToken(nickname, hash string, expires time.Time) RefreshToken {
	return RefreshToken{
		Nickname: nickname,
		Hash: 	  hash,
		Expires:  expires,
	}
}