#ifdef TWOFISH_H
#define TWOFISH_H
#include <twofish.h>
class twofish {
    public:
        void twofish_encrypt(const uint8_t *plaintext, size_t len, const uint8_t *key, uint8_t *ciphertext);
        void twofish_decrypt(const uint8_t *ciphertext, size_t len, const uint8_t *key, uint8_t *plaintext);
    private:
        //

};

#endif
