#ifdef _WIN32
#include <iostream>
#include <bitset>
#include <cstring>
#include <cstdlib>
#include <sstream>
#include <iomanip>
#elif __linux__
#include <bits/stdc++.h>
#endif
#include "AES/aes.h"
using namespace std;

int main(){
    string plaintext;
    cout<<"Plaintext? -> ";
    getline(cin, plaintext);
    string key;
    cout<<"Key? ->";
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
    Byte plaintext_array[10000][4*Nk];
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

    cout<<"\n\nTotal length of the plaintext = "<<plaintext.size()<<endl;

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
    Byte cbc_decrypted_text[1000][16];
    Byte cbc_encrypted_text[1000][16];

    cout<<"\n\nTotal plaintext blocks each of size 128 bit = "<<endl<<c<<"\n\n";

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

    

    /*AES-CBC 128 bit encryption*/
    if(choice == "y"){
        for(int i = 0;i<16; i++){
            initialization_vector[i] = (int)iv[i];
            stringstream ss_iv;
            ss_iv << hex << (int)iv[i];
            string res_iv(ss_iv.str());
            iv_hex_values[i] = res_iv;
        }
        cout<<"\n\nAES - CBC Encrypted tex\n";
        for(int i=0; i<c; i++){
            for(int j=0; j<16; j++){
                state_array[j] = plaintext_array[i][j];
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
                if (q>0 && q%4==0){
                    cout<<endl;
                }
                cout<<hex<<state_array[q].to_ulong()<<"\t";
                cbc_encrypted_text[i][q] = state_array[q];
            }
            cout<<"\n\n";
        }

        cout<<"AES-CBC decrypted text\n";
        for(int i=0; i<c; i++){
            for(int j=0 ;j<16; j++){
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
            for(int q=0; q<16; q++){
                cout<<(char)cipher_state_array[q].to_ulong();
                cbc_decrypted_text[i][q] = cipher_state_array[q];
            }
        }
    }
    cout<<endl;
    return 0;

}