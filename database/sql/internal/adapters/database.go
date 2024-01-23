package api

import (
	"fmt"
	"time"

	"gorm.io/driver/postgres"
	"gorm.io/gorm"
)

type User struct {
	UserID   uint     `gorm:"primaryKey;autoIncrement;not null"`
	Nickname string   `gorm:"column:nickname;uniqueIndex;not null;type:varchar(60)"`
	IPv4     string   `gorm:"column:ip_v4;type:varchar(15)"`
	UserInfo UserInfo `gorm:"foreignKey:UserID;references:UserID;constraint:OnUpdate:CASCADE,OnDelete:SET NULL;"`
}

type UserInfo struct {
	UserID                uint      `gorm:"primaryKey;uniqueIndex;not null;autoIncrement"`
	Name                  string    `gorm:"column:name;not null;type:varchar(60)"`
	BirthDate             time.Time `gorm:"column:birth_date"`
	AdditionalInformation string    `gorm:"column:additional_information;type:varchar(255)"`
}

func (User) TableName() string {
	return "Users"
}

func (UserInfo) TableName() string {
	return "UserInfo"
}

type UsersManager struct {
	db *gorm.DB
}

func NewUsersManager(dsn string) (*UsersManager, error) {
	db, err := gorm.Open(postgres.Open(dsn), &gorm.Config{})
	if err != nil {
		return nil, err
	}

	db.AutoMigrate(&User{}, &UserInfo{})

	return &UsersManager{db: db}, nil
}

func (m *UsersManager) InsertUser(nickname, ipv4, name string,
	birthDate time.Time,
	additionalInformation string) error {
	user := User{
		Nickname: nickname,
		IPv4:     ipv4,
	}

	userInfo := UserInfo{
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

func (m *UsersManager) SelectUser(nickname string) (User, error) {
	var user User
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

func (m *UsersManager) UpdateUserByNickname(nickname, ipv4, name string,
	birthDate time.Time,
	additionalInformation string) error {
	var user User

	err := m.db.Preload("UserInfo").First(&user, nickname).Error
	if err != nil {
		return err
	}

	user.IPv4 = ipv4
	user.UserInfo.Name = name
	user.UserInfo.BirthDate = birthDate
	user.UserInfo.AdditionalInformation = additionalInformation

	return m.db.Save(&user).Error
}

func (m *UsersManager) SelectAllUsers() ([]User, error) {
	var users []User

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
	values ...interface{}) ([]User, error) {
	var users []User
	result := m.db.Raw(query, values...).Scan(&users)

	return users, result.Error
}
