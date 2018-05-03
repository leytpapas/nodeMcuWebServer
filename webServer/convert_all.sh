#!/bin/bash

#
# This script walks through the html folder and minify all JS, HTML and CSS files. It also generates
# the corresponding constants that is added to the data.h file on esp8266_deauther folder.
#
# @Author Erick B. Tedeschi < erickbt86 [at] gmail [dot] com >
#

outputfile="$(dirname "$(pwd)")/progMEMData.h"

rm $outputfile

function miniEsc {
    file=$1
    var=$(cat $file)
    length=${#var}
    echo "starting length $length"
    var=$(echo $var | sed -e 's,\/\*(\*(?!\/)|[^*])*\*\/,,g' )
    

    var=$(echo $var | sed -e 's,(\r\n)|(\r)|(\n),,g')
    

    var=$(echo $var | sed -e 's,\s\s+, ,g' )
    
    var=$(echo $var | sed -e 's,",\",g')

    length=${#var}
    echo "length after minify $length"
    (echo $var) > /tmp/converter.temp

    
}

function ascii2hexCstyle {
    file_name=$(constFileName $1)
    result=$(cat /tmp/converter.temp | hexdump -ve '1/1 "0x%.2x,"')
    result=$(echo $result | sed 's/,$//')
    echo "const char data_${file_name}[] PROGMEM = {$result};"
}

function constFileName {
   extension=$(echo $1 | egrep -io "(css|js|html)$" | tr "[:lower:]" "[:upper:]")
   file=$(echo $1 | sed 's/\.css//' | sed 's/\.html//' | sed 's/\.js//' | sed 's/\.\///' | tr '/' '_' | tr '.' '_')
   echo $file$extension
}


cd html
file_list=$(find . -type f)

for file in $file_list; do
  echo "Processing: $file"
  if [[ "$file" == *.js ]]; then
    echo "-> JS minifier"
    miniEsc $file
    #sleep 10
    ascii2hexCstyle $file >> $outputfile
  elif [[ "$file" == *.html ]] || [[ "$file" == *.css ]]; then
    echo "-> HTML and CSS minifier"
    miniEsc $file
    #sleep 10
    ascii2hexCstyle $file >> $outputfile
  else
    echo "-> without minifier"
    cat $file > /tmp/converter.temp
    ascii2hexCstyle $file >> $outputfile
  fi
  #sleep 1
done
