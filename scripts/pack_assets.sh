#!/bin/bash
cd assets/
atlaspkr atlas01.png blocks/*
export GLOBIGNORE="blocks"
assetpkr assets.pak *
mv assets.pak ../
unset GLOBIGNORE
