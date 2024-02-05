package app

import (
	"context"
	"time"

	"auth/internal/models"
)

type JWTRepo interface {
	GetTokenByNickname(
		ctx context.Context,
		nickname string,
	) (models.RefreshToken, error)
	GenerateToken(ctx context.Context, token models.RefreshToken) error
	FindUser(ctx context.Context, user models.UserAuthRequest) error
	CreateUser(ctx context.Context, user models.UserAuthRequest) error
}

type UsersRepo interface {
	RegisterUser(
		nickname string,
		password string,
		Ipv4 string,
		birthDate time.Time,
		name string,
		additionalInformation string,
	) error
	AuthUser(
		nickname string,
		password string,
	) (models.User, error)
}

type Hasher interface {
	HashFunction(hashingString string) (string, error)
	CompareStrings(originalString, hashString string) error
}
