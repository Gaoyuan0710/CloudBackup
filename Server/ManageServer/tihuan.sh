find . -regextype posix-egrep -regex ".*\.(cpp|h|hpp)" | xargs perl -pi -e 's|\bepollfd\b|epoll_fd_|g'

find . -regextype posix-egrep -regex ".*\.(cpp|h|hpp)" | xargs perl -pi -e 's|\bsocketfd\b|socket_fd_|g'

find . -regextype posix-egrep -regex ".*\.(cpp|h|hpp)" | xargs perl -pi -e 's|\bServerIP\b|server_ip_|g'

find . -regextype posix-egrep -regex ".*\.(cpp|h|hpp)" | xargs perl -pi -e 's|\bPort\b|server_port_|g'

find . -regextype posix-egrep -regex ".*\.(cpp|h|hpp)" | xargs perl -pi -e 's|\bSetNonBlocking\b|setNonBlock|g'

find . -regextype posix-egrep -regex ".*\.(cpp|h|hpp)" | xargs perl -pi -e 's|\baddfd\b|AddFd|g'

find . -regextype posix-egrep -regex ".*\.(cpp|h|hpp)" | xargs perl -pi -e 's|\bSetEpollWait\b|setEpollFd|g'

find . -regextype posix-egrep -regex ".*\.(cpp|h|hpp)" | xargs perl -pi -e 's|\bEpollThreadID\b|epoll_thread_id|g'

find . -regextype posix-egrep -regex ".*\.(cpp|h|hpp)" | xargs perl -pi -e 's|\bManageServerIp\b|manage_server_ip|g'

find . -regextype posix-egrep -regex ".*\.(cpp|h|hpp)" | xargs perl -pi -e 's|\bManageServerPort\b|manage_server_port|g'

find . -regextype posix-egrep -regex ".*\.(cpp|h|hpp)" | xargs perl -pi -e 's|\bWorkPort\b|work_port|g'

find . -regextype posix-egrep -regex ".*\.(cpp|h|hpp)" | xargs perl -pi -e 's|\bWorkIp\b|work_ip|g'

