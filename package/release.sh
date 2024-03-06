#! /bin/bash 

function get_file_size(){
    local file="$1"
    if [[ "$OSTYPE" == "darwin"* ]]; then
        eval `stat -s "$file"`
        local res="$?"
        echo "$st_size"
        return $res
    else
        stat --printf="%s" "$file"
        return $?
    fi
}

echo "Creating Release Files ..."
pushd "../../" >/dev/null

ZIP=arduino-sophgo.zip
DIR=arduino-sophgo

rm -rf "$ZIP"
zip -qr "$ZIP" "$DIR" -x "${DIR}/.git**" -x "${DIR}/tools/*"

if [ $? -ne 0 ]; then echo "ERROR: Failed to create $ZIP ($?)"; exit 1; fi


VERSION=`cat ${DIR}/Version`
SHA=`shasum -a 256 "$ZIP" | cut -f 1 -d ' '`
SIZE=`get_file_size "$ZIP"`

# Construct JQ argument with package data
jq_arg=".packages[0].platforms[0].version = \"$VERSION\" |\
    .packages[0].platforms[0].size = \"$SIZE\" |\
    .packages[0].platforms[0].checksum = \"SHA-256:$SHA\""

JSON_TEMPLATE="${DIR}/package/package_sg200x_index.template.json"
JSON_REL_TMP1="package_sg200x_index_tmp1.json"
cat "$JSON_TEMPLATE" | jq "$jq_arg" > "$JSON_REL_TMP1"

BURNZIP=burntool.zip
BURNTAR=burntool.tar.gz
BURNDIR=arduino-sophgo/tools/
BURNNAME=burntool

rm -rf "$BURNZIP"
rm -rf "$BURNTAR"

pushd $BURNDIR >/dev/null
zip -qr "$BURNZIP" "$BURNNAME"
if [ $? -ne 0 ]; then echo "ERROR: Failed to create $BURNZIP ($?)"; exit 1; fi
mv $BURNZIP ../../
popd  >/dev/null

SHA=`shasum -a 256 "$BURNZIP" | cut -f 1 -d ' '`
SIZE=`get_file_size "$BURNZIP"`

jq_arg=".packages[0].tools[1].systems[0].size = \"$SIZE\" |\
    .packages[0].tools[1].systems[0].checksum = \"SHA-256:$SHA\""

# JSON_REL_TMP1="package_sg200x_index_tmp1.json"
JSON_REL_TMP2="package_sg200x_index_tmp2.json"
cat "$JSON_REL_TMP1" | jq "$jq_arg" > "$JSON_REL_TMP2"

pushd $BURNDIR >/dev/null
tar -zcf "$BURNTAR" "$BURNNAME"
if [ $? -ne 0 ]; then echo "ERROR: Failed to create $BURNTAR ($?)"; exit 1; fi
mv $BURNTAR ../../
popd  >/dev/null

SHA=`shasum -a 256 "$BURNTAR" | cut -f 1 -d ' '`
SIZE=`get_file_size "$BURNTAR"`

jq_arg=".packages[0].tools[1].systems[1].size = \"$SIZE\" |\
    .packages[0].tools[1].systems[1].checksum = \"SHA-256:$SHA\" |\
    .packages[0].tools[1].systems[2].size = \"$SIZE\" |\
    .packages[0].tools[1].systems[2].checksum = \"SHA-256:$SHA\" |\
    .packages[0].tools[1].systems[3].size = \"$SIZE\" |\
    .packages[0].tools[1].systems[3].checksum = \"SHA-256:$SHA\" |\
    .packages[0].tools[1].systems[4].size = \"$SIZE\" |\
    .packages[0].tools[1].systems[4].checksum = \"SHA-256:$SHA\""

# JSON_REL_TMP2="package_sg200x_index.json"
JSON_REL="package_sg200x_index.json"
cat "$JSON_REL_TMP2" | jq "$jq_arg" > "$JSON_REL"
rm $JSON_REL_TMP1 $JSON_REL_TMP2