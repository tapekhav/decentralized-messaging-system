package mongo

import (
	"log"
	"time"
	"context"

	"auth/internal/models"

	"go.mongodb.org/mongo-driver/bson"
	"go.mongodb.org/mongo-driver/mongo"
	"go.mongodb.org/mongo-driver/mongo/options"
	"go.mongodb.org/mongo-driver/bson/primitive"
)

type Repo struct {
	tokens *mongo.Collection
}

func (r *Repo) GetTokenByNickname(ctx context.Context, 
								  nickname string) (models.RefreshToken, error) {
	res := r.tokens.FindOne(ctx, bson.M{"_id": nickname})
	
	if err := res.Err(); err != nil {
		return models.RefreshToken{}, err
	}
	
	token := models.Token{}
	if err := res.Decode(&token); err != nil {
		return models.RefreshToken{}, err
	}

	return models.NewRefreshToken(nickname, token.Hash, token.Expires.Time()), nil
} 

func (r *Repo) GenerateToken(ctx context.Context, 
							 token models.RefreshToken) error {
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

func New(connectionString string) Repo {
	ctx, cancel := context.WithTimeout(context.Background(), 10*time.Second)
	defer cancel()

	clientOptions := options.Client().ApplyURI(connectionString)

	client, err := mongo.Connect(ctx, clientOptions)
	if err != nil {
		log.Fatal(err)	
	}

	err = client.Ping(ctx, nil)
	if err != nil {
		log.Fatal(err)
	}
		
	db := client.Database("nicknameTokensDB")
	
	return Repo{tokens: db.Collection("tokens")}
}