#include "encryptionservice.h"

EncryptionService::EncryptionService(QString & value)
{
    key = value;
    CryptoPP::SecByteBlock keyblock(0x00, CryptoPP::AES::DEFAULT_KEYLENGTH);
    rnd.GenerateBlock(keyblock, keyblock.size());
    encryption = CryptoPP::AES::Encryption(keyblock, CryptoPP::AES::DEFAULT_KEYLENGTH);
    rnd.GenerateBlock(iv, CryptoPP::AES::BLOCKSIZE);
    cfbEncryption = CryptoPP::CFB_Mode_ExternalCipher::Encryption(encryption, iv);
    char hello[] = "Hello!";
    int h_sz = 7;

    CryptoPP::CFB_Mode<CryptoPP::AES>::Encryption cfbenc(keyblock, keyblock.size(), iv);
    cfbenc.ProcessData((byte*)hello, (byte*)hello, h_sz);

    CryptoPP::CFB_Mode<CryptoPP::AES>::Decryption cfbdec(keyblock, keyblock.size(), iv);
    cfbdec.ProcessData((byte*)hello, (byte*)hello, h_sz);

    qDebug() << hello << "is hello!";
}

void EncryptionService::encode(QByteArray & data)
{
    qDebug() << "encoded with " << key;
    emit encoded(data);
}

void EncryptionService::decode(QByteArray & data)
{
    qDebug() << "encoded with " << key;
    emit decoded(data);
}
