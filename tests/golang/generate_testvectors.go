package main

// TODO: make the output deterministic so that we can re-run this script without
// all vectors changing

import (
	"crypto/ecdsa"
	"crypto/elliptic"
	"crypto/rand"
	"crypto/sha256"
	"fmt"
	mr "math/rand"
	"os"
	"strings"
	"text/template"
)

type TestData struct {
	Explanation string
	Message     string
	Pubkey      string
	Signature   string
	Valid       bool
}

func ToCArray(x []byte) string {
	s := fmt.Sprintf("%d ", x)
	s = strings.TrimSpace(s)
	s = strings.Replace(s, " ", ",", -1)
	s = strings.Replace(s, "[", "", -1)
	s = strings.Replace(s, "]", "", -1)
	return s
}

func randomMessage(len int) []byte {
	randomBadMsg := make([]byte, 4)
	_, err := rand.Read(randomBadMsg)
	if err != nil {
		panic(err)
	}
	return randomBadMsg
}

func generateTestData(explanation string) TestData {
	privateKey, err := ecdsa.GenerateKey(elliptic.P256(), rand.Reader)
	if err != nil {
		panic(err)
	}

	msgLen := mr.Intn(100)
	msg := randomMessage(msgLen)
	hash := sha256.Sum256(msg)

	sig_r, sig_s, err := ecdsa.Sign(rand.Reader, privateKey, hash[:])
	if err != nil {
		panic(err)
	}

	sig := make([]byte, 64)
	sig_r.FillBytes(sig[0:32])
	sig_s.FillBytes(sig[32:64])

	pk := privateKey.PublicKey

	publicKeyBytes := elliptic.Marshal(pk.Curve, pk.X, pk.Y)

	return TestData{
		Explanation: explanation,
		Message:     ToCArray(msg),
		Pubkey:      ToCArray(publicKeyBytes),
		Signature:   ToCArray(sig),
		Valid:       true,
	}
}

func generateInvalidTestData(explanation string) TestData {
	testData := generateTestData(explanation)
	testData.Message = ToCArray(randomMessage(4))
	testData.Valid = false
	return testData
}

func print(testData TestData) {
	t := template.Must(template.New("t").Parse(`
TEST_CASE(" {{.Explanation}} ") {
    byte_vec_t msg { {{.Message}} };
    byte_vec_t sig { {{.Signature}} };
    byte_vec_t pk { {{.Pubkey}} };

    REQUIRE( p256_verify(msg.data(),
                            static_cast< unsigned >(msg.size()),
                            sig.data(),
                            pk.data()) == {{if .Valid}} P256_SUCCESS {{else}} P256_INVALID_SIGNATURE {{end}}  );
    }

`))
	t.Execute(os.Stdout, testData)
}

func main() {
	for i := 0; i < 1000; i++ {
		print(generateTestData(fmt.Sprintf("test golang valid %d", i)))
		print(generateInvalidTestData(fmt.Sprintf("test golang invalid %d", i)))
	}
}
