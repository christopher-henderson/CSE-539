package main

import "log"

var key = []byte(`MIIF4TCCBMmgAwIB`)

var plaintext = []byte(`this is a super embarassing secret you guys seriously don't tell mom`)

func xor(text, key []byte) []byte {
	r := make([]byte, len(text))
	for i, c := range text {
		r[i] = c ^ key[i%len(key)]
	}
	return r
}

func breakXor(c []byte) []byte {
	candidate := make([]byte, len(c))
	copy(candidate, c)
	shiftright(candidate)
	coincidence := countCoincedences(candidate, c)
	count := 1
	for ; coincidence < 0.06; count++ {
		shiftright(candidate)
		coincidence = countCoincedences(candidate, c)
	}
	log.Println(count, len(key))
	result := xor(c, candidate)

	cmp(result, count)

	return result
}

func cmp(r []byte, shift int) {
	p2 := make([]byte, len(plaintext))
	copy(p2, plaintext)
	shiftrightCount(p2, shift)
	r2 := xor(plaintext, p2)
	log.Println(r)
	log.Println(r2)
}

func shiftright(c []byte) {
	shiftrightCount(c, 1)
}

func shiftrightCount(c []byte, count int) {
	for i := 0; i < count; i++ {
		prev := c[len(c)-1]
		var tmp byte
		for i, _ := range c {
			tmp = c[i]
			c[i] = prev
			prev = tmp
		}
	}
}

func shiftleft(c []byte) {
	for i, b := range c {
		c[i] = b << 1
	}
}

func countCoincedences(a, b []byte) float64 {
	count := 0
	for i, c := range a {
		if b[i] == c {
			count++
		}
	}
	return float64(count) / float64(len(a))
}

func main() {
	// panic("asd")
	log.SetFlags(log.Lshortfile | log.LstdFlags)
	ciphertext := xor(plaintext, key)
	// decrypted := xor(ciphertext, key)
	// log.Println(string(ciphertext))
	b := breakXor(ciphertext)
	log.Println(b)
}
