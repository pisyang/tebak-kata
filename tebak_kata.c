/* ============================================================
   TEBAK KATA - Robotika Edition  (KERANGKA / STARTER)
   Proyek Akhir Praktikum Dasar Pemrograman
   ------------------------------------------------------------
   Nama  : _______________________
   NIM   : _______________________
   Kelas : _______________________
   ------------------------------------------------------------
   PETUNJUK:
   - struct, variabel global, prototipe, dan menu sudah disediakan.
   - Lengkapi setiap fungsi yang diberi tanda  // TODO
   - Pada setiap fungsi yang harus dilengkapi telah disediakan
     PENJELASAN VARIABEL (parameter, variabel lokal yang disarankan,
     dan nilai balik). Gunakan penjelasan ini sebagai panduan.
   - Kerangka ini SUDAH bisa di-compile. Lengkapi sedikit demi
     sedikit, lalu compile & uji ulang setiap selesai 1 fungsi.

   Kompilasi:  gcc -Wall -o tebak_kata tebak_kata.c
   Menjalankan: ./tebak_kata        (Linux/Mac)
                tebak_kata.exe       (Windows)
   ============================================================ */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#define MAX_WRONG   6      /* kepala, badan, 2 tangan, 2 kaki */
#define MAX_WORDS   50
#define HOF_SIZE    10
#define NAME_LEN    20
#define WORD_LEN    30

/* ---------- STRUCT (SUDAH DISEDIAKAN) ---------- */
struct Date {                  /* struct bersarang */
    int day, month, year;
};

struct ScoreEntry {
    char name[NAME_LEN];
    int  points;
    struct Date achieved;      /* Date bersarang di dalam ScoreEntry */
};

struct Game {
    char secret[WORD_LEN];     /* kata rahasia            */
    char display[WORD_LEN];    /* topeng tampilan: _ _ _  */
    char guessed[WORD_LEN];    /* huruf yang sudah ditebak*/
    int  guessed_count;
    int  wrong;                /* jumlah tebakan salah    */
};

/* ---------- VARIABEL GLOBAL (SUDAH DISEDIAKAN) ----------
   word_bank   : array of pointers berisi daftar kata. HARUS selalu
                 terurut alfabetis (syarat agar binary search benar).
   word_count  : jumlah kata yang sedang tersimpan di word_bank.
   hall_of_fame: array of struct papan peringkat (berisi ScoreEntry).
   hof_count   : jumlah entri yang sedang tersimpan di hall_of_fame.   */
const char *word_bank[MAX_WORDS] = {
    "ALGORITMA", "MOTOR", "NEURON", "ROBOTIKA", "SENSOR"
};
int word_count = 5;

struct ScoreEntry hall_of_fame[HOF_SIZE];
int hof_count = 0;

/* ---------- PROTOTIPE FUNGSI (SUDAH DISEDIAKAN) ---------- */
void print_banner(void);
int  show_menu(void);
void play_game(void);
void draw_hangman(int wrong);
int  process_guess(struct Game *g, char letter);
int  linear_search_char(const char *arr, int n, char target);
int  is_won(const struct Game *g);
int  binary_search_word(const char *bank[], int n, const char *target);
void insert_word_sorted(const char *bank[], int *n, const char *new_word);
void bubble_sort_hof(struct ScoreEntry hof[], int n);
void show_hall_of_fame(void);
void add_word(void);
void save_score(const char *name, int points);
void to_upper_str(char *s);

/* ============================================================
   MAIN (SUDAH DISEDIAKAN) - menu menggunakan do-while
   ============================================================ */
int main(void) {
    int choice;
    srand((unsigned) time(NULL));

    do {
        choice = show_menu();
        switch (choice) {
            case 1: play_game();         break;
            case 2: show_hall_of_fame(); break;
            case 3: add_word();          break;
            case 0: printf("\nTerima kasih telah bermain!\n"); break;
            default: printf("\nPilihan tidak valid. Coba lagi.\n");
        }
    } while (choice != 0);

    return 0;
}

/* ---------- (SUDAH DISEDIAKAN) ---------- */
void print_banner(void) {
    printf("\n========================================\n");
    printf("   TEBAK KATA - Robotika Edition\n");
    printf("========================================\n");
}

int show_menu(void) {
    int choice;
    int ch;
    print_banner();
    printf("  [1] Main game baru\n");
    printf("  [2] Lihat Hall of Fame\n");
    printf("  [3] Tambah kata ke bank\n");
    printf("  [0] Keluar\n");
    printf("Pilih: ");

    if (scanf("%d", &choice) != 1) {
        choice = -1;
    }

    while ((ch = getchar()) != '\n' && ch != EOF);

    return choice;
}

/* Menggambar tiang gantungan (SUDAH DISEDIAKAN sebagai bonus) */
void draw_hangman(int wrong) {
    printf("\n   +---+\n");
    printf("   |   |\n");
    printf("   %c   |\n", (wrong >= 1) ? 'O' : ' ');
    printf("  %c%c%c  |\n",
           (wrong >= 3) ? '/' : ' ',
           (wrong >= 2) ? '|' : ' ',
           (wrong >= 4) ? '\\' : ' ');
    printf("  %c %c  |\n",
           (wrong >= 5) ? '/' : ' ',
           (wrong >= 6) ? '\\' : ' ');
    printf("  =========\n");
}

/* ============================================================
   BAGIAN YANG HARUS DILENGKAPI MAHASISWA
   (Tiap fungsi disertai PENJELASAN VARIABEL sebagai panduan.)
   ============================================================ */

/* -------------------------------------------------------------
   play_game : inti permainan. Pilih kata acak, tampilkan topeng,
               baca tebakan, perbarui keadaan, tentukan menang/kalah,
               hitung dan simpan skor.

   PARAMETER : (tidak ada)

   VARIABEL LOKAL (sebagian sudah dideklarasikan di bawah):
     g     (struct Game)    : seluruh keadaan permainan saat ini
                              (secret, display, guessed, guessed_count,
                              wrong). Diisi dan diperbarui selama main.
     name  (char[NAME_LEN]) : nama pemain.
     idx   (int)            : indeks kata terpilih dari word_bank.
     len   (int)            : panjang kata rahasia (hasil strlen).
     i     (int)            : pencacah untuk perulangan.
   VARIABEL LOKAL yang perlu Anda TAMBAHKAN sendiri:
     letter (char)          : huruf yang ditebak pemain pada tiap giliran.

   NILAI BALIK: (void)
   ------------------------------------------------------------- */
void play_game(void) {
    struct Game g;
    char name[NAME_LEN];
    char letter;
    int  idx, len, i;

    /* TODO 1: pilih indeks kata acak -> idx = rand() % word_count; */
    idx = rand() % word_count;

    /* TODO 2: salin kata terpilih ke g.secret (strcpy) dan hitung len */
    strcpy(g.secret, word_bank[idx]);
    len = strlen(g.secret);

    /* TODO 3: inisialisasi g.wrong = 0; g.guessed_count = 0; */
    g.wrong         = 0;
    g.guessed_count = 0;

    /* TODO 4: isi g.display dengan '_' sebanyak len, beri '\0' di akhir */
    for (i = 0; i < len; i++) {
        g.display[i] = '_';
    }
    g.display[len] = '\0';
    g.guessed[0]   = '\0';

    /* TODO 5: minta nama pemain (scanf) */
    printf("\nMasukkan nama: ");
    fgets(name, NAME_LEN, stdin);
    {
        int nl = strlen(name) - 1;
        if (nl >= 0 && name[nl] == '\n') name[nl] = '\0';
    }

    printf("Kata dipilih! Panjang: %d huruf.\n", len);

    /* TODO 6: LOOP UTAMA (while): selama g.wrong < MAX_WRONG DAN belum menang
         - draw_hangman(g.wrong)
         - cetak g.display dan daftar g.guessed
         - baca 1 huruf (letter), ubah ke huruf besar (toupper)
         - LINEAR SEARCH: jika huruf sudah ada di g.guessed -> pesan + continue
         - catat huruf ke g.guessed
         - panggil process_guess(&g, letter)  (pass by reference)
    */
    while (g.wrong < MAX_WRONG && !is_won(&g)) {

        draw_hangman(g.wrong);

        printf("\n Kata : ");
        for (i = 0; i < len; i++) {
            printf("%c ", g.display[i]);
        }

        printf("\n Nyawa: %d   Salah ditebak: ", MAX_WRONG - g.wrong);
        if (g.guessed_count == 0) {
            printf("(belum ada)");
        } else {
            for (i = 0; i < g.guessed_count; i++) {
                printf("%c ", g.guessed[i]);
            }
        }

        printf("\n Tebak huruf: ");
        scanf(" %c", &letter);
        letter = toupper(letter);

        {
            int extra = 0;
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) {
                if (ch != ' ' && ch != '\r') {
                    extra = 1;
                }
            }
            if (extra) {
                printf(" -> Input tidak valid. Masukkan satu huruf saja.\n");
                continue;
            }
        }

        if (linear_search_char(g.guessed, g.guessed_count, letter) != -1) {
            printf(" -> Huruf '%c' sudah pernah ditebak! Coba huruf lain.\n",
                   letter);
            continue;
        }

        g.guessed[g.guessed_count] = letter;
        g.guessed_count++;
        g.guessed[g.guessed_count] = '\0';

        process_guess(&g, letter);
    }

    /* TODO 7: setelah loop, jika is_won(&g) -> tampilkan kata, hitung skor
         dengan RUMUS:  skor = (MAX_WRONG - g.wrong) * 100 + len * 20;
         lalu panggil save_score(name, skor).
         Jika kalah -> tampilkan "GAME OVER" dan kata yang benar.
    */
    if (is_won(&g)) {
        int skor = (MAX_WRONG - g.wrong) * 100 + len * 20;

        printf("\n Kata : ");
        for (i = 0; i < len; i++) {
            printf("%c ", g.display[i]);
        }
        printf("\n");

        printf("\n*** SELAMAT! Kamu menebak kata: %s ***\n", g.secret);
        printf(" Nyawa tersisa: %d\n", MAX_WRONG - g.wrong);
        printf(" Skor = (%d x 100) + (%d x 20) = %d\n",
               MAX_WRONG - g.wrong, len, skor);

        save_score(name, skor);
    } else {
        draw_hangman(g.wrong);

        printf("\n====================================\n");
        printf(" GAME OVER!\n");
        printf(" Kata yang benar adalah: %s\n", g.secret);
        printf("====================================\n");
    }
}

/* -------------------------------------------------------------
   process_guess : memproses satu tebakan huruf. Membuka huruf yang
                   cocok pada g->display; bila tidak ada yang cocok,
                   menambah g->wrong.

   PARAMETER:
     g      (struct Game *) : POINTER ke data permainan. Data diubah
                              LANGSUNG di sini (pass by reference),
                              mis. g->display dan g->wrong.
     letter (char)          : huruf yang ditebak (sudah huruf besar).

   VARIABEL LOKAL yang disarankan:
     p     (char *) : pointer penelusur untuk berjalan di g->secret
                      satu karakter demi satu (aritmatika pointer).
     idx   (int)    : indeks posisi huruf (0..len-1), bergerak bersama p.
     found (int)    : penanda; 1 bila huruf ditemukan, 0 bila tidak.

   NILAI BALIK (int): 1 bila huruf ada di kata; 0 bila tidak
                      (pada kasus 0, g->wrong bertambah 1).
   ------------------------------------------------------------- */
int process_guess(struct Game *g, char letter) {
    char *p;
    int   idx   = 0;
    int   found = 0;

    /* TODO: telusuri g->secret; buka g->display pada posisi yang cocok.
             Jika tidak ada yang cocok, g->wrong++. */
    p = g->secret;
    while (*p != '\0') {
        if (*p == letter) {
            g->display[idx] = letter;
            found = 1;
        }
        p++;
        idx++;
    }

    if (!found) {
        g->wrong++;
        printf(" -> Salah! '%c' tidak ada. (nyawa berkurang)\n", letter);
    } else {
        printf(" -> Benar! Huruf '%c' ada di kata.\n", letter);
    }

    return found;
}

/* -------------------------------------------------------------
   linear_search_char : mencari sebuah huruf di dalam array huruf
                        secara berurutan (linear search).

   PARAMETER:
     arr    (const char *) : array huruf yang ditelusuri (mis. g.guessed).
     n      (int)          : jumlah elemen valid pada arr.
     target (char)         : huruf yang dicari.

   VARIABEL LOKAL yang disarankan:
     i (int) : pencacah perulangan.

   NILAI BALIK (int): indeks (0..n-1) bila huruf ditemukan; -1 bila tidak.
   ------------------------------------------------------------- */
int linear_search_char(const char *arr, int n, char target) {
    int i;

    /* TODO */
    for (i = 0; i < n; i++) {
        if (arr[i] == target) {
            return i;
        }
    }

    return -1;
}

/* -------------------------------------------------------------
   is_won : memeriksa apakah pemain sudah menang, yaitu tidak ada
            lagi karakter '_' pada g->display.

   PARAMETER:
     g (const struct Game *) : POINTER ke data permainan (hanya dibaca;
                               const berarti tidak boleh diubah).

   VARIABEL LOKAL yang disarankan:
     i   (int) : pencacah perulangan.
     len (int) : panjang g->display (hasil strlen).

   NILAI BALIK (int): 1 bila menang (tidak ada '_'); 0 bila belum.
   ------------------------------------------------------------- */
int is_won(const struct Game *g) {
    int i;
    int len = strlen(g->display);

    /* TODO */
    for (i = 0; i < len; i++) {
        if (g->display[i] == '_') {
            return 0;
        }
    }

    return 1;
}

/* -------------------------------------------------------------
   binary_search_word : mencari sebuah kata di dalam bank kata yang
                        SUDAH TERURUT alfabetis (binary search).

   PARAMETER:
     bank   (const char *[]) : array of pointers berisi kata; HARUS terurut.
     n      (int)            : jumlah kata pada bank.
     target (const char *)   : kata yang dicari.

   VARIABEL LOKAL yang disarankan:
     low  (int) : batas bawah rentang pencarian (mula-mula 0).
     high (int) : batas atas rentang pencarian (mula-mula n-1).
     mid  (int) : indeks tengah = (low + high) / 2.
     cmp  (int) : hasil strcmp(bank[mid], target)
                  (0 = sama, <0 = bank[mid] lebih kecil, >0 = lebih besar).

   NILAI BALIK (int): indeks kata bila ditemukan; -1 bila tidak ada.
   ------------------------------------------------------------- */
int binary_search_word(const char *bank[], int n, const char *target) {
    int low  = 0;
    int high = n - 1;
    int mid, cmp;

    /* TODO */
    while (low <= high) {
        mid = (low + high) / 2;
        cmp = strcmp(bank[mid], target);

        if (cmp == 0) {
            return mid;
        } else if (cmp < 0) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    return -1;
}

/* -------------------------------------------------------------
   insert_word_sorted : menyisipkan kata baru ke posisi yang benar
                        agar bank tetap terurut (langkah inti dari
                        insertion sort: geser yang lebih besar ke kanan).

   PARAMETER:
     bank     (const char *[]) : array of pointers kata; akan dimodifikasi.
     n        (int *)          : POINTER ke jumlah kata. Naikkan dengan
                                 (*n)++ setelah menyisipkan.
     new_word (const char *)   : kata baru yang akan disisipkan.

   VARIABEL LOKAL yang disarankan:
     i    (int)    : indeks untuk menggeser elemen dari kanan ke kiri.
     copy (char *) : salinan dari new_word (alokasikan dengan malloc lalu
                     strcpy) supaya kata tetap tersimpan setelah fungsi
                     selesai.

   NILAI BALIK: (void)
   ------------------------------------------------------------- */
void insert_word_sorted(const char *bank[], int *n, const char *new_word) {
    int   i;
    char *copy;

    /* TODO: alokasi salinan kata, geser elemen yang lebih besar ke kanan,
             sisipkan, lalu (*n)++ */
    copy = (char *) malloc(strlen(new_word) + 1);
    if (copy == NULL) {
        printf("Gagal mengalokasikan memori!\n");
        return;
    }
    strcpy(copy, new_word);

    i = *n - 1;
    while (i >= 0 && strcmp(bank[i], copy) > 0) {
        bank[i + 1] = bank[i];
        i--;
    }

    bank[i + 1] = copy;
    (*n)++;
}

/* -------------------------------------------------------------
   bubble_sort_hof : mengurutkan papan peringkat MENURUN (descending)
                     berdasarkan points menggunakan bubble sort
                     (dua perulangan bersarang).

   PARAMETER:
     hof (struct ScoreEntry[]) : array papan peringkat yang diurutkan.
     n   (int)                 : jumlah entri pada hof.

   VARIABEL LOKAL yang disarankan:
     i, j (int)               : pencacah untuk dua perulangan bersarang.
     temp (struct ScoreEntry) : penampung sementara saat menukar dua entri
                                (seluruh struct ditukar, termasuk Date).

   NILAI BALIK: (void)
   ------------------------------------------------------------- */
void bubble_sort_hof(struct ScoreEntry hof[], int n) {
    int i, j;
    struct ScoreEntry temp;

    /* TODO */
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (hof[j].points < hof[j + 1].points) {
                temp       = hof[j];
                hof[j]     = hof[j + 1];
                hof[j + 1] = temp;
            }
        }
    }
}

/* -------------------------------------------------------------
   show_hall_of_fame : menampilkan isi papan peringkat dalam bentuk
                       tabel (rank, nama, skor, tanggal).

   PARAMETER : (tidak ada). Memakai variabel global hall_of_fame dan
               hof_count.

   VARIABEL LOKAL yang disarankan:
     i (int) : pencacah untuk mencetak tiap baris.

   NILAI BALIK: (void)

   Catatan: tanggal diambil dari struct bersarang, mis.
            hall_of_fame[i].achieved.day / .month / .year
   ------------------------------------------------------------- */
void show_hall_of_fame(void) {
    int i;

    /* TODO: jika hof_count == 0 tampilkan pesan kosong; selain itu cetak tabel */
    if (hof_count == 0) {
        printf("\nHall of Fame masih kosong.\n");
        return;
    }

    printf("\n========== HALL OF FAME ==========\n");
    printf(" %-4s %-20s %-8s %-10s\n", "Rank", "Nama", "Skor", "Tanggal");
    printf(" ---- -------------------- -------- ----------\n");

    for (i = 0; i < hof_count; i++) {
        printf(" %-4d %-20s %-8d %02d/%02d/%04d\n",
               i + 1,
               hall_of_fame[i].name,
               hall_of_fame[i].points,
               hall_of_fame[i].achieved.day,
               hall_of_fame[i].achieved.month,
               hall_of_fame[i].achieved.year);
    }

    printf("==================================\n");
}

/* -------------------------------------------------------------
   save_score : menanyakan apakah skor disimpan; bila ya, isi entri
                baru pada papan peringkat lalu urutkan (bubble_sort_hof).

   PARAMETER:
     name   (const char *) : nama pemain yang akan disimpan.
     points (int)          : skor yang diperoleh pemain.

   VARIABEL LOKAL yang disarankan:
     ans   (char)         : jawaban pemain ('Y' atau 'T').
     today (struct Date)  : tanggal hari ini (day, month, year).
     t     (time_t)       : waktu sistem (dari time(NULL)).
     lt    (struct tm *)  : hasil localtime(&t) untuk mengisi today
                            (lt->tm_mday, lt->tm_mon + 1, lt->tm_year + 1900).

   NILAI BALIK: (void). Memakai/ubah global hall_of_fame dan hof_count;
                panggil bubble_sort_hof setelah menambah entri.
   ------------------------------------------------------------- */
void save_score(const char *name, int points) {
    char        ans;
    struct Date today;
    time_t      t;
    struct tm  *lt;
    int         ch;

    /* TODO */
    do {
        printf("\nApakah skor ini layak masuk Hall of Fame? (Y/T): ");
        if (scanf(" %c", &ans) != 1) ans = '\0';
        while ((ch = getchar()) != '\n' && ch != EOF);
        ans = toupper(ans);
        if (ans != 'Y' && ans != 'T') {
            printf(" -> Input tidak valid. Masukkan Y atau T.\n");
        }
    } while (ans != 'Y' && ans != 'T');

    if (ans != 'Y') {
        return;
    }

    if (hof_count >= HOF_SIZE) {
        printf("Hall of Fame penuh.\n");
        return;
    }

    t  = time(NULL);
    lt = localtime(&t);

    today.day   = lt->tm_mday;
    today.month = lt->tm_mon + 1;
    today.year  = lt->tm_year + 1900;

    strcpy(hall_of_fame[hof_count].name, name);
    hall_of_fame[hof_count].points   = points;
    hall_of_fame[hof_count].achieved = today;
    hof_count++;

    bubble_sort_hof(hall_of_fame, hof_count);

    printf("Skor %s (%d) tersimpan, tanggal %02d/%02d/%04d.\n",
           name, points, today.day, today.month, today.year);
}

/* -------------------------------------------------------------
   add_word : menambah kata baru ke bank. Cek duplikat dengan binary
              search, lalu sisipkan terurut dengan insert_word_sorted.

   PARAMETER : (tidak ada). Memakai/ubah global word_bank dan word_count.

   VARIABEL LOKAL yang disarankan:
     buf (char[WORD_LEN]) : menampung kata baru yang diketik pemain.
     i   (int)            : pencacah saat mencetak isi bank kata.

   NILAI BALIK: (void).
   Alur: baca kata -> to_upper_str(buf) ->
         binary_search_word (jika != -1 berarti sudah ada) ->
         bila belum ada, insert_word_sorted.
   ------------------------------------------------------------- */
void add_word(void) {
    char buf[WORD_LEN];
    int  i;

    /* TODO */
    if (word_count >= MAX_WORDS) {
        printf("\nBank kata sudah penuh.\n");
        return;
    }

    printf("\nMasukkan kata baru: ");
    scanf("%29s", buf);

    to_upper_str(buf);

    printf("  Mengecek bank kata (binary search)...\n");

    if (binary_search_word(word_bank, word_count, buf) != -1) {
        printf("  '%s' sudah ada di bank kata.\n", buf);
    } else {
        printf("  '%s' belum ada. Menyisipkan pada posisi terurut...\n", buf);

        insert_word_sorted(word_bank, &word_count, buf);

        printf("  Bank kata sekarang: ");
        for (i = 0; i < word_count; i++) {
            printf("%s ", word_bank[i]);
        }
        printf("\n  Kata berhasil ditambahkan.\n");
    }
}

/* -------------------------------------------------------------
   to_upper_str : mengubah seluruh isi string menjadi huruf besar
                  (diubah di tempat / in-place).

   PARAMETER:
     s (char *) : string yang akan diubah. Isi pada alamat ini diubah
                  langsung sehingga pemanggil ikut melihat hasilnya.

   VARIABEL LOKAL: tidak wajib menambah variabel; Anda boleh memakai s
                   sendiri sebagai pointer berjalan (s++), atau memakai
                   sebuah pencacah int bila lebih mudah.

   NILAI BALIK: (void)
   ------------------------------------------------------------- */
void to_upper_str(char *s) {
    /* TODO */
    while (*s != '\0') {
        *s = toupper(*s);
        s++;
    }
}
