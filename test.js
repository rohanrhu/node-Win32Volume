var Win32Volume = require('./index.js');

Win32Volume.setVolume(0.7, function (success) {
    console.log('setVolume:', success);
});

Win32Volume.setMute(true, function (success) {
    console.log('setMute:', success);
});

// var result = Win32Volume.setMuteSync(true);
// var result = Win32Volume.setVolumeSync(0.7);