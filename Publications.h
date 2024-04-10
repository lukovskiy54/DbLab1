#ifndef BMS_DATABASE_PUBLICATIONS_H
#define BMS_DATABASE_PUBLICATIONS_H




#include <string>

# define file_m "publications_file_m.bin"

struct Publication {
    int id;
    char name[20];
    char description[100];
    char date[12];
    char author[20];
    int journalId;
    int first_review;
};

#endif //BMS_DATABASE_PUBLICATIONS_H
