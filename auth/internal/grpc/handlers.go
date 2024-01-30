package server

import (
	"context"
	"log"

	pb "auth/pb"
	"google.golang.org/protobuf/types/known/emptypb"
)

type AuthServiceServer struct {
	pb.UnimplementedAuthServiceServer
}

func (s *AuthServiceServer) SignUp(ctx context.Context, req *pb.SignUpRequest) (*emptypb.Empty, error) {
	log.Printf("Received signup request: %v", req)
	return &emptypb.Empty{}, nil
}

func (s *AuthServiceServer) LogIn(ctx context.Context, req *pb.LogInRequest) (*emptypb.Empty, error) {
	log.Printf("Received login request: %v", req)
	return &emptypb.Empty{}, nil
}
