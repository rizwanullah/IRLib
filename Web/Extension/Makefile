CPP             = g++
RM              = rm -f
CPP_FLAGS       = -Wall -c -I. -O2 -std=c++11

PREFIX			= /usr
#Edit these lines to correspond with your own directories
LIBRARY_DIR		= $(shell php-config --extension-dir)
PHP_CONFIG_DIR	= /etc/php/7.0/cli/conf.d

LD              = g++
LD_FLAGS        = -Wall -shared -O2 
RESULT          = pelir.so

PHPINIFILE		= 30-pelir.ini

#SOURCES			= $(wildcard *.cpp)
SOURCES			= pelir.cpp \
				  ../../IRLib/Common.cpp \
				  ../../IRLib/Encoder.cpp \
				  ../../IRLib/Decoder.cpp \
				  ../../IRLib/RemoteFinder.cpp \
				  ../../IRLib/Remotes/RmtAux1.cpp \
				  ../../IRLib/Remotes/RmtTcl1.cpp \
				  ../../IRLib/Remotes/RmtChigo1.cpp \
				  ../../IRLib/Remotes/RmtChigo2.cpp \
				  ../../IRLib/Remotes/RmtLG1.cpp
OBJECTS         = $(SOURCES:%.cpp=%.o)

all:	${OBJECTS} ${RESULT}

${RESULT}: ${OBJECTS}
		${LD} ${LD_FLAGS} -o $@ ${OBJECTS} -lphpcpp

clean:
		${RM} *.obj *~* ${OBJECTS} ${RESULT}

${OBJECTS}: 
		${CPP} ${CPP_FLAGS} -fpic -o $@ ${@:%.o=%.cpp}

install:
		cp -f ${RESULT} ${LIBRARY_DIR}
		cp -f ${PHPINIFILE}	${PHP_CONFIG_DIR}
