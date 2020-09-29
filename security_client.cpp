#ifdef _WIN32
#include <iostream>
#include <bitset>
#include <cstring>
#include <cstdlib>
#include <sstream>
#include <iomanip>
#include <fstream>
#elif __linux__
#include <bits/stdc++.h>
#endif
#include "AES/aes.h"
using namespace std;

int main(int argc, char* argv[]){
    string key;
    cout<<"AES Key? ->";
    getline(cin, key);
    string choice, iv;
    cout<<"Do you want to use AES-CBC mode of encryption?y or n ->";
    getline(cin, choice);
    if(choice == "y"){
        cout<<"Initialization Vector? ->";
        getline(cin, iv);
    } else if(choice == "n"){
        cout<<"Encryption using AES-ECB mode\n";
    }

    Byte key_array[4*Nk];
    Byte state_array[4*Nk];
    Byte cipher_state_array[4*Nk];
    Byte initialization_vector[4*Nk];

    u_i dec_values[16];
    string hex_values[16];
    string iv_hex_values[16];

    Word key_array_32[4*(Nr+1)];

    /*Covert key to bits format*/
    for(int i = 0;i<16; i++){
        dec_values[i] = (int)key[i];
        key_array[i] = dec_values[i];
        stringstream ss;
        ss << hex << dec_values[i];
        string res(ss.str());
        hex_values[i] = res;
    }

    Byte encrypted_text[10000][16];
    Byte cbc_decrypted_text[10000][16];
    Byte cbc_encrypted_text[10000][16];
    Byte text_matrix[10000][4*Nk];



    /*print binary format of the key*/
    cout<<"Key:\n";
    for(int i=0 ;i<16; i++){
        if(i>0 && i%4==0){
            cout<<endl;
        }
        cout<<hex<<key_array[i].to_ulong()<< "\t";
    }

    cout<<"\n\n";

    /*Expanded key*/
    cout<<"Expanded Key: "<<endl;
    expandedKey(key_array, key_array_32);
    for(int i=0;i<4*(Nr+1); i++){
        cout<<"expandedKey_w["<<dec<<i<<"]\t-->\t"<<hex<<key_array_32[i].to_ulong()<<"\n";
    }
    if(choice == "y"){
        cout<<"\n\nAES - CBC Mode of encryption and decryption\n";
        fstream plain_file, encrypted_file;
        plain_file.open(argv[1], ios::in);
        encrypted_file.open(argv[2], ios::trunc | ios::out | ios::in);
        int row = 0, col = 0;
        int drow=0, dcol=0;
        
        if(!plain_file){
            cout<<"No such file exists\n";
        } else{
            char ch;
            while(!plain_file.eof()){
                plain_file.get(ch);
                if(col == 16){
                    row += 1;
                    col = 0;
                }
                text_matrix[row][col] = (int)ch;
                col+=1;
            }
        }
        for(int i=0; i<=row; i++){
            for(int j=0; j<16; j++){
                state_array[j] = text_matrix[i][j];
            }
            if(i == 0){
                for(int a=0; a<16; a++){
                    state_array[a] = state_array[a] ^ initialization_vector[a];
                }
            } else{
                for(int a=0; a<16; a++){
                    state_array[a] = state_array[a] ^ cbc_encrypted_text[i-1][a];
                }
            }
            aes_encrypt(state_array, key_array_32);
            for(int q=0; q<16; q++){
                cbc_encrypted_text[i][q] = state_array[q];
            }
        }
        while(encrypted_file){
            for(int i=0; i<=row; i++){
                for(int j=0; j<16; j++){
                    encrypted_file<< (char)cbc_encrypted_text[i][j].to_ulong();
                }
            }
            cout<<"\n\nEncrypted file is created\n\n";
            break;
        }
        encrypted_file.close();
        plain_file.close();
        cout<<"\n\n";
        encrypted_file.open(argv[2], ios::in);
        if(!encrypted_file){
            cout<<"\nfile is not encrypted or doesnot exixts\n";   
        }else{
            char e_ch;
            while(!encrypted_file.eof()){
                encrypted_file.get(e_ch);
                if(dcol == 16){
                    drow += 1;
                    dcol = 0;
                }
                cbc_encrypted_text[drow][dcol] = (int)e_ch;
                dcol+=1;
            }
        }
        cout<<endl;
        cout<<"\n\nDecrypted file contents:\n\n";
        for(int i=0; i<drow; i++){
            for(int j=0; j<16; j++){
                cipher_state_array[j] = cbc_encrypted_text[i][j];
            }
            aes_decrypt(cipher_state_array, key_array_32);
            if(i == 0){
                for(int a=0; a<16; a++){
                    cipher_state_array[a] = cipher_state_array[a] ^ initialization_vector[a];
                }
            } else{
                for(int a=0; a<16; a++){
                    cipher_state_array[a] = cipher_state_array[a] ^ cbc_encrypted_text[i-1][a];
                }
            }
            for(int a=0; a<16; a++){
                cout<<(char)cipher_state_array[a].to_ulong();
            }
        }
        encrypted_file.close();
    } else if(choice == "n"){
        cout<<"\n\nAES - ECB Mode of encryption and decryption\n";
        fstream plain_file, encrypted_file;
        plain_file.open(argv[1], ios::in);
        encrypted_file.open(argv[2], ios::trunc | ios::out | ios::in);
        int row = 0, col = 0;
        int drow=0, dcol=0;
        
        if(!plain_file){
            cout<<"No such file exists\n";
        } else{
            char ch;
            while(!plain_file.eof()){
                plain_file.get(ch);
                if(col == 16){
                    row += 1;
                    col = 0;
                }
                text_matrix[row][col] = (int)ch;
                col+=1;
            }
        }
        for(int i=0; i<=row; i++){
            for(int j=0; j<16; j++){
                state_array[j] = text_matrix[i][j];
            }
            aes_encrypt(state_array, key_array_32);
            for(int q=0; q<16; q++){
                cbc_encrypted_text[i][q] = state_array[q];
            }
        }
        while(encrypted_file){
            for(int i=0; i<=row; i++){
                for(int j=0; j<16; j++){
                    encrypted_file<< (char)cbc_encrypted_text[i][j].to_ulong();
                }
            }
            cout<<"\n\nEncrypted file is created\n\n";
            break;
        }
        encrypted_file.close();
        plain_file.close();
        cout<<"\n\n";
        encrypted_file.open(argv[2], ios::in);
        if(!encrypted_file){
            cout<<"\nfile is not encrypted or doesnot exixts\n";   
        }else{
            char e_ch;
            while(!encrypted_file.eof()){
                encrypted_file.get(e_ch);
                if(dcol == 16){
                    drow += 1;
                    dcol = 0;
                }
                cbc_encrypted_text[drow][dcol] = (int)e_ch;
                dcol+=1;
            }
        }
        cout<<endl;
        cout<<"\n\nDecrypted file contents:\n\n";
        for(int i=0; i<drow; i++){
            for(int j=0; j<16; j++){
                cipher_state_array[j] = cbc_encrypted_text[i][j];
            }
            aes_decrypt(cipher_state_array, key_array_32);
            for(int a=0; a<16; a++){
                cout<<(char)cipher_state_array[a].to_ulong();
            }
        }
        encrypted_file.close();
    }
    
    cout<<endl;
    return 0;
}