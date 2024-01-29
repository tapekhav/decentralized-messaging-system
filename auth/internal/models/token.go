package models

import "go.mongodb.org/mongo-driver/bson/primitive"

type Token struct {
	Hash    string             `json:"hash" bson:"hash"`
	Expires primitive.DateTime `json:"expires" bson:"expires"`
}

func NewToken(hash string, expires primitive.DateTime) Token {
	return Token{
		Hash:    hash,
		Expires: expires,
	}
}
