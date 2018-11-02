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