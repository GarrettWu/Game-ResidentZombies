#ifndef _MTBASE64_H_
#define _MTBASE64_H_

#include <string>
#include <vector>

class MTBase64
{
public:
	/**
	* 对字符串进行base64编码
	* @param data :       需要编码的数据
	* @param bChangeLine: 是否需要在最终编码数据加入换行符(RFC中建议每76个字符后加入回车换行)，默认为不添加换行
	* @return std::string :    编码后的数据
    */
    static std::string encode(const std::string &data, bool bChangeLine = false);
    
    /**
    * 对字符串进行base64解码
    * @param data :    需要解码的数据
    * @return std::string : 解码后的数据
    */    
    static std::string decode(const std::string &data);
	
    /**
    * 对字符串进行base64编码 
    * @param pSrc :        需要编码的数据
    * @param nSrcLen :     需要编码的数据长度
    * @param pDst :        编码后的数据      
    * @param bChangeLine:  是否需要在最终编码数据加入换行符(RFC中建议每76个字符后加入回车换行)，默认为不添加换行
    * @return 编码后的字符串的长度
    */
    static int encode(const unsigned char* pSrc, int nSrcLen, char* pDst, bool bChangeLine = false);
    
    /**
    * 对字符串进行base64解码
    * @param pSrc :        需要解码的数据    
    * @param nSrcLen :     需要解码的数据长度
    * @param pDst :        解码后的数据
    * @return 解码后的字符串的长度
    */    
    static int decode(const char* pSrc, int nSrcLen, unsigned char* pDst);
    
protected:
	
    static const char EnBase64Tab[];
    static const char DeBase64Tab[];
};

#endif // _MTBASE64_H_
