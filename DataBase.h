
#ifndef BMS_DATABASE_DATABASE_H
#define BMS_DATABASE_DATABASE_H
#include "GarbageCollector.h"
#include "Publications.h"
#include "Reviews.h"
class DataBase {
public:
    static void init_files();
    void init_collector();
    void load_collector(bool mode);
    void sort_collector();
    int get_collector_address(int key);
    int get_collector_index(int key);

    void insert_m(Publication* publication);
    void insert_s(Review* review);

    void delete_m(int id);
    void delete_s(int key_id_m, int key_id_s);

    void get_m(int key_id);
    void get_s(int key_id_m, int key_id_s);

    void update_m(int id, char name[20], char description[100],char date[12],char author[20],int journalId);
    void update_s(int key_id_m, int key_id_s, char reviewer_name[20], char comment[2000], char date[11]);
    void calc_m(int id);
    void calc_m();
    void calc_s();
    void ut_m();
    void ut_s() const;

private:
    collectorElement collector[MAX_SIZE]{};

    int cnt_publications = 0;
    int del_publications = 0;

    int cnt_review = 0;
    int del_review = 0;

};
#endif //BMS_DATABASE_DATABASE_H
