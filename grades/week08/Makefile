CFLAGS += -std=c99
CFLAGS += $$(pkg-config vips --cflags)
LDLIBS += $$(pkg-config vips --libs) -lm
LDLIBS += -lssl -lcrypto
all: pictDBM
pictDBM: db_create.o db_utils.o error.o db_list.o db_delete.o image_content.o pictDBM_tools.o dedup.o
