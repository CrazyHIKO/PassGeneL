#include <iostream>
#include <string>
#include <Windows.h>
#include <bcrypt.h>
#pragma comment(lib, "bcrypt.lib")

std::string generate_password(int length, bool include_symbols) {
    const std::string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()-_=+[]{}|;:'\",.<>?/";
    std::string password(length, '\0');

    BCRYPT_ALG_HANDLE hAlgorithm;
    BCryptOpenAlgorithmProvider(&hAlgorithm, BCRYPT_RNG_ALGORITHM, NULL, 0);
    BCryptGenRandom(hAlgorithm, reinterpret_cast<PUCHAR>(&password[0]), length, 0);
    BCryptCloseAlgorithmProvider(hAlgorithm, 0);

    size_t char_set_size = include_symbols ? chars.size() : 62;
    for (int i = 0; i < length; ++i) {
        password[i] = chars[password[i] % char_set_size];
    }
    return password;
}

int get_valid_input() {
    int length;
    while (true) {
        std::string input;
        std::cout << "桁数? (1-50): ";
        std::cin >> input;
        if (input.length() > 2) continue;
        try {
            length = std::stoi(input);
            if (length >= 1 && length <= 50) return length;
        }
        catch (...) {}
    }
}

bool get_yes_no_input(const std::string& prompt) {
    std::string choice;
    while (true) {
        std::cout << prompt;
        std::cin >> choice;
        if (choice == "y" || choice == "Y") return true;
        if (choice == "n" || choice == "N") return false;
    }
}

int main() {
    std::cout << "PassGeneL1.0\nUses BCryptGenRandom\n(C) 2024/06/08 hiko & ChatGPT (GPT-4o)\n\n";

    int length = 18;
    bool include_symbols = true;
    bool first_run = true;

    while (true) {
        if (first_run || !include_symbols) {
            length = get_valid_input();
            include_symbols = get_yes_no_input("記号? (y/n): ");
        }

        std::string password = generate_password(length, include_symbols);
        std::cout << password << std::endl << std::endl;

        first_run = !get_yes_no_input("再生成? 同設定(y) 新設定(n): ");
    }

    return 0;
}
