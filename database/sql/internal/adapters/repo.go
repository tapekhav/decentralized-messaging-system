package api

import (
	"fmt"
	"time"

	"gorm.io/driver/postgres"
	"gorm.io/gorm"

	"sql/internal/models"
)

type UsersManager struct {
	db *gorm.DB
}

func NewUsersManager(dsn string) (*UsersManager, error) {
	db, err := gorm.Open(postgres.Open(dsn), &gorm.Config{})
	if err != nil {
		return nil, err
	}

	db.AutoMigrate(&models.User{}, &models.UserInfo{})

	return &UsersManager{db: db}, nil
}

func (m *UsersManager) InsertUser(nickname, ipv4, password, name string,
	birthDate time.Time,
	additionalInformation string) error {
	user := models.User{
		Nickname: nickname,
		IPv4:     ipv4,
		Password: password,
	}

	userInfo := models.UserInfo{
		Name:                  name,
		BirthDate:             birthDate,
		AdditionalInformation: additionalInformation,
	}

	if err := m.db.Create(&user).Error; err != nil {
		fmt.Println("Error during Inserting in Users:", err)
		return err
	}

	if err := m.db.Create(&userInfo).Error; err != nil {
		fmt.Println("Error during Inserting in UserInfo:", err)
		return err
	}

	return nil
}

func (m *UsersManager) SelectUser(nickname string) (models.User, error) {
	var user models.User
	err := m.db.
		Joins("JOIN \"UserInfo\" ON \"Users\".user_id = \"UserInfo\".user_id").
		Preload("UserInfo").
		Where("\"Users\".nickname = ?", nickname).
		First(&user).Error

	return user, err
}

func (m *UsersManager) DeleteUser(nickname string) error {
	return m.db.Delete(nickname).Error
}

func (m *UsersManager) UpdateUserByNickname(nickname, ipv4, password, name string,
	birthDate time.Time,
	additionalInformation string) error {
	var user models.User

	err := m.db.Preload("UserInfo").First(&user, nickname).Error
	if err != nil {
		return err
	}

	user.IPv4 = ipv4
	user.Password = password
	user.UserInfo.Name = name
	user.UserInfo.BirthDate = birthDate
	user.UserInfo.AdditionalInformation = additionalInformation

	return m.db.Save(&user).Error
}

func (m *UsersManager) SelectAllUsers() ([]models.User, error) {
	var users []models.User

	err := m.db.
		Joins("JOIN \"UserInfo\" ON \"Users\".user_id = \"UserInfo\".user_id").
		Preload("UserInfo").
		Find(&users).Error

	return users, err
}

func (m *UsersManager) ModifyingRawQuery(query string,
										 values ...interface{}) error {
	result := m.db.Exec(query, values...)

	return result.Error
}

func (m *UsersManager) SelectRawSQLQuery(query string,
										 values ...interface{}) ([]models.User, error) {
	var users []models.User
	result := m.db.Raw(query, values...).Scan(&users)

	return users, result.Error
}
