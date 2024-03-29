package jwt

import (
	"fmt"
	"time"
	"errors"
	"context"

	"auth/internal/models"

	"github.com/dgrijalva/jwt-go"
)

type AccessRefresh struct {
	secretKey   []byte
	hasher      Hasher
	jwtRepo     JWTRepo
	accessTime  time.Duration
	refreshTime time.Duration
}

func (a AccessRefresh) GenerateJWTPair(ctx context.Context, nickname string) (models.JWTPair, error) {
	accessToken, err := a.CreateAccessToken(nickname)
	if err != nil {
		return models.JWTPair{}, err
	}

	refreshToken, err := a.CreateRefreshToken(ctx, nickname)
	if err != nil {
		return models.JWTPair{}, err
	}

	hash, _ := a.hasher.HashFunction(refreshToken)
	a.jwtRepo.GenerateToken(ctx, models.NewRefreshToken(
		nickname,
		hash,
		time.Now().UTC().Add(a.refreshTime),
	))

	return models.JWTPair{
		Access:  accessToken,
		Refresh: refreshToken,
	}, nil
}

func (a AccessRefresh) CreateAccessToken(nickname string) (string, error) {
	token := jwt.New(jwt.SigningMethodHS256)
	claims := token.Claims.(jwt.MapClaims)
	claims["sub"] = nickname
	claims["exp"] = time.Now().Add(a.accessTime).Unix()

	tokenString, err := token.SignedString(a.secretKey)
	if err != nil {
		return "", err
	}

	return tokenString, nil
}

func (a AccessRefresh) CreateRefreshToken(
	ctx context.Context,
	nickname string,
) (string, error) {
	token := jwt.New(jwt.SigningMethodHS256)
	claims := token.Claims.(jwt.MapClaims)

	t := time.Now().Add(a.refreshTime)
	claims["exp"] = t.Unix()

	tokenStirng, err := token.SignedString(a.secretKey)
	if err != nil {
		return "", err
	}

	a.jwtRepo.GenerateToken(ctx, models.NewRefreshToken(
		nickname,
		tokenStirng,
		t,
	))

	return tokenStirng, nil
}

func (a AccessRefresh) decodeToken(tokenString string) (jwt.MapClaims, error) {
	token, err := a.getToken(tokenString)

	if err != nil {
		return jwt.MapClaims{}, err
	}

	if !token.Valid {
		return jwt.MapClaims{}, fmt.Errorf("Invalid token")
	}

	claims, ok := token.Claims.(jwt.MapClaims)
	if !ok {
		return jwt.MapClaims{}, fmt.Errorf("Invalid claims")
	}

	return claims, nil
}

func (a AccessRefresh) getToken(tokenString string) (*jwt.Token, error) {
	token, err := jwt.Parse(
		tokenString,
		func(token *jwt.Token) (interface{}, error) {
			return a.secretKey, nil
		},
	)

	if err != nil {
		return &jwt.Token{}, err
	}

	return token, nil
}

func (a AccessRefresh) ValidateAccessToken(
	ctx context.Context,
	accessToken string,
) (bool, error) {
	token, err := a.getToken(accessToken)

	if err != nil {
		return false, err
	}

	if !token.Valid {
		return false, errors.New("Invalid access token")
	}

	return true, nil
}

func (a AccessRefresh) RefreshAccessToken(
	ctx context.Context,
	refreshTokenString string,
) (models.JWTPair, error) {
	claims, err := a.decodeToken(refreshTokenString)
	if err != nil {
		return models.JWTPair{}, err
	}

	nickname, err := a.CreateAccessToken(claims["sub"].(string))
	if err != nil {
		return models.JWTPair{}, err
	}

	token, err := a.jwtRepo.GetTokenByNickname(ctx, nickname)
	if err != nil {
		return models.JWTPair{}, err
	}

	if token.Expires.Before(time.Now().UTC()) {
		return models.JWTPair{}, fmt.Errorf("Refresh token expired")
	}

	if err := a.hasher.CompareStrings(refreshTokenString, token.Hash); err != nil {
		return models.JWTPair{}, err
	}

	return a.GenerateJWTPair(ctx, nickname)
}

func NewApp(secretKey []byte,
	hasher Hasher,
	jwtRepo JWTRepo,
	accessTime time.Duration,
	refreshTime time.Duration,
) AccessRefresh {
	return AccessRefresh{
		secretKey:   secretKey,
		hasher:      hasher,
		jwtRepo:     jwtRepo,
		accessTime:  accessTime,
		refreshTime: refreshTime,
	}
}
