#include "filehandle.h"
#include <QVariantMap>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QDir>

FileHandle::FileHandle(QObject *parent)
	: QObject(parent)
{
}

FileHandle::~FileHandle()
{
}

QVariant FileHandle::getValue(QString json, QString key)
{
	QVariant ret;
	QJsonDocument document;
	QJsonParseError parseJsonErr;
	document = QJsonDocument::fromJson(json.toUtf8(), &parseJsonErr);
	if (parseJsonErr.error != QJsonParseError::NoError) {
		LOG4_ERROR( "json error:" << parseJsonErr.errorString().toStdString().c_str());
	}

	QJsonObject root_obj = document.object();
	QJsonValue value = root_obj.value(key);
	return value.toVariant();
}

bool FileHandle::copyFiles2DestDir(const QString& sourceDirStr, const QString& destDirStr, bool isCoverCopy, bool isRecursiveCopy)
{
	if (sourceDirStr == destDirStr)
		return true;

	if (!QFile::exists(sourceDirStr))
		return false;

	//源文件夹
	QDir sourceDir(sourceDirStr);
	
	//目的文件夹
	QDir destDir(destDirStr);
	if (!destDir.exists())
	{
		destDir.mkpath(destDirStr);
	}

	//逐一拷贝
	QFileInfoList fileList = sourceDir.entryInfoList();
	for (auto file : fileList)
	{
		if (file.fileName() == "." || file.fileName() == "..")
			continue;

		//拷贝目录
		if (file.isDir())
		{
			if (isRecursiveCopy)
			{
				//递归拷贝
				copyFiles2DestDir(file.filePath(), destDir.filePath(file.fileName()), isCoverCopy, true);
			}
			else
			{
				//跳过文件夹
				continue;
			}
		}
		//拷贝文件
		else
		{
			//是否覆盖
			if (destDir.exists(file.fileName()) && isCoverCopy)
			{
				destDir.remove(file.fileName());
			}

			if (!QFile::copy(file.filePath(),destDir.filePath(file.fileName())))
			{
				return false;
			}
		}
	}

	return true;
}

bool FileHandle::copyFile2DestDir(const QString& sourceFileFullPath, const QString& destDir)
{
	QFileInfo fi(sourceFileFullPath);
	if (!fi.isFile())
	{
		LOG4_ERROR("source is not a file");
		return false;
	}

	//目的文件夹
	QDir dest(destDir);
	if (!dest.exists())
	{
		dest.mkpath(destDir);
	}

	if (!QFile::copy(fi.filePath(), dest.filePath(fi.fileName())))
	{
		LOG4_ERROR("copy error");
		return false;
	}

	return true;
}

void FileHandle::clearFolder(const QString& folderFullPath)
{
	QDir             dir(folderFullPath);
	QFileInfoList    fileList;
	QFileInfo        curFile;
	QFileInfoList    fileListTemp;

	/* 首先获取目标文件夹内所有文件及文件夹信息 */
	fileList = dir.entryInfoList(QDir::Dirs | QDir::Files
		| QDir::Readable | QDir::Writable
		| QDir::Hidden | QDir::NoDotAndDotDot
		, QDir::Name);

	while (fileList.size() > 0)
	{
		int infoNum = fileList.size();

		for (int i = infoNum - 1; i >= 0; i--)
		{
			curFile = fileList[i];
			if (curFile.isFile()) /* 如果是文件&#xff0c;删除文件 */
			{
				QFile fileTemp(curFile.filePath());
				fileTemp.remove();
				fileList.removeAt(i);
			}

			if (curFile.isDir()) /* 如果是文件夹 */
			{
				QDir dirTemp(curFile.filePath());
				fileListTemp = dirTemp.entryInfoList(QDir::Dirs | QDir::Files
					| QDir::Readable | QDir::Writable
					| QDir::Hidden | QDir::NoDotAndDotDot
					, QDir::Name);
				if (fileListTemp.size() == 0) /* 下层没有文件或文件夹 则直接删除*/
				{
					dirTemp.rmdir(".");
					fileList.removeAt(i);
				}
				else /* 下层有文件夹或文件 则将信息添加到列表*/
				{
					for (int j = 0; j < fileListTemp.size(); j++)
					{
						if (!(fileList.contains(fileListTemp[j])))
						{
							fileList.append(fileListTemp[j]);
						}
					}
				}
			}
		}
	}
}

bool FileHandle::createFilePath(const QString& fullPath)
{
	QDir dir(fullPath);
	if (dir.exists())
	{
		return true;
	}
	else
	{
		bool ok = dir.mkpath(fullPath); //创建多级目录
		return ok;
	}
}
