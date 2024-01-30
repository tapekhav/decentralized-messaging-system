package handlers

import (
	"net/http"

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

func (h *Handlers) insertUser(c *gin.Context) {
	var insertRequest userInsertRequest

	err := c.ShouldBindJSON(&insertRequest)
	if err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}

	error := h.usersManager.InsertUser(
		insertRequest.Nickname,
		insertRequest.IPv4,
		insertRequest.Password,
		insertRequest.Name,
		insertRequest.BirthDate,
		insertRequest.AdditionalInformation,
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

	var updateRequest userUpdateRequest

	if err := c.ShouldBindJSON(&updateRequest); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}

	err := h.usersManager.UpdateUserByNickname(
		nickname,
		updateRequest.IPv4,
		updateRequest.Password,
		updateRequest.Name,
		updateRequest.BirthDate,
		updateRequest.AdditionalInformation,
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