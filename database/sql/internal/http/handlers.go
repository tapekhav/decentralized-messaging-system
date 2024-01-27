package handlers

import (
	"net/http"
	"time"

	"sql/internal/adapters"

	"github.com/gin-gonic/gin"
)

type Handlers struct {
	usersManager *api.UsersManager
}

func NewHandlers(dsn string) (*Handlers, error) {
	usersManager, err := api.NewUsersManager(dsn)
	if err != nil {
		return nil, err
	}

	return &Handlers{
		usersManager: usersManager,
	}, nil
}

func (h *Handlers) Register(r *gin.Engine) {
	userGroup := r.Group("/users") 
	{
		userGroup.POST("/", h.insertUser)
		userGroup.GET("/:nickname", h.getUser)
		userGroup.GET("/getAll", h.getAllUsers)
		userGroup.PUT("/:nickname", h.updateUser)
		userGroup.DELETE("/:nickname", h.deleteUser)
	}
}

func (h *Handlers) insertUser(c *gin.Context) {
	var userInsertRequest struct {
		Nickname 			  string 	`json:"nickname" binding:"required"`
		IPv4 				  string 	`json:"ipv4" binding:"required"`
		Password 			  string 	`json:"password" binding:"required"`
		Name                  string    `json:"name" binding:"required"`
		BirthDate             time.Time `json:"birthDate" binding:"required"`
		AdditionalInformation string    `json:"additionalInformation" binding:"required"`
	}

	err := c.ShouldBindJSON(&userInsertRequest)
	if err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}

	error := h.usersManager.InsertUser(
		userInsertRequest.Nickname,
		userInsertRequest.IPv4,
		userInsertRequest.Password,
		userInsertRequest.Name,
		userInsertRequest.BirthDate,
		userInsertRequest.AdditionalInformation,
	)
	
	if error != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": "Failed to create user"})
		return
	}

	c.JSON(http.StatusOK, gin.H{"message": "User inserted successfully"})
}

func (h *Handlers) getUser(c *gin.Context) {
	nickname := c.Param("nickname")
	user, err := h.usersManager.SelectUser(nickname)

	if err != nil {
		c.JSON(http.StatusNotFound, gin.H{"error": "User not found"})
		return
	}

	c.JSON(http.StatusOK, user)
}

func (h *Handlers) getAllUsers(c *gin.Context) {
	users, err := h.usersManager.SelectAllUsers()

	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": "Failed to fetch users"})
		return
	}

	c.JSON(http.StatusOK, users)
}

func (h *Handlers) updateUser(c *gin.Context) {
	nickname := c.Param("nickname")

	var userUpdateRequest struct {
		IPv4                  string    `json:"ipv4"`
		Name                  string    `json:"name"`
		Password			  string 	`json:"password"`
		BirthDate             time.Time `json:"birthDate"`
		AdditionalInformation string    `json:"additionalInformation"`
	}

	if err := c.ShouldBindJSON(&userUpdateRequest); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}

	err := h.usersManager.UpdateUserByNickname(
		nickname,
		userUpdateRequest.IPv4,
		userUpdateRequest.Password,
		userUpdateRequest.Name,
		userUpdateRequest.BirthDate,
		userUpdateRequest.AdditionalInformation,
	)

	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": "Failed to update user"})
		return
	}

	c.JSON(http.StatusOK, gin.H{"message": "User updated successfully"})
}

func (h *Handlers) deleteUser(c *gin.Context) {
	nickname := c.Param("nickname")

	err := h.usersManager.DeleteUser(nickname)

	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": "Failed to delete user"})
		return
	}

	c.JSON(http.StatusOK, gin.H{"message": "User deleted successfully"})
}