#include "qaes.h"
#include <QCryptographicHash>
#include <QScopedPointer>

QByteArray QAES::encrypt(QByteArray plaintext, QByteArray key, QByteArray iv)
{
	ERR_clear_error();
	QByteArray cyphertext;
	//检查密钥的合法性(使用EVP_aes_256_cbc，因此密钥需要32字节，iv需要16字节)
	if (key.size() != 32 || iv.size() != 16)
		return cyphertext;
	
	QScopedPointer<EVP_CIPHER_CTX, ScopedPointerCipherCtxDeleter> ctx(EVP_CIPHER_CTX_new());

	//初始化
	if (!EVP_EncryptInit_ex(ctx.data(), EVP_aes_256_cbc(), NULL, (unsigned char*)key.data(), (unsigned char*)iv.data()))
	{
		cyphertext.clear();
		return cyphertext;
	}
	//设置填充模式，保证固定块大小
	if (!EVP_CIPHER_CTX_set_padding(ctx.data(), EVP_PADDING_PKCS7))
	{
		cyphertext.clear();
		return cyphertext;
	}
	//加密
	int outlen = 0;
	cyphertext.resize(plaintext.size() + 16);
	if (!EVP_EncryptUpdate(ctx.data(), (unsigned char*)cyphertext.data(), &outlen, (const unsigned char*)plaintext.data(), plaintext.size()))
	{
		cyphertext.clear();
		return cyphertext;
	}
	//加密余下部分
	int tmplen = 0;
	if (!EVP_EncryptFinal_ex(ctx.data(), (unsigned char*)cyphertext.data() + outlen, &tmplen))
	{
		cyphertext.clear();
		return cyphertext;
	}

	return cyphertext.left(outlen + tmplen);
}

QByteArray QAES::decrypt(QByteArray cyphertext, QByteArray key, QByteArray iv)
{
	ERR_clear_error();
	QByteArray plaintext;
	//检查密钥的合法性(使用EVP_aes_256_cbc，因此密钥需要32字节，iv需要16字节)
	if (key.size() != 32 || iv.size() != 16)
		return plaintext;

	QScopedPointer<EVP_CIPHER_CTX, ScopedPointerCipherCtxDeleter> ctx(EVP_CIPHER_CTX_new());

	if (!EVP_DecryptInit_ex(ctx.data(), EVP_aes_256_cbc(), NULL, (unsigned char*)key.data(), (unsigned char*)iv.data()))
	{
		plaintext.clear();
		return plaintext;
	}
	if (!EVP_CIPHER_CTX_set_padding(ctx.data(), EVP_PADDING_PKCS7))
	{
		plaintext.clear();
		return plaintext;
	}
	int outlen = 0;
	plaintext.resize(cyphertext.size());
	if (!EVP_DecryptUpdate(ctx.data(), (unsigned char*)plaintext.data(), &outlen, (unsigned char*)cyphertext.data(), cyphertext.size()))
	{
		plaintext.clear();
		return plaintext;
	}
	int tmplen = 0;
	if (!EVP_DecryptFinal_ex(ctx.data(), (unsigned char*)plaintext.data() + outlen, &tmplen))
	{
		plaintext.clear();
		return plaintext;
	}

	return plaintext.left(outlen + tmplen);
}

QString QAES::GetLastError()
{
	unsigned long lastError = ERR_peek_last_error();
	if (lastError == 0)
	{
		return "";
	}
	char errorString[256];
	ERR_error_string_n(lastError, errorString, sizeof(errorString));
	return errorString;
}
