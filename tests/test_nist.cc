#include "../p256.h"
#include "catch.hpp"

using byte_vec_t = std::vector< unsigned char >;
// 
// Tests taken from NIST
// https://csrc.nist.gov/projects/cryptographic-algorithm-validation-program/digital-signatures
// publication FIPS 186-4
TEST_CASE("Couple test vectors from OpenSSL", "[p256_verify]") {
  
  SECTION("Negative test 0") {
    byte_vec_t msg {
        0xe4,0x79,0x6d,0xb5,0xf7,0x85,0xf2,0x07,
        0xaa,0x30,0xd3,0x11,0x69,0x3b,0x37,0x02,
        0x82,0x1d,0xff,0x11,0x68,0xfd,0x2e,0x04,
        0xc0,0x83,0x68,0x25,0xae,0xfd,0x85,0x0d,
        0x9a,0xa6,0x03,0x26,0xd8,0x8c,0xde,0x1a,
        0x23,0xc7,0x74,0x53,0x51,0x39,0x2c,0xa2,
        0x28,0x8d,0x63,0x2c,0x26,0x4f,0x19,0x7d,
        0x05,0xcd,0x42,0x4a,0x30,0x33,0x6c,0x19,
        0xfd,0x09,0xbb,0x22,0x96,0x54,0xf0,0x22,
        0x2f,0xcb,0x88,0x1a,0x4b,0x35,0xc2,0x90,
        0xa0,0x93,0xac,0x15,0x9c,0xe1,0x34,0x09,
        0x11,0x1f,0xf0,0x35,0x84,0x11,0x13,0x3c,
        0x24,0xf5,0xb8,0xe2,0x09,0x0d,0x6d,0xb6,
        0x55,0x8a,0xfc,0x36,0xf0,0x6c,0xa1,0xf6,
        0xef,0x77,0x97,0x85,0xad,0xba,0x68,0xdb,
        0x27,0xa4,0x09,0x85,0x9f,0xc4,0xc4,0xa0}; 
    byte_vec_t sig {
        /* r */
        0xd1,0x9f,0xf4,0x8b,0x32,0x49,0x15,0x57,
        0x64,0x16,0x09,0x7d,0x25,0x44,0xf7,0xcb,
        0xdf,0x87,0x68,0xb1,0x45,0x4a,0xd2,0x0e,
        0x0b,0xaa,0xc5,0x0e,0x21,0x1f,0x23,0xb0,

        /* incorrect  s */
        0xa3,0xe8,0x1e,0x59,0x31,0x1c,0xdf,0xff,
        0x2d,0x47,0x84,0x94,0x9f,0x7a,0x2c,0xb5,
        0x0b,0xa6,0xc3,0xa9,0x1f,0xa5,0x47,0x10,
        0x56,0x8e,0x61,0xac,0xa3,0xe8,0x47,0xc6};

    byte_vec_t pk {
        /* constant */ 0x04,

        /* x-coord */
        0x87,0xf8,0xf2,0xb2,0x18,0xf4,0x98,0x45,
        0xf6,0xf1,0x0e,0xec,0x38,0x77,0x13,0x62,
        0x69,0xf5,0xc1,0xa5,0x47,0x36,0xdb,0xdf,
        0x69,0xf8,0x99,0x40,0xca,0xd4,0x15,0x55, 

        /* y-coord */
        0xe1,0x5f,0x36,0x90,0x36,0xf4,0x98,0x42,
        0xfa,0xc7,0xa8,0x6c,0x8a,0x2b,0x05,0x57,
        0x60,0x97,0x76,0x81,0x44,0x48,0xb8,0xf5,
        0xe8,0x4a,0xa9,0xf4,0x39,0x52,0x05,0xe9
    };

    REQUIRE( p256_verify(msg.data(),
                         static_cast< unsigned >(msg.size()),
                         sig.data(),
                         pk.data()) == P256_INVALID_SIGNATURE );
  }

  SECTION("Negative test 1") {
    byte_vec_t msg {
        0x06,0x9a,0x6e,0x6b,0x93,0xdf,0xee,0x6d,
        0xf6,0xef,0x69,0x97,0xcd,0x80,0xdd,0x21,
        0x82,0xc3,0x66,0x53,0xce,0xf1,0x0c,0x65,
        0x5d,0x52,0x45,0x85,0x65,0x54,0x62,0xd6,
        0x83,0x87,0x7f,0x95,0xec,0xc6,0xd6,0xc8,
        0x16,0x23,0xd8,0xfa,0xc4,0xe9,0x00,0xed,
        0x00,0x19,0x96,0x40,0x94,0xe7,0xde,0x91,
        0xf1,0x48,0x19,0x89,0xae,0x18,0x73,0x00,
        0x45,0x65,0x78,0x9c,0xbf,0x5d,0xc5,0x6c,
        0x62,0xae,0xdc,0x63,0xf6,0x2f,0x3b,0x89,
        0x4c,0x9c,0x6f,0x77,0x88,0xc8,0xec,0xaa,
        0xdc,0x9b,0xd0,0xe8,0x1a,0xd9,0x1b,0x2b,
        0x35,0x69,0xea,0x12,0x26,0x0e,0x93,0x92,
        0x4f,0xdd,0xdd,0x39,0x72,0xaf,0x52,0x73,
        0x19,0x8f,0x5e,0xfd,0xa0,0x74,0x62,0x19,
        0x47,0x50,0x17,0x55,0x76,0x16,0x17,0x0e}; 
    byte_vec_t sig {
        /* incorrect r */
        0xdc,0x23,0xd1,0x30,0xc6,0x11,0x7f,0xb5,
        0x75,0x12,0x01,0x45,0x5e,0x99,0xf3,0x6f,
        0x59,0xab,0xa1,0xa6,0xa2,0x1c,0xf2,0xd0,
        0xe7,0x48,0x1a,0x97,0x45,0x1d,0x66,0x93,

        /* s */
        0xd6,0xce,0x77,0x08,0xc1,0x8d,0xbf,0x35,
        0xd4,0xf8,0xaa,0x72,0x40,0x92,0x2d,0xc6,
        0x82,0x3f,0x2e,0x70,0x58,0xcb,0xc1,0x48,
        0x4f,0xca,0xd1,0x59,0x9d,0xb5,0x01,0x8c};

    byte_vec_t pk {
        /* constant */ 0x04,

        /* x-coord */
        0x5c,0xf0,0x2a,0x00,0xd2,0x05,0xbd,0xfe,
        0xe2,0x01,0x6f,0x74,0x21,0x80,0x7f,0xc3,
        0x8a,0xe6,0x9e,0x6b,0x7c,0xcd,0x06,0x4e,
        0xe6,0x89,0xfc,0x1a,0x94,0xa9,0xf7,0xd2, 

        /* y-coord */
        0xec,0x53,0x0c,0xe3,0xcc,0x5c,0x9d,0x1a,
        0xf4,0x63,0xf2,0x64,0xd6,0x85,0xaf,0xe2,
        0xb4,0xdb,0x4b,0x58,0x28,0xd7,0xe6,0x1b,
        0x74,0x89,0x30,0xf3,0xce,0x62,0x2a,0x85
    };

    REQUIRE( p256_verify(msg.data(),
                         static_cast< unsigned >(msg.size()),
                         sig.data(),
                         pk.data()) == P256_INVALID_SIGNATURE );
  }

  SECTION("Negative test 2") {
    byte_vec_t msg {
        0xdf,0x04,0xa3,0x46,0xcf,0x4d,0x0e,0x33,
        0x1a,0x6d,0xb7,0x8c,0xca,0x2d,0x45,0x6d,
        0x31,0xb0,0xa0,0x00,0xaa,0x51,0x44,0x1d,
        0xef,0xdb,0x97,0xbb,0xeb,0x20,0xb9,0x4d,
        0x8d,0x74,0x64,0x29,0xa3,0x93,0xba,0x88,
        0x84,0x0d,0x66,0x16,0x15,0xe0,0x7d,0xef,
        0x61,0x5a,0x34,0x2a,0xbe,0xdf,0xa4,0xce,
        0x91,0x2e,0x56,0x2a,0xf7,0x14,0x95,0x98,
        0x96,0x85,0x8a,0xf8,0x17,0x31,0x7a,0x84,
        0x0d,0xcf,0xf8,0x5a,0x05,0x7b,0xb9,0x1a,
        0x3c,0x2b,0xf9,0x01,0x05,0x50,0x03,0x62,
        0x75,0x4a,0x6d,0xd3,0x21,0xcd,0xd8,0x61,
        0x28,0xcf,0xc5,0xf0,0x46,0x67,0xb5,0x7a,
        0xa7,0x8c,0x11,0x24,0x11,0xe4,0x2d,0xa3,
        0x04,0xf1,0x01,0x2d,0x48,0xcd,0x6a,0x70,
        0x52,0xd7,0xde,0x44,0xeb,0xcc,0x01,0xde}; 
    byte_vec_t sig {
        /* r */
        0x99,0x13,0x11,0x1c,0xff,0x6f,0x20,0xc5,
        0xbf,0x45,0x3a,0x99,0xcd,0x2c,0x20,0x19,
        0xa4,0xe7,0x49,0xa4,0x97,0x24,0xa0,0x87,
        0x74,0xd1,0x4e,0x4c,0x11,0x3e,0xdd,0xa8,

        /* s */
        0x94,0x67,0xcd,0x4c,0xd2,0x1e,0xcb,0x56,
        0xb0,0xca,0xb0,0xa9,0xa4,0x53,0xb4,0x33,
        0x86,0x84,0x54,0x59,0x12,0x7a,0x95,0x24,
        0x21,0xf5,0xc6,0x38,0x28,0x66,0xc5,0xcc};

    byte_vec_t pk {
        /* constant */ 0x04,

        /* incorrect x-coord */
        0x2d,0xdf,0xd1,0x45,0x76,0x78,0x83,0xff,
        0xbb,0x0a,0xc0,0x03,0xab,0x4a,0x44,0x34,
        0x6d,0x08,0xfa,0x25,0x70,0xb3,0x12,0x0d,
        0xcc,0xe9,0x45,0x62,0x42,0x22,0x44,0xcb, 

        /* incorrect y-coord */
        0x5f,0x70,0xc7,0xd1,0x1a,0xc2,0xb7,0xa4,
        0x35,0xcc,0xfb,0xba,0xe0,0x2c,0x3d,0xf1,
        0xea,0x6b,0x53,0x2c,0xc0,0xe9,0xdb,0x74,
        0xf9,0x3f,0xff,0xca,0x7c,0x6f,0x9a,0x64
    };

    REQUIRE( p256_verify(msg.data(),
                         static_cast< unsigned >(msg.size()),
                         sig.data(),
                         pk.data()) == P256_INVALID_SIGNATURE );
  }

  SECTION("Positive test 0") {
    byte_vec_t msg {
        0xe1,0x13,0x0a,0xf6,0xa3,0x8c,0xcb,0x41,
        0x2a,0x9c,0x8d,0x13,0xe1,0x5d,0xbf,0xc9,
        0xe6,0x9a,0x16,0x38,0x5a,0xf3,0xc3,0xf1,
        0xe5,0xda,0x95,0x4f,0xd5,0xe7,0xc4,0x5f,
        0xd7,0x5e,0x2b,0x8c,0x36,0x69,0x92,0x28,
        0xe9,0x28,0x40,0xc0,0x56,0x2f,0xbf,0x37,
        0x72,0xf0,0x7e,0x17,0xf1,0xad,0xd5,0x65,
        0x88,0xdd,0x45,0xf7,0x45,0x0e,0x12,0x17,
        0xad,0x23,0x99,0x22,0xdd,0x9c,0x32,0x69,
        0x5d,0xc7,0x1f,0xf2,0x42,0x4c,0xa0,0xde,
        0xc1,0x32,0x1a,0xa4,0x70,0x64,0xa0,0x44,
        0xb7,0xfe,0x3c,0x2b,0x97,0xd0,0x3c,0xe4,
        0x70,0xa5,0x92,0x30,0x4c,0x5e,0xf2,0x1e,
        0xed,0x9f,0x93,0xda,0x56,0xbb,0x23,0x2d,
        0x1e,0xeb,0x00,0x35,0xf9,0xbf,0x0d,0xfa,
        0xfd,0xcc,0x46,0x06,0x27,0x2b,0x20,0xa3};
    byte_vec_t sig {
        /* r */
        0xbf,0x96,0xb9,0x9a,0xa4,0x9c,0x70,0x5c,
        0x91,0x0b,0xe3,0x31,0x42,0x01,0x7c,0x64,
        0x2f,0xf5,0x40,0xc7,0x63,0x49,0xb9,0xda,
        0xb7,0x2f,0x98,0x1f,0xd9,0x34,0x7f,0x4f,

        /* s */
        0x17,0xc5,0x50,0x95,0x81,0x90,0x89,0xc2,
        0xe0,0x3b,0x9c,0xd4,0x15,0xab,0xdf,0x12,
        0x44,0x4e,0x32,0x30,0x75,0xd9,0x8f,0x31,
        0x92,0x0b,0x9e,0x0f,0x57,0xec,0x87,0x1c};

    byte_vec_t pk {
        /* constant */ 0x04,

        /*  x-coord */
        0xe4,0x24,0xdc,0x61,0xd4,0xbb,0x3c,0xb7,
        0xef,0x43,0x44,0xa7,0xf8,0x95,0x7a,0x0c,
        0x51,0x34,0xe1,0x6f,0x7a,0x67,0xc0,0x74,
        0xf8,0x2e,0x6e,0x12,0xf4,0x9a,0xbf,0x3c,

        /*  y-coord */
        0x97,0x0e,0xed,0x7a,0xa2,0xbc,0x48,0x65,
        0x15,0x45,0x94,0x9d,0xe1,0xdd,0xda,0xf0,
        0x12,0x7e,0x59,0x65,0xac,0x85,0xd1,0x24,
        0x3d,0x6f,0x60,0xe7,0xdf,0xae,0xe9,0x27
    };

    REQUIRE( p256_verify(msg.data(),
                         static_cast< unsigned >(msg.size()),
                         sig.data(),
                         pk.data()) == P256_SUCCESS );
  }

  SECTION("Positive test 1") {
    byte_vec_t msg {
        0x73,0xc5,0xf6,0xa6,0x74,0x56,0xae,0x48,
        0x20,0x9b,0x5f,0x85,0xd1,0xe7,0xde,0x77,
        0x58,0xbf,0x23,0x53,0x00,0xc6,0xae,0x2b,
        0xdc,0xeb,0x1d,0xcb,0x27,0xa7,0x73,0x0f,
        0xb6,0x8c,0x95,0x0b,0x7f,0xca,0xda,0x0e,
        0xcc,0x46,0x61,0xd3,0x57,0x82,0x30,0xf2,
        0x25,0xa8,0x75,0xe6,0x9a,0xaa,0x17,0xf1,
        0xe7,0x1c,0x6b,0xe5,0xc8,0x31,0xf2,0x26,
        0x63,0xba,0xc6,0x3d,0x0c,0x7a,0x96,0x35,
        0xed,0xb0,0x04,0x3f,0xf8,0xc6,0xf2,0x64,
        0x70,0xf0,0x2a,0x7b,0xc5,0x65,0x56,0xf1,
        0x43,0x7f,0x06,0xdf,0xa2,0x7b,0x48,0x7a,
        0x6c,0x42,0x90,0xd8,0xba,0xd3,0x8d,0x48,
        0x79,0xb3,0x34,0xe3,0x41,0xba,0x09,0x2d,
        0xde,0x4e,0x4a,0xe6,0x94,0xa9,0xc0,0x93,
        0x02,0xe2,0xdb,0xf4,0x43,0x58,0x1c,0x08};
    byte_vec_t sig {
        /* r */
        0x1d,0x75,0x83,0x0c,0xd3,0x6f,0x4c,0x9a,
        0xa1,0x81,0xb2,0xc4,0x22,0x1e,0x87,0xf1,
        0x76,0xb7,0xf0,0x5b,0x7c,0x87,0x82,0x4e,
        0x82,0xe3,0x96,0xc8,0x83,0x15,0xc4,0x07,

        /* s */
        0xcb,0x2a,0xcb,0x01,0xda,0xc9,0x6e,0xfc,
        0x53,0xa3,0x2d,0x4a,0x0d,0x85,0xd0,0xc2,
        0xe4,0x89,0x55,0x21,0x47,0x83,0xec,0xf5,
        0x0a,0x4f,0x04,0x14,0xa3,0x19,0xc0,0x5a};

    byte_vec_t pk {
        /* constant */ 0x04,

        /*  x-coord */
        0xe0,0xfc,0x6a,0x6f,0x50,0xe1,0xc5,0x74,
        0x75,0x67,0x3e,0xe5,0x4e,0x3a,0x57,0xf9,
        0xa4,0x9f,0x33,0x28,0xe7,0x43,0xbf,0x52,
        0xf3,0x35,0xe3,0xee,0xaa,0x3d,0x28,0x64,

        /*  y-coord */
        0x7f,0x59,0xd6,0x89,0xc9,0x1e,0x46,0x36,
        0x07,0xd9,0x19,0x4d,0x99,0xfa,0xf3,0x16,
        0xe2,0x54,0x32,0x87,0x08,0x16,0xdd,0xe6,
        0x3f,0x5d,0x4b,0x37,0x3f,0x12,0xf2,0x2a
    };

    REQUIRE( p256_verify(msg.data(),
                         static_cast< unsigned >(msg.size()),
                         sig.data(),
                         pk.data()) == P256_SUCCESS );
  }

}