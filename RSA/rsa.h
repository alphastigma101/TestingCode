#include <openssl/rsa.h>                                                                         
class RSA {                                                                                                  
    public:
        void rsa_encrypt(const uint8_t *plaintext, size_t plaintext_len, RSA *key, uint8_t *ciphertext); 
        void rsa_decrypt(const uint8_t *ciphertext, size_t ciphertext_len, RSA *key, uint8_t *plaintext); 
    private:
        //
};
