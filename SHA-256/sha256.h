#ifdef SHA256_H
#define SHA256_H
#include <openssl/sha.h>
class sha256 {
    public:
        sha256();
        void sha256(const uint8_t *data, size_t len, uint8_t *hash);
    private:
        //
};
#endif
