
find ./proto -name "*.proto" | xargs protoc -I=./proto/ --cpp_out=./gen_cpp/

cp ./gen_cpp/*.*   ../../../server/servers/pb/src/
cp ./txt/*.* 	   ../../../server/release/server_config/scp/
cp ./lua/*.*	   ../../../server/release/server_config/scp_lua/
