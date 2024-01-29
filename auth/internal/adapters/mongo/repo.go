package mongo

import (
	"context"

	"auth/internal/models"

	"go.mongodb.org/mongo-driver/bson"
	"go.mongodb.org/mongo-driver/bson/primitive"
	"go.mongodb.org/mongo-driver/mongo"
	"go.mongodb.org/mongo-driver/mongo/options"
)

type Repo struct {
	tokens *mongo.Collection
}

func (r *Repo) getTokenByNickname(ctx context.Context, 
								 nickname string) (models.RefreshToken, error) {
	res := r.tokens.FindOne(ctx, bson.M{"nickname": nickname})

	if err := res.Err(); err != nil {
		return models.RefreshToken{}, err
	}
	
	token := models.Token{}
	if err := res.Decode(&token); err != nil {
		return models.RefreshToken{}, err
	}

	return models.NewRefreshToken(nickname, token.Hash, token.Expires.Time()), nil
} 

func (r *Repo) GenerateToken(ctx context.Context, token models.RefreshToken) (error) {
	updateFields := bson.D{
		primitive.E{Key: "hash", Value: token.Hash},
		primitive.E{Key: "expires", Value: primitive.NewDateTimeFromTime(token.Expires)},
	}

	updateDoc := bson.D{
		primitive.E{
			Key:   "$set",
			Value: updateFields,
		},
	}

	options := options.Update().SetUpsert(true)
	_, err := r.tokens.UpdateByID(ctx, token.Nickname, updateDoc, options)

	if err != nil {
		return nil
	}

	return nil
}