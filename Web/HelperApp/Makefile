CPP             = g++
RM              = rm -f
CPP_FLAGS       = -Wall -c -I. -O2 -std=c++11

LD              = g++
LD_FLAGS        = -Wall -O2 
RESULT          = pelirapp

INSTALL_DIR	= /usr/local/bin

SOURCES			= src/pelirapp.cpp \
				  src/utils/base64.cpp \
				  ../../IRLib/Common.cpp \
				  ../../IRLib/Encoder.cpp \
				  ../../IRLib/Decoder.cpp \
				  ../../IRLib/RemoteFinder.cpp \
				  ../../IRLib/Remotes/RmtAux1.cpp \
				  ../../IRLib/Remotes/RmtTcl1.cpp \
				  ../../IRLib/Remotes/RmtChigo1.cpp \
				  ../../IRLib/Remotes/RmtChigo2.cpp \
				  ../../IRLib/Remotes/RmtLG1.cpp \
				  ../../IRLib/Remotes/RmtMits1.cpp \
				  ../../IRLib/Remotes/RmtGreeAll.cpp
OBJECTS         = $(SOURCES:%.cpp=%.o)

all:	${OBJECTS} ${RESULT}

${RESULT}: ${OBJECTS}
		${LD} ${LD_FLAGS} -o $@ ${OBJECTS}

clean:
		${RM} *.obj *~* ${OBJECTS} ${RESULT}

${OBJECTS}: 
		${CPP} ${CPP_FLAGS} -fpic -o $@ ${@:%.o=%.cpp}

install:
		cp -f ${RESULT} ${INSTALL_DIR}

uninstall:
		rm -f ${INSTALL_DIR}/${RESULT}
