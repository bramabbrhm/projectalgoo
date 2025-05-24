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

void inputSkor() {
    cout << "\n=== Pilih Tim yang Akan Bertanding ===\n";
    Node* bantu = kepala;
    int urutan = 1;
    while (bantu) {
        cout << urutan << ". " << bantu->data.namaTim << endl;
        bantu = bantu->next;
        urutan++;
    }

    int idx1, idx2;
    cout << "\nPilih nomor tim pertama: "; cin >> idx1;
    cout << "Pilih nomor tim kedua: "; cin >> idx2;

    if (idx1 == idx2) {
        cout << "Tim tidak boleh bertanding melawan dirinya sendiri!\n";
        return;
    }

    Node* tim1 = kepala;
    Node* tim2 = kepala;
    for (int i = 1; i < idx1 && tim1; i++) tim1 = tim1->next;
    for (int i = 1; i < idx2 && tim2; i++) tim2 = tim2->next;

    if (!tim1 || !tim2) {
        cout << "Salah satu tim tidak ditemukan!\n";
        return;
    }

    cout << "\nPertandingan: " << tim1->data.namaTim << " vs " << tim2->data.namaTim << endl;
    cout << "Masukkan hasil pertandingan (best of 3):\n";
    int win1 = 0, win2 = 0;
    for (int i = 1; i <= 3 && win1 < 2 && win2 < 2; i++) {
        int hasil;
        cout << "Game " << i << " (1: " << tim1->data.namaTim << " menang, 2: " << tim2->data.namaTim << " menang): ";
        cin >> hasil;
        if (hasil == 1) {
            win1++;
            tim1->data.gameW++;
            tim2->data.gameL++;
        } else if (hasil == 2) {
            win2++;
            tim2->data.gameW++;
            tim1->data.gameL++;
        } else {
            cout << "Input tidak valid.\n";
            i--;
        }
    }

    if (win1 > win2) {
        tim1->data.matchW++;
        tim2->data.matchL++;
        tim1->data.skor += 3;
    } else {
        tim2->data.matchW++;
        tim1->data.matchL++;
        tim2->data.skor += 3;
    }

    simpanSemuaKeFile();
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
    muatDariFile();
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
        inputSkor();
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
