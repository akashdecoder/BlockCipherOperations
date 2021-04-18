#ifdef _WIN32
#include <iostream>
#include <bitset>
#include <cstring>
#include <cstdlib>
#include <sstream>
#include <iomanip>
#elif __unix_
#include <bits/stdc++.h>
#endif

#include "aes.h"

using namespace std;

Word getWord(Byte b1, Byte b2, Byte b3, Byte b4){
    Word result(0x00000000);
    Word temp;

    temp = b1.to_ulong();
    temp <<= 24;
    result |= temp;

    temp = b2.to_ulong();
    temp <<= 16;
    result |= temp;

    temp = b3.to_ulong();
    temp <<= 8;
    result |= temp;

    temp = b4.to_ulong();
    result |= temp;

    return result;
}

Word rotWord(Word org_Word){
    Word w1 = org_Word << 8;
    Word w2 = org_Word >> 24;
    return w1 | w2;
}

Word subWord(Word org_Word){
    Word temp;
    for(int i=0; i<32; i+=8){
        int row = org_Word[i+7]*8 + org_Word[i+6]*4 + org_Word[i+5]*2 + org_Word[i+4];
        int col = org_Word[i+3]*8 + org_Word[i+2]*4 + org_Word[i+1]*2 + org_Word[i];

        Byte val = s_box[row][col];
        for(int j=0; j<8; j++){
            temp[i+j] = val[j];
        }
    }
    return temp;
}

/*Expanding the input key*/
void expandedKey(Byte key[4*Nk], Word key_array_32[4*(Nr+1)]){
    Word temp;
    int i = 0;

    while(i < Nk){
        key_array_32[i] = getWord(key[4*i], key[4*i +1], key[4*i +2], key[4*i + 3]);
        ++i;
    }
    i = Nk;
    while(i < 4*(Nr+1)){
        temp = key_array_32[i-1];
        if(i%4 == 0){
            key_array_32[i] = key_array_32[i-Nk] ^ subWord(rotWord(temp)) ^ r_con[i/4 - 1];
        } else {
            key_array_32[i] = key_array_32[i - Nk] ^ temp;
        }
        ++i;
    }
}

/*Key Addition Dunction*/
void addRoundKey(Byte state_array[16], Word in_key[4]){
    for(int i=0; i<4; i++){
        Word k1 = in_key[i] >> 24;
        Word k2 = (in_key[i] << 8) >> 24;
        Word k3 = (in_key[i] << 16) >> 24;
        Word k4 = (in_key[i] << 24) >> 24;

        state_array[i] = state_array[i] ^ Byte(k1.to_ulong());
        state_array[i+4] = state_array[i+4] ^ Byte(k2.to_ulong());
        state_array[i+8] = state_array[i+8] ^ Byte(k3.to_ulong());
        state_array[i+12] = state_array[i+12] ^ Byte(k4.to_ulong());
    }
}

/*Substition layer*/
void subBytes(Byte state_array[16]){
    for(int i=0; i<16; i++){
        int row = state_array[i][7]*8 + state_array[i][6]*4 + state_array[i][5]*2 + state_array[i][4];
        int col = state_array[i][3]*8 + state_array[i][2]*4 + state_array[i][1]*2 + state_array[i][0];
        state_array[i] = s_box[row][col];
    }
}

/*Shift Rows sub layer*/
void shiftRows(Byte state_array[16]){
    /*second row of the state_matrix(one bit to left)*/
    Byte temp = state_array[4];
    for(int i=0; i<3; i++){
        state_array[i+4] = state_array[i+5];
    }
    state_array[7] = temp;

    /*third row of the state matrix(two bit to left)*/
    for(int i=0; i<2; i++){
        temp = state_array[i+8];
        state_array[i+8] = state_array[i+10];
        state_array[i+10] = temp;
    }

    /*fourth row of the state matrix(three bit to left)*/
    temp = state_array[15];
    for(int i=3; i>0; i--){
        state_array[i+12] = state_array[i+11];
    }
    state_array[12] = temp;
}

/*Doing Galois Field Multiplication: Internet Source*/
Byte GFMul(Byte b1, Byte b2){
    Byte x = 0;
    Byte high_bit_set;
    for(int i=0; i<8; i++){
        if((b2 & Byte(1)) != 0){
            x ^= b1;
        }
        high_bit_set = (Byte)(b1 & Byte(0x80));
        b1 <<= 1;

        if(high_bit_set != 0){
            b1 ^= 0x1b;
        }
        b2 >>= 1;
    }
    return x;  
}

/*Mix Column Sublayer*/
void mixColumn(Byte state_array[16]){
    Byte temp[4];
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            temp[j] = state_array[i+j*4];
        }
        state_array[i] = GFMul(0x02, temp[0]) ^ GFMul(0x03, temp[1]) ^ temp[2] ^ temp[3];
        state_array[i+4] = temp[0] ^ GFMul(0x02, temp[1]) ^ GFMul(0x03, temp[2]) ^ temp[3];
        state_array[i+8] = temp[0] ^ temp[1] ^ GFMul(0x02, temp[2]) ^ GFMul(0x03, temp[3]);
        state_array[i+12] = GFMul(0x03, temp[0]) ^ temp[1] ^ temp[2] ^ GFMul(0x02, temp[3]);
    }
}


/*Encryption Function*/
void aes_encrypt(Byte state_array[16], Word key_array_32[4*(Nr+1)]){
    Word aes_key[4];
    for(int i=0; i<4; i++){
        aes_key[i] = key_array_32[i];
    }

    addRoundKey(state_array, aes_key);

    for(int aes_round=1; aes_round < Nr; aes_round++){
        subBytes(state_array);
        shiftRows(state_array);
        mixColumn(state_array);
        for(int j=0; j<4; j++){
            aes_key[j] = key_array_32[4*aes_round+j];
        }
        addRoundKey(state_array, aes_key);
    }

    subBytes(state_array);
    shiftRows(state_array);
    for(int i=0; i<4; i++){
        aes_key[i] = key_array_32[4*Nr+i];
    }
    addRoundKey(state_array, aes_key);
}


/*Inverse Substitution Layer*/
void inv_subBytes(Byte cipher_state_array[16]){
    for(int i=0; i<16; i++){
        int row = cipher_state_array[i][7]*8 + cipher_state_array[i][6]*4 + cipher_state_array[i][5]*2 + cipher_state_array[i][4];
        int col = cipher_state_array[i][3]*8 + cipher_state_array[i][2]*4 + cipher_state_array[i][1]*2 + cipher_state_array[i][0];
        cipher_state_array[i] = inv_s_box[row][col];
    }
}

/*Inverse Shift Rows Sub Layer*/
void inv_shiftRows(Byte cipher_state_array[16]){
    /*second row of the state_matrix(one bit to right)*/
    Byte temp = cipher_state_array[7];
    for(int i=3; i>0; i--){
        cipher_state_array[i+4] = cipher_state_array[i+3];
    }
    cipher_state_array[4] = temp;

    /*third row of the state matrix(two bit to left)*/
    for(int i=0; i<2; i++){
        temp = cipher_state_array[i+8];
        cipher_state_array[i+8] = cipher_state_array[i+10];
        cipher_state_array[i+10] = temp;
    }

    /*fourth row of the state matrix(three bit to left)*/
    temp = cipher_state_array[12];
    for(int i=0; i<3; i++){
        cipher_state_array[i+12] = cipher_state_array[i+13];
    }
    cipher_state_array[15] = temp;
}

/*Inverse Mix Column Sub Layer*/
void inv_mixColumn(Byte cipher_state_array[16]){
    Byte temp[4];
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            temp[j] = cipher_state_array[i+j*4];
        }
        cipher_state_array[i] = GFMul(0x0e, temp[0]) ^ GFMul(0x0b, temp[1]) ^ GFMul(0x0d, temp[2]) ^ GFMul(0x09, temp[3]);
        cipher_state_array[i+4] = GFMul(0x09, temp[0]) ^ GFMul(0x0e, temp[1]) ^ GFMul(0x0b, temp[2]) ^ GFMul(0x0d, temp[3]);
        cipher_state_array[i+8] = GFMul(0x0d, temp[0]) ^ GFMul(0x09, temp[1]) ^ GFMul(0x0e, temp[2]) ^ GFMul(0x0b, temp[3]);
        cipher_state_array[i+12] = GFMul(0x0b, temp[0]) ^ GFMul(0x0d, temp[1]) ^ GFMul(0x09, temp[2]) ^ GFMul(0x0e, temp[3]);
    }
}

/*Decryption Function*/
void aes_decrypt(Byte cipher_state_array[16], Word key_array_32[4*(Nr+1)]){
    Word aes_key[4];
    for(int i=0; i<4; i++){
        aes_key[i] = key_array_32[4*Nr+i];
    }
    addRoundKey(cipher_state_array, aes_key);

    for(int aes_rounds=Nr-1; aes_rounds>0; aes_rounds--){
        inv_shiftRows(cipher_state_array);
        inv_subBytes(cipher_state_array);
        for(int j=0; j<4; j++){
            aes_key[j] = key_array_32[4*aes_rounds+j];
        }
        addRoundKey(cipher_state_array, aes_key);
        inv_mixColumn(cipher_state_array);
    }

    inv_shiftRows(cipher_state_array);
    inv_subBytes(cipher_state_array);
    for(int i=0; i<4; i++){
        aes_key[i] = key_array_32[i];
    }
    addRoundKey(cipher_state_array, aes_key);
}