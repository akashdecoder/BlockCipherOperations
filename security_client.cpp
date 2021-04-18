<<<<<<< HEAD
#include <bits/stdc++.h>
#include "AES/aes.h"
using namespace std;



int main(){
    string plaintext;
    getline(cin, plaintext);
    string key;
    getline(cin, key);

    Byte key_array[4*Nk];
    Byte plaintext_array[10000][4*Nk];
    Byte state_array[4*Nk];
    Byte cipher_state_array[4*Nk];

    u_i dec_values[16];
    string hex_values[16];

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

    cout<<plaintext.size()<<endl;


    /*Create state matrix for the plaintext*/
    int index = 0, count=0, c=0;
    u_i i=0;
    while(i<plaintext.size()){
        c+=1;
        for(int j=0;j<16;j++){
            plaintext_array[index][j] = (int)plaintext[count];
            count+=1;
        }
        index+=1;
        i = count;
    }
    Byte encrypted_text[1000][16];
    Byte decrypted_text[1000][16];

    cout<<endl<<c<<"\n\n";

    /*print state mtrix*/
    for(int i=0;i<c; i++){
        cout<<"plain_text_block["<<i<<"]\t--->\t";
        for(int j=0;j<16;j++){
            cout<<dec<<plaintext_array[i][j].to_ulong()<<"\t";
        }
        cout<<endl;
    }

    cout<<endl;

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

    /*Encryption*/
    cout<<"\n\nEncrypted Text:\n";
    for(int i=0; i<c; i++){
        for(int j=0; j<16; j++){
            state_array[j] = plaintext_array[i][j];
        }
        aes_encrypt(state_array, key_array_32);
        for(int q=0; q<16; q++){
            encrypted_text[i][q] = state_array[q];
        }
        for(int k=0;k<16; k++){
            if(k>0 && k%4 == 0)
                cout<<endl;
            cout<<hex<<state_array[k].to_ulong()<<"\t";
        }
        cout<<"\n\n";
    }

    /*Decryption*/
    cout<<"\n\nDecrypted Text:\n";
    for(int i=0; i<c; i++){
        for(int j=0; j<16; j++){
            cipher_state_array[j] = encrypted_text[i][j];
        }
        aes_decrypt(cipher_state_array, key_array_32);
        for(int k=0;k<16; k++){
            // if(k>0 && k%4 == 0)
            //     cout<<endl;
            cout<<(char)cipher_state_array[k].to_ulong();
        }
        // cout<<"\n\n";
    }
    cout<<endl;
    return 0;

=======
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
    string choice, iv;
    string hex_values[16];
    string iv_hex_values[16];

    u_i dec_values[16];
    int ch;
    int row = 0, col = 0;
    int drow=0, dcol=0;

    Byte key_array[4*Nk];
    Byte state_array[4*Nk];
    Byte cipher_state_array[4*Nk];
    Byte initialization_vector[4*Nk];
    Byte encrypted_text[10000][16];
    Byte ecb_decrypted_text[10000][16];
    Byte ecb_encrypted_text[10000][16];
    Byte cbc_decrypted_text[10000][16];
    Byte cbc_encrypted_text[10000][16];
    Byte text_matrix[10000][4*Nk];

    Word key_array_32[4*(Nr+1)];
    string argv1 = argv[1];
    string argv2 = argv[2];
    string argv3 = argv[3];
    string argv4 = argv[4];
    string argv5 = argv[5];
    string argv6 = argv[6];
    fstream plain_file, encrypted_file;
    if(argc > 3 || argc < 7){
        cout<<"AES KEY? ->";
        getline(cin, key);

        /*Covert key to bits format*/
        for(int i = 0;i<16; i++){
            dec_values[i] = (int)key[i];
            key_array[i] = dec_values[i];
            stringstream ss;
            ss << hex << dec_values[i];
            string res(ss.str());
            hex_values[i] = res;
        }

        expandedKey(key_array, key_array_32);

        if((argv1.compare("enc") == 0) && (argv2.compare("aes-128-ecb") == 0)){
            if((argv3.compare("-in") == 0) && argv4.empty() != true && (argv5.compare("-out") == 0) && (argv6.empty() != true)){
                plain_file.open(argv4, ios::in);
                encrypted_file.open(argv6, ios::trunc | ios::out | ios::in);
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
                        ecb_encrypted_text[i][q] = state_array[q];
                    }
                }
                while(encrypted_file){
                    for(int i=0; i<=row; i++){
                        for(int j=0; j<16; j++){
                            encrypted_file<< (char)ecb_encrypted_text[i][j].to_ulong();
                        }
                    }
                    cout<<"\n\nEncrypted file is created\n\n";
                    break;
                }
                encrypted_file.close();
                plain_file.close();
                cout<<"\n\n";
            }
        } else if((argv1.compare("dec") == 0) && (argv2.compare("aes-128-ecb") == 0)){
            if((argv3.compare("-in") == 0) && (argv4.empty() != true)){
                encrypted_file.open(argv4, ios::in);
                if(!encrypted_file){
                    cout<<"\nfile is not encrypted or doesnot exixts\n";   
                } else{
                    char e_ch;
                    while(!encrypted_file.eof()){
                        encrypted_file.get(e_ch);
                        if(dcol == 16){
                            drow += 1;
                            dcol = 0;
                        }
                        ecb_encrypted_text[drow][dcol] = (int)e_ch;
                        dcol+=1;
                    }
                }
                cout<<endl;
                cout<<"\n\nDecrypted file contents:\n\n";
                for(int i=0; i<drow; i++){
                    for(int j=0; j<16; j++){
                        cipher_state_array[j] = ecb_encrypted_text[i][j];
                    }
                    aes_decrypt(cipher_state_array, key_array_32);
                    for(int a=0; a<16; a++){
                        cout<<(char)cipher_state_array[a].to_ulong();
                    }
                }
                encrypted_file.close();
                cout<<"\n\n\n";
            }
        } else if((argv1.compare("enc") == 0) && (argv2.compare("aes-128-cbc") == 0)){
            if((argv3.compare("-in") == 0) && argv4.empty() != true && (argv5.compare("-out") == 0) && (argv6.empty() != true)){
                cout<<"Initialization Vector? ->";
                getline(cin, iv);
                for(int i=0;i<16;i++){
                    initialization_vector[i] = (int)iv[i];
                }
                plain_file.open(argv4, ios::in);
                encrypted_file.open(argv6, ios::trunc | ios::out | ios::in);
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
            }
        } else if((argv1.compare("dec") == 0) && (argv2.compare("aes-128-cbc") == 0)){
            if((argv3.compare("-in") == 0) && (argv4.empty() != true)){
                cout<<"Initialization Vector? ->";
                getline(cin, iv);
                for(int i=0;i<16;i++){
                    initialization_vector[i] = (int)iv[i];
                }
                encrypted_file.open(argv4, ios::in);
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
                cout<<"\n\n\n";
            }
        }
    }
    
    return 0;
>>>>>>> 908d147137e3d74f1073f874be7757426f0e7062
}