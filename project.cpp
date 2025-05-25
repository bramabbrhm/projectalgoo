#include <iostream>
#include <cstring>
#include <iomanip>
using namespace std;

struct Player
{
    char nama[50];
    char nickname[50];
    char role[30];
};

struct Tim
{
    char namaTim[50];
    Player pemain[5];
    int skor;
    int matchW;
    int matchL;
    int gameW;
    int gameL;
};

struct Node
{
    Tim data;
    Node *next;
};

Node *kepala = nullptr, *ekor = nullptr;

FILE *file;
Tim tim;

void simpanSemuaKeFile()
{
    file = fopen("tim.txt", "wb");
    if (!file)
    {
        cout << "Gagal menyimpan ke file!\n";
        return;
    }

    Node *bantu = kepala;
    while (bantu)
    {
        fwrite(&(bantu->data), sizeof(Tim), 1, file);
        bantu = bantu->next;
    }

    fclose(file);
    cout << "Data berhasil disimpan ke peringkat.txt\n";
}

void muatDariFile()
{
    file = fopen("tim.txt", "rb");
    if (!file)
    {
        cout << "Gagal memuat dari file!\n";
        return;
    }

    Tim bacatim;
    while (fread(&bacatim, sizeof(Tim), 1, file))
    {
        Node *nodeBaru = new Node{bacatim, nullptr};
        if (kepala == NULL)
            kepala = ekor = nodeBaru;
        else
        {
            ekor->next = nodeBaru;
            ekor = nodeBaru;
        }
    }
    fclose(file);
}

void clear()
{
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
    cout << "3. Input Hasil Pertandingan (Best of 3)" << endl;
    cout << "4. Lihat Peringkat" << endl;
    cout << "5. Edit Tim" << endl;
    cout << "6. Hapus Tim " << endl;
    cout << "7. Searching (Tim / Player)" << endl;
    cout << "8. Keluar " << endl;
}

void tambahTim()
{
    int jumlah;
    cout << "Berapa banyak tim yang ingin ditambahkan? ";
    cin >> jumlah;
    cin.ignore();

    for (int t = 0; t < jumlah; t++)
    {
        Tim newTim = {"", {}, 0, 0, 0, 0, 0};
        cout << "\nInput Tim ke-" << t + 1 << endl;
        cout << "Nama Tim: ";
        cin.getline(newTim.namaTim, 50);

        for (int i = 0; i < 5; i++)
        {
            cout << "  Pemain ke-" << i + 1 << endl;
            cout << "    Nama     : ";
            cin.getline(newTim.pemain[i].nama, 50);
            cout << "    Nickname : ";
            cin.getline(newTim.pemain[i].nickname, 50);
            cout << "    Role     : ";
            cin.getline(newTim.pemain[i].role, 30);
        }

        Node *newNode = new Node{newTim, nullptr};
        if (!kepala)
            kepala = ekor = newNode;
        else
        {
            ekor->next = newNode;
            ekor = newNode;
        }
    }

    simpanSemuaKeFile();
    clear();
}

void lihatTim()
{
    if (!kepala)
    {
        cout << "Belum ada tim.\n";
        clear();
        return;
    }

    Node *bantu = kepala;
    int no = 1;
    while (bantu)
    {
        cout << "\nTim #" << no++ << ": " << bantu->data.namaTim << " (Skor: " << bantu->data.skor << ")\n";
        for (int i = 0; i < 5; i++)
        {
            cout << "  - " << bantu->data.pemain[i].nickname << " (" << bantu->data.pemain[i].role << ")\n";
        }
        bantu = bantu->next;
    }
    clear();
}

void inputSkor()
{
    cout << "\n=== Pilih Tim yang Akan Bertanding ===\n";
    Node *bantu = kepala;
    int urutan = 1;
    while (bantu)
    {
        cout << urutan << ". " << bantu->data.namaTim << endl;
        bantu = bantu->next;
        urutan++;
    }

    int idx1, idx2;
    cout << "\nPilih nomor tim pertama: ";
    cin >> idx1;
    cout << "Pilih nomor tim kedua: ";
    cin >> idx2;

    if (idx1 == idx2)
    {
        cout << "Tim tidak boleh bertanding melawan dirinya sendiri!\n";
        clear();
        return;
    }

    Node *tim1 = kepala;
    Node *tim2 = kepala;
    for (int i = 1; i < idx1 && tim1; i++)
        tim1 = tim1->next;
    for (int i = 1; i < idx2 && tim2; i++)
        tim2 = tim2->next;

    if (!tim1 || !tim2)
    {
        cout << "Salah satu tim tidak ditemukan!\n";
        return;
    }

    cout << "\nPertandingan: " << tim1->data.namaTim << " vs " << tim2->data.namaTim << endl;
    cout << "Masukkan hasil pertandingan (best of 3):\n";
    int win1 = 0, win2 = 0;
    for (int i = 1; i <= 3 && win1 < 2 && win2 < 2; i++)
    {
        int hasil;
        cout << "Game " << i << " (1: " << tim1->data.namaTim << " menang, 2: " << tim2->data.namaTim << " menang): ";
        cin >> hasil;
        if (hasil == 1)
        {
            win1++;
            tim1->data.gameW++;
            tim2->data.gameL++;
        }
        else if (hasil == 2)
        {
            win2++;
            tim2->data.gameW++;
            tim1->data.gameL++;
        }
        else
        {
            cout << "Input tidak valid.\n";
            i--;
        }
    }

    if (win1 > win2)
    {
        tim1->data.matchW++;
        tim2->data.matchL++;
        tim1->data.skor += 3;
    }
    else
    {
        tim2->data.matchW++;
        tim1->data.matchL++;
        tim2->data.skor += 3;
    }

    simpanSemuaKeFile();
    clear();
}

void lihatPeringkat()
{
    if (!kepala)
    {
        cout << "Belum ada tim!\n";
        clear();
        return;
    }

    bool tukar;
    do
    {
        tukar = false;
        Node *bantu = kepala;
        while (bantu && bantu->next)
        {
            if (bantu->data.skor < bantu->next->data.skor)
            {
                swap(bantu->data, bantu->next->data);
                tukar = true;
            }
            bantu = bantu->next;
        }
    } while (tukar);

    cout << left << setw(18) << "Nama Tim"
         << right << setw(6) << "Poin"
         << setw(10) << "W-L(M)"
         << setw(10) << "W-L(G)"
         << setw(10) << "WinRate"
         << setw(10) << "GameRate" << endl;

    cout << string(64, '-') << endl;

    Node *bantu = kepala;
    while (bantu)
    {
        int mw = bantu->data.matchW;
        int ml = bantu->data.matchL;
        int gw = bantu->data.gameW;
        int gl = bantu->data.gameL;
        float wr;
        float gr;
        if (mw + ml != 0)
        {
            wr = mw * 100.0 / (mw + ml);
        }
        else
        {
            wr = 0;
        }

        if (gw + gl != 0)
        {
            gr = gw * 100.0 / (gw + gl);
        }
        else
        {
            gr = 0;
        }

        cout << left << setw(18) << bantu->data.namaTim
             << right << setw(6) << bantu->data.skor
             << setw(5) << mw << "-" << setw(4) << ml
             << setw(5) << gw << "-" << setw(4) << gl
             << setw(9) << fixed << setprecision(1) << wr << "%"
             << setw(9) << fixed << setprecision(1) << gr << "%" << endl;

        bantu = bantu->next;
    }
    clear();
}

void editTim()
{
    cin.ignore();
    char cari[50];
    cout << "Masukkan nama tim yang ingin diedit: ";
    cin.getline(cari, 50);

    Node *bantu = kepala;
    while (bantu)
    {
        if (strcmp(bantu->data.namaTim, cari) == 0)
        {
            cout << "\nEdit nama tim (lama: " << bantu->data.namaTim << "): ";
            cin.getline(bantu->data.namaTim, 50);
            for (int i = 0; i < 5; i++)
            {
                cout << "\nEdit data pemain ke-" << i + 1 << " (" << bantu->data.pemain[i].nickname << ")" << endl;
                cout << "  Nama     : ";
                cin.getline(bantu->data.pemain[i].nama, 50);
                cout << "  Nickname : ";
                cin.getline(bantu->data.pemain[i].nickname, 50);
                cout << "  Role     : ";
                cin.getline(bantu->data.pemain[i].role, 30);
            }
            simpanSemuaKeFile();
            cout << "Data tim berhasil diperbarui.\n";
            clear();
            return;
        }
        bantu = bantu->next;
    }
    cout << "Tim tidak ditemukan.\n";
    clear();
}

void hapusTim()
{
    cin.ignore();
    char cari[50];
    cout << "Masukkan nama tim yang ingin dihapus: ";
    cin.getline(cari, 50);

    if (kepala != nullptr && strcmp(kepala->data.namaTim, cari) == 0)
    {
        Node *hapus = kepala;
        kepala = kepala->next;
        delete hapus;
        simpanSemuaKeFile();
        cout << "Tim berhasil dihapus.\n";
        return;
    }

    Node *bantu = kepala;

    while (bantu != nullptr && bantu->next != nullptr)
    {
        if (strcmp(bantu->next->data.namaTim, cari) == 0)
        {
            Node *hapus = bantu->next;
            bantu->next = hapus->next;
            delete hapus;
            simpanSemuaKeFile();
            cout << "Tim berhasil dihapus.\n";
            return;
        }
        bantu = bantu->next;
    }
    cout << "Tim tidak ditemukan.\n";
    clear();
}

void searching()
{
    // cin.ignore();
    char cari[50];
    int pilih;

    cout << "apa yang ingin dicari : " << endl;
    cout << "1. Tim" << endl;
    cout << "2. Nickname Player" << endl;
    cout << "pilih : ";
    cin >> pilih;

    switch (pilih)
    {
    case 1:
    {
        cin.ignore();
        cout << "Masukkan nama tim yang ingin dicari: ";
        cin.getline(cari, 50);

        Node *bantu = kepala;
        while (bantu != nullptr)
        {
            if (strcmp(bantu->data.namaTim, cari) == 0)
            {
                cout << "\nTim ditemukan:\n";
                cout << "Nama Tim: " << bantu->data.namaTim << endl;
                for (int i = 0; i < 5; i++)
                {
                    cout << "  Pemain " << i + 1 << ": " << bantu->data.pemain[i].nama
                         << " (" << bantu->data.pemain[i].nickname << "), Role: "
                         << bantu->data.pemain[i].role << endl;
                }
                clear();
                return;
            }
            bantu = bantu->next;
        }
        cout << "Tim tidak ditemukan.\n";
        clear();
    }
    break;
    case 2:
    {
        cin.ignore();
        cout << "Masukkan nickname player yang ingin dicari: ";
        cin.getline(cari, 50);

        {
            Node *bantu = kepala;
            bool ditemukan = false;
            while (bantu != nullptr)
            {
                for (int i = 0; i < 5; i++)
                {
                    if (strcmp(bantu->data.pemain[i].nickname, cari) == 0)
                    {
                        cout << "\nPlayer ditemukan dalam tim:\n";
                        cout << "Nama Tim: " << bantu->data.namaTim << endl;
                        cout << "Nama Pemain: " << bantu->data.pemain[i].nama << endl;
                        cout << "Nickname: " << bantu->data.pemain[i].nickname << endl;
                        cout << "Role: " << bantu->data.pemain[i].role << endl;
                        ditemukan = true;
                    }
                    
                }
                bantu = bantu->next;
            }

            if (!ditemukan)
            {
                cout << "Player dengan nickname tersebut tidak ditemukan.\n";
            }
        }
        clear();
    }
    break;
    default:
        cout << "Pilihan tidak valid!";
        clear();
        break;
    }
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
            lihatPeringkat();
            break;

        case 5:
            editTim();
            break;

        case 6:
            hapusTim();
            break;

        case 7:
            searching();
            break;

        case 8:
            cout << "Terimakasih sudah menggunakan SiMTES";
            break;

        default:
            cout << "Pilihan tidak valid!\n";
            clear();
            break;
        }
    } while (pilih != 8);
    return 0;
}
