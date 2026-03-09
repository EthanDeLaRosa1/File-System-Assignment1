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
    FILE *fp = fopen("students.db", "rb");
    if (fp == NULL) {
        cout << "Error opening file." << endl;
        return 1;
    }

    Student s;

    // jump directly to record #3
    fseek(fp, 2 * sizeof(Student), SEEK_SET);

    fread(&s, sizeof(Student), 1, fp);

    fclose(fp);

    cout << "Record #3:" << endl;
    cout << "ID: " << s.id << endl;
    cout << "Name: " << s.name << endl;
    cout << "Age: " << s.age << endl;

    return 0;
}