



/*
 * Project: Caesar Cipher Encryption & Decryption
 * Author: Syed Mujtaba Zaidi
 * Date: 2025-08-10
 * Description:
 *   A C++ program to encrypt and decrypt text using Caesar Cipher,
 *   with colorful console UI.
 */


#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <filesystem>
#include <thread>
#include <chrono>


namespace fs = std::filesystem;
using namespace std;

// Color codes
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define RESET   "\033[0m"

// Progress bar function
void showProgressBar(const string& task, int length = 30, int delay_ms = 50) {
    cout << CYAN << "\n" << task << ": " << RESET;
    for (int i = 0; i <= length; i++) {
        cout << "\r" << CYAN << task << ": [" << GREEN;
        for (int j = 0; j < i; j++) cout << "#";
        for (int j = i; j < length; j++) cout << " ";
        cout << CYAN << "] " << YELLOW << (i * 100 / length) << "%" << RESET;
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(delay_ms));
    }
    cout << GREEN << "\n✔ Done!\n" << RESET;
}




// Random folder name generator
string generate_random_folder_name(int length = 10) {
    const string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    string result;
    for (int i = 0; i < length; i++) {
        result += chars[rand() % chars.size()];
    }
    return result;
}

// Caesar cipher encryption

string encrypt(const string& text, int shift) {
    string encrypted_text = "";
    for (char c : text) {
        if (isalpha(c)) {
            char base = isupper(c) ? 'A' : 'a';
            encrypted_text += char((c - base + shift) % 26 + base);
        } else {
            encrypted_text += c;
        }
    }
    return encrypted_text;
}


// Caesar cipher decryption
string decrypt(const string& text, int shift) {
    return encrypt(text, 26 - (shift % 26));
}

int main() {
    srand(time(NULL));  

    cout << YELLOW << "\n\n  ╔══════════════════════════════════════════════╗\n";
    cout << "  ║   Welcome to Encryption & Decryption         ║\n";
    cout << "  ║         Program (Caesar Cipher)              ║\n";
    cout << "  ╚══════════════════════════════════════════════╝\n\n" << RESET;

    string folder_name = "";  

    while (true) {
        cout << CYAN << "\n What do you want to do?\n" << RESET;
        cout << YELLOW << "\n1- Encryption\n" << RESET;
        cout << YELLOW << "2- Decryption\n" << RESET;
        cout << RED << "0- Exit\n" << RESET;
        cout << CYAN << "\n Enter choice: " << RESET;

        string choice;
        getline(cin, choice);

        if (choice == "0") {
            cout << GREEN << "\n Exiting program. Bye!\n" << RESET;
            break;
        }
        else if (choice == "1") {
            if (folder_name.empty()) {
                folder_name = generate_random_folder_name();
                try {
                    fs::create_directory(folder_name);
                    cout << GREEN << "\n📁 Folder created: '" << folder_name << "'\n" << RESET;
                } catch (const fs::filesystem_error& e) {
                    cerr << RED << " Error creating folder: " << e.what() << RESET << "\n";
                    return 1;
                }
            }

            cout << BLUE << "\n Enter your message (or press 0 to exit): " << RESET;
            string message;
            getline(cin, message);
            if (message == "0") break;

            int shift;
            while (true) {
                cout << CYAN << "\n 🔑 Enter shift value (1-25): " << RESET;
                string shift_input;
                getline(cin, shift_input);
                


                if (shift_input == "0") return 0;
                try {
                    shift = stoi(shift_input);
                    if (shift >= 1 && shift <= 25) break;
                    else cout << RED << " Please enter between 1 and 25.\n" << RESET;
                }
                catch (...) {
                    cout << RED << " Invalid number. Try again.\n" << RESET;
                }
            }

            string encrypted = encrypt(message, shift);


            showProgressBar("Encrypting");


            ofstream original_file(folder_name + "/original.txt");
            if (!original_file.is_open()) {
                cerr << RED << " Error: Could not create original.txt" << RESET << "\n";
                continue;
            }
            original_file << message;
            original_file.close();

            ofstream encrypted_file(folder_name + "/encrypted.txt");
            if (!encrypted_file.is_open()) {
                cerr << RED << " Error: Could not create encrypted.txt" << RESET << "\n";
                continue;
            }
            encrypted_file << encrypted;
            encrypted_file.close();

            cout << GREEN << "\n✔ Original saved: " << folder_name << "/original.txt" << RESET;
            cout << GREEN << "\n✔ Encrypted saved: " << folder_name << "/encrypted.txt" << RESET;
            cout << YELLOW << "\n🔒 Encrypted Text: " << encrypted << RESET << "\n";
        }
        else if (choice == "2") {
            if (folder_name.empty()) {
                cout << RED << "\n⚠ No folder found! Encrypt something first.\n" << RESET;
            }

            int shift;
            while (true) {
                cout << CYAN << "\n 🔑 Enter shift used for encryption: " << RESET;
                string shift_input;
                getline(cin, shift_input);


                if (shift_input == "0") return 0;
                try {
                    shift = stoi(shift_input);
                    if (shift >= 1 && shift <= 25) break;
                    else cout << RED << " Please enter between 1 and 25.\n" << RESET;
                }
                catch (...) {
                    cout << RED << " Invalid number. Try again.\n" << RESET;
                }
            }

            cout << BLUE << "\n 📄 Enter file path (Enter for default, 0 to exit): " << RESET;
            string file_path;
            getline(cin, file_path);

            if (file_path == "0") break;
            if (file_path.empty()) file_path = folder_name + "/encrypted.txt";

            if (fs::is_directory(file_path)) {
                cout << RED << " Path is a directory. Provide file path.\n" << RESET;
                continue;
            }

            ifstream file(file_path);
            if (!file.is_open()) {
                cerr << RED << " Error: Could not open file '" << file_path << "'" << RESET << "\n";
                continue;
            }

            string encrypted_from_file((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
            file.close();

            string decrypted = decrypt(encrypted_from_file, shift);

            showProgressBar("Decrypting");


            string save_folder = folder_name.empty() ? "." : folder_name;
            ofstream decrypted_file(save_folder + "/decrypted.txt");
            if (!decrypted_file.is_open()) {
                cerr << RED << " Error: Could not create decrypted.txt" << RESET << "\n";
                continue;
            }
            decrypted_file << decrypted;
            decrypted_file.close();


            cout << GREEN << "\n✔ Decrypted Text: " << decrypted << RESET;
            cout << GREEN << "\n✔ Saved: " << save_folder << "/decrypted.txt\n" << RESET;
        }
        else {
            cout << RED << " Invalid choice! Enter 0, 1, or 2.\n" << RESET;
        }
    }

    return 0;
}
