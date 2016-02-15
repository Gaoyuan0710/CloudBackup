find . -regextype posix-egrep -regex ".*\.(cpp|h)" | xargs perl -pi -e 's|\bworkServer\b|WorkServer|g'

find . -regextype posix-egrep -regex ".*\.(cpp|h)" | xargs perl -pi -e 's|\bmaxCon\b|MaxConnctionNum|g'

find . -regextype posix-egrep -regex ".*\.(cpp|h)" | xargs perl -pi -e 's|\bm_ip\b|local_ip|g'

find . -regextype posix-egrep -regex ".*\.(cpp|h)" | xargs perl -pi -e 's|\bm_port\b|local_port|g'

find . -regextype posix-egrep -regex ".*\.(cpp|h)" | xargs perl -pi -e 's|\bl_ip\b|balance_ip|g'

find . -regextype posix-egrep -regex ".*\.(cpp|h)" | xargs perl -pi -e 's|\bl_port\b|balance_port|g'

find . -regextype posix-egrep -regex ".*\.(cpp|h)" | xargs perl -pi -e 's|\bhandler_upload\b|HandlerUpload|g'

find . -regextype posix-egrep -regex ".*\.(cpp|h)" | xargs perl -pi -e 's|\bhandler_download\b|HandlerDownload|g'
