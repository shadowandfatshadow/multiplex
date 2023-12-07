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

	//����
	static QByteArray encrypt(QByteArray plaintext, QByteArray key, QByteArray iv);

	//����
	static QByteArray decrypt(QByteArray cyphertext, QByteArray key, QByteArray iv);

	//��ȡ���´�����Ϣ
	static QString GetLastError();
};

