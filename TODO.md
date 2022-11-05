# Internet Radio PIC32
Internet radio based on PIC32

### Tasks
- [ ] Write WebUI
  - [x] Write function printing audio files from given directory as json accessed with http server
  - [x] Add funcionality of printing "root" folder
  - [x] Add funcionality of reporting list of available streams
  - [x] Add contextual next button (play next stream or next file in directory)
  - [ ] Add aditional info about currently played file or audio stream
  - [ ] Add volume control
  - [ ] Add option od setting loop flag  
- [ ] Create HD44780 UI

### Completed Column ✓
- [x] Debug SD card
  - [x] Find out why SD card doesn't work when ENC28J60 is enabled
- [x] Find the reason why transfer from USB is slow, causing choppy playback
- [x] Implement circular buffer for audio data
- [x] Implement memory friendly HTTP header parser
- [x] Update FatFS library to the newest version
- [x] Determine why SPI Flash is so slow
- [x] Move list of streams to file
