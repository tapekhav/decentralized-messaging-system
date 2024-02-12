package config

import (
	"log"

	"github.com/ilyakaznacheev/cleanenv"
)

type Config struct {
	Port 		string `env:"PORT" env-default:"50055"`
	Cost 		string `env:"FACTOR_COST" env-default:"10"`
	Key  		string `env:"KEY" env-default:"example-key"`
	MongoURL 	string `env:"MONGO_URL" env-default:"mongodb://127.0.0.1:27017/"`
	MongoName   string `env:"MONGODB_NAME" env-default:"nicknameTokensDB"`
	AccessTime  string `env:"ACCESS_TIME" env-default:"600"`
	RefreshTime string `env:"REFRESH_TIME" env-default:"604800"`
}

func MustLoad() Config {
	var cfg Config

	err := cleanenv.ReadConfig("../../server.env", &cfg)

	if err != nil {
		log.Fatalf("Cannot read config: %s", err)
	}

	return cfg
}