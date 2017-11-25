/*
* https://github.com/rohanrhu/node-Win32Volume
*
* Win32Volume is a node.js module for setting volume level in Windows platform
*
* Copyright (C) 2017 Oğuzhan Eroğlu <rohanrhu2@gmail.com>
* Licensed under The MIT License (MIT)
*
*/

var Win32Volume;

try {
    Win32Volume = require('./build/Release/win32-volume.node');
} catch (error) {
    Win32Volume = require('./build/Debug/win32-volume.node');
}

module.exports = Win32Volume;