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

void muatDariFile() {
    file = fopen("tim.txt", "rb");
    if (!file){
        cout << "Gagal memuat dari file!\n";
        return;
    }

    Tim bacatim;
    while (fread(&bacatim, sizeof(Tim), 1, file)) {
        Node* nodeBaru = new Node{bacatim, nullptr};
        if (kepala == NULL) kepala = ekor = nodeBaru;
        else {
            ekor->next = nodeBaru;
            ekor = nodeBaru;
        }
    }
    fclose(file);
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


void tambahTim() {
    int jumlah;
    cout << "Berapa banyak tim yang ingin ditambahkan? ";
    cin >> jumlah;
    cin.ignore();

    for (int t = 0; t < jumlah; t++) {
        Tim newTim = {"", {}, 0, 0, 0, 0, 0};
        cout << "\nInput Tim ke-" << t + 1 << endl;
        cout << "Nama Tim: ";
        cin.getline(newTim.namaTim, 50);

        for (int i = 0; i < 5; i++) {
            cout << "  Pemain ke-" << i + 1 << endl;
            cout << "    Nama     : "; cin.getline(newTim.pemain[i].nama, 50);
            cout << "    Nickname : "; cin.getline(newTim.pemain[i].nickname, 50);
            cout << "    Role     : "; cin.getline(newTim.pemain[i].role, 30);
        }

        Node* newNode = new Node{newTim, nullptr};
        if (!kepala) kepala = ekor = newNode;
        else {
            ekor->next = newNode;
            ekor = newNode;

        }
    }

    simpanSemuaKeFile();
    clear();
}

void lihatTim() {
    if (!kepala) {
        cout << "Belum ada tim.\n";
        clear();
        return;
    }

    Node* bantu = kepala;
    int no = 1;
    while (bantu) {
        cout << "\nTim #" << no++ << ": " << bantu->data.namaTim << " (Skor: " << bantu->data.skor << ")\n";
        for (int i = 0; i < 5; i++) {
            cout << "  - " << bantu->data.pemain[i].nickname << " (" << bantu->data.pemain[i].role << ")\n";
        }
        bantu = bantu->next;
    }
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
        tambahTim();
        break;
    
    case 2:
        lihatTim();
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
