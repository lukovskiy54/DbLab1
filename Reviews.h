#ifndef BMS_DATABASE_REVIEWS_H
#define BMS_DATABASE_REVIEWS_H






# define file_s "reviews_file_s.bin"

struct Review{
    int id;
    int publication_id;
    char reviewer_name[20];
    char comment[2000];
    char date[12];
    int next_address;
    int deleted;
};









#endif //BMS_DATABASE_REVIEWS_H
