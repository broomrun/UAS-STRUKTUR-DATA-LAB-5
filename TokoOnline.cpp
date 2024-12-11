#include <bits/stdc++.h>
using namespace std;

struct Produk {
    int id;
    string nama;
    string kategori;
    int harga;
};

struct RiwayatPembayaran {
    int idTransaksi;
    vector<pair<int, int>> keranjang; // ID produk dan kuantitas
    int totalHarga;
    int biayaPengiriman;
};

bool sortByHargaAsc(const Produk &a, const Produk &b) {
    return a.harga < b.harga;
}

void tampilkanProduk(const vector<Produk> &produk) {
    for (const auto &p : produk) {
        cout << "ID: " << p.id << ", Nama: " << p.nama
             << ", Kategori: " << p.kategori
             << ", Harga: Rp " << p.harga << endl;
    }
}

vector<Produk> filterByKategori(const vector<Produk> &produk, const string &kategori) {
    vector<Produk> hasil;
    for (const auto &p : produk) {
        if (p.kategori == kategori) {
            hasil.push_back(p);
        }
    }
    return hasil;
}

void tambahKeKeranjang(vector<pair<int, int>> &keranjang, const vector<Produk> &produk, int idProduk) {
    for (auto &item : keranjang) {
        if (item.first == idProduk) {
            item.second++; // Tambahkan kuantitas
            cout << "Produk dengan ID " << idProduk << " telah diperbarui (jumlah ditambah).\n";
            return;
        }
    }
    for (const auto &p : produk) {
        if (p.id == idProduk) {
            keranjang.push_back({p.id, 1}); // Tambahkan produk baru
            cout << "Produk dengan ID " << idProduk << " berhasil ditambahkan ke keranjang.\n";
            return;
        }
    }
    cout << "Produk dengan ID " << idProduk << " tidak ditemukan.\n";
}

void hapusDariKeranjang(vector<pair<int, int>> &keranjang, int idProduk) {
    auto it = find_if(keranjang.begin(), keranjang.end(), [idProduk](const pair<int, int>& item) {
        return item.first == idProduk;
    });

    if (it != keranjang.end()) {
        keranjang.erase(it);
        cout << "Produk dengan ID " << idProduk << " berhasil dihapus dari keranjang.\n";
    } else {
        cout << "Produk dengan ID " << idProduk << " tidak ditemukan di keranjang.\n";
    }
}

void tampilkanKeranjang(const vector<pair<int, int>> &keranjang, const vector<Produk> &produk) {
    if (keranjang.empty()) {
        cout << "Keranjang belanja kosong.\n";
        return;
    }

    int totalHarga = 0;
    cout << "\nIsi keranjang belanja:\n";
    cout << "------------------------------------------\n";
    for (const auto &item : keranjang) {
        for (const auto &p : produk) {
            if (p.id == item.first) {
                int subtotal = p.harga * item.second;
                totalHarga += subtotal;
                cout << "ID: " << p.id
                     << ", Nama: " << p.nama
                     << ", Harga: Rp " << p.harga
                     << ", Kuantitas: " << item.second
                     << ", Subtotal: Rp " << subtotal << endl;
                break;
            }
        }
    }
    cout << "------------------------------------------\n";
    cout << "Total Harga: Rp " << totalHarga << "\n";
}

void hitungBiayaPengiriman(int &biayaPengiriman) {
    int n = 10;
    vector<vector<pair<int, int>>> adj(n);
    
    adj[0] = {{2, 7}, {3, 4}, {6, 4}, {8, 5}};
    adj[1] = {};
    adj[2] = {{1, 5}};
    adj[3] = {};
    adj[4] = {{7, 7}};
    adj[5] = {{4, 9}, {7, 8}, {9, 3}};
    adj[6] = {{1, 4}, {5, 11}, {4, 2}};
    adj[7] = {};
    adj[8] = {{5, 10}};
    adj[9] = {{7, 4}};

    double costPerKm = 3000;
    int start = 0, end;

    cout << "Masukkan lokasi tujuan (1-9): ";
    cin >> end;

    // Algoritma Dijkstra
    vector<int> dist(n, INT_MAX);
    vector<bool> visited(n, false);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    dist[start] = 0;
    pq.push({0, start});

    while (!pq.empty()) {
        auto [currDist, currNode] = pq.top();
        pq.pop();

        if (visited[currNode]) continue;
        visited[currNode] = true;

        for (auto [nextNode, weight] : adj[currNode]) {
            if (dist[currNode] + weight < dist[nextNode]) {
                dist[nextNode] = dist[currNode] + weight;
                pq.push({dist[nextNode], nextNode});
            }
        }
    }

    if (dist[end] == INT_MAX) {
        cout << "Tidak ada jalur ke lokasi tujuan.\n";
        biayaPengiriman = 0;
    } else {
        biayaPengiriman = dist[end] * costPerKm;
        cout << "Biaya pengiriman ke lokasi tujuan: Rp " << biayaPengiriman << endl;
    }
}

void lakukanPembayaran(vector<pair<int, int>> &keranjang, const vector<Produk> &produk, vector<RiwayatPembayaran> &riwayat, int &idTransaksiBerikutnya) {
    if (keranjang.empty()) {
        cout << "Keranjang kosong. Tidak ada pembayaran yang dapat dilakukan.\n";
        return;
    }

    int totalHarga = 0;
    cout << "\nDetail pembayaran:\n";
    for (const auto &item : keranjang) {
        for (const auto &p : produk) {
            if (p.id == item.first) {
                int subtotal = p.harga * item.second;
                totalHarga += subtotal;
                cout << "ID: " << p.id
                     << ", Nama: " << p.nama
                     << ", Kuantitas: " << item.second
                     << ", Subtotal: Rp " << subtotal << endl;
                break;
            }
        }
    }
    
    int biayaPengiriman;
    hitungBiayaPengiriman(biayaPengiriman);
    totalHarga += biayaPengiriman;

    cout << "Total Harga (termasuk pengiriman): Rp " << totalHarga << "\n";
    char konfirmasi;
    cout << "Konfirmasi pembayaran? (y/n): ";
    cin >> konfirmasi;
    if (konfirmasi == 'y' || konfirmasi == 'Y') {
        riwayat.push_back({idTransaksiBerikutnya++, keranjang, totalHarga, biayaPengiriman});
        keranjang.clear();
        cout << "Pembayaran berhasil. Riwayat pembayaran disimpan.\n";
    } else {
        cout << "Pembayaran dibatalkan.\n";
    }
}

void lihatRiwayatPembayaran(const vector<RiwayatPembayaran> &riwayat, const vector<Produk> &produk) {
    if (riwayat.empty()) {
        cout << "Belum ada riwayat pembayaran.\n";
        return;
    }

    cout << "\nRiwayat pembayaran:\n";
    for (const auto &r : riwayat) {
        cout << "ID Transaksi: " << r.idTransaksi << "\n";
        cout << "Detail:\n";
        for (const auto &item : r.keranjang) {
            for (const auto &p : produk) {
                if (p.id == item.first) {
                    cout << "  ID: " << p.id
                         << ", Nama: " << p.nama
                         << ", Kuantitas: " << item.second
                         << ", Subtotal: Rp " << p.harga * item.second << endl;
                    break;
                }
            }
        }
        cout << "Total Harga: Rp " << r.totalHarga << "\n\n";
    }
}

void lihatDetailTransaksi(const vector<RiwayatPembayaran> &riwayat, const vector<Produk> &produk, int idTransaksi) {
    auto it = find_if(riwayat.begin(), riwayat.end(), [idTransaksi](const RiwayatPembayaran& r) {
        return r.idTransaksi == idTransaksi;
    });

    if (it == riwayat.end()) {
        cout << "Transaksi dengan ID " << idTransaksi << " tidak ditemukan.\n";
        return;
    }

    cout << "\nDetail Transaksi ID: " << idTransaksi << "\n";
    for (const auto &item : it->keranjang) {
        for (const auto &p : produk) {
            if (p.id == item.first) {
                cout << "  ID: " << p.id
                     << ", Nama: " << p.nama
                     << ", Kuantitas: " << item.second
                     << ", Subtotal: Rp " << p.harga * item.second << endl;
                break;
            }
        }
    }
    cout << "Total Harga: Rp " << it->totalHarga << "\nBiaya Pengiriman: Rp " << it->biayaPengiriman << endl;
}

int main() {
    vector<Produk> produk = {
        {1, "Smartphone Samsung Galaxy S23", "Elektronik", 12000000},
        {2, "Laptop ASUS ROG Zephyrus G14", "Elektronik", 25000000},
        {3, "TV LED LG 43 Inch", "Elektronik", 6500000},
        {4, "Earbuds Apple AirPods Pro", "Elektronik", 3500000},
        {5, "Kamera DSLR Canon EOS 90D", "Elektronik", 17000000},
        {6, "Sepeda MTB Polygon", "Fitness", 5000000},
        {7, "Dumbbell 5kg", "Fitness", 200000},
        {8, "Matras Yoga", "Fitness", 250000},
        {9, "Mesin Elliptical", "Fitness", 7500000},
        {10, "Resistance Band", "Fitness", 100000},
        {11, "Serum Wajah Vitamin C", "Kecantikan", 150000},
        {12, "Masker Wajah Aloe Vera", "Kecantikan", 75000},
        {13, "Lipstik Matte L'Oréal", "Kecantikan", 120000},
        {14, "Parfum Chanel", "Kecantikan", 2500000},
        {15, "Sabun Cuci Muka Himalaya", "Kecantikan", 35000},
        {16, "Roti Tawar Serba Roti", "Konsumsi", 15000},
        {17, "Kopi Arabica 100g", "Konsumsi", 50000},
        {18, "Mie Instan", "Konsumsi", 5000},
        {19, "Susu UHT Indomilk 1 Liter", "Konsumsi", 18000},
        {20, "Teh Kotak Sosro 500ml", "Konsumsi", 7500}
    };

    vector<pair<int, int>> keranjang;
    vector<RiwayatPembayaran> riwayatPembayaran;
    int idTransaksiBerikutnya = 1;

    int pilihan;
    do {
        cout << "\nMenu:\n"
             << "1. Tampilkan Produk Berdasarkan Kategori\n"
             << "2. Urutkan Produk Berdasarkan Harga\n"
             << "3. Tambah Produk ke Keranjang\n"
             << "4. Tampilkan Keranjang\n"
             << "5. Hapus Produk dari Keranjang\n"
             << "6. Melakukan Pembayaran\n"
             << "7. Melihat Riwayat Pembayaran\n"
             << "8. Melihat Detail Transaksi\n"
             << "9. Keluar\n"
             << "Pilihan: ";
        cin >> pilihan;

        if (pilihan == 1) {
            string kategori;
            cout << "Masukkan kategori (Elektronik/Fitness/Kecantikan/Konsumsi): ";
            cin >> kategori;
            vector<Produk> hasil = filterByKategori(produk, kategori);
            if (hasil.empty()) {
                cout << "Tidak ada produk dengan kategori tersebut.\n";
            } else {
                tampilkanProduk(hasil);
            }
        } else if (pilihan == 2) {
            sort(produk.begin(), produk.end(), sortByHargaAsc);
            tampilkanProduk(produk);
        } else if (pilihan == 3) {
            int idProduk;
            cout << "Masukkan ID produk yang ingin ditambahkan ke keranjang: ";
            cin >> idProduk;
            tambahKeKeranjang(keranjang, produk, idProduk);
        } else if (pilihan == 4) {
            tampilkanKeranjang(keranjang, produk);
        } else if (pilihan == 5) {
            int idProduk;
            cout << "Masukkan ID produk yang ingin dihapus dari keranjang: ";
            cin >> idProduk;
            hapusDariKeranjang(keranjang, idProduk);
        } else if (pilihan == 6) {
            lakukanPembayaran(keranjang, produk, riwayatPembayaran, idTransaksiBerikutnya);
        } else if (pilihan == 7) {
            lihatRiwayatPembayaran(riwayatPembayaran, produk);
        } else if (pilihan == 8) {
            int idTransaksi;
            cout << "Masukkan ID transaksi yang ingin dilihat detailnya: ";
            cin >> idTransaksi;
            lihatDetailTransaksi(riwayatPembayaran, produk, idTransaksi);
        } else if (pilihan == 9) {
            cout << "Terima kasih! Sampai jumpa.\n";
        } else {
            cout << "Pilihan tidak valid.\n";
        }
    } while (pilihan != 9);

    return 0;
}