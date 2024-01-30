package handlers

import "time"

type userInsertRequest struct {
	Nickname 			  string 	`json:"nickname" binding:"required"`
	IPv4 				  string 	`json:"ipv4" binding:"required"`
	Password 			  string 	`json:"password" binding:"required"`
	Name                  string    `json:"name" binding:"required"`
	BirthDate             time.Time `json:"birthDate" binding:"required"`
	AdditionalInformation string    `json:"additionalInformation" binding:"required"`
}

type userUpdateRequest struct {
	IPv4                  string    `json:"ipv4"`
	Name                  string    `json:"name"`
	Password			  string 	`json:"password"`
	BirthDate             time.Time `json:"birthDate"`
	AdditionalInformation string    `json:"additionalInformation"`
}