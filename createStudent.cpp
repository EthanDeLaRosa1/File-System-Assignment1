#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;

struct Student {
    int id;
    char name[50];
    int age;
};

int main() {
    FILE *fp = fopen("students.db", "wb");
    if (fp == NULL) {
        cout << "Error creating file." << endl;
        return 1;
    }

    Student students[5] = {
        {1, "Alice", 20},
        {2, "Bob", 21},
        {3, "Charlie", 22},
        {4, "David", 23},
        {5, "Emma", 24}
    };

    fwrite(students, sizeof(Student), 5, fp);

    fclose(fp);

    cout << "5 student records written to students.db" << endl;
    return 0;
}