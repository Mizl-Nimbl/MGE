#!/bin/sh
pushd ~/Documents/MGE
git add .
git commit -m "Change"
git push -u origin main
popd
