//
//  fs.h
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

#import <Foundation/Foundation.h>

//! Project version number for fs.
FOUNDATION_EXPORT double fsVersionNumber;

//! Project version string for fs.
FOUNDATION_EXPORT const unsigned char fsVersionString[];

// In this header, you should import all the public headers of your framework using statements like #import <fs/PublicHeader.h>
#import <fs/_fs.h>
#import <fs/access.h>
#import <fs/encoding.h>

#pragma mark - Utilities

#define smFOREVER      for(;;)
#define smRANGE(i,y,x) for(i=(y);(((x)>=(y))?(i<=(x)):(i>=x));          \
                       (((x)>=(y))?((i)++):((i)--)))
#define smFOREACH(i,A) for(int _keep=1,                                 \
                       _count=0,                                        \
                       _size=sizeof (A) / sizeof *(A);                  \
                       _keep && _count != _size;                        \
                       _keep = !_keep, _count++)                        \
                       for(i = (A)+_count; _keep; _keep = !_keep)

#define smBIT(x)       (1<<(x))
#define smSETBIT(x,p)  ((x)|(1<<(p)))
#define smCLRBIT(x,p)  ((x)&(~(1<<(p))))
#define smGETBIT(x,p)  (((x)>>(p))&1)
#define smTGLBIT(x,p)  ((x)^(1<<(p)))

#define smARRSIZE(a)   (sizeof(a) / sizeof((a)[0]))
#define smSET(d,n,y)   do{ size_t i_, n_;                               \
                       for ( n_ = (n), i_ = 0; n_ > 0; --n_, ++i_)      \
                       (d)[i_] = (v); } while(0)
#define smZERO(d,n)    smSET(d,n,0)
#define smCOLUMNS(S,E) ((E) - (S) + 1)
#define smIS_ARRAY(a)  ((void *)&a == (void *)a)

#define smSATINC(w,L)  (w = (((w) == (L)) ? (w) : ((w) + 1)) )
#define smSATDEC(w,L)  (w = (((w) == (L)) ? (w) : ((w) - 1)) )

#if defined(_WIN32) || defined(__CYGWIN__)
    #ifdef FSK_DLL_EXPORT
        #ifdef __GNUC__
            #define FSK_EXPORT __attribute__((dllexport))
        #else
            #define FSK_EXPORT __declspec(dllexport)
        #endif
    #else
        #ifdef __GNUC__
            #define FSK_EXPORT __attribute__((dllimport))
        #else
            #define FSK_EXPORT __declspec(dllimport)
        #endif
    #endif
#else
    #if __GNUC__ >= 4
        #define FSK_EXPORT __attribute__((visibility("default")))
    #else
        #define FSK_EXPORT
    #endif
#endif