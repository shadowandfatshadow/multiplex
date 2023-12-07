#pragma once

#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/aes.h>
#include <QString>
#include <QByteArray>

class QAES
{
public:
	struct ScopedPointerCipherCtxDeleter
	{
		static inline void cleanup(EVP_CIPHER_CTX* pointer)
		{
			EVP_CIPHER_CTX_free(pointer);
		}
	};

	//加密
	static QByteArray encrypt(QByteArray plaintext, QByteArray key, QByteArray iv);

	//解密
	static QByteArray decrypt(QByteArray cyphertext, QByteArray key, QByteArray iv);

	//获取最新错误信息
	static QString GetLastError();
};

