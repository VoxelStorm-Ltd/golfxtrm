#!/bin/bash

git config --local "merge.highest_version_merge.name" "Keep the highest of two version.h files"
git config --local "merge.highest_version_merge.driver" "resources/merge_highest_version.sh %O %A %B %P"
