package mongo

import (
	"log"
	"time"
	"context"

	"auth/internal/models"
	"auth/internal/config"

	"go.mongodb.org/mongo-driver/bson"
	"go.mongodb.org/mongo-driver/bson/primitive"
	"go.mongodb.org/mongo-driver/mongo"
	"go.mongodb.org/mongo-driver/mongo/options"
)

type Repo struct {
	cfg 	config.Config
	users  *mongo.Collection
	tokens *mongo.Collection
}

func (r *Repo) GetTokenByNickname(
	ctx context.Context,
	nickname string,
) (models.RefreshToken, error) {
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

func (r *Repo) FindUsers(
	ctx context.Context,
	user models.UserAuthRequest,
) error {
	res := r.users.FindOne(ctx, bson.M{"_id": user.Nickname})

	if err := res.Err(); err != nil {
		return err
	}

	var foundUser models.UserAuthRequest
	if err := res.Decode(&foundUser); err != nil {
		return nil
	}

	if foundUser.Password != user.Password {
		return config.MismatchPasswordError
	}

	return nil
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

func (r *Repo) CreateUser(
	ctx context.Context,
	user models.UserAuthRequest,
) error {
	updateFields := bson.D{
		primitive.E{Key: "password", Value: user.Password},
	}

	updateDoc := bson.D{
		primitive.E{
			Key:   "$set",
			Value: updateFields,
		},
	}

	options := options.Update().SetUpsert(true)
	_, err := r.users.UpdateByID(ctx, user.Nickname, updateDoc, options)

	if err != nil {
		return nil
	}

	return nil
}

func New(
	connectionString string, 
	config config.Config,
) Repo {
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

	return Repo{
		cfg:	config, 
		users:  db.Collection("users"),
		tokens: db.Collection("tokens"),
	}
}
