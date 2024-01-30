package handlers

import "github.com/gin-gonic/gin"

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