/*
* https://github.com/rohanrhu/node-Win32Volume
*
* Win32Volume is a node.js module for setting volume level in Windows platform
*
* Copyright (C) 2017 Oğuzhan Eroğlu <rohanrhu2@gmail.com>
* Licensed under The MIT License (MIT)
*
*/

var path = require('path');

var Win32Volume;

try {
    Win32Volume = require(path.join(__dirname, 'build', 'Release', 'win32-volume.node'));
} catch (error) {
    if (error.code == 'MODULE_NOT_FOUND') {
        Win32Volume = require(path.join(__dirname, 'build', 'Debug', 'win32-volume.node'));
    } else {
        throw error;
    }
}

module.exports = Win32Volume;