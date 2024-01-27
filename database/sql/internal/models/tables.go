package models

import "time"

type User struct {
	UserID   uint     `gorm:"primaryKey;autoIncrement;not null"`
	Nickname string   `gorm:"column:nickname;uniqueIndex;not null;type:varchar(60)"`
	IPv4     string   `gorm:"column:ip_v4;type:varchar(15)"`
	Password string   `gorm:"column:password;type:varchar(255)"`
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