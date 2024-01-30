package server

import (
	"log"
	"net"

	"auth/internal/config"
	pb "auth/pb"

	"google.golang.org/grpc"
)

func RunServer() {
	port := config.MustLoad().Port

	authService := &AuthServiceServer{}
	server := grpc.NewServer()

	pb.RegisterAuthServiceServer(server, authService)

	listener, err := net.Listen("tcp", ":"+port)
	if err != nil {
		log.Fatalf("Failed to listen: %v", err)
	}

	log.Printf("Server is listening on port %s...", port)

	if err := server.Serve(listener); err != nil {
		log.Fatalf("Failed to serve: %v", err)
	}
}
