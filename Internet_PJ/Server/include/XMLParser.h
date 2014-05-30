#ifndef __XML_PARSER__
#define __XML_PARSER__

#include "Common.h"
	
class XMLParser
{
public:
    XMLParser() ;
    XMLParser(string xmlFileName ,string xmlFilePath) ;
    void XmlWriteVersion(string version,ofstream &xmlFile) ;//写入版本号
    void XmlTagBegin(string tags ,ofstream &xmlFile) ;//标签开始
    void XmlTagEnd(ofstream &xmlFile) ; //标签结束
    void XmlWriteValue(string strValue ,ofstream &xmlFile) ;//插入值
    void XmlEnd(ofstream &xmlFile) ; //文件结束
    string sXmlFileAllPath ;
    string GetValueByTag(string tagValue ,ifstream &xmlFile) ;//获取指定tag的值,单值
    static vector<string> GetValuesByTag(string tagValue ,ifstream &xmlFile) ;//获取指定tag的值，vector

	string X_GetValueByTag(string tagValue,ifstream &xmlFile);
	string file;
	void Get_file(ifstream &xmlFile);
    
	int curTagTab ;
private:
    string sXmlFilePath ;//xml 文件路径
    string sXmlFileName ; //xml 文件名
    stack<string> sCurTag ;//xml 当前需要结束标签
} ;
#endif