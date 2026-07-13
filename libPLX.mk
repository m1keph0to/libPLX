# make fragment for PLX devices data library
# input argument LIBPLX
# outputs OBJ_DIR to create

# Other macros used by 



PLX_PATH  = $(LIBPLX)/src
PLX_SRC   = $(PLX_PATH)
PLX_INC   = $(LIBPLX)/inc
PLX_OBJ   = $(OBJPATH)/libPLX
PLX_OBJECTS = $(PLX_OBJ)/plx.o
PLX_OBJECTS = $(PLX_OBJ)/displayinfo.o
PLX_OBJECTS += $(PLX_OBJ)/commsLib.o

# build these sources 
$(PLX_OBJ)/%.o: $(PLX_SRC)/%.c $(DEPENDS)
	$(CC) $(CFLAGS)   $< -o $@

# C_INCLUDES and C_OBJECTS used by simple makefiles, 
# other macros are for more complex makefiles 
C_INCLUDES += -I$(PLX_INC)

C_SOURCES += $(LIBPLX)/src/plx.c
C_SOURCES += $(LIBPLX)/src/displayinfo.c
C_SOURCES += $(LIBPLX)/src/commsLib.c


ifeq ($(PREBUILT_LIBS),1)

LIBS += -lplx
LIBDIR += -L $(LIBPLX)/lib

endif