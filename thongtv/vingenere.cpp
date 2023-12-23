#include <iostream>
#include <string>
#include <conio.h> // Thư viện để kiểm tra phím ESC

using namespace std;

string vigenereEncrypt(const string &message, const string &key) {
    string encryptedMessage = message;
    for (int i = 0; i < message.length(); i++) {
        char messageChar = message[i];
        char keyChar = key[i % key.length()];
        encryptedMessage[i] = ((messageChar + keyChar - 2 * 'A') % 26) + 'A';
    }
    return encryptedMessage;
}

string vigenereDecrypt(const string &cipherText, const string &key) {
    string decryptedMessage = cipherText;
    for (int i = 0; i < cipherText.length(); i++) {
        char cipherChar = cipherText[i];
        char keyChar = key[i % key.length()];
        decryptedMessage[i] = ((cipherChar - keyChar + 26) % 26) + 'A';
    }
    return decryptedMessage;
}

int main() {
    int choice;
    string input;
    cout << "Ho Ten: Tran Van Thong\n";
    cout << "MSV: 20T1020574 \n";
    cout << "-----------------------------------------------";
    do {    
        cout << "\nChon 1 de ma hoa hoac 2 de giai ma: ";
        cin >> choice;

        if (choice == 1) {
            cout << "Nhap bach van: ";
            cin >> input;
            string key;
            cout << "Nhap key: ";
            cin >> key;

            string encryptedMessage = vigenereEncrypt(input, key);
            cout << "Bach van vua nhap la: " << input << endl;
            cout << "Ban ma: " << encryptedMessage << endl;
        } else if (choice == 2) {
            cout << "Nhap ban ma: ";
            cin >> input;
            string key;
            cout << "Nhap key: ";
            cin >> key;

            string decryptedMessage = vigenereDecrypt(input, key);
            cout << "Ban ma vua nhap la: " << input << endl;
            cout << "Chuoi sau khi giai ma: " << decryptedMessage << endl;
        } else {
            cout << "Lua chon khong hop le, vui long chon lai." << endl;
        }
        cout << "-----------------------------------------------\n";
        cout << "Nhap ESC de thoat hoac nhap mot so bat ki de tiep tuc: ";
    } while ((choice != 1 || choice != 2) && _getch() != 27); // Sử dụng _getch() để kiểm tra phím ESC

    return 0;
}