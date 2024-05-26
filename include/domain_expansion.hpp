#ifndef DOMAIN_EXPANSION_HPP
#define DOMAIN_EXPANSION_HPP

#include <common.hpp>
#include <tlwe.hpp>
#include <tgsw.hpp>

// DomainExpansion class is the encryptor/decryptor
// named like this because encryption is like expanding the domain of the torus
class DomainExpansion{
private:
    Bitmap secret_key;
    s_int secret_key_number;

public:
    DomainExpansion();

    TLWESample encrypt(t32 mu);
    TLWESample message_space_encrypt(int m);
    TLWESample message_space_encrypt(t32 mu);
    TGSWSample encrypt(int m);

    t32 decrypt(TLWESample c);
    int message_space_decrypt(TLWESample c);
    int decrypt(TGSWSample C);
};

#endif // !DOMAIN_EXPANSION_HPP
