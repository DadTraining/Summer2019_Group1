LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos/audio/include)

LOCAL_MODULE := MyGame_shared

LOCAL_MODULE_FILENAME := libMyGame

LOCAL_SRC_FILES := $(LOCAL_PATH)/hellocpp/main.cpp \
                   $(LOCAL_PATH)/../../../Classes/AppDelegate.cpp \
                   $(LOCAL_PATH)/../../../Classes/HelloWorldScene.cpp \
                   $(LOCAL_PATH)/../../../Classes/GamePlay.cpp \
                   $(LOCAL_PATH)/../../../Classes/HomeScene.cpp \
                   $(LOCAL_PATH)/../../../Classes/InputNameScene.cpp \
                   $(LOCAL_PATH)/../../../Classes/Level1Scene.cpp \
                   $(LOCAL_PATH)/../../../Classes/LoadingScene.cpp \
                   $(LOCAL_PATH)/../../../Classes/MainCharacter.cpp \
                   $(LOCAL_PATH)/../../../Classes/MainMenuScene.cpp \
                   $(LOCAL_PATH)/../../../Classes/MapScene.cpp \
                   $(LOCAL_PATH)/../../../Classes/ResourceManager.cpp \
                   $(LOCAL_PATH)/../../../Classes/SelectScene.cpp \
                   $(LOCAL_PATH)/../../../Classes/Armor.cpp \
                   $(LOCAL_PATH)/../../../Classes/Arrow.cpp \
                   $(LOCAL_PATH)/../../../Classes/Enemy.cpp \
                   $(LOCAL_PATH)/../../../Classes/Inventory.cpp \
                   $(LOCAL_PATH)/../../../Classes/Item.cpp \
                   $(LOCAL_PATH)/../../../Classes/Level2Scene.cpp \
                   $(LOCAL_PATH)/../../../Classes/Monster.cpp \
                   $(LOCAL_PATH)/../../../Classes/Pierce.cpp \
                   $(LOCAL_PATH)/../../../Classes/Potion.cpp \
                   $(LOCAL_PATH)/../../../Classes/Slash.cpp \
                   $(LOCAL_PATH)/../../../Classes/SpearMoblin.cpp \
                   $(LOCAL_PATH)/../../../Classes/Weapon.cpp 

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../Classes

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-add-path, $(LOCAL_PATH)/../../../cocos2d)
$(call import-module, cocos)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
