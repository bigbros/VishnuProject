VISHNU_PATH		:= ../..
VISHNU_ENGINE_PATH	:= $(VISHNU_PATH)/Engine
LUA_PATH		:= $(VISHNU_PATH)/External/lua5.2.3
YAJL_PATH		:= $(VISHNU_PATH)/External/yajl-1.0.12
LIBPNG_PATH		:= $(VISHNU_PATH)/External/libpng
ZLIB_PATH		:= $(VISHNU_PATH)/External/zlib
UNIXMOD_PATH		:= $(VISHNU_PATH)/Platform/UnixModules
VR_PATH			:= $(VISHNU_PATH)/Platfoem/Oculus

LOCAL_CFLAGS += -I$(VISHNU_ENGINE_PATH)/Core/include
LOCAL_CFLAGS += -I$(VISHNU_ENGINE_PATH)/Draw
LOCAL_CFLAGS += -I$(VISHNU_ENGINE_PATH)/Draw/GLES2_2D
LOCAL_CFLAGS += -I$(VISHNU_ENGINE_PATH)/Draw/GLES2_3D
LOCAL_CFLAGS += -I$(VISHNU_ENGINE_PATH)/Platform
LOCAL_CFLAGS += -I$(VISHNU_ENGINE_PATH)/Script
LOCAL_CFLAGS += -I$(VISHNU_ENGINE_PATH)/Utility
LOCAL_CFLAGS += -I$(VISHNU_ENGINE_PATH)/SystemTask
LOCAL_CFLAGS += -I$(LUA_PATH)
LOCAL_CFLAGS += -I$(YAJL_PATH)
LOCAL_CFLAGS += -I$(LIBPNG_PATH)
LOCAL_CFLAGS += -I$(ZLIB_PATH)
LOCAL_CFLAGS += -I$(UNIXMOD_PATH)
LOCAL_CFLAGS += -I$(VR_PATH)

VISHNU_SRC_FILES	:= $(wildcard $(VISHNU_ENGINE_PATH)/Core/*.cpp)
VISHNU_SRC_FILES	+= $(wildcard $(VISHNU_ENGINE_PATH)/Draw/*.cpp)
VISHNU_SRC_FILES	+= $(wildcard $(VISHNU_ENGINE_PATH)/Draw/GLES2_2D/*.cpp)
VISHNU_SRC_FILES	+= $(wildcard $(VISHNU_ENGINE_PATH)/Draw/GLES2_3D/*.cpp)
VISHNU_SRC_FILES	+= $(wildcard $(VISHNU_ENGINE_PATH)/Platform/*.cpp)
VISHNU_SRC_FILES	+= $(wildcard $(VISHNU_ENGINE_PATH)/Script/*.cpp)
VISHNU_SRC_FILES	+= $(wildcard $(VISHNU_ENGINE_PATH)/Utility/*.cpp)
VISHNU_SRC_FILES	+= $(wildcard $(VISHNU_ENGINE_PATH)/SystemTask/*.cpp)

LUA_SRC_FILES		:=	$(wildcard $(LUA_PATH)/*.c)
YAJL_SRC_FILES		:=	$(wildcard $(YAJL_PATH)/*.c)
LIBPNG_SRC_FILES	:=	$(wildcard $(LIBPNG_PATH)/*.c)
ZLIB_SRC_FILES		:=	$(wildcard $(ZLIB_PATH)/*.c)

EXTERNAL_SRC_FILES	:= $(LUA_SRC_FILES)
EXTERNAL_SRC_FILES	+= $(YAJL_SRC_FILES)
EXTERNAL_SRC_FILES	+= $(LIBPNG_SRC_FILES)
EXTERNAL_SRC_FILES	+= $(ZLIB_SRC_FILES)

UNIXMOD_SRC_FILES	:=	$(wildcard $(UNIXMOD_PATH)/*.cpp)
VR_SRC_FILES		:=	$(wildcard $(VR_PATH)/*.cpp)

# for UserTask
USERTASK_PATH		:= $(VISHNU_PATH)/UserTask
TEST_PATH			:= $(VISHNU_PATH)/Test
LOCAL_CFLAGS		+= -I$(USERTASK_PATH)
LOCAL_CFLAGS		+= -I$(TEST_PATH)
USERTASK_SRC_FILES	:=	$(wildcard $(USERTASK_PATH)/*.cpp)
USERTASK_SRC_FILES	+=	$(wildcard $(TEST_PATH)/*.cpp)

LOCAL_SRC_FILES := main.c raspi.cpp CVSNRasPi.cpp
LOCAL_SRC_FILES	+= $(patsubst $(LOCAL_PATH)/%,%,$(UNIXMOD_SRC_FILES))
LOCAL_SRC_FILES	+= $(patsubst $(LOCAL_PATH)/%,%,$(VR_SRC_FILES))
LOCAL_SRC_FILES	+= $(patsubst $(LOCAL_PATH)/%,%,$(VISHNU_SRC_FILES))
LOCAL_SRC_FILES	+= $(patsubst $(LOCAL_PATH)/%,%,$(EXTERNAL_SRC_FILES))
LOCAL_SRC_FILES	+= $(patsubst $(LOCAL_PATH)/%,%,$(USERTASK_SRC_FILES))

LOCAL_LDLIBS    := -lc -lm -lbcm_host -lpthread -lEGL -lGLESv2 -L/opt/vc/lib


CC			:=	gcc
CPP			:=	g++

INCLUDE		:=	-I/opt/vc/include -I/opt/vc/include/interface/vcos/pthreads
LIBS		:=	$(LOCAL_LDLIBS)
CFLAGS		:=	$(INCLUDE) $(LOCAL_CFLAGS) -O2 -Wall -DRAS_PI
CPPFLAGS	:=	$(CFLAGS)

C_OBJS		:=	$(LOCAL_SRC_FILES:%.c=%.o)
OBJS		:=	$(C_OBJS:%.cpp=%.o)

TARGET	:=	vishnu

all:	$(TARGET)

clean:;	rm $(OBJS)

$(TARGET):	$(OBJS)
		$(CC) $(LIBS) -o $@ $(OBJS)

.c.o:;	$(CC) $(CFLAGS) -c $< -o $@

.cpp.o:;	$(CPP) $(CPPFLAGS) -c $< -o $@

















