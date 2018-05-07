#ifndef _MyTextureManager_H_
#define _MyTextureManager_H_

#include "Singleton.h"
#include <algorithm>
#include "Global.h"
#include "WiEngine.h"

class MyTextureManager : public Singleton<MyTextureManager>
{
    DECLARE_SINGLETON_CLASS(MyTextureManager);
    
public:
    vector<unsigned int> *resIds;
    vector<unsigned int> *retainedResIds;
    
    MyTextureManager(void) {
//        resIds = new vector<unsigned int>;
//        resIds->clear();
//        
//        retainedResIds = new vector<unsigned int>;
//        retainedResIds->clear();
//
//        retainedResIds->push_back(getHash("dialog_deco.png"));
//        
//        retainedResIds->push_back(getHash("s01.png"));
//        retainedResIds->push_back(getHash("s02.png"));
//        retainedResIds->push_back(getHash("s03.png"));
//        
//        retainedResIds->push_back(getHash("bamboo_0.png"));
//        retainedResIds->push_back(getHash("bamboo_2.png"));
//        
//        retainedResIds->push_back(getHash("cave_0.png"));
//        retainedResIds->push_back(getHash("cave_3.png"));
//        
//        retainedResIds->push_back(getHash("grass_0.png"));
//        retainedResIds->push_back(getHash("grass_4.png"));
//        
//        retainedResIds->push_back(getHash("enemy_0.png"));
//        retainedResIds->push_back(getHash("enemy_1.png"));
//        retainedResIds->push_back(getHash("enemy_2.png"));
//        retainedResIds->push_back(getHash("effects.png"));
//        retainedResIds->push_back(getHash("hero.png"));
    }
    
    ~MyTextureManager(void) {
//        delete resIds;
//        delete retainedResIds;
    }
    
//    unsigned int getHash(const char *name) {
//        return wyUtils::strHash(name);
//    }
//    
//    void addToResIds(unsigned int resId) {
////        LOGE("add to resId:%d", resId);
//        vector<unsigned int>::iterator result = find_if(resIds->begin(), resIds->end(), resId); //查找resId
//        if (result == resIds->end()) {
//            //没找到
//            resIds->push_back(resId);
////            LOGE("res:%s", resId);
//        }
//    }
//
//    void addToRetainedRes(unsigned int resId) {
//        vector<unsigned int>::iterator result = find_if(retainedResIds->begin(), retainedResIds->end(), resId); //查找resId
//        if (result == retainedResIds->end()) {
//            //没找到
//            retainedResIds->push_back(resId);
//        }
//    }
//    
    void removeAllTextures() {
//        vector<unsigned int>::iterator it;
//        for (it = resIds->begin(); it != resIds->end(); ) {
//            unsigned int resId = *it;
//            //如果不在retain列表中，则删除
//            if (find_if(retainedResIds->begin(), retainedResIds->end(), resId) == retainedResIds->end()) {
//                wyTextureManager::getInstance()->removeTexture(resId);
//                it = resIds->erase(it);
//            } else {
//                ++it;
//            }
//        }
        
        wyTextureManager::getInstance()->removeAllTextures();
    }
    
    static wyTexture2D* makeJPG(const char* assetPath, wyTexturePixelFormat format = WY_TEXTURE_PIXEL_FORMAT_RGBA4444) {
#if IOS
        return wyTexture2D::makeJPG(assetPath, 0, WY_TEXTURE_PIXEL_FORMAT_RGBA4444);
#endif
        
#if ANDROID
        return wyTexture2D::make(assetPath, 0, WY_TEXTURE_PIXEL_FORMAT_RGBA8888);
#endif
    }
    
    static wyTexture2D* makePNG(const char* assetPath, wyTexturePixelFormat format = WY_TEXTURE_PIXEL_FORMAT_RGBA8888) {
#if IOS
        return wyTexture2D::makePNG(assetPath, format);
#endif

#if ANDROID
        return wyTexture2D::make(assetPath, 0, WY_TEXTURE_PIXEL_FORMAT_RGBA8888);
#endif
    }
    
//    static wyTexture2D* makeJPG(int resId, wyTexturePixelFormat format = WY_TEXTURE_PIXEL_FORMAT_RGBA4444) {
////        MyTextureManager::instance()->addToResIds(assetPath);
//        return wyTexture2D::make(resId, WY_TEXTURE_PIXEL_FORMAT_RGB565);
//    }
//    
//    static wyTexture2D* makePNG(int resId, wyTexturePixelFormat format = WY_TEXTURE_PIXEL_FORMAT_RGBA4444) {
////        MyTextureManager::instance()->addToResIds(assetPath);
//        return wyTexture2D::make(resId, format);
//    }
};

#define sTextureMgr MyTextureManager::instance()

#endif