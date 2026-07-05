# Tebak Kata - Robotika Edition

Proyek Akhir Praktikum Pemrograman Dasar

Aplikasi permainan Hangman berbasis Command Line Interface (CLI) bertema Robotika.
Program dibangun menggunakan bahasa C murni dengan menerapkan konsep-konsep inti pemrograman seperti manajemen memori, aritmatika pointer, binary search, insertion sort, dan bubble sort.

---

## Kompilasi dan Menjalankan Program

Program ditulis dalam standar ANSI C dan dapat dikompilasi menggunakan `gcc`.

### Kompilasi

```bash
gcc -Wall -o tebak_kata "tebak kata kiro.c"
```

Parameter `-Wall` digunakan untuk mengaktifkan semua peringatan guna memastikan kualitas kode.

---

## Menjalankan Program
### Windows:
```bash
tebak_kata.exe
```

### Linux / macOS:
```bash
./tebak_kata
```

---

## Daftar Fitur
- Main Game Baru: Permainan tebak kata klasik. Komputer memilih kata secara acak dari bank kata, pemain menebak huruf demi huruf. Input nama pemain mendukung nama dengan spasi.
- Sistem Skor Dinamis: Skor dihitung berdasarkan sisa nyawa dan panjang kata rahasia dengan rumus: `Skor = (Sisa Nyawa x 100) + (Panjang Kata x 20)`.
- Visual Hangman: Gambar tiang gantungan ditampilkan secara dinamis sesuai jumlah kesalahan, maksimal 6 kesalahan yang merepresentasikan kepala, badan, dua tangan, dan dua kaki.
- Hall of Fame: Menyimpan hingga 10 skor terbaik yang diurutkan secara descending menggunakan algoritma Bubble Sort, lengkap dengan tanggal pencapaian yang diambil otomatis dari sistem melalui `time.h`.
- Tambah Kata ke Bank: Pemain dapat menambah kata baru ke bank kata. Program mengecek duplikat menggunakan Binary Search, lalu menyisipkan kata secara alfabetis menggunakan Insertion Sort agar bank kata selalu terurut.
- Penanganan Input Robust: Membersihkan buffer input untuk mencegah infinite loop saat user memasukkan huruf di menu angka, validasi input Y/T pada konfirmasi Hall of Fame dengan perulangan hingga input valid, serta validasi agar tebakan hanya boleh berupa satu huruf per giliran.

## Keterbatasan Program
- Penyimpanan Sementara: Data kata baru dan Hall of Fame hanya tersimpan di memori selama program berjalan. Semua data akan hilang ketika program ditutup karena belum menggunakan File I/O.
- Kapasitas Terbatas: Bank kata dibatasi maksimal 50 entri (`MAX_WORDS`) dan Hall of Fame maksimal 10 entri (`HOF_SIZE`).
- Karakter Alfabet: Kata rahasia dan input tebakan hanya mendukung karakter huruf. Input dikonversi otomatis ke huruf kapital.
- Panjang Nama dan Kata: Nama pemain maksimal 19 karakter dan panjang kata rahasia maksimal 29 karakter.
