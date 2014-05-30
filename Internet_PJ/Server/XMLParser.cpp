#include "include/XMLParser.h"

XMLParser::XMLParser()
{
    curTagTab = 0 ;
}
//给xml文件以及路径赋初值
XMLParser::XMLParser(string xmlFileName,string xmlFilePath)
{
    sXmlFileName = xmlFileName ;
    sXmlFilePath = xmlFilePath ;
    sXmlFileAllPath = sXmlFilePath + '\\' + sXmlFileName ;
    curTagTab = 0 ;
}

//写入版本
void XMLParser::XmlWriteVersion(string version,ofstream &xmlFile)
{
    
    xmlFile<<"<?xml version=\"" ;
    xmlFile<<version ;
    xmlFile<<"\" encoding=\"ISO-8859-1\"?>" ;
    xmlFile<<"\n" ;
}
//开始一个标签
void XMLParser::XmlTagBegin(string tags ,ofstream &xmlFile)
{
    for(int i = 0 ;i < curTagTab ; i ++)
        xmlFile<<"\t" ;
    xmlFile<<"<" ;
    xmlFile<<tags;
    xmlFile<<">" ;
    xmlFile<<"\n" ;
    sCurTag.push(tags) ;
    curTagTab ++ ;
}
//标签结束
void XMLParser::XmlTagEnd(ofstream &xmlFile)
{
    for(int i = 0 ;i < curTagTab - 1; i ++)
        xmlFile<<"\t" ;
    string sEndTag = sCurTag.top() ;
    sCurTag.pop() ;
    xmlFile<<"</" ;
    xmlFile<<sEndTag;
    xmlFile<<">" ;
    xmlFile<<"\n" ;
    curTagTab -- ;
}
//写入标签值
void XMLParser::XmlWriteValue(string strValue ,ofstream &xmlFile)
{
    for(int i = 0 ;i < curTagTab ;i ++)
        xmlFile<<"\t" ;
    xmlFile<<strValue ;
    xmlFile<<"\n" ;
}
//xml文件结束
void XMLParser::XmlEnd(ofstream &xmlFile)
{
    xmlFile.close() ;
    
}
//获取指定标签的值,此时值为单值
string XMLParser::GetValueByTag(string tagValue ,ifstream &xmlFile)
{
/*	
    while(! xmlFile.eof())
    {
        string tempStr ;
        xmlFile>>tempStr ;
        if(tempStr.find(tagValue) != -1)
        {
            string ans ;
            xmlFile>>ans ;
            
            return ans ;
        }
    }
*/
	int flag=0;	//进入获取状态 
	string ans;
    string TagValue="<"+tagValue+">";
    string anti_TagValue="</"+tagValue+">";
    while(! xmlFile.eof())
    {
        string tempStr;
        string tempStr2;
        xmlFile>>tempStr ;
        if(tempStr.find(TagValue) != -1)
        { 
			flag=1;                    
            xmlFile>>tempStr2;
            if(tempStr2.find(anti_TagValue)!=-1){
				cout<<"find1"<<tempStr2<<endl;
				break;
            }
            ans=ans+tempStr2;
        }
        else if(tempStr.find(anti_TagValue)!=-1){
           	break;
        }
        else if(1==flag){
        	ans=ans+tempStr;
        }
    }
    return ans ;
}
//获取指定标签的值，此时值为vector
vector<string> XMLParser::GetValuesByTag(string tagValue ,ifstream &xmlFile)
{
	int flag=0;
	string item;
	vector<string> ans ;
    string TagValue="<"+tagValue+">";
    string anti_TagValue="</"+tagValue+">";
    while(! xmlFile.eof())
    {
        string tempStr;
        string tempStr2;
        xmlFile>>tempStr;
		//*****************************
		if(tempStr.compare("</query>")==0){
			break;
		}
		//*****************************
        if(tempStr.find(TagValue) != -1)
        { 
			flag=1;                    
            xmlFile>>tempStr2;
            if(tempStr2.find(anti_TagValue)!=-1){
				cout<<"find1"<<tempStr2<<endl;
				continue;
            }
            item=item+tempStr2;
        }
        else if(tempStr.find(anti_TagValue)!=-1){
        	ans.push_back(item);
        	item.clear();
           	continue;
        }
        else if(1==flag){
        	item=item+tempStr;
        }
    }
    return ans ;
}

void 
XMLParser::Get_file(ifstream &xmlFile){
	
	while(!xmlFile.eof()){
		string tempStr;
		xmlFile>>tempStr;
		file=file+tempStr;
	}
}

string 
XMLParser::X_GetValueByTag(string tagValue,ifstream &xmlFile){
	string value;
    string TagValue="<"+tagValue+">";
    string anti_TagValue="</"+tagValue+">";
	int tag_length=TagValue.length();
	int start=file.find(TagValue.c_str());
	int end=file.find(anti_TagValue.c_str());
	value.assign(file,start+tag_length,end-start-tag_length);
	return value;
}


// int main()
// {
// 	XMLParser xml("","");
// 	ifstream xmlInfile;
// 	xmlInfile.open("index_data/inverted.txt",ifstream::in);
// 	xmlInfile.seekg(62035395);
// 	vector<string> items=xml.GetValuesByTag("index",xmlInfile);
// 	xmlInfile.close();

// 	vector<string>::iterator i_vec=items.begin();
// 	for(;i_vec!=items.end();++i_vec){
// 		cout<<*i_vec<<endl;
//     }
//     return 0 ;
// }

