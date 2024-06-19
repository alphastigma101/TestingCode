#ifdef CAMELLIA_H
#define CAMELLIA_H
#include <openssl/camellia.h>
class camellia {
    public:
        void camellia_encrypt(const uint8_t *plaintext, size_t len, const uint8_t *key, uint8_t *ciphertext);
        void camellia_decrypt(const uint8_t *ciphertext, size_t len, const uint8_t *key, uint8_t *plaintext);
    private:
        //
};
#endif
