#ifdef BLOWFISH_H
#define BLOWFISH_H
#include <openssl/blowfish.h>                                                                                                                                                                          
class blowfish {
    public:
        blowfish();
        void blowfish_encrypt(const uint8_t *plaintext, size_t len, const uint8_t *key, uint8_t *ciphertext);
        void blowfish_decrypt(const uint8_t *ciphertext, size_t len, const uint8_t *key, uint8_t *plaintext);
    private:
        //

};
#endif
