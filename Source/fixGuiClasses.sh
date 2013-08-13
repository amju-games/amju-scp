#perl -pi -e 's/Gui/PoolGui/g' *.cpp
#perl -pi -e 's/Gui/PoolGui/g' *.h

perl -pi -e 's/"PoolGui/"Gui/g' *.cpp
perl -pi -e 's/"PoolGui/"Gui/g' *.h
