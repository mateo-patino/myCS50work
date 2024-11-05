#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>

using namespace std;

string encrypt(string key, string txt);
bool unvalid_chars(string key);

char toLowercase(char c) {
    return std::tolower(static_cast<unsigned char>(c));
}

int main(int argc, char *argv[]) {
    // Check for correct usage; convert to char* to string first by using constructor
    string key(argv[1]);
    string plaintext(argv[2]);
    if (argc != 3) {
        cout << "Usage: ./cipher KEY PLAINTEXT" << endl;
        return 1;
    }
    else if (key.length() != 26){
        cout << "KEY must have 26 non-repeated characters" << endl;
    }
    else if (unvalid_chars(key)){
        cout << "KEY cannot have repeated or non-alphabetical characters." << endl;
    }
    else {
        cout << "Encrypted text: " << encrypt(key, plaintext) << endl;
        return 0;
    }
}

string encrypt(string key, string txt) {
    // Copying the string assures same size and that non-alphabetical characters stay in place (untouched by for-loop)
    string ciphertext = txt;
    int txt_length = txt.length();
    int index = -1;
    for (int i = 0; i < txt_length; i++){
        if (islower(txt[i])) {
            index = int(txt[i]) - int('a');
            ciphertext[i] = tolower(key[index]);
        }
        else if (isupper(txt[i])) {
            index = int(txt[i]) - int('A');
            ciphertext[i] = toupper(key[index]);
        }
    }
    return ciphertext;
}

bool unvalid_chars(string key){
    // Lowercase the key, then check non-alpha chars.
    transform(key.begin(), key.end(), key.begin(), toLowercase);
    int key_length = key.length();

    for (int i = 0; i < key_length; i++){
        if (key[i] < int('a') || key[i] > int('z')){
            return true;
        }
    }
    // Check for repeated chars by storing checked chars in an array and using their value as index.
    bool abecedary[26] = {};
    int index = -1;
    for (int i = 0; i < key_length; i++){
        index = int(key[i]) - int('a');
        if (abecedary[index]){
            return true;
        }
        else {
            abecedary[index] = true;
        }
    }
    return false; // Only happens if previous return trues aren't triggered.
}
