include ../build.mk

SGX_SDK ?= /opt/intel/sgxsdk
SGX_MODE ?= HW
SGX_ARCH ?= x64
SGX_DEBUG ?= 1

ifeq ($(shell getconf LONG_BIT), 32)
	SGX_ARCH := x86
else ifeq ($(findstring -m32, $(CXXFLAGS)), -m32)
	SGX_ARCH := x86
endif

ifeq ($(SGX_ARCH), x86)
	SGX_COMMON_CFLAGS := -m32
	SGX_LIBRARY_PATH := $(SGX_SDK)/lib
	SGX_ENCLAVE_SIGNER := $(SGX_SDK)/bin/x86/sgx_sign
	SGX_EDGER8R := $(SGX_SDK)/bin/x86/sgx_edger8r
else
	SGX_COMMON_CFLAGS := -m64
	SGX_LIBRARY_PATH := $(SGX_SDK)/lib64
	SGX_ENCLAVE_SIGNER := $(SGX_SDK)/bin/x64/sgx_sign
	SGX_EDGER8R := $(SGX_SDK)/bin/x64/sgx_edger8r
endif


ifneq ($(SGX_MODE), HW)
	Urts_Library_Name := sgx_urts_sim
else
	Urts_Library_Name := sgx_urts
endif

ifeq ($(SGX_DEBUG), 1)
        SGX_COMMON_CFLAGS += -O0 -g
else
        SGX_COMMON_CFLAGS += -O2
endif




LIBS := -L$(SGX_LIBRARY_PATH) -l$(Urts_Library_Name) -lpthread -Wl,-rpath=$(CURDIR) -pthread

ifneq ($(SGX_MODE), HW)
	LIBS += -lsgx_uae_service_sim
else
	LIBS += -lsgx_uae_service
endif


INCLUDES = -I$(SGX_SDK)/include

CFLAGS := $(SGX_COMMON_CFLAGS) -fPIC -Wno-attributes $(INCLUDES)
CPPFLAGS := $(CFLAGS) -std=c++11
CFLAGS := -std=gnu11 $(CFLAGS)
