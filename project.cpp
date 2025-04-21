#include <iostream>
using namespace std;

struct Player {
    char nama[50];
    char nickname[50];
    char role[30];
};

struct Tim {
    char namaTim[50];
    Player pemain[5]; 
};

void menu()
{
    cout << "silahkan pilih menu :" << endl;
    cout << "1. Masukkan data " << endl;
    cout << "2. Lihat daftar tim " << endl;
    cout << "3. Masukkan data skor tim" << endl;
    cout << "4. Sorting tim (berdasar skor total) " << endl;
    cout << "5. Searching(pemain dan tim)" << endl;
    cout << "6. Simpan hasil pertandingan " << endl;
    cout << "7. keluar " << endl;
}

int menu1()
{
    Tim tim;
    FILE *file;

    file = fopen("tim.txt", "ab"); 
    if (file == NULL) {
        cout << "Gagal membuka file untuk menyimpan!" << endl;
        return 1;
    }

    cin.ignore();
    cout << "Masukkan Nama Tim: ";
    cin.getline(tim.namaTim, 50);

    for (int i = 0; i < 5; i++) {
        cout << "\nPlayer ke-" << i + 1 << endl;
        cout << "Nama Asli          : ";
        cin.getline(tim.pemain[i].nama, 50);
        cout << "Nickname in game   : ";
        cin.getline(tim.pemain[i].nickname, 50);
        cout << "Role               : ";
        cin.getline(tim.pemain[i].role, 30);
    }

    fwrite(&tim, sizeof(Tim), 1, file);
    fclose(file);

    cout << "\nData tim dan 5 pemain berhasil disimpan ke tim.txt\n";
    return 0;
}
