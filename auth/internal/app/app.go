package app

import (
	"fmt"
	"time"
	"context"

	"auth/internal/models"

	"github.com/dgrijalva/jwt-go"
)

type App struct {
	signingKey  string
	hasher      Hasher
	jwtRepo     JWTRepo
	database    UsersRepo
	accessTime  time.Duration
	refreshTime time.Duration
}

func (a App) GenerateJWTPair(
	ctx context.Context, 
	nickname string,
	) (models.JWTPair, error) {

		return models.JWTPair{}, nil
}

func (a App) generateToken(tokenType string) (string, error) {

	token := jwt.New(jwt.SigningMethodHS256)

	var expires time.Time
	if tokenType == "refresh" {
		expires = time.Now().Add(time.Second * a.refreshTime)
	} else if tokenType == "access" {
		expires = time.Now().Add(time.Second * a.accessTime)
	} else {
		return "", fmt.Errorf("Incorrect token type: %v", tokenType)
	}

	claims := jwt.MapClaims{
		"type": tokenType,
		"expires": expires.Unix(),
	}

	token.Claims = claims

	tokenString, err := token.SignedString(a.signingKey)
	if err != nil {
		return "", err
	}

	return tokenString, nil
}

func NewApp(hasher Hasher, jwtRepo JWTRepo, db UsersRepo) App {
	return App{
		hasher:   hasher,
		jwtRepo:  jwtRepo,
		database: db,
	}
}
