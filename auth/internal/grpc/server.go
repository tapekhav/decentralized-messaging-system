package server

import (
	"log"
	"net"
	"time"
	"strconv"

	pb "auth/pb"
	"auth/internal/jwt"
	"auth/internal/config"
	"auth/internal/adapters/mongo"
	"auth/internal/adapters/crypto"
	"auth/internal/adapters/database"

	"google.golang.org/grpc"
)

func RunServer() {
	cfg := config.MustLoad()
	num, err := strconv.Atoi(cfg.Cost)
	if err != nil {
		log.Fatalf("err:%v\n with num: %s", err, cfg.Cost)	
	}

	hasher := crypto.New(uint8(num))
	mongoRepo := mongo.New(cfg.MongoURL, cfg)

	access, err := time.ParseDuration(cfg.AccessTime + "s")
	if err != nil {
		log.Fatalf("err: %v\n with access time: %s", err, cfg.AccessTime)
	}

	refresh, err := time.ParseDuration(cfg.RefreshTime + "s")
	if err != nil {
		log.Fatalf("err: %v\n with refresh time: %s", err, cfg.RefreshTime)
	}

	authService := NewAuthServiceService(
		hasher,
		&mongoRepo,
		&database.Database{},
		jwt.NewApp(
			[]byte(cfg.Key),
			hasher,
			&mongoRepo,
			access, 
			refresh,
		),
	)
	server := grpc.NewServer()

	pb.RegisterAuthServiceServer(server, authService)

	listener, err := net.Listen("tcp", ":"+cfg.Port)
	if err != nil {
		log.Fatalf("Failed to listen: %v", err)
	}

	log.Printf("Server is listening on port %s...", cfg.Port)

	if err := server.Serve(listener); err != nil {
		log.Fatalf("Failed to serve: %v", err)
	}
}
