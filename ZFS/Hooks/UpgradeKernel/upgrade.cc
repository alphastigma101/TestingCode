#include "upgrade.h"
// Initialize static members outside the class
kmap Kernel::skmap = {
    {"Archives", "https://archive.archlinux.org/packages/l/"},
    {"ZFS", "https://wiki.archlinux.org/title/ZFS"}
};

std::map<const char*, kmap> Kernel::urls = {
    {"ArchLinux", Kernel::skmap}
};


char* Kernel::search() {
    // Need to use the gumbo parser to parse the web in search for upgrade
    // Return it 
    return (char*)('\0');
}

void Kernel::Parser() {
    // Parse whatever the search function returns 
}


int Kernel::upgrade() {
    // Upgrade it if possible 
    return 0;
}

