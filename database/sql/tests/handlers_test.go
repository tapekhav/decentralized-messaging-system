package handlers_test

import (
	"bytes"
	"fmt"
	"net/http"
	"net/http/httptest"
	"testing"

	"github.com/gin-gonic/gin"

	handlers "sql/internal/http"
)

func TestHandlers_InsertUser(t *testing.T) {
	h, err := handlers.NewHandlers("host=127.0.0.1 user=postgres dbname=users sslmode=disable password=1234 port=5433")
	if err != nil {
		t.Fatal(err)
	}

	router := gin.New()
	h.Register(router)

	recorder := httptest.NewRecorder()
	payload := []byte(`{"nickname": "testuser", "ipv4": "192.168.1.1", "password":1234, "name": "Test User", "birthDate": "2003-06-27T12:00:00Z", "additionalInformation": "info"}`)

	request, err := http.NewRequest("POST", "/users/", bytes.NewBuffer(payload))
	if err != nil {
		t.Fatal(err)
	}

	request.Header.Set("Content-Type", "application/json")

	router.ServeHTTP(recorder, request)

	if recorder.Result().StatusCode != http.StatusOK {
		t.Errorf("Expected status code 200, but got %d", recorder.Result().StatusCode)
		fmt.Println(recorder.Body.String())
	}
}

/*
func TestHandlers_GetUser(t *testing.T) {
    h, _ := NewHandlers("host=127.0.0.1 user=postgres dbname=users sslmode=disable password=1234 port=5433")
    router := gin.New()
    h.Register(router)

    recorder := httptest.NewRecorder()

    request, err := http.NewRequest("GET", "/users/test4", nil)
    if err != nil {
        t.Fatal(err)
    }

    request.Header.Set("Content-Type", "application/json")

    router.ServeHTTP(recorder, request)

    assert.Equal(t, http.StatusOK, recorder.Result().StatusCode)
}
*/
