package crypto

import (
	"errors"
	"fmt"

	"golang.org/x/crypto/bcrypt"
)


type BCryptHasher struct {
	costFactor uint8
}

func (h BCryptHasher) HashFunction(hashingString string) (string, error) {
	hash, err := bcrypt.GenerateFromPassword([]byte(hashingString), int(h.costFactor))
	if err != nil {
		return "", err
	}

	return string(hash), nil
}

func (h BCryptHasher) CompareString(hash, originalString string) error {
	err := bcrypt.CompareHashAndPassword([]byte(hash), []byte(originalString))
	
	if errors.Is(err, bcrypt.ErrMismatchedHashAndPassword) {
		return fmt.Errorf("Original string: %s and hash: %s mismatch", 
						  originalString, 
						  hash)
	} else if err != nil {
		return err
	}

	return nil
}

func New(cost uint8) BCryptHasher {
	return BCryptHasher{
		costFactor: cost, 
	}
}
