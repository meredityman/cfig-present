#!/bin/bash

#make -j8

#src_dir="/media/meredityman/data-ssd/data/work/production/cfig/Code/cfig-outputs/docs_3"
src_dir="/home/ana/cfig-outputs/docs_3"

# Symbolic link path
link_path="$PWD"/bin/data/current-jobs
echo $link_path

# # Check if the source directory exists
if [ ! -d "$src_dir" ]; then
   echo "Source directory $src_dir does not exist"
   exit 1
else
   ls -l "$src_dir"
fi

# Check if the symbolic link already exists
if [ -L "$link_path" ]; then
  # If the existing link points to the correct source, there’s nothing to do
  if [ "$(readlink "$link_path")" == "$src_dir" ]; then
    echo "Symbolic link $link_path already exists and points to $src_dir"
  else
    # Otherwise, remove the existing link before creating a new one
    echo "Removing existing symbolic link $link_path"
    rm "$link_path"

    # Create the symbolic link
    ln -s "$src_dir" "$link_path"
    echo "Symbolic link $link_path created, pointing to $src_dir"
  fi
elif [ -e "$link_path" ]; then
  # If a file or directory exists at the link path, abort to avoid data loss
  echo "A file or directory already exists at $link_path"
  exit 1
else
    ln -s "$src_dir" "$link_path"
    echo "Symbolic link $link_path created, pointing to $src_dir"

fi
echo "Running program"
bin/cfig-present
