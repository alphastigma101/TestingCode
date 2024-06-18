#ifndef UPGRADE_H
#define UPGRADE_H
#include <map>
typedef std::map<const char*, const char*> kmap;
class Kernel {
    public:
        Kernel();
        char* search();
        void Parser();
        int upgrade();
    private:
        static kmap skmap;
        static std::map<const char*, kmap> urls;
};

#endif 
