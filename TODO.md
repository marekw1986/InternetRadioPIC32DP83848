# Internet Radio PIC32
Internet radio based on PIC32

### Tasks
- [ ] Write WebUI
  - [x] Write function printing audio files from given directory as json accessed with http server
  - [x] Add funcionality of printing "root" folder
  - [x] Add funcionality of reporting list of available streams
  - [x] Add contextual next button (play next stream or next file in directory)
  - [x] Add aditional info about currently played file or audio stream
  - [x] Add volume control
  - [x] Add option od setting loop flag
  - [ ] Add possibility to save config from web UI
  - [ ] Add reset from web UI
  - [ ] Option of adding streams to list
- [ ] Create HD44780 UI
  - [x] Show volume value
  - [x] Show currently played stream/file
  - [ ] Create interactive menu
	- [x] Add option to select stream from list
	- [ ] Add option to play file from local storage
	- [ ] Add option to change playback settings (for example playing in loop)
  - [x] Scrolled text (info about played song/stream)
- [ ] Move I2C (HD44780) driver from plib to Harmony.

### Completed Column ✓
- [x] Debug SD card
  - [x] Find out why SD card doesn't work when ENC28J60 is enabled
- [x] Find the reason why transfer from USB is slow, causing choppy playback
- [x] Implement circular buffer for audio data
- [x] Implement memory friendly HTTP header parser
- [x] Update FatFS library to the newest version
- [x] Determine why SPI Flash is so slow
- [x] Move list of streams to file
- [x] Move project from MLA to Harmony libraries
- [x] Add backlight timeout
