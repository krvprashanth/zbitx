project('zbitx', 'c',
  version : '0.1',
  default_options : ['warning_level=1', 'c_std=gnu11'])

cc = meson.get_compiler('c')

# Dependencies
gtk3_dep = dependency('gtk+-3.0')
alsa_dep = dependency('alsa')
fftw3_dep = dependency('fftw3')
fftw3f_dep = dependency('fftw3f')
ncurses_dep = dependency('ncurses')
sqlite3_dep = dependency('sqlite3')
threads_dep = dependency('threads')
m_dep = cc.find_library('m', required : true)

# Optional dependencies from reference (kept for compatibility/future use)
libgpiod_dep = dependency('libgpiod', required : false) 
libpulse_dep = dependency('libpulse-simple', required : false)
# Legacy dependency used in current codebase
wiringpi_dep = cc.find_library('wiringPi', required : false)

# FT8 Library
ft8_lib_sources = files(
  'libs/ft8_lib/ft8/constants.c',
  'libs/ft8_lib/ft8/encode.c',
  'libs/ft8_lib/ft8/pack.c',
  'libs/ft8_lib/ft8/text.c',
  'libs/ft8_lib/common/wave.c',
  'libs/ft8_lib/ft8/crc.c',
  'libs/ft8_lib/fft/kiss_fftr.c',
  'libs/ft8_lib/fft/kiss_fft.c',
  'libs/ft8_lib/ft8/decode.c',
  'libs/ft8_lib/ft8/ldpc.c',
  'libs/ft8_lib/ft8/unpack.c',
)

ft8_inc = include_directories('libs/ft8_lib')

ft8_lib = static_library('ft8',
  ft8_lib_sources,
  include_directories : ft8_inc,
  c_args : ['-std=c11']
)

ft8_dep = declare_dependency(
  include_directories : ft8_inc,
  link_with : ft8_lib
)

# Source files (mapped from Makefile)
sbitx_sources = files(
  'src/radio/vfo.c',
  'src/drivers/si570.c',
  'src/core/sbitx_sound.c',
  'src/dsp/fft_filter.c',
  'src/core/sbitx_gtk.c',
  'src/core/sbitx_utils.c',
  'src/drivers/i2cbb.c',
  'src/drivers/si5351v2.c',
  'src/logbook/ini.c',
  'src/radio/hamlib.c',
  'src/core/queue.c',
  'src/modems/modems.c',
  'src/logbook/logbook.c',
  'src/modems/modem_cw.c',
  'src/core/settings_ui.c',
  'src/drivers/oled.c',
  'src/display/hist_disp.c',
  'src/logbook/ntputil.c',
  'src/radio/telnet.c',
  'src/radio/macros.c',
  'src/modems/modem_ft8.c',
  'src/radio/remote.c',
  'src/web/mongoose.c',
  'src/web/webserver.c',
  'src/main.c'
  # 'src/drivers/i2cBitBangingBus.cpp' # Not in Makefile SRCS, not adding here unless needed
)

inc_dir = include_directories('include')

executable('sbitx',
  sbitx_sources,
  include_directories : inc_dir,
  dependencies : [
    gtk3_dep,
    alsa_dep,
    fftw3_dep,
    fftw3f_dep,
    ncurses_dep,
    sqlite3_dep,
    threads_dep,
    m_dep,
    wiringpi_dep,
    ft8_dep
  ],
  install : true
)
