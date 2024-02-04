package config

import (
	"log"

	"github.com/ilyakaznacheev/cleanenv"
)

type Config struct {
	Port string `json:"PORT" env-default:50055`
	Key  string `json:"KEY" env-default:example-key`
	Cost string `json:"FACTOR_COST" env-default:10`
}

func MustLoad() Config {
	var cfg Config

	err := cleanenv.ReadConfig("../../server.env", &cfg)

	if err != nil {
		log.Fatalf("Cannot read config: %s", err)
	}

	return cfg
}