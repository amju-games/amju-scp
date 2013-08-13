
perl -pi -e 's/SharedPtr/RCPtr/g' *.cpp
perl -pi -e 's/SharedPtr/RCPtr/g' *.h

perl -pi -e 's/Shareable/RefCounted/g' *.h

