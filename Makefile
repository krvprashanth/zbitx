CC = gcc
CFLAGS = -g -I./include -pthread `pkg-config --cflags gtk+-3.0`
LDFLAGS = -lwiringPi -lasound -lm -lfftw3 -lfftw3f -pthread -lncurses -lsqlite3 `pkg-config --libs gtk+-3.0`
FT8_LIB = libs/ft8_lib/libft8.a

SRCS = \
	src/radio/vfo.c \
	src/drivers/si570.c \
	src/core/sbitx_sound.c \
	src/dsp/fft_filter.c \
	src/core/sbitx_gtk.c \
	src/core/sbitx_utils.c \
	src/drivers/i2cbb.c \
	src/drivers/si5351v2.c \
	src/logbook/ini.c \
	src/radio/hamlib.c \
	src/core/queue.c \
	src/modems/modems.c \
	src/logbook/logbook.c \
	src/modems/modem_cw.c \
	src/core/settings_ui.c \
	src/drivers/oled.c \
	src/display/hist_disp.c \
	src/logbook/ntputil.c \
	src/radio/telnet.c \
	src/radio/macros.c \
	src/modems/modem_ft8.c \
	src/radio/remote.c \
	src/web/mongoose.c \
	src/web/webserver.c \
	src/main.c

OBJS = $(SRCS:.c=.o)
TARGET = sbitx

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS) $(FT8_LIB) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
