# win32-volume
Set volume level or mute functions for Win32 platform.

#### Install

Install module with npm
`npm i win32-volume`

#### Example

Set volume or mute..

```javascript
var Win32Volume = require('win32-volume');

Win32Volume.setVolume(0.7, function (success) {
    console.log('setVolume:', success);
});

Win32Volume.setMute(true, function (success) {
    console.log('setMute:', success);
});

// or you can use blocking functions..

var result = Win32Volume.setMuteSync(true);
var result = Win32Volume.setVolumeSync(0.7);
```

## License

MIT