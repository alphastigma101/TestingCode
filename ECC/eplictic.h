#ifdef ELIPTIC_H
#define ELIPTIC_H
#include <openssl/ec.h>
class ECC {
    public:
        ECC();
        void ecc_encrypt(const uint8_t *plaintext, size_t plaintext_len, EC_KEY *key, uint8_t *ciphertext);
        void ecc_decrypt(const uint8_t *ciphertext, size_t ciphertext_len, EC_KEY *key, uint8_t *plaintext);
    private:
        //
};
#endif 
