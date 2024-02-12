package server

import (
	"context"
	"log"
	"time"

	"auth/internal/jwt"
	"auth/internal/models"

	pb "auth/pb"

	"google.golang.org/protobuf/types/known/emptypb"
)

type AuthServiceServer struct {
	pb.UnimplementedAuthServiceServer

	hasher    jwt.Hasher
	jwtRepo   jwt.JWTRepo
	usersRepo jwt.UsersRepo
	app       jwt.AccessRefresh
}

func (s *AuthServiceServer) SignUp(
	ctx context.Context,
	req *pb.SignUpRequest,
) (*emptypb.Empty, error) {
	log.Printf("Received signup request: %v", req)

	layout := "2006-01-02T15:04:05Z"
	parsedTime, _ := time.Parse(layout, req.GetBirthDate())

	s.usersRepo.RegisterUser(
		req.GetNickname(),
		req.GetPassword(),
		req.GetIp(),
		parsedTime,
		req.GetName(),
		req.GetAdditionalInformation(),
	)

	hashPassword, err := s.hasher.HashFunction(req.GetPassword())
	if err != nil {
		return &emptypb.Empty{}, err
	}

	s.jwtRepo.CreateUser(ctx, models.UserAuthRequest{
		Nickname: req.GetNickname(),
		Password: hashPassword,
	})

	jwt, err := s.app.GenerateJWTPair(ctx, req.GetNickname())
	if err != nil {
		return &emptypb.Empty{}, err
	}

	log.Printf(
		"access: %s,\nrefresh: %s\n",
		jwt.Access,
		jwt.Refresh,
	)

	return &emptypb.Empty{}, nil
}

func (s *AuthServiceServer) RefreshAccessToken(
	ctx context.Context,
	req *pb.RefreshAccessTokenRequest,
) (*pb.RefreshAccessTokenResponce, error) {
	pair, err := s.app.RefreshAccessToken(ctx, req.GetRefreshToken())
	if err != nil {
		return &pb.RefreshAccessTokenResponce{}, nil
	}

	return &pb.RefreshAccessTokenResponce{
		Access:  pair.Access,
		Refresh: pair.Refresh,
	}, nil
}

func (s *AuthServiceServer) LogIn(
	ctx context.Context,
	req *pb.LogInRequest,
) (*emptypb.Empty, error) {
	log.Printf("Received login request: %v", req)

	hashPassword, err := s.hasher.HashFunction(req.GetPassword())

	if err != nil {
		return &emptypb.Empty{}, err
	}

	s.jwtRepo.FindUser(ctx, models.UserAuthRequest{
		Nickname: req.GetNickname(),
		Password: hashPassword,
	})

	jwt, err := s.app.GenerateJWTPair(ctx, req.GetNickname())
	if err != nil {
		return &emptypb.Empty{}, err
	}

	log.Printf(
		"access: %s,\nrefresh: %s\n",
		jwt.Access,
		jwt.Refresh,
	)

	return &emptypb.Empty{}, nil
}

func NewAuthServiceService(
	hasher jwt.Hasher,
	jwtRepo jwt.JWTRepo,
	usersRepo jwt.UsersRepo,
	app jwt.AccessRefresh,
) *AuthServiceServer {
	return &AuthServiceServer{
		app:       app,
		hasher:    hasher,
		jwtRepo:   jwtRepo,
		usersRepo: usersRepo,
	}
}
