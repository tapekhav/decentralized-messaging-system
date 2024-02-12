package database

import (
	"bytes"
	"encoding/json"
	"errors"
	"fmt"
	"io"
	"net/http"
	"time"

	"auth/internal/models"
)

type Database struct{}

func (db *Database) RegisterUser(nickname, password, Ipv4 string,
							     birthDate time.Time,
								 name, additionalInformation string) error {
	url := fmt.Sprintf("http://127.0.0.1:8080/users/%s", nickname)
	userData := models.NewUser(
		nickname,
		password,
		Ipv4,
		birthDate,
		name,
		additionalInformation,
	)

	jsonData, err := json.Marshal(userData)
	if err != nil {
		return err
	}

	responce, err := http.Post(url, "application/json", bytes.NewBuffer(jsonData))
	if err != nil {
		return err
	}
	defer responce.Body.Close()

	return nil
}

func (db *Database) AuthUser(nickname, password string) (models.User, error) {
	url := fmt.Sprintf("http://127.0.0.1:8080/users/%s", nickname)

	responce, err := http.Get(url)
	if err != nil {
		return models.User{}, err
	}
	defer responce.Body.Close()

	if responce.StatusCode == http.StatusOK {
		body, err := io.ReadAll(responce.Body)
		
		if err != nil {
			return models.User{}, err
		}

		var jsonData map[string]interface{}
		if err := json.Unmarshal(body, &jsonData); err != nil {
			return models.User{}, err
		}

		info, _ := jsonData["UserInfo"].(map[string]interface{})

		nickname, _ 	  := db.getDataFromJson(jsonData, "Nickname")
		pswd, _ 		  := db.getDataFromJson(jsonData, "Password")
		ipv4, _ 		  := db.getDataFromJson(jsonData, "IPv4")
		name, _ 		  := db.getDataFromJson(info, "Name")
		additionalInfo, _ := db.getDataFromJson(info, "AdditionalInformation")
		birthDate, _ 	  := jsonData["BirthDate"].(time.Time)

		return models.NewUser(
				nickname, 
				pswd, 
				ipv4, 
				birthDate, 
				name, 
				additionalInfo,
			), nil
	}

	return models.User{}, nil
}

func (db *Database) getDataFromJson(jsonData map[string]interface{},
	field string) (string, error) {
	f, ok := jsonData[field].(string)
	if !ok {
		return "", errors.New(fmt.Sprintf("Некорректный формат поля \"%s\" в JSON",
										  field))
	}

	return f, nil
}
