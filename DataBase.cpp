
#include <cstdio>
#include <algorithm>


#include "DataBase.h"
#include "Publications.h"
#include "Reviews.h"
#include "GarbageCollector.h"

void DataBase::init_files(){
    FILE* publications_file_m = fopen(file_m, "wb+");
    fclose(publications_file_m);
    FILE* reviews_file_s = fopen(file_s, "wb+");
    fclose(reviews_file_s);
    FILE* indexes_file_i = fopen(file_i, "wb+");
    fclose(indexes_file_i);

}

void DataBase::init_collector(){
    for(int i = 0; i < MAX_SIZE; i += 1)
    {
        collector[i].key = -1;
        collector[i].address = -1;
        collector[i].deleted = 0;
    }
}

void  DataBase::load_collector(bool mode){ // load if mode == True else rewrite collector
    FILE * collector_file = fopen(file_i,mode ? "rb+" : "wb+");

    for (int i = 0; i < cnt_publications; i += 1) {
        mode ? fread(&collector[i].key, sizeof(int), 1, collector_file):fwrite(&collector[i].key, sizeof(int), 1, collector_file);
        mode ? fread(&collector[i].address, sizeof(int), 1, collector_file):fwrite(&collector[i].address, sizeof(int), 1, collector_file);
        mode ? fread(&collector[i].deleted, sizeof(int), 1, collector_file):fwrite(&collector[i].deleted, sizeof(int), 1, collector_file);
    }
    fclose(collector_file);
}

void DataBase::sort_collector(){
   std::qsort(collector,cnt_publications, sizeof(collectorElement),
              [](const void * a, const void * b)
              { return (((collectorElement *)a)->key  - ((collectorElement *)b)->key);}
    );
}

int DataBase::get_collector_address(int key) {

        for(int i = 0; i < cnt_publications + del_publications; i += 1){
            if (collector[i].key == key && collector[i].deleted == 0) {
                    return collector[i].address;
            }
        }

        return -1;

}

int DataBase::get_collector_index(int key) {
    for(int i = 0; i < cnt_publications + del_publications; i += 1){ // can be faster --> log(cnt_publications)
        if (collector[i].key == key && collector[i].deleted == 0) {
            return i;
        }
    }
    return -1;
}

void DataBase::insert_m(Publication* publication)
{
    for (int i = 0; i < cnt_publications+1; i+=1) {
        if (collector[i].key == publication->id) {
            printf("Reserved\n");
            return;
        }
    }
    FILE* publication_file_m = fopen(file_m, "rb+");
    load_collector(true);
    collector[cnt_publications + del_publications].key= publication -> id;
    collector[cnt_publications + del_publications].address = (cnt_publications + del_publications) * (int)sizeof(Publication);
    cnt_publications++;
    fseek(publication_file_m, 0, SEEK_END);
    fwrite(&publication->id, sizeof(int), 1, publication_file_m);
    fwrite(&publication->name, sizeof(publication->name), 1, publication_file_m);
    fwrite(&publication->description, sizeof(publication->description), 1, publication_file_m);
    fwrite(&publication->date, sizeof(publication->date), 1, publication_file_m);
    fwrite(&publication->author, sizeof(publication->author), 1, publication_file_m);
    fwrite(&publication->journalId, sizeof(int), 1, publication_file_m);
    fwrite(&publication->first_review, sizeof(int), 1, publication_file_m);
    sort_collector();
    load_collector(false);
    fclose(publication_file_m);
}

void DataBase::insert_s(Review* review)
{
    int flag = 0;
    for (int i = 0; i < cnt_publications + del_publications; i++) {
        if (collector[i].deleted != 1 && collector[i].key == review->publication_id ) {
            flag = 1;
        }
    }
    if(flag == 0){
        printf("Помилка\n");
        return;
    }

    FILE* reviews_file_s = fopen(file_s, "rb+");
    fseek(reviews_file_s, 0, SEEK_END);
    fwrite(&review->id, sizeof(int), 1, reviews_file_s);
    fwrite(&review->publication_id, sizeof (int), 1, reviews_file_s);
    fwrite(&review->reviewer_name, sizeof(review->reviewer_name), 1, reviews_file_s);
    fwrite(&review->comment, sizeof(review->comment), 1, reviews_file_s);
    fwrite(&review->date, sizeof(review->date), 1, reviews_file_s);
    fwrite(&review->next_address, sizeof(int), 1, reviews_file_s);
    fwrite(&review->deleted, sizeof(int), 1, reviews_file_s);

    FILE* publications_file_m = fopen(file_m, "rb+");
    int to_first_review_address = get_collector_address(review->publication_id) + 160;
    fseek(publications_file_m, to_first_review_address, SEEK_SET);

    int first_review_address;
    fread(&first_review_address, sizeof(int), 1, publications_file_m);

    int address = (cnt_review + del_review) * (int) sizeof(Review);
    if (first_review_address == -1) {
        fseek(publications_file_m, -4, SEEK_CUR);
        fwrite(&address, sizeof(int), 1, publications_file_m);
    } else {
        int next_review = first_review_address;
        while (next_review != -1) {
            fseek(reviews_file_s, next_review + 2040, SEEK_SET);
            fread(&next_review, sizeof(int), 1, reviews_file_s);
        }
        fseek(reviews_file_s, -4, SEEK_CUR);
        fwrite(&address, sizeof(int), 1, reviews_file_s);
    }
    cnt_review += 1;
    fclose(reviews_file_s);
    fclose(publications_file_m);
}


void DataBase::ut_m()
{
    FILE* publications_file_m = fopen(file_m, "rb+");
    printf("ut_m:\n");

    int id;
    char name[20];
    char description[100];
    char date[12];
    char author[20];
    int journalId;
    int first_review;

    for(int i = 0; i < cnt_publications + del_publications; i+=1)
    {
        if(collector[i].deleted == 0)
        {
            fseek(publications_file_m, get_collector_address(collector[i].key), SEEK_SET);
            fread(&id, sizeof(int), 1, publications_file_m);
            fread(&name, sizeof(name), 1, publications_file_m);
            fread(&description, sizeof(description), 1, publications_file_m);
            fread(&date, sizeof(date), 1, publications_file_m);
            fread(&author, sizeof(author), 1, publications_file_m);
            fread(&journalId, sizeof(int), 1, publications_file_m);
            fread(&first_review, sizeof(int), 1, publications_file_m);
            printf("Id: %d, Name: %s, Description: %s, Date: %s, Author: %s, JournalId: %d, First Review: %d\n", id, name, description, date, author, journalId, first_review);
        }
    }

    fclose(publications_file_m);


}

void DataBase::ut_s() const{
    FILE* reviews_file_s = fopen(file_s, "rb+");

    printf("ut_s:\n");
    int id;
    int publication_id;
    char reviewer_name[20];
    char comment[2000];
    char date[12];
    int next_address;
    int deleted;

    // printf("cnt filial: %d \n", cnt_review);
    for (int i = 0; i < cnt_review + del_review; i += 1) {
        fseek(reviews_file_s, 2044, SEEK_CUR);
        fread(&deleted, sizeof(int), 1, reviews_file_s);

        if(deleted == 0) {
            fseek(reviews_file_s, -2048, SEEK_CUR);
            fread(&id, sizeof(int), 1, reviews_file_s);
            fread(&publication_id, sizeof(int), 1, reviews_file_s);
            fread(&reviewer_name, sizeof(reviewer_name), 1, reviews_file_s);
            fread(&comment, sizeof(comment), 1, reviews_file_s);
            fread(&date, sizeof(date), 1, reviews_file_s);
            fread(&next_address, sizeof(int), 1, reviews_file_s);
            fread(&deleted, sizeof (int), 1, reviews_file_s);
            printf("Id: %d, Publication id: %d, Reviewer name: %s, Comment: %s, Date: %s, Next address: %d\n",
                   id, publication_id, reviewer_name, comment, date, next_address);
        }
    }

    fclose(reviews_file_s);
}

void DataBase::delete_m(int id)
{
    int tmp = get_collector_index(id);
    if (tmp == -1) {
        printf("Error, not presented id %d\n", id);
        return;
    }
    collector[tmp].deleted = 1;

    int offset = collector[tmp].address;
    FILE* publications_file_m = fopen(file_m, "rb+");
    int first_review;
    fseek(publications_file_m, offset + 160 , SEEK_SET);
    fread(&first_review, sizeof(int), 1, publications_file_m);
    fclose(publications_file_m);
    if(first_review != -1)
    {
        FILE* reviews_file_s = fopen(file_s, "rb+");
        int new_del = 1;
        fseek(reviews_file_s, first_review, SEEK_SET);
        int next_review = first_review;
        int current_address;
        while(next_review != -1)
        {
            current_address = next_review;
            fseek(reviews_file_s, current_address + 2044, SEEK_SET);
            fwrite(&new_del, sizeof(int), 1, reviews_file_s);
            del_review += 1;
            cnt_review -= 1;
            fseek(reviews_file_s, next_review + 2040, SEEK_SET);
            fread(&next_review, sizeof(int), 1, reviews_file_s);
        }

        fclose(reviews_file_s);

    }
    cnt_publications -= 1;
    del_publications += 1;
    sort_collector();
    load_collector(false);
    fclose(publications_file_m);
}

void DataBase :: delete_s(int key_id_m, int key_id_s) {
    int skip_m = get_collector_address(key_id_m);
    if (skip_m == -1) {
        printf("Error, key id %d\n", key_id_m);
        return;
    }

    FILE *publications_file_m = fopen(file_m, "rb+");
    //OPENING FILE WITH PUBLICATIONS
    fseek(publications_file_m, skip_m, SEEK_SET);
    //SET SEEK TO START OF PUBLICATION
    int first_review;
    fseek(publications_file_m, skip_m + 160, SEEK_SET);
    fread(&first_review, sizeof(int), 1, publications_file_m);
    //READING FIRST REVIEW ADDRESS

    if (first_review == -1) {
        printf("Publication without reviews\n");
        return;
    } else {
        FILE *reviews_file_s = fopen(file_s, "rb+");
        //OPENING FILE WITH REVIEWS
        int new_key_id_s; //id of FIRST review in reviews file
        int next_review_new = -1;

        int new_del = 1;

        int new_next_review_address = -1;
        int next_review_address = first_review;
        //NEXT REVIEW TO FIRST REVIEW OF PUBLICATION
        int current_address = first_review;


        fseek(reviews_file_s, first_review, SEEK_SET);
        fread(&new_key_id_s, sizeof(int), 1, reviews_file_s);
        //READING ID OF FIRST REVIEW
        fseek(reviews_file_s, first_review + 2040, SEEK_SET);
        fread(&next_review_address, sizeof(int), 1, reviews_file_s);

        if (new_key_id_s == key_id_s) {

            fseek(publications_file_m, skip_m + 160, SEEK_SET);
            fwrite(&next_review_address, sizeof(int), 1, publications_file_m);

            fseek(reviews_file_s, first_review + 2040, SEEK_SET);
            fwrite(&next_review_new, 4, 1, reviews_file_s);

            fwrite(&new_del, 4, 1, reviews_file_s);
            fflush(reviews_file_s);
            fflush(publications_file_m);
            cnt_review -= 1;
            del_review += 1;

            return;
        }else{
            while(next_review_address != -1){
                fseek(reviews_file_s, next_review_address, SEEK_SET);
                fread(&new_key_id_s, sizeof (int), 1, reviews_file_s);
                if (new_key_id_s == key_id_s)
                {
                    fseek(reviews_file_s, next_review_address + 2040, SEEK_SET);      //finding new nextAddress
                    fread(&new_next_review_address, sizeof(int), 1, reviews_file_s);

                    fseek(reviews_file_s, current_address + 2040, SEEK_SET);         // writing new nextAddress
                    fwrite(&new_next_review_address, sizeof(int), 1, reviews_file_s);

                    fseek(reviews_file_s, next_review_address + 2040, SEEK_SET);
                    fwrite(&next_review_new, 4, 1, reviews_file_s);
                    fwrite(&new_del, 4, 1, reviews_file_s);

                    fflush(reviews_file_s);

                    cnt_review -= 1;
                    del_review += 1;
                    return;
                }

                current_address = next_review_address;
                fseek(reviews_file_s, next_review_address + 2040, SEEK_SET);
                fread(&next_review_address, sizeof (int), 1, reviews_file_s);

            }
        }
        fclose(reviews_file_s);
    }
    fclose(publications_file_m);
}

void DataBase::update_m(int id, char name[20], char description[100],char date[12],char author[20],int journalId){
    int skip = get_collector_address(id);
    if (skip == -1){
        printf("Error update_m, key id %d\n", id);
        return;
    }

    FILE* publications_file_m = fopen(file_m, "rb+");
    fseek(publications_file_m, skip + 4, SEEK_SET);
    fwrite(name, 20, 1, publications_file_m);
    fwrite(description, 100, 1, publications_file_m);
    fwrite(date, 12, 1, publications_file_m);
    fwrite(author, 20, 1, publications_file_m);
    fwrite(&journalId, sizeof(int), 1, publications_file_m);
    fclose(publications_file_m);
}

void DataBase::update_s(int key_id_m, int key_id_s, char reviewer_name[20], char comment[2000], char date[11]) {
    int skip_m = get_collector_address(key_id_m);
    if (skip_m == -1) {
        printf("ERROR update_s, key id %d\n", key_id_m);
        return;
    }

    FILE *publications_file_m = fopen(file_m, "rb+");
    fseek(publications_file_m, skip_m, SEEK_SET);

    int first_review;
    fseek(publications_file_m, skip_m + 160, SEEK_SET);
    fread(&first_review, sizeof(int), 1, publications_file_m);

    fclose(publications_file_m);

    if (first_review == -1) {
        printf("ERROR, publications without reviews\n");
        return;
    } else {
        FILE *reviews_file_s = fopen(file_s, "rb+");
        fseek(reviews_file_s, first_review, SEEK_SET);
        int new_key_id_s;
        int next_review = first_review;
        int current_address = first_review;
        fread(&new_key_id_s, sizeof(int), 1, reviews_file_s);
        while (new_key_id_s != key_id_s) {
            if (next_review == -1) {
                printf("ERROR, update_s");
                return;
            }
            fseek(reviews_file_s, next_review, SEEK_SET);
            fread(&new_key_id_s, sizeof(int), 1, reviews_file_s);
            current_address = next_review;
            fseek(reviews_file_s, next_review + 2040, SEEK_SET);
            fread(&next_review, sizeof(int), 1, reviews_file_s);
        }


        fseek(reviews_file_s, current_address + 8, SEEK_SET);
        fwrite(reviewer_name, 20, 1, reviews_file_s);
        fwrite(comment, 2000, 1, reviews_file_s);

        fwrite(date, 12, 1, reviews_file_s);

        fclose(reviews_file_s);
    }
}

void DataBase::get_m(int key_id){
    int skip = get_collector_address(key_id);
    if (skip == -1) {
        printf("Error, key_id %d \n", key_id);
        return;
    }

    FILE* publications_file_m = fopen(file_m, "rb+");
    fseek(publications_file_m, skip + 4, SEEK_SET);
    int journalId;
    char name[20];
    char description[100];
    char date[12];
    char author[20];

    int  first_review;
    fread(&name, sizeof(name), 1, publications_file_m);
    fread(&description, sizeof(description), 1, publications_file_m);
    fread(&date, sizeof(date), 1, publications_file_m);
    fread(&author, sizeof(author), 1, publications_file_m);
    fread(&journalId, sizeof(int), 1, publications_file_m);
    fread(&first_review, sizeof(int), 1, publications_file_m);

    printf("Id: %d, Name: %s, Description: %s, Date: %s, Author: %s, JournalId: %d, First Review: %d\n", key_id, name, description, date, author, journalId, first_review);
    fclose(publications_file_m);
}

void DataBase:: get_s(int key_id_m, int key_id_s){
    int skip_m = get_collector_address(key_id_m);
    if (skip_m == -1) {
        printf("Error, key_id %d \n", key_id_m);
        return;
    }

    FILE* publications_file_m = fopen(file_m, "rb+");
    fseek(publications_file_m, skip_m, SEEK_SET);
    int  first_review;
    fseek(publications_file_m, skip_m + 160 , SEEK_SET);
    fread(&first_review, sizeof(int), 1, publications_file_m);
    fclose(publications_file_m);


    if(first_review == -1){
        printf("Publication without reviews\n");
        return;
    }
    else {
        FILE *review_file_s = fopen(file_s, "rb+");
        fseek(review_file_s, first_review, SEEK_SET);

        char reviewer_name[20];
        char comment[2000];
        char date[11];

        int new_key_id_s;
        int next_filial = first_review;
        int current_address = first_review;
        fread(&new_key_id_s, sizeof(int), 1, review_file_s);
        while (new_key_id_s != key_id_s) {
            if (next_filial == -1) {
                printf("Publication %d doesn't have reviews %d\n", key_id_m, key_id_s);
                return;
            }
            fseek(review_file_s, next_filial, SEEK_SET);
            fread(&new_key_id_s, sizeof(int), 1, review_file_s);
            current_address = next_filial;
            fseek(review_file_s, next_filial + 2040, SEEK_SET);
            fread(&next_filial, sizeof(int), 1, review_file_s);
        }
        int deleted = -1;
        fseek(review_file_s, current_address + 8, SEEK_SET);
        fread(reviewer_name, sizeof(reviewer_name), 1, review_file_s);
        fread(comment, sizeof(comment), 1, review_file_s);
        fread(date, sizeof(date), 1, review_file_s);

        fseek(review_file_s, sizeof(int), SEEK_CUR);
        fread(&deleted, sizeof(int), 1, review_file_s);

        if (deleted == 0) {
            printf("Id: %d, Publication id: %d, Reviewer name: %s, Comment: %s, Date: %s\n", key_id_s, key_id_m, reviewer_name, comment, date);
        } else {
            printf("Publication %d doesn't have reviews %d(prev_deleted)\n", key_id_m, key_id_s);
        }

        fclose(publications_file_m);

    }
    }

void DataBase::calc_m(int id) {
    int count = 0;
    FILE* reviews_file_s = fopen(file_s, "rb+");
    int publication_id;

    for (int i = 0; i < cnt_review + del_review; i++) {
        fseek(reviews_file_s, i * sizeof(Review) + sizeof(int), SEEK_SET);
        fread(&publication_id, sizeof(int), 1, reviews_file_s);
        if (publication_id == id) {
            count++;
        }
    }

    fclose(reviews_file_s);
    printf("Publication %d has %d reviews\n", id, count);
}
void  DataBase::calc_m() {
    printf("Total number of publications: %d\n", cnt_publications);
}
void  DataBase::calc_s() {
    printf("Total number of reviews: %d\n", cnt_review+1);
}

