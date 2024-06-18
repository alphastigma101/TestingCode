#ifndef UPGRADE_H
#define UPGRADE_H
#include <map>
#include <vector>
class Software {
    public:
        // Create the function and other signatures
        Software();
        char* search();
        int install(const char* root);
        void cd();
    private:
        std::map<const char*, const char*> dmap; // Store the directories where the software failed to install.. need to use try and except statements 
        std::vector<const char*> nodes; // store the directories here
};

#endif 
