# Sliding-Window-UDP

## Cara Penggunaan
1. Pastikan Anda menggunakan sistem operasi "unix-like"
2. Tuliskan code berikut pada terminal (pastika berada pada *directory* yang tepat)
```
make
```
3. Cara menjalankan programnya
- Untuk sendfile
```
./sendfile <filename> <windowsize> <buffersize> <destination ip> <destination port>
```
- Untuk recvfile
```
./recvfile <filename> <windowsize> <buffersize> <port>
```

## Cara Kerja Sliding Window
### Receiver
Pada program ini digunakan beberapa tipe data bentukan untuk mengimplementasikan protokol UDP dan sliding window. Tipe data tersebut antara lain:
- <b>Frame</b>, yang memiliki atribut:
    - `SOH`: start of header (1 byte)
    - `sequenceNumber`: nomor urut paket (4 byte)
    - `dataLength`: panjang data yang dikirimkan pada paket (4 byte)
    - `data`: isi data yang ingin dikirimikan (1024 byte)
    - `checksum`: untuk error checking pada isi data (1 byte)
- <b>PacketACK</b>, yang memiliki atribut:
    - `ACK`: 1 jika ACK, 0 jika NACK (1 byte)
    - `nextSequenceNumber`: nomor urut paket selanjutnya (4 byte)
    - `checksum`: untuk error checking pada ACK (1 byte)

Algoritma sliding window diimplementasikan pada dua program utama, yaitu `recvfile.cpp` yang berfungsi sebagai receiver, dan `sendfile.cpp` yang berfungsi sebagai sender.

Sliding window pada receiver memiliki cara kerja sebagai berikut:
1. Menginisiasi receiver window dengan menentukan **LFR** (Last Frame Received) dan **LAF** (Largest Acceptable Frame). LFS pertama kali di-assign dengan nilai 0, dan LAF di-assign dengan nilai `LFS + WindowSize`.
2. Ketika menerima frame, receiver akan mengecek apakah `sequenceNumber <= LAF`. Jika tidak memenuhi, maka frame akan ditolak oleh receiver. Jika memenuhi, maka frame akan disimpan pada buffer.
3. Selanjutnya receiver akan mengecek apakah `sequenceNumber = LFR + 1`. 
    - Ketika syarat tersebut terpenuhi, sliding window akan bergeser sebanyak jumlah window yang sudah terisi. LFS akan diubah ke index windows baru yang belum terisi, sedangkan `LAF <- LFS + WindowSize`. Selain itu, data pada buffer dituliskan pada file eksternal.
    - Jika syarat tidak terpenuhi, receiver tetap menerima paket, namun tidak melakukan sliding window sampai paket dengan `sequenceNumber = LFR + 1` diterima.
4. Receiver terus menerus melakukan iterasi langkah 2 dan 3 (menerima file dari sender) sampai ditemukan frame `endOfTransfer`. Frame `endOfTransfer` diimplementasikan sebagai frame dengan sequence number 0. Setelah file ini diterima, maka receiver akan menghentikan iterasi.
5. Seluruh data pada buffer telah ditulis pada file eksternal yang diinginkan, data berhasil ditransfer. 

Fungsi yang digunakan pada sliding window di receiver adalah:
1. `readPacket`: membaca paket yang diterima dan mengubahnya ke bentuk frame.
2. `createACK`: membuat suatu variabel dengan tipe data PacketACK.
3. `convertToAckFrame`: mengonversi variabel dengan tipe data PacketACK ke tipe unsigned char * agar dapat dikirim lewat protokol.

### Sender