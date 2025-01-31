#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
using namespace std;

const int MAX_ITEMS = 100;       
const int MAX_TRANSACTIONS = 50; 

struct Item {
    string name;
    double price;
    int stock;
};

struct Transaction {
    string orderedItems[MAX_ITEMS];
    int itemCount;
    double totalPrice;
    double paidAmount;
    double change;
};

void loadMenu(Item menu[], int& itemCount);
void saveMenu(const Item menu[], int itemCount);
void displayMenu(const Item menu[], int itemCount);
void order(Item menu[], int& itemCount, Transaction transactions[], int& transactionCount);
void displayTransactions(const Transaction transactions[], int transactionCount);
void saveTransactions(const Transaction transactions[], int transactionCount);

int main() {
    Item menu[MAX_ITEMS];
    Transaction transactions[MAX_TRANSACTIONS];
    int itemCount = 0, transactionCount = 0;

    loadMenu(menu, itemCount);

    int choice;
    do {
        cout << "\n======= Kasir Toko Sembako ========\n";
        cout << "1. Tampilkan Menu\n2. Pesan Barang\n3. Tampilkan Riwayat Transaksi\n4. Keluar\n";
        cout << "Pilih menu: ";
        cin >> choice;
        //cout << "===================================";

        switch (choice) {
            case 1: displayMenu(menu, itemCount); break;
            case 2: order(menu, itemCount, transactions, transactionCount); break;
            case 3: displayTransactions(transactions, transactionCount); break;
            case 4: cout << "Terima kasih telah menggunakan sistem kasir.\n"; break;
            default: cout << "Pilihan tidak valid. Coba lagi.\n";
        }
    } while (choice != 4);

    saveMenu(menu, itemCount);
    saveTransactions(transactions, transactionCount);

    return 0;
}

void loadMenu(Item menu[], int& itemCount) {
    ifstream file("menu.txt");
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string name;
            double price;
            int stock;
            ss >> name >> price >> stock;

            if (ss.fail()) {
                cout << "Format data menu salah pada baris: " << line << "\n";
                continue;
            }

            menu[itemCount].name = name;
            menu[itemCount].price = price;
            menu[itemCount].stock = stock;
            itemCount++;

            if (itemCount >= MAX_ITEMS) break;
        }
        file.close();
    } else {
        cout << "File menu tidak ditemukan. Membuat menu baru.\n";
    }
}

void saveMenu(const Item menu[], int itemCount) {
    ofstream file("menu.txt");
    for (int i = 0; i < itemCount; i++) {
        file << menu[i].name << " " << menu[i].price << " " << menu[i].stock << endl;
    }
    file.close();
}

void displayMenu(const Item menu[], int itemCount) {
    if (itemCount == 0) {
        cout << "Menu kosong. Tambahkan data terlebih dahulu.\n";
        return;
    }

    cout << "\n===================================\n";
    cout << "\tNama Barang\t\t\n";
    cout << "===================================\n";
    cout << left << setw(20) << "Nama" << setw(10) << "Harga" << "Stok\n";
    cout << "===================================" << endl;
    for (int i = 0; i < itemCount; i++) {
        cout << left << setw(20) << menu[i].name << "Rp " << setw(8) << menu[i].price << menu[i].stock << endl;
    }
    cout << "===================================\n";
}

void order(Item menu[], int& itemCount, Transaction transactions[], int& transactionCount) {
    if (transactionCount >= MAX_TRANSACTIONS) {
        cout << "Transaksi penuh. Tidak dapat memproses pesanan baru.\n";
        return;
    }

    Transaction& transaction = transactions[transactionCount];
    transaction.itemCount = 0;
    transaction.totalPrice = 0;

    string itemName;
    int quantity;

    cout << "\n=== Pembelian Barang ===\n";
    while (true) {
        displayMenu(menu, itemCount);
        cout << "Masukkan nama item (atau 'selesai' untuk selesai): ";
        cin >> itemName;

        if (itemName == "selesai") break;

        bool found = false;
        for (int i = 0; i < itemCount; i++) {
            if (menu[i].name == itemName) {
                found = true;
                cout << "Masukkan jumlah: ";
                cin >> quantity;

                if (quantity <= menu[i].stock) {
                    menu[i].stock -= quantity;
                    double itemPrice = quantity * menu[i].price;
                    transaction.totalPrice += itemPrice;

                    stringstream ss;
                    ss << itemName << " x" << quantity << " = Rp" << fixed << setprecision(2) << itemPrice;
                    transaction.orderedItems[transaction.itemCount++] = ss.str();
                } else {
                    cout << "Stok tidak mencukupi.\n";
                }
                break;
            }
        }

        if (!found) {
            cout << "Item tidak ditemukan.\n";
        }
    }

    if (transaction.totalPrice == 0) {
        cout << "Tidak ada item yang dipesan.\n";
        return;
    }
	
	cout << "\n==============================================\n";
    cout << "Total harga                      : Rp " << fixed << setprecision(2) << transaction.totalPrice << endl;
    double paidAmount;
    do {
        cout << "Masukkan jumlah uang dibayarkan  : Rp ";
        cin >> paidAmount;

        if (paidAmount >= transaction.totalPrice) {
            transaction.paidAmount = paidAmount;
            transaction.change = paidAmount - transaction.totalPrice;
            cout << "Pembayaran berhasil. Kembalian   : Rp " << fixed << setprecision(2) << transaction.change << endl;
            transactionCount++;
        } else {
            cout << "Uang tidak cukup. Coba lagi.\n";
        }
    } while (paidAmount < transaction.totalPrice);
}

void displayTransactions(const Transaction transactions[], int transactionCount) {
    if (transactionCount == 0) {
        cout << "Belum ada transaksi.\n";
        return;
    }

    cout << "\n========== Riwayat Transaksi ==========\n";
    for (int i = 0; i < transactionCount; i++) {
        cout << "Transaksi #" << i + 1 << ":\n";
        for (int j = 0; j < transactions[i].itemCount; j++) {
            cout << "- " << transactions[i].orderedItems[j] << endl;
        }
        cout << "Total     : Rp " << fixed << setprecision(2) << transactions[i].totalPrice << "\nDibayar   : Rp " 
             << fixed << setprecision(2) << transactions[i].paidAmount << "\nKembalian : Rp " 
             << fixed << setprecision(2) << transactions[i].change << "\n\n";
    }
}

void saveTransactions(const Transaction transactions[], int transactionCount) {
    ofstream file("transactions.txt");
    for (int i = 0; i < transactionCount; i++) {
        for (int j = 0; j < transactions[i].itemCount; j++) {
            file << transactions[i].orderedItems[j] << endl;
        }
        file << "Total: " << fixed << setprecision(2) << transactions[i].totalPrice 
             << " Dibayar: " << fixed << setprecision(2) << transactions[i].paidAmount
             << " Kembalian: " << fixed << setprecision(2) << transactions[i].change << "\n\n";
    }
    file.close();
}
