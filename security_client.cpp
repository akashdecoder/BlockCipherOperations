#include <bits/stdc++.h>
#include "AES/aes.h"
using namespace std;



int main(){
    string plaintext;
    getline(cin, plaintext);
    string key;
    getline(cin, key);

    byte key_array[4*Nk];
    byte plaintext_array[10000][4*Nk];
    byte state_array[4*Nk];
    byte cipher_state_array[4*Nk];

    u_i dec_values[16];
    string hex_values[16];

    word key_array_32[4*(Nr+1)];

    
    
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
    byte encrypted_text[c][16];
    byte decrypted_text[c][16];

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

}