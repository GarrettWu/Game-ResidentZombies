#ifndef _MTBASE64_H_
#define _MTBASE64_H_

#include <string>
#include <vector>

class MTBase64
{
public:
	/**
	* ���ַ�������base64����
	* @param data :       ��Ҫ���������
	* @param bChangeLine: �Ƿ���Ҫ�����ձ������ݼ��뻻�з�(RFC�н���ÿ76���ַ������س�����)��Ĭ��Ϊ����ӻ���
	* @return std::string :    ����������
    */
    static std::string encode(const std::string &data, bool bChangeLine = false);
    
    /**
    * ���ַ�������base64����
    * @param data :    ��Ҫ���������
    * @return std::string : ����������
    */    
    static std::string decode(const std::string &data);
	
    /**
    * ���ַ�������base64���� 
    * @param pSrc :        ��Ҫ���������
    * @param nSrcLen :     ��Ҫ��������ݳ���
    * @param pDst :        ����������      
    * @param bChangeLine:  �Ƿ���Ҫ�����ձ������ݼ��뻻�з�(RFC�н���ÿ76���ַ������س�����)��Ĭ��Ϊ����ӻ���
    * @return �������ַ����ĳ���
    */
    static int encode(const unsigned char* pSrc, int nSrcLen, char* pDst, bool bChangeLine = false);
    
    /**
    * ���ַ�������base64����
    * @param pSrc :        ��Ҫ���������    
    * @param nSrcLen :     ��Ҫ��������ݳ���
    * @param pDst :        ����������
    * @return �������ַ����ĳ���
    */    
    static int decode(const char* pSrc, int nSrcLen, unsigned char* pDst);
    
protected:
	
    static const char EnBase64Tab[];
    static const char DeBase64Tab[];
};

#endif // _MTBASE64_H_
