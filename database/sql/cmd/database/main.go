package main

import (
	"fmt"
	"log"

	"github.com/spf13/viper"
	"github.com/gin-gonic/gin"

	"sql/internal/http"
)

type Config struct {
	Database DatabaseConfig `mapstructure:"database"`
	Server   ServerConfig   `mapstructure:"server"`
}

type DatabaseConfig struct {
	DSN string `mapstructure:"dsn"`
}

type ServerConfig struct {
	Port string `mapstructure:"port"`
}

func LoadConfig(filePath string) (Config, error) {
	var config Config

	viper.SetConfigFile(filePath)

	if err := viper.ReadInConfig(); err != nil {
		return Config{}, err
	}

	if err := viper.Unmarshal(&config); err != nil {
		return Config{}, err
	}

	return config, nil
}

func main() {
	configPath := "../../config/database_config.toml"
	config, err := LoadConfig(configPath)
	if err != nil {
		log.Fatalf("Error loading configuration: %s", err)
	}

	dsn := config.Database.DSN
	port := config.Server.Port

	handler, err := handlers.NewHandlers(dsn)
	if err != nil {
		log.Fatalf("Error creating handlers: %s", err)
	}

	router := gin.Default()
	handler.Register(router)

	err = router.Run(fmt.Sprintf(":%s", port))
	if err != nil {
		log.Fatalf("Error starting server: %s", err)
	}
}