# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

LOCAL_PATH := $(call my-dir)
TARGET_ARCH_ABI := $(APP_ABI)

rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

include $(CLEAR_VARS)
LOCAL_MODULE := il2cpp-codegen
LOCAL_SRC_FILES := extern/codegen/libil2cpp_codegen.so
LOCAL_EXPORT_C_INCLUDES := extern/codegen/include
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_LDLIBS     += -llog
LOCAL_CFLAGS     += -DMOD_ID='"BSML"' -DVERSION='"1.3.2"' -I'C:\Unity\2019.3.2f1\Editor\Data\il2cpp\libil2cpp'
LOCAL_MODULE     := BSML
LOCAL_C_INCLUDES := ./include ./src
LOCAL_SRC_FILES  := $(call rwildcard,extern/beatsaber-hook/shared/inline-hook/,*.cpp) $(call rwildcard,extern/beatsaber-hook/shared/utils/,*.cpp) $(call rwildcard,extern/beatsaber-hook/shared/inline-hook/,*.c) $(call rwildcard,extern/questui/,*.cpp)
# In order to add configuration support to your project, uncomment the following line:
LOCAL_SRC_FILES  += $(call rwildcard,extern/beatsaber-hook/shared/config/,*.cpp)
# In order to add custom UI support to your project, uncomment the following line:
# LOCAL_SRC_FILES  += $(call rwildcard,extern/beatsaber-hook/shared/customui/,*.cpp)
# Add any new SRC includes from beatsaber-hook or other external libraries here
LOCAL_SRC_FILES  += $(call rwildcard,src/,*.cpp)
LOCAL_SHARED_LIBRARIES := il2cpp_codegen
include $(BUILD_SHARED_LIBRARY)
