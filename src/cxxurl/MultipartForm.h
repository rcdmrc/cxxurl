/**
 * @author : xiaozhuai
 * @date   : 17/1/3
 */

#ifndef CXXURL_MULTIPARTFORM_H
#define CXXURL_MULTIPARTFORM_H


#include <string>
#include <map>
#include <iostream>
#include <vector>
#include <curl/curl.h>
#include "RequestBody.h"
#include "FormItem.h"

namespace CXXUrl {

class MultipartForm : public RequestBody {
    public:
        MultipartForm() :
                RequestBody(RequestBody::MULTIPART_FORM_DATA),
                m_FormHeadPtr(nullptr),
                m_FormTailPtr(nullptr) { }

        ~MultipartForm() {
            if(m_FormHeadPtr!=nullptr) curl_formfree(m_FormHeadPtr);
        }

    public:
        MultipartForm &add(const std::string& key, const std::string& value) {
            FormItem item;
            item.type = FormItem::KEY_VALUE;
            item.key = key;
            item.value = value;
            m_FormDataList.push_back(item);
            return *this;
        }

        MultipartForm &addFile(const std::string& key, const std::string& filePath, const std::string& fileName="") {
            FormItem item;
            item.type = FormItem::FILE;
            item.key = key;
            item.filePath = filePath;
            item.fileName = fileName;
            m_FormDataList.push_back(item);
            return *this;
        }

        MultipartForm &addMemoryFile(const std::string& key, const std::vector<unsigned char>& fileContent) {
            FormItem item;
            item.type = FormItem::MEMORY_BUFFER;
            item.key = key;
            item.buffer = fileContent;
            m_FormDataList.push_back(item);
            return *this;
        }

        struct curl_httppost* getData() {

            if(m_FormHeadPtr!=nullptr){
                curl_formfree(m_FormHeadPtr);
                m_FormHeadPtr = nullptr;
                m_FormTailPtr = nullptr;
            }

            for(auto& i : m_FormDataList){
                switch(i.type){
                    case FormItem::ITEM_TYPE::KEY_VALUE:
                        curl_formadd(&m_FormHeadPtr,
                                     &m_FormTailPtr,
                                     CURLFORM_COPYNAME, i.key.c_str(),
                                     CURLFORM_COPYCONTENTS, i.value.c_str(),
                                     CURLFORM_END);
                        break;
                    case FormItem::ITEM_TYPE::FILE:
                        if(i.fileName.empty()){
                            curl_formadd(&m_FormHeadPtr,
                                         &m_FormTailPtr,
                                         CURLFORM_COPYNAME, i.key.c_str(),
                                         CURLFORM_FILE, i.filePath.c_str(),
                                         CURLFORM_END);
                        }else{
                            curl_formadd(&m_FormHeadPtr,
                                         &m_FormTailPtr,
                                         CURLFORM_COPYNAME, i.key.c_str(),
                                         CURLFORM_FILE, i.filePath.c_str(),
                                         CURLFORM_FILENAME, i.fileName.c_str(),
                                         CURLFORM_END);
                        }
                        break;
                    case FormItem::ITEM_TYPE::MEMORY_BUFFER:
                        curl_formadd(&m_FormHeadPtr,
                                     &m_FormTailPtr,
                                     CURLFORM_COPYNAME, i.key.c_str(),
                                     CURLFORM_BUFFER, i.key.c_str(),
                                     CURLFORM_BUFFERPTR, i.buffer.data(),
                                     CURLFORM_BUFFERLENGTH, (long)i.buffer.size(),
                                     CURLFORM_END);
                        break;
                }
            }

            return m_FormHeadPtr;

        }

        void clear() {
            m_FormDataList.erase(m_FormDataList.begin(), m_FormDataList.end());
        }

    protected:
        struct curl_httppost* m_FormHeadPtr;
        struct curl_httppost* m_FormTailPtr;
        std::vector<FormItem> m_FormDataList;
};

}

#endif //CXXURL_MULTIPARTFORM_H
