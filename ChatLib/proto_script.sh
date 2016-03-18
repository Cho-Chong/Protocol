#!/bin/sh
#  proto_script.sh
#  ChatLib
#
#  Created by Cho Chong on 3/16/16.
#  Copyright © 2016 Cho Chong. All rights reserved.

echo "Generating protobuf files..."

cd /Users/Cho/Documents/Programming/Chat/ChatLib/ChatLib
./Google/protobuf/bin/protoc --proto_path=. records.proto --cpp_out=.

cd /Users/Cho/Documents/Programming/Chat/ChatLib/ChatLib
./Google/protobuf/bin/protoc --proto_path=. packet.proto --cpp_out=.

echo "Built protobuf files"

exit