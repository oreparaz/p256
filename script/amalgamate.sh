#!/bin/bash

set -euxo pipefail

# We amalgamate a hopefully minimal set of files from BearSSL
# that are needed for ECDSA verification

# This is not how BearSSL was designed in mind, so this is not
# going to be pretty. My apologies!

# insecure is OK since we are checking the tar.gz integrity after downloading
curl -LO --insecure https://bearssl.org/bearssl-0.6.tar.gz
sha256sum -c checksum
tar -xzvf bearssl-0.6.tar.gz

pushd bearssl-0.6
patch -p1 < ../0001-feat-add-NIST-P-256-point-decompression.patch
popd

OUTFILE=`pwd`/../p256.c

rm -f $OUTFILE
touch $OUTFILE

pushd bearssl-0.6

echo '/* DO NOT EDIT - THIS FILE WAS AUTOGENERATED */ ' >> $OUTFILE
echo "/* generated on" `date -u +"%Y-%m-%dT%H:%M:%SZ"` "        */" >> $OUTFILE
echo >> $OUTFILE
echo '/* single-file ECDSA verifier from BearSSL   */' >> $OUTFILE
echo '/* source: https://github.com/oreparaz/p256  */' >> $OUTFILE

echo >> $OUTFILE
echo >> $OUTFILE

# add includes in specific order to avoid forward dependencies

cat src/config.h >> $OUTFILE
cat inc/bearssl_hash.h >> $OUTFILE
cat inc/bearssl_block.h >> $OUTFILE

sed -e 's/#include \"bearssl_hash.h\"//g' \
    inc/bearssl_hmac.h >> $OUTFILE

cat inc/bearssl_prf.h >> $OUTFILE
cat inc/bearssl_pem.h >> $OUTFILE

sed -e 's/#include \"bearssl_hash.h\"//g'  \
    -e 's/#include \"bearssl_hmac.h\"//g'  \
    inc/bearssl_kdf.h >> $OUTFILE

sed -e 's/#include \"bearssl_block.h\"//g' \
    -e 's/#include \"bearssl_hash.h\"//g'  \
    inc/bearssl_aead.h >> $OUTFILE

sed -e 's/#include \"bearssl_hash.h\"//g'  \
    -e 's/#include \"bearssl_block.h\"//g' \
    inc/bearssl_rand.h >> $OUTFILE

sed -e 's/#include \"bearssl_rand.h\"//g'  \
    inc/bearssl_rsa.h >> $OUTFILE

sed -e 's/#include \"bearssl_rand.h\"//g'  \
    inc/bearssl_ec.h >> $OUTFILE

sed -e 's/#include \"bearssl_ec.h\"//g'    \
    -e 's/#include \"bearssl_hash.h\"//g'  \
    -e 's/#include \"bearssl_rsa.h\"//g'   \
    inc/bearssl_x509.h >> $OUTFILE

sed -e 's/#include \"bearssl_block.h\"//g' \
    -e 's/#include \"bearssl_hash.h\"//g'  \
    -e 's/#include \"bearssl_hmac.h\"//g'  \
    -e 's/#include \"bearssl_prf.h\"//g'   \
    -e 's/#include \"bearssl_rand.h\"//g'  \
    -e 's/#include \"bearssl_x509.h\"//g'  \
    inc/bearssl_ssl.h >> $OUTFILE

sed -e 's/#include \"config.h\"//g'         \
    -e 's/#include \"bearssl.h\"//g'        \
    src/inner.h >> $OUTFILE

sed -e 's/#include \"inner.h\"//g' src/codec/ccopy.c >> $OUTFILE
sed -e 's/#include \"inner.h\"//g' src/codec/dec32be.c >> $OUTFILE

sed -e 's/#include \"inner.h\"//g' src/ec/ec_p256_m31.c >> $OUTFILE

sed -e 's/#include \"inner.h\"//g' src/ec/ecdsa_i31_bits.c >> $OUTFILE
sed -e 's/#include \"inner.h\"//g' src/ec/ecdsa_i31_vrfy_raw.c >> $OUTFILE
sed -e 's/#include \"inner.h\"//g' src/codec/enc32be.c >> $OUTFILE
sed -e 's/#include \"inner.h\"//g' src/int/i31_add.c >> $OUTFILE
sed -e 's/#include \"inner.h\"//g' src/int/i31_bitlen.c >> $OUTFILE
sed -e 's/#include \"inner.h\"//g' src/int/i31_decmod.c >> $OUTFILE
sed -e 's/#include \"inner.h\"//g' src/int/i31_decode.c >> $OUTFILE
sed -e 's/#include \"inner.h\"//g' src/int/i31_encode.c >> $OUTFILE
sed -e 's/#include \"inner.h\"//g' src/int/i31_fmont.c >> $OUTFILE
sed -e 's/#include \"inner.h\"//g' src/int/i31_iszero.c >> $OUTFILE
sed -e 's/#include \"inner.h\"//g' src/int/i31_modpow.c >> $OUTFILE
sed -e 's/#include \"inner.h\"//g' src/int/i31_montmul.c >> $OUTFILE
sed -e 's/#include \"inner.h\"//g' src/int/i31_muladd.c >> $OUTFILE
sed -e 's/#include \"inner.h\"//g' src/int/i31_ninv31.c >> $OUTFILE
sed -e 's/#include \"inner.h\"//g' src/int/i31_rshift.c >> $OUTFILE
sed -e 's/#include \"inner.h\"//g' src/int/i31_sub.c >> $OUTFILE
sed -e 's/#include \"inner.h\"//g' src/int/i31_tmont.c >> $OUTFILE
sed -e 's/#include \"inner.h\"//g' src/int/i32_div32.c >> $OUTFILE
sed -e 's/#include \"inner.h\"//g' src/hash/sha2small.c >> $OUTFILE


staticize () {
        # first line does this kind of conversion
        # void
        # br_ccopy(
        # ->
        # static void
        # br_ccopy(
        #
        # second line is more straightforward: void br_ccopy( -> static void br_ccopy(
        perl -0pi -e "s/$1\n$2\(/static $1\n$2\(/g" $OUTFILE
        perl -0pi -e "s/$1 $2\(/static $1 $2\(/g" $OUTFILE
}

staticize void br_ccopy
staticize uint32_t br_divrem
staticize void br_ecdsa_i31_bits2int
staticize uint32_t br_ecdsa_i31_vrfy_raw
staticize uint32_t br_i31_add
staticize uint32_t br_i31_bit_length
staticize void br_i31_decode
staticize uint32_t br_i31_decode_mod
staticize void br_i31_encode
staticize void br_i31_from_monty
staticize uint32_t br_i31_iszero
staticize void br_i31_modpow
staticize void br_i31_montymul
staticize void br_i31_muladd_small
staticize uint32_t br_i31_ninv31
staticize void br_i31_rshift
staticize uint32_t br_i31_sub
staticize void br_i31_to_monty
staticize void br_range_dec32be
staticize void br_range_enc32be
staticize void br_sha224_init
staticize void br_sha224_out
staticize void br_sha224_set_state
staticize uint64_t br_sha224_state
staticize void br_sha224_update
staticize void br_sha256_init
staticize void br_sha256_out
staticize void br_sha2small_round

cat <<EOF >> $OUTFILE

#include "p256.h"

const br_ec_curve_def br_secp256r1 = {
	BR_EC_secp256r1,
	P256_N, sizeof P256_N,
	P256_G, sizeof P256_G
};

p256_ret_t p256_verify(uint8_t *msg, size_t msg_len, uint8_t *sig, const uint8_t *pk)
{
    unsigned char hash[64];
    size_t hash_len = 32; // SHA-256
    br_hash_compat_context hc;
    const br_hash_class *hf = &br_sha256_vtable;
    br_ecdsa_vrfy vrfy = &br_ecdsa_i31_vrfy_raw;

    hf->init(&hc.vtable);
    hf->update(&hc.vtable, msg, msg_len);
    hf->out(&hc.vtable, hash);
 
    size_t sig_len = 64; // P-256

    // TODO: p256_verify should take a pk_len argument so that we can verify
    // here the following precondition:
    // pk must be at least 65 bytes long, or 33 if the first byte is 0x02 or 0x03.

    unsigned char public_key[65];
    if ((pk[0] != 0x04) && (pk[0] != 0x02) && (pk[0] != 0x03)) {
        return P256_INVALID_SIGNATURE; // TODO: more specific errors
    }

    size_t pk_len = (pk[0]==0x04) ? 65 : 33;
    memcpy(public_key, pk, pk_len);

    const br_ec_public_key pub = {
        .curve = BR_EC_secp256r1,
        .q =  public_key,
        .qlen = pk_len,
    };

    if (vrfy(&br_ec_p256_m31, hash, hash_len,
        &pub, sig, sig_len) != 1)
    {
        return P256_INVALID_SIGNATURE;
    }

    return P256_SUCCESS;
}

EOF

echo "done, output file $OUTFILE"
