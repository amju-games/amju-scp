export DEST_FILE=level$1.amju2
cp level0.amju2 $DEST_FILE
# Replace room file names
rfindrep  -m '\.amju2$' -f 'level-0-room' -r "level-$1-room" $DEST_FILE

# Copy room files
cp level-0-room-0.room level-$1-room-0.room
cp level-0-room-1.room level-$1-room-1.room
cp level-0-room-2.room level-$1-room-2.room
cp level-0-room-3.room level-$1-room-3.room

# Replace .comp filename in room files
rfindrep -m '\.room$' -f 'level-0-room' -r "level-$1-room" "level-$1-room-0.room"
rfindrep -m '\.room$' -f 'level-0-room' -r "level-$1-room" "level-$1-room-1.room"
rfindrep -m '\.room$' -f 'level-0-room' -r "level-$1-room" "level-$1-room-2.room"
rfindrep -m '\.room$' -f 'level-0-room' -r "level-$1-room" "level-$1-room-3.room"

# Copy .comp files
cp level-0-room-0.comp level-$1-room-0.comp
cp level-0-room-1.comp level-$1-room-1.comp
cp level-0-room-2.comp level-$1-room-2.comp
cp level-0-room-3.comp level-$1-room-3.comp

# Copy bitmap buttons
cp thumb-0-0.bmp thumb-$1-0.bmp 
cp thumb-0-1.bmp thumb-$1-1.bmp 
cp thumb-0-2.bmp thumb-$1-2.bmp 
cp thumb-0-3.bmp thumb-$1-3.bmp 


