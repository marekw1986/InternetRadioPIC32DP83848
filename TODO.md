# Internet Radio PIC32
Internet radio based on PIC32

### Tasks
- [ ] Create WebUI
  - [x] Write function writting audio files from given directory as json accessed by http server
  - [ ] Add funcionality printing "root" folder
  - [ ] Add funcionality of reporting list of available streams
- [ ] Create HD44780 UI

### Completed Column ✓
- [x] Debug SD card
  - [x] Find out why SD card doesn't work when ENC28J60 is enabled
- [x] Find the reason why transfer from USB is slow, causing choppy playback
- [x] Implement circular buffer for audio data
- [x] Implement memory friendly HTTP header parser
- [x] Update FatFS library to the newest version
- [x] Determine why SPI Flash is so slow
