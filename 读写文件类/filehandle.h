#pragma once

#include <QObject>
#include <iostream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonArray>

class FileHandle : public QObject
{
	Q_OBJECT

private:
	FileHandle(QObject *parent = nullptr);
	~FileHandle();

public:
	static FileHandle* getInstance()
	{
		static FileHandle instance;
		return &instance;
	}

#pragma region json相关

	/***************************************************************
	*	FunctionName:	write2Json
	*	Purpose:		写json文件
	*	Parameter:		QString path			存放路径
	*	Parameter:		Args... args			保存内容
	*	Return:			保存成功返回true,否则返回false
	*	Remark:			类成员模板函数的定义与实现需要都在头文件中
	****************************************************************/
	template<typename... Args>
	bool write2Json(QString path, Args... args);

	template<typename R>
	void fillMap(QVariantMap& map, typename QPair<QString, R> p);			//特化qpair的模板函数，普通的模板应该用不到就没写出来

	/***************************************************************
	*	FunctionName:	update2Json
	*	Purpose:		更新json文件，其实就是读出来修改后重写
	*	Parameter:		QString path			文件路径
	*	Parameter:		Args... args			修改内容
	*	Return:			修改成功返回true，否则返回false
	*	Remark:
	****************************************************************/
	template<typename... Args>
	bool update2Json(QString path, Args... args);

	/***************************************************************
	*	FunctionName:	update2JsonStr
	*	Purpose:		修改json字符串
	*	Parameter:		QString json			json字符串
	*	Parameter:		Args... args			修改内容
	*	Return:			QString					修改后的json字符串
	*	Remark:			目前主要用在更新数据库中的detail字段
	****************************************************************/
	template<typename... Args>
	QString update2JsonStr(QString json, Args... args);

	template<typename R>
	void updateObj(QVariantMap& obj, typename QPair<QString, R> p);			//特化qpair的模板函数

		/***************************************************************
	*	FunctionName:	insert2JsonStr
	*	Purpose:		修改json字符串
	*	Parameter:		QString json			json字符串
	*	Parameter:		Args... args			新增内容
	*	Return:			QString					修改后的json字符串
	*	Remark:			
	****************************************************************/
	template<typename... Args>
	QString insert2JsonStr(QString json, Args... args);

	template<typename R>
	void insertObj(QVariantMap& obj, typename QPair<QString, R> p);			//特化qpair的模板函数

	QVariant getValue(QString json, QString key);							//从json字符串获取键对应的值

#pragma endregion

#pragma region 通用功能
	
	/***************************************************************
	*	FunctionName:	copyFiles2DestDir
	*	Purpose:		从源目录拷贝文件到目标目录
	*	Parameter:		QString sourceDir				源目录
	*	Parameter:		QString destDir					目标目录
	*	Parameter:		bool isCoverCopy				是否覆盖已存在的文件
	*	Parameter:		bool isRecursiveCopy			是否递归拷贝
	*	Return:			成功与否
	*	Remark:
	****************************************************************/
	bool copyFiles2DestDir(const QString& sourceDir, const QString& destDir, bool isCoverCopy = true, bool isRecursiveCopy = true);

	//复制文件到目标目录下
	bool copyFile2DestDir(const QString& sourceFileFullPath, const QString& destDir);

	//清空文件夹
	void clearFolder(const QString& folderFullPath);

	//创建目录(支持多级)
	bool createFilePath(const QString& fullPath);

#pragma endregion

};


template<typename... Args>
bool FileHandle::write2Json(QString path, Args... args)
{
	QVariantMap map;
	int arr[] = { (fillMap(map,args),0)... };

	if (map.size() != sizeof...(Args))
	{
		LOG4_ERROR("Incorrect number of parameter conversions");
		return false;
	}

	QJsonObject obj = QJsonObject::fromVariantMap(map);
	QJsonDocument doc;
	doc.setObject(obj);
	QByteArray array = doc.toJson(QJsonDocument::Indented);

	QFile loadFile(path);
	if (!loadFile.open(QIODevice::WriteOnly))
	{
		LOG4_ERROR("could't open json file");
		return false;
	}

	loadFile.write(array);
	loadFile.close();

	return true;
}

template<typename R>
void FileHandle::fillMap(QVariantMap& map, typename QPair<QString, R> p)
{
	map.insert(p.first, p.second);
}

template<typename... Args>
bool FileHandle::update2Json(QString path, Args... args)
{
	QFile file(path);
	if (!file.open(QFile::ReadOnly | QFile::Text))
	{
		LOG4_ERROR("can't open the file : " << path.toStdString().c_str());
		return false;
	}

	QTextStream stream(&file);
	stream.setCodec("UTF-8");
	QString str = stream.readAll();
	file.close();

	QJsonParseError jsonError;
	QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8(), &jsonError);
	if (jsonError.error != QJsonParseError::NoError && !doc.isNull()) {
		LOG4_ERROR("json is wrong : " << jsonError.errorString().toStdString().c_str());
		return false;
	}

	QJsonObject root_obj = doc.object();

	QVariantMap map = root_obj.toVariantMap();

	int arr[] = { (updateObj(map,args),0)... };

	root_obj = QJsonObject::fromVariantMap(map);
	doc.setObject(root_obj);
	QByteArray array = doc.toJson(QJsonDocument::Indented);

	QFile loadFile(path);
	if (!loadFile.open(QIODevice::WriteOnly))
	{
		LOG4_ERROR("could't open json file");
		return false;
	}

	loadFile.write(array);
	loadFile.close();

	return true;

}

template<typename... Args>
QString FileHandle::update2JsonStr(QString json, Args... args)
{
	QJsonParseError jsonError;
	QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8(), &jsonError);
	if (jsonError.error != QJsonParseError::NoError && !doc.isNull()) {
		LOG4_ERROR("json is wrong : " << jsonError.errorString().toStdString().c_str());
		return false;
	}

	QJsonObject root_obj = doc.object();

	QVariantMap map = root_obj.toVariantMap();

	int arr[] = { (updateObj(map,args),0)... };

	root_obj = QJsonObject::fromVariantMap(map);
	doc.setObject(root_obj);
	QByteArray array = doc.toJson(QJsonDocument::Indented);

	return QString(array);

}

template<typename R>
void FileHandle::updateObj(QVariantMap& map, typename QPair<QString, R> p)
{
	QString key = p.first;
	if (!map.contains(key))
	{
		LOG4_WARN("json has no such key : " << key.toStdString().c_str());
		return;
	}
	map[key] = p.second;
}

template<typename... Args>
QString FileHandle::insert2JsonStr(QString json, Args... args)
{
	QJsonParseError jsonError;
	QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8(), &jsonError);
	if (jsonError.error != QJsonParseError::NoError && !doc.isNull()) {
		LOG4_ERROR("json is wrong : " << jsonError.errorString().toStdString().c_str());
		return false;
	}

	QJsonObject root_obj = doc.object();

	QVariantMap map = root_obj.toVariantMap();

	int arr[] = { (insertObj(map,args),0)... };

	root_obj = QJsonObject::fromVariantMap(map);
	doc.setObject(root_obj);
	QByteArray array = doc.toJson(QJsonDocument::Compact);

	return QString(array);
}

template<typename R>
void FileHandle::insertObj(QVariantMap& map, typename QPair<QString, R> p)
{
	QString key = p.first;
	map[key] = p.second;
}
