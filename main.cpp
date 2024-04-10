
#include <iostream>
#include "DataBase.h"
#include <cstring>
void insert_publication(DataBase& data_base) {
    int id;
    char name[20];
    char description[100];
    char date[12];
    char author[20];
    int journalId;
    int first_review = -1; // Assuming no reviews at the time of creation

    std::cout << "Enter publication id: ";
    std::cin >> id;
    std::cout << "Enter publication name: ";
    std::cin >> name;
    std::cout << "Enter publication description: ";
    std::cin >> description;
    std::cout << "Enter publication date: ";
    std::cin >> date;
    std::cout << "Enter publication author: ";
    std::cin >> author;
    std::cout << "Enter publication journalId: ";
    std::cin >> journalId;

    Publication publication = {.id = id, .name = "" , .description = "", .date = "" , .author = "" , .journalId = journalId, .first_review = first_review};
    strcpy(publication.name, name);
    strcpy(publication.description, description);
    strcpy(publication.date, date);
    strcpy(publication.author, author);
    data_base.insert_m(&publication);
}
void insert_review(DataBase& data_base) {
    int id;
    int publication_id;
    char reviewer_name[20];
    char comment[2000];
    char date[12];
    int next_address = -1; // Assuming no next review at the time of creation
    int deleted = 0; // Assuming the review is not deleted at the time of creation

    std::cout << "Enter review id: ";
    std::cin >> id;
    std::cout << "Enter publication id for the review: ";
    std::cin >> publication_id;
    std::cout << "Enter reviewer name: ";
    std::cin >> reviewer_name;
    std::cout << "Enter comment: ";
    std::cin >> comment;
    std::cout << "Enter date: ";
    std::cin >> date;

    Review review = {.id = id, .publication_id = publication_id, .reviewer_name = "", .comment = "", .date = "", .next_address = next_address, .deleted = deleted};
    strcpy(review.reviewer_name, reviewer_name);
    strcpy(review.comment, comment);
    strcpy(review.date, date);
    data_base.insert_s(&review);
}
void delete_publication(DataBase& data_base) {
    int id;
    std::cout << "Enter the id of the publication you want to delete: ";
    std::cin >> id;
    data_base.delete_m(id);
}
void delete_review(DataBase& data_base) {
    int publication_id;
    int review_id;
    std::cout << "Enter the id of the publication for which you want to delete a review: ";
    std::cin >> publication_id;
    std::cout << "Enter the id of the review you want to delete: ";
    std::cin >> review_id;
    data_base.delete_s(publication_id, review_id);
}
void get_publication(DataBase& data_base) {
    int id;
    std::cout << "Enter the id of the publication you want to get: ";
    std::cin >> id;
    data_base.get_m(id);
}
void get_review(DataBase& data_base) {
    int publication_id;
    int review_id;
    std::cout << "Enter the id of the publication for which you want to get a review: ";
    std::cin >> publication_id;
    std::cout << "Enter the id of the review you want to get: ";
    std::cin >> review_id;
    data_base.get_s(publication_id, review_id);
}
void update_publication(DataBase& data_base) {
    int id;
    char name[20];
    char description[100];
    char date[12];
    char author[20];
    int journalId;

    std::cout << "Enter the id of the publication you want to update: ";
    std::cin >> id;
    std::cout << "Enter new publication name: ";
    std::cin >> name;
    std::cout << "Enter new publication description: ";
    std::cin >> description;
    std::cout << "Enter new publication date: ";
    std::cin >> date;
    std::cout << "Enter new publication author: ";
    std::cin >> author;
    std::cout << "Enter new publication journalId: ";
    std::cin >> journalId;

    data_base.update_m(id, name, description, date, author, journalId);
}
void update_review(DataBase& data_base) {
    int publication_id;
    int review_id;
    char reviewer_name[20];
    char comment[2000];
    char date[12];

    std::cout << "Enter the id of the publication for which you want to update a review: ";
    std::cin >> publication_id;
    std::cout << "Enter the id of the review you want to update: ";
    std::cin >> review_id;
    std::cout << "Enter new reviewer name: ";
    std::cin >> reviewer_name;
    std::cout << "Enter new comment: ";
    std::cin >> comment;
    std::cout << "Enter new date: ";
    std::cin >> date;

    data_base.update_s(publication_id, review_id, reviewer_name, comment, date);
}
void calc_reviews_for_publication(DataBase& data_base) {
    int publication_id;
    std::cout << "Enter the id of the publication for which you want to calculate the number of reviews: ";
    std::cin >> publication_id;
    data_base.calc_m(publication_id);
}
void calc_publications(DataBase& data_base) {
    data_base.calc_m();
}
void calc_reviews(DataBase& data_base) {
    data_base.calc_s();
}

void user_interface(DataBase& data_base) {
    int choice;
    do {
        std::cout << "\nMenu:\n";
        std::cout << "1. Insert publication\n";
        std::cout << "2. Insert review\n";
        std::cout << "3. Delete publication\n";
        std::cout << "4. Delete review\n";
        std::cout << "5. Get publication\n";
        std::cout << "6. Get review\n";
        std::cout << "7. Update publication\n";
        std::cout << "8. Update review\n";
        std::cout << "9. Calculate number of reviews for a publication\n";
        std::cout << "10. Calculate total number of publications\n";
        std::cout << "11. Calculate total number of reviews\n";
        std::cout << "12. Print all publications\n";
        std::cout << "13. Print all reviews\n";
        std::cout << "0. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                insert_publication(data_base);
                break;
            case 2:
                insert_review(data_base);
                break;
            case 3:
                delete_publication(data_base);
                break;
            case 4:
                delete_review(data_base);
                break;
            case 5:
                get_publication(data_base);
                break;
            case 6:
                get_review(data_base);
                break;
            case 7:
                update_publication(data_base);
                break;
            case 8:
                update_review(data_base);
                break;
            case 9:
                calc_reviews_for_publication(data_base);
                break;
            case 10:
                calc_publications(data_base);
                break;
            case 11:
                calc_reviews(data_base);
                break;
            case 12:
                data_base.ut_m();
                break;
            case 13:
                data_base.ut_s();
                break;
            case 0:
                std::cout << "Exiting...\n";
                break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 0);
}


int main() {
    printf("Author: Lukovskiy Maxim\n");
    DataBase data_base;
    DataBase::init_files();
    data_base.init_collector();


    int id[7] = {1,2,3,4,5, 6,7};
    char publication_name[7][20] = {"publication1_name", "publication2_name", "publication3_name", "publication4_name", "publication5_name", "publication6_name", "publication7_name"};
    char publication_description[7][100] = {"publication1_description", "publication2_description", "publication3_description", "publication4_description", "publication5_description", "publication6_description", "publication7_description"};
    char publication_date[7][12] = {"04/12/2016", "19/08/2022", "23/01/2017", "05/03/2007", "13/06/2004", "24/05/2002","12/05/2012"};
    char publication_author[7][20] = {"author1", "author2", "author3", "author4", "author5", "author6", "author7"};
    int publication_journalId[7] = {13,21,34,64,85, 62, 98};
    int first_review[7] = {-1, -1, -1, -1, -1, -1, -1};

    for(int i = 0; i < 5; i += 1){
        Publication publication = {.id = id[i], .name = "" , .description = "",.date = "" ,.author = "" ,.journalId = publication_journalId[i], .first_review = first_review[i]};
        strcpy(publication.name, publication_name[i]);
        strcpy(publication.description, publication_description[i]);
        strcpy(publication.date, publication_date[i]);
        strcpy(publication.author, publication_author[i]);
        data_base.insert_m(&publication);
    }


    int review_id[7] = {1, 2, 3, 4, 5, 6, 8};
    int publication_id[7] = {4, 5, 5, 5, 2, 4, 6};
    char reviewer_name[7][20] = {"Review1", "Review2", "Review3", "Review4", "Review5", "Review6", "Review7"};
    char comment[7][20] = {"Comment1", "Comment2", "Comment3", "Comment4", "Comment5", "Comment6", "Comment7"};
    char date[7][11] = {"04/12/2016", "19/08/2022", "23/01/2017", "05/03/2007", "13/06/2004", "24/05/2002", "24/05/2002"};



    for(int i = 0; i < 6; i += 1){
        Review review = {.id = review_id[i],.publication_id = publication_id[i] ,.reviewer_name = "", .comment = "", .date = "", .next_address = -1, .deleted = 0};
        strcpy(review.reviewer_name, reviewer_name[i]);
        strcpy(review.comment, comment[i]);
        strcpy(review.date, date[i]);
        data_base.insert_s(&review);
    }

    data_base.ut_m();
    data_base.ut_s();

    std::cout<<"DEL-M(4)\n";
    data_base.delete_m(4);
    data_base.ut_m();
    data_base.ut_s();

   std::cout<<"DEL-S(5,2)\n";
   data_base.delete_s(5,2);

    data_base.ut_m();
    data_base.ut_s();

    Publication publication = {.id = id[6], .name = "" , .description="", .date="", .author="", .journalId=publication_journalId[6], .first_review = -1};

    strcpy(publication.name, publication_name[6]);
    strcpy(publication.description, publication_description[6]);
    strcpy(publication.date, publication_date[6]);
    strcpy(publication.author, publication_author[6]);

    data_base.insert_m(&publication);
    Review new_review = {.id = 7,.publication_id = 7 ,.reviewer_name = "", .comment = "",.date="", .next_address = -1, .deleted = 0};
    strcpy(new_review.reviewer_name, reviewer_name[6]);
    strcpy(new_review.comment, comment[6]);
    strcpy(new_review.date, date[6]);
    data_base.insert_s(&new_review);

    std::cout<<"ADDED NEW ENTITIES\n";
    data_base.ut_m();
    data_base.ut_s();

    char new_name[20] = {"publication_another"};
    char new_description[100] = {"publication_another_description"};
    char new_date[12] = {"04/12/2016"};
    char new_author[20] = {"author_another"};

    int new_journalId = 13;



    data_base.update_m(7, new_name, new_description, new_date, new_author, new_journalId);

    char new_reviewer_name[20] = "Oleg";
    char new_comment[20] = "new_comment";
    char new_date_review[11] = "04/12/2016";
    data_base.update_s(7, 7, new_reviewer_name, new_comment, new_date_review);


    std::cout<<"UPDATED ENTITIES\n";
    data_base.ut_m();
    data_base.ut_s();

    user_interface(data_base);
    return 0;
}

