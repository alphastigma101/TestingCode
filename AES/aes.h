 #include <stdint.h>                                                                  
#ifdef AES_H
#define AES_H

class AES {
    public:
        AES();
        void aes_encrypt(const uint8_t *plaintext, const uint8_t *key, uint8_t *ciphertext); 
        void aes_decrypt(const uint8_t *ciphertext, const uint8_t *key, uint8_t *plaintext);
    private:
        //
};
#endif 
