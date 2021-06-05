#!/bin/sh
if type dot >/dev/null 2>&1; then
  exit
fi

if [ "_$HOME" != "_/home/runner" ]; then
  echo '`dot` not found. Please install Graphviz.'
  echo '   For Mac users: `brew install graphviz`'
  echo '   For Ubuntu users: `apt-get install graphviz`'
  exit 1
fi

# repl.it
dir=$(cd $(dirname $0) && pwd)
if [ ! -d $dir/usr ]; then
  sh install.dot.sh
  mv /home/runner/.apt/usr $dir/usr
fi

mkdir -p /home/runner/.apt
ln -s $dir/usr /home/runner/.apt/usr
