#include <iostream>
#include <cstring>
#include <iomanip>
using namespace std;

struct Player {
    char nama[50];
    char nickname[50];
    char role[30];
};

struct Tim {
    char namaTim[50];
    Player pemain[5]; 
    int skor;
    int matchW;
    int matchL;
    int gameW;
    int gameL;
};

struct Node {
    Tim data;
    Node* next;
};

Node *kepala = nullptr, *ekor = nullptr;

    FILE *file;
    Tim tim;

void simpanSemuaKeFile() {
    file = fopen("tim.txt", "wb");
    if (!file) {
        cout << "Gagal menyimpan ke file!\n";
        return;
    }

    Node* bantu = kepala;
    while (bantu) {
        fwrite(&(bantu->data), sizeof(Tim), 1, file);
        bantu = bantu->next;
    }

    fclose(file);
    cout << "Data berhasil disimpan ke peringkat.txt\n";
}

void clear(){
    cout << "Tekan tombol apapun untuk melanjutkan";
    cin.ignore();
    cin.ignore();
    system("cls");
}

void menu()
{
    cout << "silahkan pilih menu :" << endl;
    cout << "1. Masukkan data tim" << endl;
    cout << "2. Lihat daftar tim " << endl;
    cout << "3. Masukkan data skor tim" << endl;
    cout << "4. Sorting tim (berdasar skor total) " << endl;
    cout << "5. Searching(pemain dan tim)" << endl;
    cout << "6. keluar " << endl;
}

int menu1()
{
    file = fopen("tim.txt", "ab"); 
    if (file == NULL) {
        cout << "Gagal membuka file untuk menyimpan!" << endl;
        return 1;
    }

    int jumlahinput;
    cout << "Berapa banyak tim yang ingin diinput : ";
    cin >> jumlahinput;
    cin.ignore();


    for (int t = 0; t < jumlahinput; t++)
    {
        cout << "\ninput tim ke-" << t+1 << endl;
        cout << "Nasukkan Nama Tim : ";
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
    }
    fclose(file);
    cout << "\nseluruh data tim berhasil disimpan ke tim.txt\n";
    clear();
    return 0;
}

void menu2() {
    file = fopen("tim.txt", "rb");
    if (file == NULL) {
        cout << "Gagal membuka file untuk membaca!" << endl;
        return;
    }

    int nomorTim = 1;
    while (fread(&tim, sizeof(Tim), 1, file)) {
        cout << "===========================" << endl;
        cout << "Tim #" << nomorTim++ << endl;
        cout << "Nama Tim: " << tim.namaTim << endl;
        cout << "===========================" << endl;

        for (int i = 0; i < 5; i++) {
            cout << "  Player " << i + 1 << endl;
            cout << "    Nama     : " << tim.pemain[i].nama << endl;
            cout << "    Nickname : " << tim.pemain[i].nickname << endl;
            cout << "    Role     : " << tim.pemain[i].role << endl;
            cout << "---------------------------" << endl;
        }
    }

    fclose(file);
    clear();
}

void menu3()
{
    Tim daftartim[100];
    int jumlahtim = 0;
    char cariTim[50];
    int skorbaru;
    bool found = false;

    file = fopen("tim.txt", "rb");
    if (file == NULL)
    {
        cout << "Gagal membuka file untuk membaca!" << endl;
        return;
    }
    while (fread(&daftartim[jumlahtim], sizeof(Tim), 1, file))
    {
        jumlahtim++;
    }
    fclose(file);

    cin.ignore();
    cout << "Masukkan nama tim : ";
    cin.getline(cariTim, 50);

    for (int i = 0; i < jumlahtim; i++)
    {
        if (strcmp(daftartim[i].namaTim, cariTim) == 0)
        {
            cout << "\n Tim dengan nama " << cariTim << " telah ditemukan";
            cout << "\n Tambahkan skor: ";
            cin >> skorbaru;
            daftartim[i].skor += skorbaru;
            found = true;
            break;
        }
    }

    if (!found)
    {
        cout << "Tim dengan nama \"" << cariTim << "\" tidak ditemukan.\n";
        return;
    }
    file = fopen("tim.txt", "wb");
    if (file == NULL)
    {
        cout << "Gagal membuka file untuk menulis ulang!" << endl;
        return;
    }

    for (int i = 0; i < jumlahtim; i++)
    {
        fwrite(&daftartim[i], sizeof(Tim), 1, file);
    }

    fclose(file);
    cout << "Skor berhasil diupdate dan disimpan kembali ke file!\n";
    clear();
}


void menu4()
{
    Tim daftartim[100];
    int jumlahtim = 0;

    file = fopen("tim.txt", "rb");
    if (file == NULL)
    {
        cout << "Gagal membuka file untuk membaca!" << endl;
        return;
    }

    while (fread(&daftartim[jumlahtim], sizeof(Tim), 1, file))
    {
        jumlahtim++;
    }
    fclose(file);

    for (int i = 0; i < jumlahtim - 1; i++)
    {
        for (int j = 0; j < jumlahtim - i - 1; j++)
        {
            if (daftartim[j].skor < daftartim[j + 1].skor)
            {
                swap(daftartim[j], daftartim[j + 1]);
            }
        }
    }

    cout << "\nDaftar Tim Berdasarkan Skor (Tertinggi ke Terendah):\n";
    cout << "=====================================================\n";
    for (int i = 0; i < jumlahtim; i++)
    {
        cout << i + 1 << ". " << daftartim[i].namaTim << " - Skor: " << daftartim[i].skor << endl;
    }

    cout << endl;
    clear();
}

int main(int argc, char const *argv[])
{
    int pilih;

do
{
    cout << "SiMTES" << endl;
    cout << "Sistem Informasi Manajemen Turnamen Esports" << endl;
    cout << endl;
    menu();
    cout << "Pilih : ";
    cin >> pilih;

    switch (pilih)
    {
    case 1:
        menu1();
        break;
    
    case 2:
        menu2();
        break;

    case 3:
        menu3();
        break;
    
    case 4:
        menu4();
        break;
    default:
        break;
    }
} while (pilih != 7);
    return 0;
}
