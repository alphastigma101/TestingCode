#ifdef CHAOSBASED_H
#define CHAOSBASHED_H
class chaosbased {
    public:
        void chaos_encrypt(const uint8_t *plaintext, size_t len, const uint8_t *key, uint8_t *ciphertext);
        void chaos_decrypt(const uint8_t *ciphertext, size_t len, const uint8_t *key, uint8_t *plaintext);
    private:
        //
};
#endif
