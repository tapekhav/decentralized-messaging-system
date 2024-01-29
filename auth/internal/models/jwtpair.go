package models

type JWTPair struct {
	Access  string
	Refresh string
}

func NewJWTPair(access, refresh string) JWTPair {
	return JWTPair{
		Access:  access,
		Refresh: refresh,
	}
}
