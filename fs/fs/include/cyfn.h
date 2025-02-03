//
//  cyfn.h
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

#ifndef CYFN_H
#define CYFN_H

#include <stdint.h>
#include <stddef.h>

#ifndef CYFN_CBC
#define CYFN_CBC 0
#endif

#ifndef CYFN_ECB
#define CYFN_ECB 0
#endif

// Prefered mode
#ifndef CYFN_CTR
#define CYFN_CTR 1
#endif

#define CYFN_AES256 1

#define CYFN_AES_BLOCKLEN 16

#if defined(CYFN_AES256) && (CYFN_AES256 == 1)
#define CYFN_AES_KEYLEN 32
#define CYFN_AES_keyExpSize 240
#elif defined(CYFN_AES192) && (CYFN_AES192 == 1)
#define CYFN_AES_KEYLEN 24
#define CYFN_AES_keyExpSize 208
#else
#define CYFN_AES_KEYLEN 16
#define CYFN_AES_keyExpSize 176
#endif

/**
 * A structure that holds the context for AES encryption and decryption.
 *
 * This structure contains the expanded key and, optionally, the initialization vector (IV).
 *
 * @field cyfn_roundKey Expanded encryption key.
 * @field cyfn_iv Initialization vector (IV) used for CBC and CTR modes.
 */
struct cyfn_ctx {
    uint8_t cyfn_roundKey[CYFN_AES_keyExpSize];
    
#if (defined(CYFN_CBC) && (CYFN_CBC == 1)) || (defined(CYFN_CTR) && (CYFN_CTR == 1))
    uint8_t cyfn_iv[CYFN_AES_BLOCKLEN];
#endif
};

/**
 * Initializes the encryption context with the provided key.
 *
 * @param ctx A pointer to the AES context structure.
 * @param key A pointer to the encryption key.
 */
void cyfn_init_ctx(struct cyfn_ctx *ctx, const uint8_t *key);

#if (defined(CYFN_CBC) && (CYFN_CBC == 1)) || (defined(CYFN_CTR) && (CYFN_CTR == 1))

/**
 * Initializes the encryption context with the provided key and IV.
 *
 * @param ctx A pointer to the AES context structure.
 * @param key A pointer to the encryption key.
 * @param iv A pointer to the initialization vector (IV).
 */
void cyfn_init_ctx_iv(struct cyfn_ctx *ctx, const uint8_t *key, const uint8_t *iv);

/**
 * Updates the IV in the encryption context.
 *
 * @param ctx A pointer to the AES context structure.
 * @param iv A pointer to the new initialization vector (IV).
 */
void cyfn_ctx_set_iv(struct cyfn_ctx *ctx, const uint8_t *iv);
#endif

#if defined(CYFN_ECB) && (CYFN_ECB == 1)

/**
 * Encrypts a single block of data using AES in ECB mode.
 *
 * @param ctx A pointer to the AES context structure.
 * @param buf A pointer to the data to be encrypted.
 */
void cyfn_ecb_encrypt(const struct cyfn_ctx* ctx, uint8_t* buf);

/**
 * Decrypts a single block of data using AES in ECB mode.
 *
 * @param ctx A pointer to the AES context structure.
 * @param buf A pointer to the data to be decrypted.
 */
void cyfn_ecb_decrypt(const struct cyfn_ctx* ctx, uint8_t* buf);
#endif

#if defined(CYFN_CBC) && (CYFN_CBC == 1)
// NOTES: you need to set IV in ctx via cyfn_init_ctx_iv() or cyfn_ctx_set_iv()
//        no IV should ever be reused with the same key

/**
 * Encrypts data using AES in CBC mode.
 *
 * @note The IV must be set via `cyfn_init_ctx_iv` or `cyfn_ctx_set_iv`.
 *       Each key-IV pair must be unique.
 *
 * @param ctx A pointer to the AES context structure.
 * @param buf A pointer to the data to be encrypted.
 * @param length The length of the data in bytes.
 */
void cyfn_cbc_encrypt_buffer(struct cyfn_ctx* ctx, uint8_t* buf, size_t length);

/**
 * Decrypts data using AES in CBC mode.
 *
 * @note The IV must be set via `cyfn_init_ctx_iv` or `cyfn_ctx_set_iv`.
 *       Each key-IV pair must be unique.
 *
 * @param ctx A pointer to the AES context structure.
 * @param buf A pointer to the data to be decrypted.
 * @param length The length of the data in bytes.
 */
void cyfn_cbc_decrypt_buffer(struct cyfn_ctx* ctx, uint8_t* buf, size_t length);

#endif

#if defined(CYFN_CTR) && (CYFN_CTR == 1)

/*
   Same function for encrypting as for decrypting.
   IV is incremented for every block, and used after encryption as XOR-compliment for output
   Suggesting https://en.wikipedia.org/wiki/Padding_(cryptography)#PKCS7 for padding scheme
   
   NOTES: you need to set IV in ctx with cyfn_init_ctx_iv() or cyfn_ctx_set_iv()
          no IV should ever be reused with the same key
*/

/**
 * Encrypts or decrypts data using AES in CTR mode.
 *
 * @note The IV is incremented for every block and used as part of the XOR operation.
 *       Each key-IV pair must be unique.
 *
 * @param ctx A pointer to the AES context structure.
 * @param buf A pointer to the data to be processed.
 * @param length The length of the data in bytes.
 */
void cyfn_ctr_xcrypt_buffer(struct cyfn_ctx* ctx, uint8_t* buf, size_t length);

#endif // #if defined(CTR) && (CTR == 1)

#endif