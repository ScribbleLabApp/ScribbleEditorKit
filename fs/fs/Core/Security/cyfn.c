//
//  cyfn.c
//  ScribbleLabApp File System Interface
//
//  Copyright (c) 2025 ScribbleLab. All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are met:
//
//  1. Redistributions of source code must retain the above copyright notice, this
//     list of conditions and the following disclaimer.
//
//  2. Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//  3. Neither the name of the copyright holder nor the names of its
//     contributors may be used to endorse or promote products derived from
//     this software without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
//  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
//  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
//  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
//  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
//  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
//  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
//  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <cyfn.h>
#include <string.h>

/*
 This is an implementation of the AES algorithm, specifically ECB, CTR and CBC mode.
 Block size can be chosen in include/cyfn.h - available choices are AES128, AES192, AES256.
 
 The implementation is verified against the test vectors in:
 National Institute of Standards and Technology Special Publication 800-38A 2001 ED
 
 ECB-AES128
 ----------
 
 plain-text:
 6bc1bee22e409f96e93d7e117393172a
 ae2d8a571e03ac9c9eb76fac45af8e51
 30c81c46a35ce411e5fbc1191a0a52ef
 f69f2445df4f9b17ad2b417be66c3710
 
 key:
 2b7e151628aed2a6abf7158809cf4f3c
 
 resulting cipher
 3ad77bb40d7a3660a89ecaf32466ef97
 f5d3d58503b9699de785895a96fdbaaf
 43b1cd7f598ece23881b00e3ed030688
 7b0c785e27e8ad3f8223207104725dd4
 
 NOTE:   String length must be evenly divisible by 16byte (str_len % 16 == 0)
 You should pad the end of the string with zeros if this is not the case.
 For AES192/256 the key size is proportionally larger.
 */

#define CYFN_Nb 4

#if defined(CYFN_AES256) && (CYFN_AES256 == 1)
#define CYFN_Nk 8
#define CYFN_Nr 14
#elif defined(CYFN_AES192) && (CYFN_AES192 == 1)
#define CYFN_Nk 6
#define CYFN_Nr 12
#else
#define CYFN_Nk 4
#define CYFN_Nr 10
#endif

#ifndef CYFN_MULTIPLY_AS_A_FUNCTION
#define CYFN_MULTIPLY_AS_A_FUNCTION 0
#endif

/* state - array holding the intermediate results during decryption. */
typedef uint8_t cyfn_state_t[4][4];

/*
 The lookup-tables are marked const so they can be placed in read-only storage instead of RAM
 The numbers below can be computed dynamically trading ROM for RAM -
 This can be useful in (embedded) bootloader applications, where ROM is often limited.
*/

/**
 * @brief A static constant array representing the S-box.
 *
 * The array contains 256 elements, each representing a byte value. The S-box is used in various cryptographic
 * transformations to enhance security by providing non-linearity.
 */
static const uint8_t cyfn_sbox[256] = {
    /* 0    1    2      3     4    5     6     7      8    9     A      B    C     D     E     F */
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 };

#if (defined(CYFN_CBC) && CYFN_CBC == 1) || (defined(CYFN_ECB) && CYFN_ECB == 1)
/**
 * The cyfn_rsbox array is a static constant array of 256 uint8_t elements.
 * It is used as a reverse S-box in cryptographic algorithms.
 *
 * The reverse S-box is a lookup table used in the decryption process of
 * symmetric key algorithms such as AES (Advanced Encryption Standard).
 * It is the inverse of the forward S-box, which is used in the encryption process.
 *
 * The values in the cyfn_rsbox array are precomputed and represent the
 * multiplicative inverses in the finite field GF(2^8) followed by an
 * affine transformation.
 *
 * @note This array is defined as static, meaning it has internal linkage
 * and is only accessible within the translation unit where it is defined.
 */
static const uint8_t cyfn_rsbox[256] = {
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
    0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
    0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d };
#endif

/*
 The round constant word array, cyfn_Rcon[i], contains the values given by
 x to the power (i-1) being powers of x (x is denoted as {02}) in the field GF(2^8)
*/

/**
 * @brief Round constants for AES key expansion.
 *
 * The cyfn_Rcon array is a static constant array of 11 uint8_t elements.
 * It is used in cryptographic algorithms, specifically in the key expansion
 * phase of the AES (Advanced Encryption Standard) algorithm. Each element
 * of the array represents a round constant used in the key schedule.
 *
 * @param cyfn_Rcon[i] The round constant for round i.
 */
static const uint8_t cyfn_Rcon[11] = {
    0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36
};

#define cyfn_getSBoxValue(num) (cyfn_sbox[(num)])

/**
 * @brief Expands the cipher key into a series of round keys for AES encryption.
 *
 * This function generates the round keys required for AES encryption by expanding
 * the provided cipher key. The expanded keys are stored in the RoundKey array.
 *
 * @param[out] RoundKey Pointer to the array where the expanded round keys will be stored.
 *                      The size of this array should be (CYFN_Nb * (CYFN_Nr + 1)) * 4 bytes.
 * @param[in]  Key      Pointer to the input cipher key. The size of this array should be CYFN_Nk * 4 bytes.
 *
 * @note The function assumes that the constants CYFN_Nk, CYFN_Nb, and CYFN_Nr are defined
 *       and correspond to the key length, block size, and number of rounds for the AES
 *       encryption, respectively.
 *
 * @note If CYFN_AES256 is defined and set to 1, the function includes additional key expansion
 *       steps specific to AES-256.
 *
 * @details
 * The key expansion process involves the following steps:
 * 1. Copy the initial key into the first part of the RoundKey array.
 * 2. For each subsequent word in the RoundKey array:
 *    a. If the word index is a multiple of CYFN_Nk, perform a key schedule core operation:
 *       - Rotate the word bytes to the left.
 *       - Apply the S-box substitution to each byte.
 *       - XOR the first byte with a round constant.
 *    b. If CYFN_AES256 is defined and the word index is a multiple of 4 (but not a multiple of CYFN_Nk),
 *       apply the S-box substitution to each byte.
 *    c. XOR the word with the word CYFN_Nk positions earlier in the RoundKey array.
 *
 * @see cyfn_getSBoxValue
 * @see cyfn_Rcon
 */
static void cyfn_keyExpansion(
                              uint8_t* RoundKey,
                              const uint8_t* Key
                              ) {
    unsigned i, j, k;
    uint8_t tempa[4];
    
    for (i = 0; i < CYFN_Nk; ++i) {
        RoundKey[(i * 4) + 0] = Key[(i * 4) + 0];
        RoundKey[(i * 4) + 1] = Key[(i * 4) + 1];
        RoundKey[(i * 4) + 2] = Key[(i * 4) + 2];
        RoundKey[(i * 4) + 3] = Key[(i * 4) + 3];
    }
    
    for (i = CYFN_Nk; i < CYFN_Nb * (CYFN_Nr + 1); ++i) {
        {
            k = (i - 1) * 4;
            tempa[0]=RoundKey[k + 0];
            tempa[1]=RoundKey[k + 1];
            tempa[2]=RoundKey[k + 2];
            tempa[3]=RoundKey[k + 3];
            
        }
        
        if (i % CYFN_Nk == 0) {
            // This function shifts the 4 bytes in a word to the left once.
            // [a0,a1,a2,a3] becomes [a1,a2,a3,a0]
            
            {
                const uint8_t u8tmp = tempa[0];
                tempa[0] = tempa[1];
                tempa[1] = tempa[2];
                tempa[2] = tempa[3];
                tempa[3] = u8tmp;
            }
            
            // SubWord() is a function that takes a four-byte input word and
            // applies the S-box to each of the four bytes to produce an output word.
            
            {
                tempa[0] = cyfn_getSBoxValue(tempa[0]);
                tempa[1] = cyfn_getSBoxValue(tempa[1]);
                tempa[2] = cyfn_getSBoxValue(tempa[2]);
                tempa[3] = cyfn_getSBoxValue(tempa[3]);
            }
            
            tempa[0] = tempa[0] ^ cyfn_Rcon[i/CYFN_Nk];
        }
        
#if defined(CYFN_AES256) && (CYFN_AES256 == 1)
        
        if (i % CYFN_Nk == 4) {
            {
                tempa[0] = cyfn_getSBoxValue(tempa[0]);
                tempa[1] = cyfn_getSBoxValue(tempa[1]);
                tempa[2] = cyfn_getSBoxValue(tempa[2]);
                tempa[3] = cyfn_getSBoxValue(tempa[3]);
            }
        }
#endif /* if defined(CYFN_AES256) && (CYFN_AES256 == 1) */
        
        j = i * 4; k=(i - CYFN_Nk) * 4;
        RoundKey[j + 0] = RoundKey[k + 0] ^ tempa[0];
        RoundKey[j + 1] = RoundKey[k + 1] ^ tempa[1];
        RoundKey[j + 2] = RoundKey[k + 2] ^ tempa[2];
        RoundKey[j + 3] = RoundKey[k + 3] ^ tempa[3];
    }
}

/**
 * @brief Initializes the cyfn context with the provided key.
 *
 * This function sets up the cyfn context by expanding the provided key
 * and storing the resulting round keys in the context structure.
 *
 * @param ctx Pointer to the cyfn context structure to be initialized.
 * @param key Pointer to the key used for initialization. The key should
 *            be a valid key for the cyfn algorithm.
 *
 * @note The key expansion process is critical for the security of the
 *       cyfn algorithm. Ensure that the key is kept secure and not
 *       exposed to unauthorized entities.
 */
void cyfn_init_ctx(
                   struct cyfn_ctx* ctx,
                   const uint8_t* key
                   ) {
    cyfn_keyExpansion(ctx->cyfn_roundKey, key);
}

#if (defined(CYFN_CBC) && (CYFN_CBC == 1)) || (defined(CYFN_CTR) && (CYFN_CTR == 1))
/**
 * @brief Initializes the cyfn context with the given key and IV (Initialization Vector).
 *
 * This function sets up the cyfn context by expanding the provided key and copying the IV.
 * It prepares the context for subsequent encryption or decryption operations.
 *
 * @param ctx Pointer to the cyfn context structure to be initialized.
 * @param key Pointer to the key used for encryption/decryption. The key length should be appropriate for the cyfn algorithm.
 * @param iv Pointer to the initialization vector. The IV length should be CYFN_AES_BLOCKLEN bytes.
 *
 * @note The key and IV should be securely generated and managed to ensure the security of the encryption/decryption process.
 */
void cyfn_init_ctx_iv(
                      struct cyfn_ctx* ctx,
                      const uint8_t* key,
                      const uint8_t* iv
                      ) {
    cyfn_keyExpansion(ctx->cyfn_roundKey, key);
    memcpy (ctx->cyfn_iv, iv, CYFN_AES_BLOCKLEN);
}

/**
 * @brief Sets the initialization vector (IV) for the given cyfn context.
 *
 * This function copies the provided initialization vector (IV) into the
 * cyfn context structure. The IV is used in cryptographic operations to
 * ensure that identical plaintext blocks produce different ciphertext
 * blocks, enhancing security.
 *
 * @param ctx A pointer to the cyfn context structure where the IV will be set.
 *            This structure must be properly initialized before calling this function.
 * @param iv  A pointer to the initialization vector. The IV must be at least
 *            CYFN_AES_BLOCKLEN bytes long.
 *
 * @note It is the caller's responsibility to ensure that the IV provided is
 *       of the correct length and properly initialized.
 */
void cyfn_ctx_set_iv(
                     struct cyfn_ctx* ctx,
                     const uint8_t* iv
                     ) {
    memcpy (ctx->cyfn_iv, iv, CYFN_AES_BLOCKLEN);
}
#endif

// This function adds the round key to state.
// The round key is added to the state by an XOR function.

/**
 * @brief Adds the round key to the state during the AES encryption process.
 *
 * This function performs the AddRoundKey step in the AES encryption algorithm.
 * It XORs each byte of the state with the corresponding byte from the round key.
 *
 * @param round The current round number of the AES encryption process.
 * @param state A pointer to the state array, which is a 4x4 matrix of bytes.
 *              The state array will be modified in place.
 * @param RoundKey A pointer to the round key array, which contains the round keys
 *                 for all rounds of the AES encryption process.
 *
 * @note The state array and the round key array must be properly initialized
 *       before calling this function. The round key array must contain enough
 *       bytes for all rounds of the AES encryption process.
 *
 * @warning This function modifies the state array in place.
 */
static void AddRoundKey(
                        uint8_t round,
                        cyfn_state_t* state,
                        const uint8_t* RoundKey
                        ) {
    uint8_t i,j;
    for (i = 0; i < 4; ++i) {
        for (j = 0; j < 4; ++j) {
            (*state)[i][j] ^= RoundKey[(round * CYFN_Nb * 4) + (i * CYFN_Nb) + j];
        }
    }
}

// The SubBytes Function Substitutes the values in the
// state matrix with values in an S-box.

/**
 * @brief Performs the SubBytes transformation on the given state.
 *
 * The SubBytes transformation is a non-linear substitution step where each byte in the state is replaced
 * with its corresponding value from the S-box (substitution box). This is a crucial step in the AES (Advanced
 * Encryption Standard) encryption algorithm, providing confusion in the cipher.
 *
 * @param state A pointer to the state array (4x4 matrix) on which the SubBytes transformation is performed.
 *
 * The state array is modified in place, with each byte being substituted by its corresponding S-box value.
 * The S-box is a fixed table used in the AES algorithm to perform byte substitution.
 *
 * @code
 * cyfn_state_t state = {
 *     {0x32, 0x88, 0x31, 0xe0},
 *     {0x43, 0x5a, 0x31, 0x37},
 *     {0xf6, 0x30, 0x98, 0x07},
 *     {0xa8, 0x8d, 0xa2, 0x34}
 * };
 * SubBytes(&state);
 * @endcode
 */
static void SubBytes(cyfn_state_t* state) {
    uint8_t i, j;
    for (i = 0; i < 4; ++i) {
        for (j = 0; j < 4; ++j) {
            (*state)[j][i] = cyfn_getSBoxValue((*state)[j][i]);
        }
    }
}

// The ShiftRows() function shifts the rows in the state to the left.
// Each row is shifted with different offset.
// Offset = Row number. So the first row is not shifted.

/**
 * @brief Performs the ShiftRows step in the AES encryption algorithm.
 *
 * This function shifts the rows of the state matrix to the left by a certain number of columns.
 * Specifically:
 * - The first row is rotated 1 column to the left.
 * - The second row is rotated 2 columns to the left.
 * - The third row is rotated 3 columns to the left.
 *
 * @param state A pointer to the state matrix (4x4 array) to be modified.
 *
 * The state matrix is represented as a 4x4 array of bytes (uint8_t). The function modifies the
 * state matrix in place.
 *
 * Example:
 * @code
 * cyfn_state_t state = {
 *     {0x00, 0x01, 0x02, 0x03},
 *     {0x10, 0x11, 0x12, 0x13},
 *     {0x20, 0x21, 0x22, 0x23},
 *     {0x30, 0x31, 0x32, 0x33}
 * };
 * ShiftRows(&state);
 *
 * // After calling ShiftRows, the state matrix will be:
 * // {
 * //     {0x00, 0x11, 0x22, 0x33},
 * //     {0x10, 0x21, 0x32, 0x03},
 * //     {0x20, 0x31, 0x02, 0x13},
 * //     {0x30, 0x01, 0x12, 0x23}
 * // }
 * @endcode
 */
static void ShiftRows(cyfn_state_t* state) {
    uint8_t temp;
    
    // Rotate first row 1 columns to left
    temp           = (*state)[0][1];
    (*state)[0][1] = (*state)[1][1];
    (*state)[1][1] = (*state)[2][1];
    (*state)[2][1] = (*state)[3][1];
    (*state)[3][1] = temp;
    
    // Rotate second row 2 columns to left
    temp           = (*state)[0][2];
    (*state)[0][2] = (*state)[2][2];
    (*state)[2][2] = temp;
    
    temp           = (*state)[1][2];
    (*state)[1][2] = (*state)[3][2];
    (*state)[3][2] = temp;
    
    // Rotate third row 3 columns to left
    temp           = (*state)[0][3];
    (*state)[0][3] = (*state)[3][3];
    (*state)[3][3] = (*state)[2][3];
    (*state)[2][3] = (*state)[1][3];
    (*state)[1][3] = temp;
}

/**
 * @brief Multiplies a given byte by 2 in the Galois Field (2^8).
 *
 * This function performs multiplication of a given byte by 2 in the
 * Galois Field (2^8), which is commonly used in cryptographic algorithms
 * such as the Advanced Encryption Standard (AES). The multiplication is
 * performed using the xtime operation, which involves a left shift and
 * a conditional XOR with the polynomial 0x1b if the most significant bit
 * of the input byte is set.
 *
 * @param x The input byte to be multiplied by 2.
 * @return The result of the multiplication in the Galois Field (2^8).
 */
static uint8_t xtime(uint8_t x) {
    return ((x<<1) ^ (((x>>7) & 1) * 0x1b));
}

// MixColumns function mixes the columns of the state matrix

/**
 * @brief Mixes the columns of the state matrix in the AES encryption algorithm.
 *
 * This function performs the MixColumns step of the AES encryption algorithm.
 * It operates on the state matrix, which is a 4x4 matrix of bytes. The MixColumns
 * transformation is a linear mixing operation that combines the four bytes in each
 * column of the state matrix.
 *
 * @param state Pointer to the state matrix (4x4 array of bytes) to be transformed.
 *
 * The MixColumns transformation is defined as follows:
 * - Each byte in a column is replaced by a linear combination of all four bytes in that column.
 * - The linear combination is defined by the fixed polynomial {03}x^3 + {01}x^2 + {01}x + {02}.
 *
 * The function uses the xtime() helper function to perform multiplication by 2 in the Galois Field (2^8).
 *
 * Example:
 * @code
 * cyfn_state_t state = {
 *     {0x87, 0xF2, 0x4D, 0x97},
 *     {0x6E, 0x4C, 0x90, 0xEC},
 *     {0x46, 0xE7, 0x4A, 0xC3},
 *     {0xA6, 0x8C, 0xD8, 0x95}
 * };
 * MixColumns(&state);
 * // The state matrix is now transformed.
 * @endcode
 */
static void MixColumns(cyfn_state_t* state) {
    uint8_t i;
    uint8_t Tmp, Tm, t;
    
    for (i = 0; i < 4; ++i) {
        t   = (*state)[i][0];
        Tmp = (*state)[i][0] ^ (*state)[i][1] ^ (*state)[i][2] ^ (*state)[i][3] ;
        Tm  = (*state)[i][0] ^ (*state)[i][1] ; Tm = xtime(Tm);  (*state)[i][0] ^= Tm ^ Tmp ;
        Tm  = (*state)[i][1] ^ (*state)[i][2] ; Tm = xtime(Tm);  (*state)[i][1] ^= Tm ^ Tmp ;
        Tm  = (*state)[i][2] ^ (*state)[i][3] ; Tm = xtime(Tm);  (*state)[i][2] ^= Tm ^ Tmp ;
        Tm  = (*state)[i][3] ^ t ;              Tm = xtime(Tm);  (*state)[i][3] ^= Tm ^ Tmp ;
    }
}

// Multiply is used to multiply numbers in the field GF(2^8)
// Note: The last call to xtime() is unneeded, but often ends up generating a smaller binary
//       The compiler seems to be able to vectorize the operation better this way.
#if MULTIPLY_AS_A_FUNCTION
static uint8_t Multiply(uint8_t x, uint8_t y) {
    return (((y & 1) * x) ^
            ((y>>1 & 1) * xtime(x)) ^
            ((y>>2 & 1) * xtime(xtime(x))) ^
            ((y>>3 & 1) * xtime(xtime(xtime(x)))) ^
            ((y>>4 & 1) * xtime(xtime(xtime(xtime(x)))))); /* this last call to xtime() can be omitted */
}
#else

#define Multiply(x, y)                          \
(  ((y & 1) * x) ^                              \
((y>>1 & 1) * xtime(x)) ^                       \
((y>>2 & 1) * xtime(xtime(x))) ^                \
((y>>3 & 1) * xtime(xtime(xtime(x)))) ^         \
((y>>4 & 1) * xtime(xtime(xtime(xtime(x))))))   \

#endif

#if (defined(CYFN_CBC) && CBC == 1) || (defined(CYFN_ECB) && CYFN_ECB == 1)

#define cyfn_getSBoxInvert(num) (cyfn_rsbox[(num)])

// MixColumns function mixes the columns of the state matrix.
// The method used to multiply may be difficult to understand for the inexperienced.
// Please use the references to gain more information.
static void InvMixColumns(cyfn_state_t* state) {
    int i;
    uint8_t a, b, c, d;
    for (i = 0; i < 4; ++i) {
        a = (*state)[i][0];
        b = (*state)[i][1];
        c = (*state)[i][2];
        d = (*state)[i][3];
        
        (*state)[i][0] = Multiply(a, 0x0e) ^ Multiply(b, 0x0b) ^ Multiply(c, 0x0d) ^ Multiply(d, 0x09);
        (*state)[i][1] = Multiply(a, 0x09) ^ Multiply(b, 0x0e) ^ Multiply(c, 0x0b) ^ Multiply(d, 0x0d);
        (*state)[i][2] = Multiply(a, 0x0d) ^ Multiply(b, 0x09) ^ Multiply(c, 0x0e) ^ Multiply(d, 0x0b);
        (*state)[i][3] = Multiply(a, 0x0b) ^ Multiply(b, 0x0d) ^ Multiply(c, 0x09) ^ Multiply(d, 0x0e);
    }
}


// The SubBytes Function Substitutes the values in the
// state matrix with values in an S-box.
static void InvSubBytes(cyfn_state_t* state) {
    uint8_t i, j;
    for (i = 0; i < 4; ++i) {
        for (j = 0; j < 4; ++j) {
            (*state)[j][i] = cyfn_getSBoxInvert((*state)[j][i]);
        }
    }
}

static void InvShiftRows(cyfn_state_t* state) {
    uint8_t temp;
    
    // Rotate first row 1 columns to right
    temp = (*state)[3][1];
    (*state)[3][1] = (*state)[2][1];
    (*state)[2][1] = (*state)[1][1];
    (*state)[1][1] = (*state)[0][1];
    (*state)[0][1] = temp;
    
    // Rotate second row 2 columns to right
    temp = (*state)[0][2];
    (*state)[0][2] = (*state)[2][2];
    (*state)[2][2] = temp;
    
    temp = (*state)[1][2];
    (*state)[1][2] = (*state)[3][2];
    (*state)[3][2] = temp;
    
    // Rotate third row 3 columns to right
    temp = (*state)[0][3];
    (*state)[0][3] = (*state)[1][3];
    (*state)[1][3] = (*state)[2][3];
    (*state)[2][3] = (*state)[3][3];
    (*state)[3][3] = temp;
}
#endif // #if (defined(CBC) && CBC == 1) || (defined(ECB) && ECB == 1)

// Cipher is the main function that encrypts the PlainText.
static void Cipher(cyfn_state_t* state, const uint8_t* RoundKey) {
    uint8_t round = 0;
    
    // Add the First round key to the state before starting the rounds.
    AddRoundKey(0, state, RoundKey);
    
    // There will be Nr rounds.
    // The first Nr-1 rounds are identical.
    // These Nr rounds are executed in the loop below.
    // Last one without MixColumns()
    for (round = 1; ; ++round) {
        SubBytes(state);
        ShiftRows(state);
        if (round == CYFN_Nr) {
            break;
        }
        MixColumns(state);
        AddRoundKey(round, state, RoundKey);
    }
    // Add round key to last round
    AddRoundKey(CYFN_Nr, state, RoundKey);
}

//#if (defined(CYFN_CBC) && CYFN_CBC == 1) || (defined(CYFN_ECB) && CYFN_ECB == 1)
#if (defined(CYFN_CBC) && CBC == 1) || (defined(CYFN_ECB) && CYFN_ECB == 1)
static void InvCipher(cyfn_state_t* state, const uint8_t* RoundKey) {
    uint8_t round = 0;
    
    // Add the First round key to the state before starting the rounds.
    AddRoundKey(CYFN_Nr, state, RoundKey);
    
    // There will be Nr rounds.
    // The first Nr-1 rounds are identical.
    // These Nr rounds are executed in the loop below.
    // Last one without InvMixColumn()
    for (round = (CYFN_Nr - 1); ; --round) {
        InvShiftRows(state);
        InvSubBytes(state);
        AddRoundKey(round, state, RoundKey);
        if (round == 0) {
            break;
        }
        InvMixColumns(state);
    }
    
}
#endif /* #if (defined(CYFN_CBC) && CYFN_CBC == 1) || (defined(CYFN_ECB) && CYFN_ECB == 1) */

/*****************************************************************************/
/* Public functions:                              */
/*****************************************************************************/
#if defined(CYFN_ECB) && (CYFN_ECB == 1)

void cyfn_ecb_encrypt(const struct cyfn_ctx* ctx, uint8_t* buf) {
    // The next function call encrypts the PlainText with the Key using AES algorithm.
    Cipher((cyfn_state_t*)buf, ctx->cyfn_roundKey);
}

void cyfn_ecb_decrypt(const struct cyfn_ctx* ctx, uint8_t* buf) {
    // The next function call decrypts the PlainText with the Key using AES algorithm.
    InvCipher((cyfn_state_t*)buf, ctx->cyfn_roundKey);
}

#endif /* #if defined(CYFN_ECB) && (CYFN_ECB == 1) */

#if defined(CYFN_CBC) && (CYFN_CBC == 1)
static void XorWithIv(uint8_t* buf, const uint8_t* Iv) {
    uint8_t i;
    
    for (i = 0; i < CYFN_AES_BLOCKLEN; ++i) {
        buf[i] ^= Iv[i];
    }
}

void cyfn_cbc_encrypt_buffer(struct cyfn_ctx *ctx, uint8_t* buf, size_t length) {
    size_t i;
    uint8_t *Iv = ctx->cyfn_iv;
    
    for (i = 0; i < length; i += CYFN_AES_BLOCKLEN) {
        XorWithIv(buf, Iv);
        Cipher((cyfn_state_t*)buf, ctx->cyfn_roundKey);
        Iv = buf;
        buf += CYFN_AES_BLOCKLEN;
    }
    /* store Iv in ctx for next call */
    memcpy(ctx->cyfn_iv, Iv, CYFN_AES_BLOCKLEN);
}

void cyfn_cbc_decrypt_buffer(struct cyfn_ctx* ctx, uint8_t* buf, size_t length) {
    size_t i;
    uint8_t storeNextIv[CYFN_AES_BLOCKLEN];
    
    for (i = 0; i < length; i += CYFN_AES_BLOCKLEN) {
        memcpy(storeNextIv, buf, CYFN_AES_BLOCKLEN);
        InvCipher((cyfn_state_t*)buf, ctx->cyfn_roundKey);
        XorWithIv(buf, ctx->cyfn_iv);
        memcpy(ctx->cyfn_iv, storeNextIv, CYFN_AES_BLOCKLEN);
        buf += CYFN_AES_BLOCKLEN;
    }
    
}

#endif /* #if defined(CYFN_CBC) && (CYFN_CBC == 1) */

#if defined(CYFN_CTR) && (CYFN_CTR == 1)

/*
 Symmetrical operation: same function for encrypting as for decrypting.
 Note any IV/nonce should never be reused with the same key
 */
void cyfn_ctr_xcrypt_buffer(struct cyfn_ctx* ctx, uint8_t* buf, size_t length) {
    uint8_t buffer[CYFN_AES_BLOCKLEN];
    
    size_t i;
    int bi;
    
    for (i = 0, bi = CYFN_AES_BLOCKLEN; i < length; ++i, ++bi) {
        if (bi == CYFN_AES_BLOCKLEN) {
            memcpy(buffer, ctx->cyfn_iv, CYFN_AES_BLOCKLEN);
            Cipher((cyfn_state_t*)buffer,ctx->cyfn_roundKey);
            
            /* Increment Iv and handle overflow */
            for (bi = (CYFN_AES_BLOCKLEN - 1); bi >= 0; --bi) {
                if (ctx->cyfn_iv[bi] == 255) {
                    ctx->cyfn_iv[bi] = 0;
                    continue;
                }
                
                ctx->cyfn_iv[bi] += 1;
                break;
            }
            
            bi = 0;
        }
        
        buf[i] = (buf[i] ^ buffer[bi]);
    }
}

#endif /* #if defined(CYFN_CTR) && (CYFN_CTR == 1) */